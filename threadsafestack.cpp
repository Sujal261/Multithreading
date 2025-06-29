#include<iostream>
#include<memory>
#include<mutex>
#include<stack>
#include<thread>

struct empty_stack:std ::exception{
     const char* what() const noexcept override {
        return "Stack is empty!";
    }
};

template<typename T>
class threadsafe_stack{
    private:
    std::stack<T> data;
    mutable std::mutex m;
    public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other){
        std::lock_guard<std::mutex>lock(other.m);
        data = other.data;
    }

    threadsafe_stack& operator = (const threadsafe_stack&)=delete;

    void push(T new_value){
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }

    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.top();
        return res;
    }

    void pop(T& value){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value = data.top();
        data.top();
    }
    bool empty() const{
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }

};

void example_push(threadsafe_stack<int>& s) {
    for (int i = 0; i < 5; ++i) {
        s.push(i);
        std::cout << "Pushed: " << i << std::endl;
    }
}

void example_pop(threadsafe_stack<int>& s) {
    try {
        for (int i = 0; i < 5; ++i) {
            int value;
            s.pop(value);
            std::cout << "Popped: " << value << std::endl;
        }
    } catch (const empty_stack& e) {
        std::cout << e.what() << std::endl;
    }
}
int main(){
    threadsafe_stack<int> s;
    std::thread t1(example_push, std::ref(s));
    std::thread t2(example_pop, std::ref(s));
    t1.join();
    t2.join();
    return 0;
}