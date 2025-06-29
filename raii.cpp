#include<iostream>
#include<thread>
using namespace std;
class thread_guard{
    std::thread &t;
    public:
    explicit thread_guard(std::thread& my_thread):t(my_thread){}

    ~thread_guard(){
        if(t.joinable())
        {
            t.join();
        }
    }
    thread_guard(thread_guard const &) = delete;
    thread_guard& operator = (thread_guard const &) = delete;


};
struct func{
    int &i;
    func(int &i_):i(i_){}
    void operator ()(){
        for (int i =0; i<20;i++){
        do_something();
        }
    }
    void do_something(){
        cout<<"Sujal";
    }
};
void do_another_thing(){
    
}

void f(){
    int some_local_state = 0;
    func my_func(some_local_state);
    thread my_thread(my_func);
    thread_guard thread1(my_thread);
    do_another_thing();
}