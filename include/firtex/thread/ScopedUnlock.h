#ifndef __FX_SCOPEDUNLOCK_H
#define __FX_SCOPEDUNLOCK_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(thread);

template <class T>
class ScopedUnlock
{
public:
    inline ScopedUnlock(T& mutex, bool unlockNow = true): m_mutex(mutex)
    {
        if (unlockNow)
            m_mutex.unlock();
    }
    inline ~ScopedUnlock()
    {
        m_mutex.lock();
    }

private:
    T& m_mutex;

    ScopedUnlock();
    ScopedUnlock(const ScopedUnlock&);
    ScopedUnlock& operator = (const ScopedUnlock&);
};

FX_NS_END

#endif //__FX_SCOPEDUNLOCK_H
