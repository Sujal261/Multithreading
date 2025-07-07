#include<iostream>
#include<thread>
#include<assert.h>
#include<atomic>
std::atomic<bool> x, y;
std::atomic<int> z;

void store_x(){
    x.store(true, std::memory_order_seq_cst);
}
void store_y(){
    y.store(true, std::memory_order_seq_cst);
}
void loady_and_x(){
    while(!y.load(std::memory_order_seq_cst))
    if(x.load(std::memory_order_seq_cst)){
        z++;
    }
}
void loadx_and_y(){
    while(!x.load(std::memory_order_seq_cst))
    if(y.load(std::memory_order_seq_cst)){
        z++;
    }
}
int main(){
    x=false;
    y=false;
    z=0;
    std::thread t1(store_x);
    std::thread t2(store_y);
    std::thread t3(loadx_and_y);
    std::thread t4(loady_and_x);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    assert(z.load()!=0);
    return 0;
}
//this code must have given the result but this gives the error because of compiler optimizations stuff



