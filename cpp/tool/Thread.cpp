#include "Thread.h"

#ifdef _LINUX_
#include <pthread.h>
#endif

static void setThreadName(std::thread* pThread, const char*threadName)
{
#ifdef _LINUX_
	auto handle = pThread->native_handle();
	pthread_setname_np(handle, threadName);
#endif
}

Thread::Thread(const ThreadFunc& func, const std::string& name) :
mCallback(func), 
mIsStart(false), 
mIsDetach(false),
mName(name)
{

}

Thread::Thread(Thread&& rhs)
{
	// assign
	mCallback = rhs.mCallback;
	mIsStart = rhs.mIsStart;
	mIsDetach = rhs.mIsDetach;
	mName = rhs.mName;
	mThread = std::move(rhs.mThread);

	////reset
	rhs.mCallback = ThreadFunc();
	rhs.mIsDetach = false;
	rhs.mIsStart = false;
	rhs.mName = std::string();
}

Thread& Thread::operator=(Thread&& rhs)
{
	if (this != &rhs)
	{
		// assign
		mCallback = rhs.mCallback;
		mIsStart = rhs.mIsStart;
		mIsDetach = rhs.mIsDetach;
		mName = rhs.mName;
		mThread = std::move(rhs.mThread);

		////reset
		rhs.mCallback = ThreadFunc();
		rhs.mIsDetach = false;
		rhs.mIsStart = false;
		rhs.mName = std::string();
	}
	return *this;
}

Thread::~Thread()
{
	if (mIsStart && mIsDetach == false)
		detach();
}

void Thread::start()
{
	if (mIsStart)
		return;

	mThread = std::thread(mCallback);  // ÒÆ¶¯¸³Öµº¯Êý
	setThreadName(&mThread, mName.c_str());
	mIsStart = true;
}

void Thread::join()
{
	if (mIsStart != true || mIsDetach == true)
		return;

	mThread.join();
}

void Thread::detach()
{
	if (mIsStart == false)
		return;

	if (mIsDetach)
		return;

	mThread.detach();
}

bool Thread::joinable()
{
	return mThread.joinable();
}

std::thread::id Thread::getId() const
{
	return mThread.get_id();
}

