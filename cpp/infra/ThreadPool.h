#pragma once
#include "Thread.h"
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <vector>

class ThreadPool
{
public:
	typedef function<void()> Task;
	explicit ThreadPool(const std::string& name = std::string());
	~ThreadPool();

	void start(int numThreads);
	void stop();

	void run(const Task& task);
private:
	void runInThread();
	Task take();
private:
	std::mutex mMutex;
	std::condition_variable mCond;
	std::string   mPoolName;
	std::vector<Thread>  mThreads;
	std::queue<Task> mTaskQueue;
	bool mRunning;
};