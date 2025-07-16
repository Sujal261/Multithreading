#include<iostream>
#include<thread>
#include<queue>
#include<memory>
template<typename T>
class queue{
    private:
           struct node
           {
            std::shared_ptr<T> data;
            std::unique_ptr<node> next;
            node():data(nullptr), next(nullptr){}
            node(T data_):
                data(std::move(data_)){}
           };
    std::unique_ptr<node> head;
    node* tail;
    
    public: 
       queue():head(new node), tail(head.get()){}
       queue(const queue& other) = delete;
       queue& operator = (const queue& other) = delete;
       std::shared_ptr<T> try_pop()
       {
        if(head.get()==tail){
            return std::shared_ptr<T> ();
        }
        std::shared_ptr<T> const res(
            head->data
        );
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
       }

       void push(T new_value){ 
       std::shared_ptr<T> new_data( std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        tail->data = new_data;
        node* const new_tail = p.get();//.get return the pointer to the object managed by the std::unique_ptr
        
            tail->next = std::move(p);
            tail = new_tail;
       }
};
int main(){
    queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);

    std::shared_ptr<int> val;
    val = q.try_pop();
    std::cout<<(val ? *val:-1)<<std::endl;
    val = q.try_pop();
    std::cout<<(val ?*val:-1)<<std::endl;
    val = q.try_pop();
    std::cout<<(val?*val:-1)<<std::endl;
    val = q.try_pop();
    std::cout<<(val? *val:-1)<<std::endl;
    val = q.try_pop();
    std::cout<<(val? *val:-1)<<std::endl;
    return 0;
}
// problem with push and try pop in the queue can access and modify shared data when the queue has one item where head ==tail. Th 
//creates a race condition in a thread safe queue has one item where head ==tail. This creates a race condition in a thread safe 
// implementation as push() updates tail->next and try pop() reads head->next which are the same when head==tail.To prevent this 
// both operations would need to lock the same mutex eliminating the beneftit of separate mutex so we 