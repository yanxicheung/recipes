#include "Thread.h"
#include <assert.h>

Thread::Thread(const ThreadFunc& func)
: func_(func), threadId_(0), started_(false), joined_(false)
{

}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        pthread_detach(threadId_);
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    pthread_create(&threadId_, NULL, threadRoutine, this);
}

void Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    pthread_join(threadId_, NULL);
}

void* Thread::threadRoutine(void* arg)
{
    Thread* thread = static_cast<Thread*>(arg);
    thread->run();
    return NULL;
}

void Thread::run()
{
    func_();
}
