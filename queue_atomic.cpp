#include<atomic>
#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
std::vector<int> queue_data;
std::atomic<int> count;
std::mutex count_mutex;

void wait_for_more_items(){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
void process(int value){
    std::lock_guard<std::mutex> lock(count_mutex);
    std::cout << "Processed: " << value << " by thread " << std::this_thread::get_id() << std::endl;
}
void populate_queue(){
    unsigned const number_of_items = 20;
    for(unsigned i =0; i<number_of_items;i++){
    queue_data.push_back(i);
    }
    count.store(number_of_items, std::memory_order_release);
}
void consume_queue_items(){
    while(true){
        int item_index;
        if((item_index = count.fetch_sub(1, std::memory_order_acquire))<=0){
             wait_for_more_items();
             continue;
        }
        process(queue_data[item_index-1]);
    }
}
int main(){
    std::thread a(populate_queue);
    std::thread b(consume_queue_items);
    std::thread c(consume_queue_items);
    a.join();
    b.join();
    c.join();
}