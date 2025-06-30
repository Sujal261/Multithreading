#include<iostream>
#include<thread>
#include<future>
#include<utility>
#include<type_traits>
template<typename F, typename A>
std::future<typename std::result_of<F(A&&)>::type>
spawn_task(F&& f, A&& a)
{
    typedef typename std::result_of<F(A&&)>::type result_type;
    std::packaged_task<result_type(A&&)> task(std::move(f));
    std::future<result_type> res = task.get_future();
    std::thread t(std::move(task), std::move(a));
    t.detach();
    return res;

}
int square(int x){
    return x*x;
}
int main(){
    std::future<int> fut = spawn_task(square, 10);
    std::cout<<"Result is"<<fut.get()<<"\n";
    return 0;
}