#include<condition_variable>
#include<mutex>
#include<iostream>
#include<chrono>
#include<thread>
std:: condition_variable cv;
bool done;
std::mutex m ;
bool wait_loop(){
    auto const timeout = std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
    std::unique_lock<std::mutex> lk(m);
    while(!done){
        if(cv.wait_until(lk, timeout)==std::cv_status::timeout){
            break;
        }
    }
    return done;
}
void worker_thread(){
    std::this_thread::sleep_for(std::chrono::milliseconds(200));//here if i keep yes than 500 it will show condition is met otherwise it will show condition not met 
    {
        std::lock_guard<std::mutex> lk(m);
        done = true;
    }
    cv.notify_one();
}


int main(){
    std::thread t(worker_thread);
    bool result = wait_loop();
    if(result)
    {
        std::cout<<"COndition met before timeout.\n";
    }
    else{
        std::cout<<"Timeout occurred before condition was met.\n";

    }
    t.join();
    return 0;
}