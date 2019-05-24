#include <thread>
#include <stack>
#include <mutex>
#include <iostream>
#include <functional>

/* Bad Interface Design */
template<typename T>
class thread_unsafe_stack
{
	std::stack<T> data;
	mutable std::mutex m;
public:
	thread_unsafe_stack() {}
	thread_unsafe_stack(const thread_unsafe_stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		this->data = other.data;
	}
	thread_unsafe_stack& operator=(const thread_unsafe_stack&) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(this->m);
		this->data.push(new_value);
	}

	T top()
	{
		std::lock_guard<std::mutex> lock(this->m);
		return this->data.top();
	}

	T pop()
	{
		std::lock_guard<std::mutex> lock(this->m);
		T return_value = this->data.top();
		this->data.pop();
		return return_value;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(this->m);
		return this->data.empty();
	}
};

void all_values(thread_unsafe_stack<int>& s)
{
	while(!s.empty())
	{
		const int value = s.top();	//result of empty() could be stale here
		s.pop();		//could be called after 2 tops execute in different threads
		std::cout << "found value " << value << std::endl;
	}
}

int main()
{
	thread_unsafe_stack<int> s;
    for(int i = 0; i < 10; ++i)
    	s.push(i);

	std::vector<std::thread> threads;
    for(int i = 0; i < 5; ++i)
        threads.push_back( std::thread(all_values, std::ref(s)) );
    for(auto& thread : threads)
        thread.join();
    return 0;
}