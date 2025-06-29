
// This is by normal object function
// #include<iostream>
// #include<thread>
// using namespace std;
// class background_task{
//     public:
//           void operator () () const{
//             do_something();
//             do_something_else();
//           }

//         void  do_something  () const{
//             cout<<"Here the is the thread"<<endl;
//           }
//         void do_something_else() const {
//             cout<<"this is the thread"<<endl;
//         }
// };

// int  main(){
//     background_task f;
//     thread my_thread(f);
//     my_thread.join();
//     return 0;
// }

//now performing by lambda expression 

#include<iostream>
 #include<thread>
 using namespace std;

void  do_something  () {
           cout<<"Here the is the thread"<<endl;
          }
       void do_something_else() {
            cout<<"this is the thread"<<endl;
        }
int  main(){
    
    thread my_thread([](){
        do_something();
        do_something_else();

    });
    my_thread.join();
    return 0;
}