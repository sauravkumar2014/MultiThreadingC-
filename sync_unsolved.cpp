#include <thread>
#include <iostream>
#include <vector>

class Counter {
    int value;
public:
    Counter() : value(0){}

    void increment() { ++value; }

    int get_value() { return this->value; }
};

int main(){
    Counter counter;

    std::vector<std::thread> threads;
    for(int i = 0; i < 1000; ++i){
        threads.push_back(std::thread([&counter](){
            for(int i = 0; i < 100; ++i){
                counter.increment();
            }
        }));
    }

    for(auto& thread : threads){
        thread.join();
    }

    std::cout << counter.get_value() << std::endl;

    return 0;
}