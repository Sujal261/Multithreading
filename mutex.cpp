#include<iostream>
#include<list>
#include<mutex>
#include<algorithm>
#include<thread>
#include<vector>
std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value){
    std:: lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
    std::cout<<"Added value"<<new_value<<std::endl;
}

bool list_contains(int value_to_find){
    std::lock_guard<std::mutex> guard(some_mutex);
    bool found = std::find(some_list.begin(), some_list.end(), value_to_find)!=some_list.end();
    std::cout<<"Value"<<value_to_find<<(found ?"found":"not found ")<<std::endl;
        return found;
}
void thread_test_function(int id){
    add_to_list(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    list_contains(id);
    list_contains(-1);
}
int main()
{
    const int num_threads = 5;
    std::vector<std::thread> threads;
    std::cout<<"Starting"<<num_threads<<"threads ...."<<std::endl;
    for(int i =0;i<num_threads;i++){
        threads.emplace_back(thread_test_function, i);

    }
    for(auto& t :threads){
        t.join();
    }
    std::cout<<"SIngle threaded test"<<std::endl;
    add_to_list(100);
    std::cout<<"COntains 100?"<<std::boolalpha<<list_contains(100)<<std::endl;
    std::cout<<"Contains 999?"<<std::boolalpha<<list_contains(999)<<std::endl;
    return 0;
}