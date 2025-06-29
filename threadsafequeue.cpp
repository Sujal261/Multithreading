#include<iostream>
#include<mutex>
#include<condition_variable>
#include<memory>
#include<utility>
#include<stdexcept>
#include<deque>
#include<thread>


template <typename T, typename Container = std::deque<T>>
class ThreadSafeQueue{
    private:
           mutable std::mutex mut;
           std::condition_variable cond;
           Container data_queue;


    public:
          ThreadSafeQueue()= default;

          ThreadSafeQueue(const ThreadSafeQueue&) = delete;
          ThreadSafeQueue& operator = (const ThreadSafeQueue)= delete;

          ThreadSafeQueue(ThreadSafeQueue&& other) noexcept{
            if(this!=&other){
                std::lock_guard<std::mutex> lock1(mut);
                std::lock_guard<std::mutex> lock2(other.mut);
                data_queue = std::move(other.data_queue);
            }
            
          }

          void push(const T& value){
            std::lock_guard<std::mutex> lock(mut);
            data_queue.push_back(std::move(value));
            cond.notify_one();
          }
          void push( T&& value){
            std::lock_guard<std::mutex> lock(mut);
            data_queue.push_back(std::move(value));
            cond.notify_one();
          }

          template <class... Args>
          void emplace(Args&&... args){
            std::lock_guard<std::mutex> lock(mut);
            // cond.wait(lock, [this]{return !data_queue.empty();});
            data_queue.emplace_back(std:: forward<Args>(args)...);
            cond.notify_one();
          }

          T wait_and_pop(){
            std::unique_lock<std::mutex> lock(mut);
            cond.wait(lock, [this]{return !data_queue.empty();});
            T value = std::move(data_queue.front());
            data_queue.pop_front();
            return value;
          }
          void wait_and_pop(T& result){
            std::unique_lock<std::mutex> lock(mut);
            cond.wait(lock, [this]{return !data_queue.empty();});
            result = std::move(data_queue.front());
            data_queue.pop_front();
          }
          bool try_pop(T& result){
            std::lock_guard<std::mutex> loc(mut);
            if(data_queue.empty())
                return false;

            result = std::move(data_queue.front());
            data_queue.pop_front();
            return true;
          }

          T& front(){
            std::lock_guard<std::mutex> lock(mut);
            return data_queue.front();
          }
          const T& front() const{
            std::lock_guard<std::mutex> lock(mut);
            return data_queue.front();
          }

          T& back(){
            std::lock_guard<std::mutex> lock(mut);
            return data_queue.back();
          }
          const T& back() const{
            std::lock_guard<std::mutex> lock(mut);
            return data_queue.back();
          }

          void pop(){
            std::lock_guard<std::mutex> lock(mut);
            if(!data_queue.empty())
                data_queue.pop_front();
          }

          size_t size() const{
            std::lock_guard<std::mutex> lock(mut);
            return data_queue.size();
          }
          void swap(ThreadSafeQueue& other)
          {
            if(this == &other) return;
            std::scoped_lock lock(mut, other.mut);
            data_queue.swap(other.data_queue);
          }
          void clear(){
            std::lock_guard<std::mutex> lock(mut);
            data_queue.clear();
          }

};
int main(){
    ThreadSafeQueue<int> tsq;

    std::thread producer([&tsq](){
        for(int i =1; i<=5;i++){
            std::cout<<"Producer:pushing"<<i<<"\n";
            tsq.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
        tsq.push(-1);
    });

    std::thread consumer([&tsq](){
        while(true){
            int value = tsq.wait_and_pop();
            if(value ==-1) break;
            std::cout<<"Container : got"<<value<<"\n";
        }
        std::cout<<"Consumer done\n";
    });
    producer.join();
    consumer.join();
    return 0;
}