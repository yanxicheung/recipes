#pragma once
#include <thread>
#include <functional>
#include <string>
using std::function;
using namespace std::placeholders;

class Thread
{
public:
	typedef function<void()> ThreadFunc;
	explicit Thread(const ThreadFunc& func, const std::string& name = std::string());
	~Thread();
	void start();
	void join();
	void detach();
	bool joinable();
	std::thread::id getId() const;
	Thread(Thread&& rhs);
	Thread& operator=(Thread&& rhs);
public:
	Thread(const Thread&) = delete;
	Thread& operator=(const Thread&) = delete;
private:
	std::thread mThread;
	ThreadFunc mCallback;
	bool mIsStart;
	bool mIsDetach;
	std::string mName;
};
