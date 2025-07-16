#include<iostream>
#include<thread>
#include<mutex>
#include<memory>
template<typename T>
class threadsafe_queue
{
    private:
           struct node{
            std::shared_ptr<T> data;
            std::unique_ptr<node> next;
           };
           std::mutex head_mutex;
           std::unique_ptr<node> head;
           std::mutex tail_mutex;
           node* tail;

           node* get_tail()
           {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            return tail;
           }
           std::unique_ptr<node> pop_head()
           {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if(head.get()==get_tail()){
                return nullptr;
            }
            std::unique_ptr<node> old_head=std::move(head);
            head = std::move(old_head->next);
            return old_head;
           }
    public: 
          threadsafe_queue():head(new node), tail(head.get()){}
          threadsafe_queue(const threadsafe_queue& other) = delete;
          threadsafe_queue& operator = (const threadsafe_queue& other)= delete;
          std::shared_ptr<T> try_pop(){
            std::unique_ptr<node> old_head = pop_head();
            return old_head? old_head->data:std::shared_ptr<T>();
          }
          void push(T new_value){
            std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
            std::unique_ptr<node> p(new node);
            node* const new_tail = p.get();
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            tail->next = std::move(p);
            tail = new_tail;
          }
};
int main(){
    threadsafe_queue<int> tq;
    tq.push(1);
    tq.push(2);
    tq.push(3);
    std::shared_ptr<int> val;
    val = tq.try_pop();
    std::cout<<"The value is"<<*val<<std::endl;
    val = tq.try_pop();
    std::cout<<"The value is"<<*val<<std::endl;
    val= tq.try_pop();
    std::cout<<"The value is"<<*val<<std::endl;
    return 0;
}
// in try pop it reads from the same nodes that push() might be changing but try pop uses a different lock(head_mutex) than push()
// if there is no common lock they can touch the same memory at the same time
// that is a data race and its undefined behavior(anything could go wrong)
// when try pop() needs to know what the tail is it calls get_tail() that function locks tail_mutex which is the same lock that
//push() uses. the shared lock ensures either try_pop() runs before push() or runs after the push() but never during the middle of it

// std::unique_ptr<node> pop_head() 
//  {
//  node* const old_tail=get_tail(); 
//  std::lock_guard<std::mutex> head_lock(head_mutex);
 
//  if(head.get()==old_tail) 
//  {
//  return nullptr;
//  }
//  std::unique_ptr<node> old_head=std::move(head);
//  head=std::move(old_head->next); 
//  return old_head;
//  }

// thread A calls pop_head() reads old_tail = get_tail() before locking head_mutex 
//while thread A is waiting to acquire head_mutex some other thread may acquire the head mutex and change tail more further
// when thread A finally acquires head_mutex the value it read as old_tail might
// no longer be the tail
// no longer be connected to head
// in extreme cases be off the list completely
// so when thread A compares  head.get() == old_tail it might wrongly think thq queue is not empty or update 
// head to an invalid node - breaking the list structure
