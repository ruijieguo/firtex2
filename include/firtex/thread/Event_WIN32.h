#ifndef __EVENT_WIN32_H
#define __EVENT_WIN32_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <windows.h>

FX_NS_DEF(thread);

class EventImpl
{
protected:
    EventImpl(bool bAutoReset = false);		
    ~EventImpl();
    void setImpl();
    void waitImpl();
    bool waitImpl(int64_t milliseconds);
    void resetImpl();
	
private:
    HANDLE m_hEvent;
};


//////////////////////////////////////////////////
// inlines
//
inline void EventImpl::setImpl()
{
    if (!SetEvent(m_hEvent))
    {
        FIRTEX_THROW(SystemException, "cannot signal event");
    }
}


inline void EventImpl::resetImpl()
{
    if (!ResetEvent(m_hEvent))
    {
        FIRTEX_THROW(SystemException, "cannot reset event");
    }
}

FX_NS_END

#endif //
