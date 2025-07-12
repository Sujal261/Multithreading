// data dependency exists when the result of one instruction is used as the input to a following instruction 
#include<iostream>
#include<thread> 
#include<atomic>
#include<assert.h>
struct  X
{
    int i ;
    std::string s;

};
std::atomic<X*> p;
std::atomic<int> a;

void create_X(){
    X* x = new X;
    x->i = 42;
    x->s ="hello";
    a.store(99, std::memory_order_relaxed);
    p.store(x, std::memory_order_release);
}
void use_x(){
    X* x;
    while(!(x = p.load(std::memory_order_consume)))
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    assert(x->i ==42);
    assert(x->s =="hello");// here these two lines cannot be evaluated before loading the x so it is data dependency
    assert(a.load(std::memory_order_relaxed)==92);//this is not data dependent becuase in create_X thread we stored with memory order relaxed

}
int main(){
    std::thread t1(create_X);
    std::thread t2(use_x);
    t1.join();
    t2.join();

}

