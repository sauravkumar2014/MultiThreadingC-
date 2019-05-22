#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#include <exception>

struct Complex {
    std::mutex mutex;
    int i;

    Complex() : i(0) {}

    void mul(int x){
        std::lock_guard<std::mutex> lock(mutex);
        i *= x;
    }

    void div(int x){
        std::lock_guard<std::mutex> lock(mutex);
        i /= x;
    }

    void both(int x, int y){
	    std::lock_guard<std::mutex> lock(mutex);
	    //lock held by above statment, can't be held by mul/div
	    mul(x);
	    div(y);
	}
};

int main(){
    Complex complex;

    //Never Terminates
    complex.both(32, 23);

    return 0;
}