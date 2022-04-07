#ifndef H05849463_A3C1_4D96_9226_E282F02AE3CA
#define H05849463_A3C1_4D96_9226_E282F02AE3CA

#include <pthread.h>
#include <functional>
#include "Noncopyable.h"
using std::function;
using std::placeholders::_1;

class Thread: Noncopyable
{
public:
    typedef function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc& func);
    ~Thread();
    void start();
    void join();
private:
    static void* threadRoutine(void* arg);
    void run();
    ThreadFunc func_;
    pthread_t threadId_;
    bool started_;
    bool joined_;
};

#endif /* H05849463_A3C1_4D96_9226_E282F02AE3CA */
