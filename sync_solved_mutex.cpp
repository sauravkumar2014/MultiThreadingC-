#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

class Counter {
    std::mutex mutex;
    int value;
public:
    Counter() : value(0){}

    void increment() {
        mutex.lock();
        ++value;
        mutex.unlock();
    }

    int get_value() { return this->value; }
};

int main(){
    Counter counter;

    std::vector<std::thread> threads;
    for(int i = 0; i < 10000; ++i){
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