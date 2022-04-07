#include "OSS.h"
#include "BlockingQueue.h"
#include "CountDownLatch.h"
#include "Thread.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <assert.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////
class Msg
{
public:
    int eventId_;
    void *data_;
    int len_;
    char instKey_[MAX_INST_KEY_LEN];
public:
    Msg(const char* instKey, int eventId, const void* data, int len)
    : eventId_(eventId), data_(nullptr), len_(0)
    {
        assignInstKey(instKey);
        if (data && len != 0)
        {
            data_ = new char[len];
            assert(data_ != nullptr);
            memcpy(data_, data, len);
            len_ = len;
        }
    }

    Msg(const Msg& rhs)
    {
        data_ = new char[rhs.len_];
        assert(data_ != nullptr);
        eventId_ = rhs.eventId_;
        len_ = rhs.len_;
        memcpy(data_, rhs.data_, rhs.len_);
        assignInstKey(rhs.instKey_);
    }

    Msg(Msg&& rhs)
    : eventId_(rhs.eventId_), data_(rhs.data_), len_(rhs.len_)
    {
        rhs.data_ = nullptr;
        rhs.len_ = 0;
        assignInstKey(rhs.instKey_);
    }

    Msg& operator=(Msg&& rhs)
    {
        if (this != &rhs)
        {
            if (data_)
            {
                char *p = (char*) data_;
                delete[] p;
            }
            assignInstKey(rhs.instKey_);
            len_ = rhs.len_;
            data_ = rhs.data_;
            rhs.len_ = 0;
            rhs.data_ = nullptr;
        }
        return *this;
    }

    Msg& operator=(const Msg& rhs)
    {
        if (this != &rhs)
        {
            Msg tmp(rhs);
            // swap pointer
            void *p = data_;
            data_ = tmp.data_;
            tmp.data_ = p;

            eventId_ = rhs.eventId_;
            len_ = rhs.len_;
            assignInstKey(tmp.instKey_);
        }
        return *this;
    }

    ~Msg()
    {
        char *p = (char*) data_;
        if (p)
        {
            delete[] p;
        }
    }
private:
    void assignInstKey(const char* instKey)
    {
        assert(sizeof(instKey_) >= strlen(instKey) + 1);
        memset(instKey_, 0, sizeof(instKey_));
        strcpy(instKey_, instKey);
    }
};
/////////////////////////////////////////////////////////////////////////

CountDownLatch* g_latch = nullptr;

class Executor: Noncopyable
{
public:
    Executor(Entry entry, const char* key)
    : mutex_(), notEmpty_(mutex_), entry(entry), thread(
    std::bind(&Executor::exec, this)), instKey(key)
    {
        thread.start();
    }

    void addMsg(const Msg& x)  // 在其他线程中调用;
    {
        MutexLockGuard lock(mutex_);
        msgs.push_back(x);
        notEmpty_.notify();
    }

    const string& getKey() const
    {
        return instKey;
    }
private:
    void exec()  // 消息处理线程;
    {
        g_latch->countDown();
        while (true)
        {
            while (msgs.empty())
            {
                notEmpty_.wait();
            }
            const Msg& msg = msgs.front();
            entry(1, msg.eventId_, msg.data_, msg.len_, nullptr);  // todo state
            msgs.pop_front();
        }
    }
private:
    MutexLock mutex_;
    Condition notEmpty_;
    std::deque<Msg> msgs;
private:
    Entry entry;
    Thread thread;
private:
    string instKey;
};

/////////////////////////////////////////////////////////////////////////
class OSS: Noncopyable
{
public:
    OSS()
    : mutex_(), notEmpty_(mutex_), dispatchThread(
    std::bind(&OSS::dispatch, this))
    {
    }

    ~OSS()
    {
        dispatchThread.join();
        for (auto executor : executors)
        {
            delete executor;
        }
    }
public:
    void send(const char *instKey, int eventId, const void* msg, int msgLen) // 在其他线程中调用;
    {
        Msg cMsg(instKey, eventId, msg, msgLen);
        {
            MutexLockGuard lock(mutex_);
            msgs.push_back(move(cMsg));  // 减少不必要拷贝;
            notEmpty_.notify();
        }
    }

    void regist(ThreadConfig* configs, uint16_t size)
    {
        assert((configs != nullptr) && (size != 0));
        g_latch = new CountDownLatch(size);
        for (int i = 0; i < size; ++i)
        {
            auto pExecutor = new Executor(configs[i].entry, configs[i].instKey);
            assert(pExecutor != nullptr);
            executors.push_back(pExecutor);
        }
        dispatchThread.start();
        g_latch->wait();  // 所有工作线程都启动完成后,发送power on 消息;
        poweron();
    }

    void poweron()
    {
        send("all", EV_STARTUP, "hello", 6);
    }

private:
    void dispatch()   // 消息投递线程;
    {
        while (true)
        {
            while (msgs.empty())
            {
                notEmpty_.wait();
            }
            assert(!msgs.empty());
            const Msg& msg = msgs.front();
            for (auto &executor : executors)
            {
                string instKey(msg.instKey_);
                if (instKey == "all" || instKey == executor->getKey())
                {
                    executor->addMsg(msg);
                }
            }
            msgs.pop_front();
        }
    }
private:
    MutexLock mutex_;
    Condition notEmpty_;
    std::deque<Msg> msgs;
private:
    Thread dispatchThread;
private:
    vector<Executor*> executors;
};

/////////////////////////////////////////////////////////////////////////

static OSS oss;

void OSS_Init()
{

}

void OSS_UserEntryRegist(ThreadConfig* configs, uint16_t size)
{
    oss.regist(configs, size);
}

void OSS_Send(const char *instKey, int eventId, const void* msg, int msgLen)
{
    oss.send(instKey, eventId, msg, msgLen);
}
