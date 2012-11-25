#ifndef __FX_CONDITION_H
#define __FX_CONDITION_H

#include <deque>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/ScopedUnlock.h"
#include "firtex/thread/Event.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(thread);

class Condition
{
public:
    Condition();
    ~Condition();

public:
    /**
     * Unlock the mutex (which must be locked upon calling
     * wait()) and waits until the Condition is signalled.
     *
     * The given mutex will be locked again upon 
     * leaving the function, even in case of an exception.
     */
    template <class Mtx>
    void wait(Mtx& mutex)
    {
        ScopedUnlock<Mtx> unlock(mutex, false);
        Event event;
        {
            FastMutex::Guard lock(m_mutex);
            mutex.unlock();
            enqueue(event);
        }
        event.wait();
    }

    /**
     * Unlock the mutex (which must be locked upon calling
     * wait()) and waits for the given time until the Condition is signalled.
     * The given mutex will be locked again upon successfully leaving the 
     * function, even in case of an exception.
     *
     * @throws TimeoutException if the Condition is not signalled
     * within the given time interval.
     */
    template <class Mtx>
    void wait(Mtx& mutex, long milliseconds)
    {
        if (!tryWait(mutex, milliseconds))
            FIRTEX_THROW(TimeoutException, "timeout");
    }

    /**
     * Unlock the mutex (which must be locked upon calling
     * tryWait()) and waits for the given time until the Condition is signalled.
     *
     * The given mutex will be locked again upon leaving the 
     * function, even in case of an exception.
     *
     * @return return true if the Condition has been signalled
     * within the given time interval, otherwise false.
     */
    template <class Mtx>
    bool tryWait(Mtx& mutex, long milliseconds)
    {
        ScopedUnlock<Mtx> unlock(mutex, false);
        Event event;
        {
            FastMutex::Guard lock(m_mutex);
            mutex.unlock();
            enqueue(event);
        }
        if (!event.tryWait(milliseconds))
        {
            FastMutex::Guard lock(m_mutex);
            dequeue(event);
            return false;
        }
        return true;
    }
    

    /** 
     * Signal the Condition and allows one waiting thread
     * to continue execution.
     */
    void signal();

    /**
     * Signal the Condition and allows all waiting
     * threads to continue their execution.    
     */
    void broadcast();

protected:
    void enqueue(Event& event);
    void dequeue();
    void dequeue(Event& event);
    
private:
    Condition(const Condition&);
    Condition& operator = (const Condition&);
    
    typedef std::deque<Event*> WaitQueue;
    
    FastMutex m_mutex;
    WaitQueue m_waitQueue;
};

FX_NS_END

#endif //__FX_CONDITION_H
