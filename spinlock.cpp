#include<iostream>
#include<atomic>
#include<thread>
#include<vector>


class spinlock_mutex{
    std::atomic_flag flag;

    public:
    spinlock_mutex():flag(ATOMIC_FLAG_INIT){}

    void lock(){
        while(flag.test_and_set(std::memory_order_acquire)){}
    }
    void unlock(){
        flag.clear(std::memory_order_release);
    }
};

int counter = 0;
spinlock_mutex spinlock;

void increment(int id){
    for(int i =0; i<10000;i++)
    {
        spinlock.lock();
        ++counter;
        spinlock.unlock();
    }
}
int main(){
    const int num_threads = 10;
    std::vector<std::thread> threads;

    for(int i =0;i<num_threads;i++){
        threads.emplace_back(increment, i);
    }

    for(auto& t:threads){
        t.join();
    }
    std::cout<<"Final counter variable is"<<counter<<std::endl;
    return 0;
}
