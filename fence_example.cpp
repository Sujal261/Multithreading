#include<atomic>
#include<thread>
#include<assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y(){
    x.store(true, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);// this should be always be between the two memory order relax otherwise we wont see the correct order
    y.store(true, std::memory_order_relaxed);
}
void read_y_then_x(){
    while(!y.load(std::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order_acquire); //same for the acquire part 
    if(x.load(std::memory_order_relaxed))
       ++z;
}
int main(){
    x=false;
    y=false;
    z=0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load()!=0);
    return 0;
}