#ifndef __SCOPEDLOCK_H
#define __SCOPEDLOCK_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(thread);

/**
 * @class ScopedLock
 * 
 * A class that simplifies thread synchronization with a mutex.
 * The constructor accepts a Mutex and locks it.
 * The destructor unlocks the mutex.
 */

template <class M>
class ScopedLock
{
public:
    inline ScopedLock(M& mutex)
        : m_mutex(mutex)
        , m_bUnlocked(false)
    {
        m_mutex.lock();
    }

    inline ~ScopedLock()
    {
        unlock();
    }

    void lock()
    {
        if (m_bUnlocked)
        {
            m_mutex.lock();
            m_bUnlocked = false;
        }
    }

    void unlock()
    {
        if (!m_bUnlocked)
        {
            m_mutex.unlock();
            m_bUnlocked = true;
        }
    }

private:
    M& m_mutex;
    bool m_bUnlocked;

    ScopedLock();
    ScopedLock(const ScopedLock&);
    ScopedLock& operator = (const ScopedLock&);
};

FX_NS_END

#endif
