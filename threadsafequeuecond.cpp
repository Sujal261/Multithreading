#include<iostream>
#include<queue>
#include<condition_variable>
#include<thread>
template<typename T>
class threadsafe_queue{
    private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    public:
          threadsafe_queue(){}
          void push(T new_value){
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(std::move(new_value));
            data_cond.notify_one();
          }

          void wait_and_pop(T& value){
            std::unique_lock<std::mutex> lk(mut);
            data_cond.wait(lk, [this]{return !data_queue.empty();});
            value = std::move(data_queue.front());
            data_queue.pop();
            
          }


          std::shared_ptr<T> wait_and_pop()
          {
            std::unique_lock<std::mutex> lk(mut);
            data_cond.wait(lk, [this]{return !data_queue.empty();});
            std::shared_ptr<T> res=std::make_shared<T> (std::move(data_queue.front()));
            
            data_queue.pop();
            return res;
          }


          bool try_pop(T& value){
            std::lock_guard<std::mutex> lk(mut);
            if(data_queue.empty())
                 return false;
            value = std::move(data_queue.front());
            data_queue.pop();
            return true;
          }


          std::shared_ptr<T> try_pop(){
            std::lock_guard<std::mutex> lk(mut);
            if(data_queue.empty())
                return std::shared_ptr<T>();
            std::shared_ptr<T> res=std::make_shared<T>(std::move(data_queue.front()));
            
            data_queue.pop();
            return res;
          }


          bool empty() const{
            std::lock_guard<std::mutex> lk(mut);
            return data_queue.empty();
          }

};
void consumer_wait_and_pop_ref(threadsafe_queue<int>&q){
    int value;
    q.wait_and_pop(value);
    std::cout<<"wait and pop got the value:"<<value<<std::endl;
}
void consumer_wait_and_pop_ptr(threadsafe_queue<int>&q){
    std::shared_ptr<int> value = q.wait_and_pop();
    std::cout<<"wait and pop ptr got the calue"<<*value<<std::endl;
}
void consumer_try_pop_ref(threadsafe_queue<int>& q){
   int value;
   if (q.try_pop(value)){
    std::cout<<"try pop got value:"<<value<<std::endl;

   }
   else{
    std::cout<<"trying to pop but the queue was empty"<<std::endl;
   }
}
void try_pop_ptr(threadsafe_queue<int>&q){
    std::shared_ptr<int> value = q.try_pop();
    if(value)
    {
        std::cout<<"value from try pop ptr is"<<*value<<std::endl;
    }else{
        std::cout<<"try pop queue was empty"<<std::endl;
    }
}
int main(){
    threadsafe_queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);
    q.push(40);

    std::thread t1(consumer_wait_and_pop_ref, std::ref(q));
    std::thread t2(consumer_wait_and_pop_ptr, std::ref(q));
    std::thread t3(consumer_try_pop_ref, std::ref(q));
    std::thread t4(try_pop_ptr, std::ref(q));

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    std::thread t5(consumer_try_pop_ref, std::ref(q));           
    std::thread t6(try_pop_ptr, std::ref(q));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    return 0;


}