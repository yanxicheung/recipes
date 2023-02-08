#define _CRT_SECURE_NO_WARNINGS
#include "ThreadPool.h"
#include <assert.h>
#include <cstdio>

#ifdef _MSC_VER
#define snprintf  _snprintf 
#endif

ThreadPool::ThreadPool(const std::string& name /*= std::string()*/) :
mPoolName(name), 
mRunning(false)
{

}

ThreadPool::~ThreadPool()
{
	if (mRunning)
	{
		stop();
	}
}

void ThreadPool::start(int numThreads)
{
	assert(mThreads.empty());
	mRunning = true;
	mThreads.reserve(numThreads);
	for (int i = 0; i < numThreads; ++i)
	{
		char id[32];
		::snprintf(id, sizeof id, "%d", i);
		Thread t(std::bind(&ThreadPool::runInThread, this), mPoolName + id);
		mThreads.push_back(std::move(t));
		mThreads[i].start();
	}
}

void ThreadPool::stop()
{
	mRunning = false;
	mCond.notify_all();
	for (auto& mThread : mThreads)
	{
		mThread.join();
	}
}

void ThreadPool::run(const Task& task)
{
	if (mThreads.empty())
	{
		task();
	}
	else
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mTaskQueue.push(task);
		mCond.notify_one();
	}
}

ThreadPool::Task ThreadPool::take()
{
	std::unique_lock<std::mutex> lock(mMutex);
	while (mTaskQueue.empty() && mRunning)
	{
		mCond.wait(lock);
	}
	Task task;
	if (!mTaskQueue.empty())
	{
		task = mTaskQueue.front();
		mTaskQueue.pop();
	}
	return task;
}

void ThreadPool::runInThread()
{
	while (mRunning)
	{
		Task task(take());
		if (task)
		{
			task();
		}
	}
}
