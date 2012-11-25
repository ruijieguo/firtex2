#ifndef __FXEVENT_H
#define __FXEVENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/NonCopyable.h"

#if defined(FX_WINDOWS)
#include "firtex/thread/Event_WIN32.h"
#else
#include "firtex/thread/Event_POSIX.h"
#endif

FX_NS_DEF(thread);

/**
 * @class Event
 *
 * An Event is a synchronization object that allows one 
 * thread to signal one or more other threads that a 
 * certain event has happened.
 *
 */
class Event : private EventImpl, private FX_NS(utility)::NonCopyable
{
public:

    /**
     * Creates the event. 
     * @param bAutoReset if true, the event is automatically reset 
     *        after wait() successfully returns.
     */
    Event(bool bAutoReset = true);
    ~Event();

    /** 
     * Signals the event. If bAutoReset is true in constructor,
     * only one thread waiting for the event can resume execution.
     * If bAutoReset is false, all waiting threads can resume execution.
     */
    void set();
    
    /**
     * Waits for the event to become signalled.
     */
    void wait();
    
    /**
     * Waits for the event to become signalled.
     * @param milliseconds time to wait
     * @exception Throw a TimeoutException if the event
     *            does not become signalled within the specified
     *            time interval.
     */
    void wait(long milliseconds);
    
    /**
     * Waits for the event to become signalled.
     * @param milliseconds time to wait
     * @return Return true if the event became signalled within 
     *         the specified time interval, false otherwise.
     */
    bool tryWait(int64_t milliseconds);
    

    /**
     * Resets the event to unsignalled state.
     */
    void reset();
};

///////////////////////////////////////////
// inlines
//
inline void Event::set()
{
    setImpl();
}

inline void Event::wait()
{
    waitImpl();
}

inline void Event::wait(long milliseconds)
{
    if (!waitImpl(milliseconds))
        FIRTEX_THROW(TimeoutException, "wait time out.");
}

inline bool Event::tryWait(int64_t milliseconds)
{
    return waitImpl(milliseconds);
}

inline void Event::reset()
{
    resetImpl();
}

FX_NS_END

#endif 
