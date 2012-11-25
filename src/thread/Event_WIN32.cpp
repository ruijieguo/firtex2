
#include "firtex/thread/Event_WIN32.h"

FX_NS_USE(utility);
FX_NS_DEF(thread);


EventImpl::EventImpl(bool bAutoReset)
{
    m_hEvent = CreateEventW(NULL, bAutoReset ? FALSE : TRUE, FALSE, NULL);
    if (!m_hEvent)
        FIRTEX_THROW(SystemException, "cannot create event");
}

EventImpl::~EventImpl()
{
    CloseHandle(m_hEvent);
}

void EventImpl::waitImpl()
{
    switch (WaitForSingleObject(m_hEvent, INFINITE))
    {
    case WAIT_OBJECT_0:
        return;
    default:
        FIRTEX_THROW(SystemException, "wait for event failed");
    }
}

bool EventImpl::waitImpl(int64_t milliseconds)
{
    switch (WaitForSingleObject(m_hEvent, (long)milliseconds + 1))
    {
    case WAIT_TIMEOUT:
        return false;
    case WAIT_OBJECT_0:
        return true;
    default:
        FIRTEX_THROW(SystemException, "wait for event failed");		
    }
}

FX_NS_END
