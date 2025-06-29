#include<iostream>
#include<thread>
using namespace std;
struct func
{
    int &i;
    func(int & i_):i(i_){}
    void operator ()(){
        for (unsigned j = 0; j<20;j++)
        {
            do_something();
        }
    }
    void do_something(){
    cout<<"Sujal"<<endl;

}


};
void oops(){
    int some_local_state = 0;
    func my_func(some_local_state);
    thread my_thread(my_func);
    my_thread.join();
}

int main(){
    oops();
    return 0;
}
