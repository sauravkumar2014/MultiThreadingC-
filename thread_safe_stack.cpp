#include <thread>
#include <stack>
#include <mutex>
#include <iostream>
#include <functional>
#include <exception>
#include <memory>

class empty_stack: std::exception
{
public:
	const char* what() const throw() { return "Not Empty Stack"; }
};

template<typename T>
class thread_safe_stack
{
	std::stack<T> data;
	mutable std::mutex m;
public:
	thread_safe_stack() {}
	thread_safe_stack(const thread_safe_stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		this->data = other.data;
	}
	thread_safe_stack& operator=(const thread_safe_stack&) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(this->m);
		this->data.push(new_value);
	}

	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(this->m);
		if(this->data.empty()) throw empty_stack();
		const std::shared_ptr<T> res(std::make_shared<T>(data.top()));
		this->data.pop();
		return res;
	}

	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(this->m);
		if(this->data.empty()) throw empty_stack();
		value = this->data.top();
		this->data.pop();
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(this->m);
		return this->data.empty();
	}
};

void all_values(thread_safe_stack<int>& s)
{
	while(!s.empty())
	{
		int value;
		try {
			s.pop(value);	//Interface takes care of consecutive top and pop
		} catch(std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		std::cout << "found value " << value << std::endl;
	}
}

int main()
{
	thread_safe_stack<int> s;
    for(int i = 0; i < 10; ++i)
    	s.push(i);

	std::vector<std::thread> threads;
    for(int i = 0; i < 5; ++i)
        threads.push_back( std::thread(all_values, std::ref(s)) );
    for(auto& thread : threads)
        thread.join();
    return 0;
}