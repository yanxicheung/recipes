#ifndef HE4A072AC_1377_4974_BC28_767F56BE3B1A
#define HE4A072AC_1377_4974_BC28_767F56BE3B1A

#include <Noncopyable.h>

#include <assert.h>
#include <pthread.h>

class MutexLock: Noncopyable
{
public:
    MutexLock()
    {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~MutexLock()
    {
        pthread_mutex_destroy(&mutex_);
    }

    // internal usage

    void lock()
    {
        pthread_mutex_lock(&mutex_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex() /* non-const */
    {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_;
};

class MutexLockGuard: Noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
    : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:

    MutexLock& mutex_;
};

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif /* HE4A072AC_1377_4974_BC28_767F56BE3B1A */
