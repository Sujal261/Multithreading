#include<iostream>
#include<thread>
using namespace std;
void hello(){
    cout<<"Hello concurrent world";
}
int main(){
    thread t(hello);
    t.join();
}