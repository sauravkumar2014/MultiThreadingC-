#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#include <exception>

struct Complex {
	//can be reacquired by the same thread
    static std::timed_mutex mutex;
    static float i;

    void mul(int x){
        std::lock_guard<std::timed_mutex> lock(mutex);
        i *= x;
    }

    void div(int x){
        std::lock_guard<std::timed_mutex> lock(mutex);
        i /= x;
    }

    void both(int x, int y){
	    std::lock_guard<std::timed_mutex> lock(mutex);
	    mul(x);
	    div(y);
	}

    void acquire_and_work() {
        
    }
};

Complex::i = 0;

int main(){
    Complex complex;
    complex.both(32, 23);

    return 0;
}