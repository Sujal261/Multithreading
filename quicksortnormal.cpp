#include<iostream>
#include<list>
#include<algorithm>
#include<iterator>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input){
    if(input.empty()){
        return input;
    }
    std::list<T> result;

    result.splice(result.begin(), input,  input.begin());
    T const& pivot = *result.begin();

    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t){return t <pivot;});
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    auto new_lower = sequential_quick_sort(std::move(lower_part));
    auto new_higher = sequential_quick_sort(std::move(input));

    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);

    return result;
}
int main(){
    std::list<int> values = {9,4,7,1,2,3};
    std::cout<<"Original value:";
    for(int v:values) std::cout<<v<<"";
    std::cout<<std::endl;
    std::list<int> sorted = sequential_quick_sort(values);
    std::cout<<"Sorted list:";
    for(int v :sorted) std::cout<<v<<"";
    return 0;
}
