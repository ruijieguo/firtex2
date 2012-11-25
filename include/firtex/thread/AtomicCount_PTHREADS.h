#ifndef __FXATOMICCOUNT_PTHREADS_H
#define __FXATOMICCOUNT_PTHREADS_H

#include <pthread.h>
#include "firtex/common/StdHeader.h"

FX_NS_DEF(thread);

class AtomicCount
{
private:

    class scoped_lock
    {
    public:

        scoped_lock(pthread_mutex_t & m): m_(m)
        {
            pthread_mutex_lock(&m_);
        }

        ~scoped_lock()
        {
            pthread_mutex_unlock(&m_);
        }

    private:

        pthread_mutex_t & m_;
    };

public:
    AtomicCount() : m_value(0) {}
    explicit AtomicCount(long v): m_value(v)
    {
        pthread_mutex_init(&m_mutex, 0);
    }

    ~AtomicCount()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    long operator++()
    {
        scoped_lock lock(m_mutex);
        return ++m_value;
    }

    long operator--()
    {
        scoped_lock lock(m_mutex);
        return --m_value;
    }

    operator long() const
    {
        scoped_lock lock(m_mutex);
        return m_value;
    }

private:

    AtomicCount(AtomicCount const &);
    AtomicCount & operator=(AtomicCount const &);

    mutable pthread_mutex_t m_mutex;
    long m_value;
};

FX_NS_END

#endif // #ifndef __FXATOMICCOUNT_PTHREADS_H
