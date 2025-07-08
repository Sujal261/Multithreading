#include<iostream>
#include<thread>
#include<atomic>
#include<vector>
#include<assert.h>
std::atomic<bool> x, y;
std::atomic<int> z;
int failed;
void storex_and_y(){
    x.store(true,std::memory_order_relaxed);
    y.store(true,std::memory_order_relaxed);
}
void loadx_and_y(){
    while(!x.load(std::memory_order_relaxed))
    if(y.load(std::memory_order_relaxed)){
        ++z;
    }
}
int main(){
    for (int i =0;i<10000;i++){
        x = false;
        y=false;
        z=0;
    std::thread t1(storex_and_y);
    std::thread t2(loadx_and_y);
    t1.join();
    t2.join();
    if (z.load() == 0) {
            failed++;
            std::cout << "Failure at iteration: " << i << '\n';
        }
    }

    std::cout << "Total failures: " << failed << " out of 10000\n";
    return 0;
}