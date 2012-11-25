#ifndef __RWLOCK_WIN32_H
#define __RWLOCK_WIN32_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <windows.h>

FX_NS_DEF(thread);

class RWLockImpl
{
protected:
    RWLockImpl();
    ~RWLockImpl();
    void readLock();
    bool tryReadLock();
    void writeLock();
    bool tryWriteLock();
    void unlock();

    friend class RWLock;
private:
    void addWriter();
    void removeWriter();

    HANDLE   m_hMutex;
    HANDLE   m_hReadEvent;
    HANDLE   m_hWriteEvent;
    uint32_t m_nReaders;
    uint32_t m_nWritersWaiting;
    uint32_t m_nWriters;
};

FX_NS_END

#endif
