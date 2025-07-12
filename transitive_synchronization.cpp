#include<thread>
#include<iostream>
#include<atomic>
#include<assert.h>
std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false); 
//std::atomic<int> sync(0);
void thread_1(){
    data[0].store(42, std::memory_order_relaxed);
    data[1].store(97, std::memory_order_relaxed);
    data[2].store(17, std::memory_order_relaxed);
    data[3].store(2003, std::memory_order_relaxed);
    data[4].store(-141, std::memory_order_relaxed);
   sync1.store(true, std::memory_order_release);

}
void thread_2(){
    while(!sync1.load(std::memory_order_acquire));
    sync2.store(true, std::memory_order_release);
    // here we can combine the load and store in the signle command using the compare and exchange strong 
    /*
    int expected = 1;
    while(!sync.compare_exchange_strong(expected, 2, std::memory_order_acq_rel))
    expected = 1;
    */
}
void thread_3(){
    while(!sync2.load(std::memory_order_acquire));
    // while(sync.load(std:memory_order_acquire)<2)
    assert(data[0].load(std::memory_order_relaxed)==42);
    assert(data[1].load(std::memory_order_relaxed)==97);
    assert(data[2].load(std::memory_order_relaxed)==17);
    assert(data[3].load(std::memory_order_relaxed)==2003);
    assert(data[4].load(std::memory_order_relaxed)==-11); // here there is error because -141 doesnot match with -11
}
int main(){
    std::thread t1(thread_1);
    std::thread t2(thread_2);
    std::thread t3(thread_3);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
//in this program there is transitive dependency formed between thread 1 thread 2 and thread 3 first the thread 1 synchronizes with thread2 using sync1
//then thread 2 is synchronized with thread 3 using the sync2 so the load performed in thread1 is seen to thread 3 
// thread 1->thread 2->thread 3 finally thread 1->thread 3