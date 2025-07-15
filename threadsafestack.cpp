#include<iostream>
#include<memory>
#include<mutex>
#include<stack>
#include<thread>

//custom exception thrown when pop() is called on empty stack 
struct empty_stack:std ::exception{
     const char* what() const noexcept override {//this overrides the what() function for error message
        return "Stack is empty!";
    }
};

template<typename T>
class threadsafe_stack{
    private:
    std::stack<T> data; //this is the stack h0lding the data
    mutable std::mutex m;
    public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other){
        std::lock_guard<std::mutex>lock(other.m);// this ensures that no other thread is updating other.data while we are copying the data
        data = other.data;
    }

    threadsafe_stack& operator = (const threadsafe_stack&)=delete;// this prevents the accidental copying of the data

    void push(T new_value){
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));// move avoids the deep copy
    }
// here we used shared ptr because it avoids the unnecessary copying if the object is large 
// it is safe and efficient when multiple owners might need access to popped value 
    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }
// shared ptr uses heap memory so it avoids the overhead of heap allocation 
    void pop(T& value){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value = std::move(data.top());
        data.pop();
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
            // this calls void pop(T& value)
            std::cout << "Popped: " << value << std::endl;
            // std::shared_ptr<int> value = s.pop();
            //  std::cout << *value << std::endl;

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