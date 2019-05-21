/*
 * Added the screenshot of commit-2
 * Every thread appends to cout without caring about any other thread
 * Hence messed up output due to no control over order of execution and preemption
*/

#include <thread>
#include <iostream>
#include <vector>

void hello(){
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

int main(){
    std::vector<std::thread> threads;

    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(hello));
    }

    for(auto& thread : threads){
    	//thread.detach for running as background thread independently
    	//usually check if thread is joinable
        thread.join();
    }

    return 0;
}