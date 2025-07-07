#include<vector>
#include<atomic>
#include<iostream>
#include<thread>
#include<chrono>
std::atomic<bool> data_ready(false);
std::vector<int> data;

void reader_thread(){
    while(!data_ready.load()){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout<<"The answer="<<data[0]<<std::endl;
}
void writer_thread(){
    data.push_back(42);
    data_ready= true;
}
int main(){
    //it uses happens before and synchronizes with relation 
    //data push happens before data ready 
    //data ready = true synchronizes with data store load 
    //data ready store happens before data[0] is read
    std::thread t1(reader_thread);
    std::thread t2(writer_thread);
    t1.join();
    t2.join();
    return 0;
}