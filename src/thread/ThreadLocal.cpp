#include "firtex/thread/ThreadLocal.h"
#include "firtex/thread/Thread.h"

FX_NS_DEF(thread);

ThreadLocalStorage::~ThreadLocalStorage()
{
    for (SlotMap::iterator it = m_slots.begin(); it != m_slots.end(); ++it)
    {
        delete it->second;	
    }
}

SlotBase*& ThreadLocalStorage::get(const void* key)
{
    SlotMap::iterator it = m_slots.find(key);
    if (it == m_slots.end())
    {
        return m_slots.insert(SlotMap::value_type(key, 
                        reinterpret_cast<SlotBase*>(0))).first->second;
    }
    else
    {
        return it->second;
    }
}

ThreadLocalStorage& ThreadLocalStorage::current()
{
    Thread* pThread = Thread::current();
    FIRTEX_ASSERT2(pThread);
    return pThread->tls();
}

void ThreadLocalStorage::clear()
{
    Thread* pThread = Thread::current();
    FIRTEX_ASSERT2(pThread);
    pThread->clearTLS();
}

FX_NS_END

