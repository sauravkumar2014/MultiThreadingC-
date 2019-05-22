#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#include <exception>
using namespace std;

class Counter {
    int value;
public:
    Counter() : value(0){}

    void increment() { ++value; }

    void decrement() {
        if(value == 0) {
            throw std::runtime_error("Value cannot be less than 0");
        }
        --value;
    }

    int get_value() { return this->value; }
};

class ConcurrentCounter {
    std::mutex mutex;
    Counter counter;

public:
    int get_value() { return counter.get_value(); }
    void increment() {
        std::lock_guard<std::mutex> guard(mutex);
        
        counter.increment();
    }

    void decrement() {
        std::lock_guard<std::mutex> guard(mutex);

        try {
            counter.decrement();
        } catch (std::runtime_error &e) {
            throw;
        } catch (...) {
            throw;
        }
    }
};

int main() {
    ConcurrentCounter counter;

    std::vector<std::thread> threads;
    for(int i = 0; i < 100; ++i) {
        threads.push_back(std::thread([&counter]() {
            for(int i = 0; i < 100; ++i){
                counter.increment();
            }
        }));
    }
    for(auto& thread : threads) {
        thread.join();
    }
    threads.clear();

    for(int i = 0; i < 101; ++i) {
        threads.push_back(std::thread([&counter]() {
            for(int j = 0; j < 100; ++j){
                try {
                    counter.decrement();
                } catch(std::runtime_error &e) {
                    std::cerr<< e.what() << std::endl;
                    return -10;
                } catch(...) {
                    std::cerr<<"Unknown one";
                    return -20;
                }
            }
        }));
    }
    
    for(auto& thread : threads) {
        thread.join();
    }

    std::cout << "Final:" << counter.get_value() << std::endl;

    return 0;
}