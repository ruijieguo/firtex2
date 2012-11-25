#ifndef __EVENT_POSIX_H
#define __EVENT_POSIX_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <pthread.h>
#include <errno.h>
#include <assert.h>

FX_NS_DEF(thread);

class EventImpl
{
protected:
    EventImpl(bool bAutoReset);		
    ~EventImpl();
    void setImpl();
    void waitImpl();
    bool waitImpl(int64_t milliseconds);
    void resetImpl();
	
private:
    bool            m_bAuto;
    volatile bool   m_bState;
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_cond;
};


///////////////////////////////////////////////
// inlines
//
inline void EventImpl::setImpl()
{
    if (pthread_mutex_lock(&m_mutex))
    {
        FIRTEX_THROW(SystemException, "cannot signal event (lock)");
    }
    m_bState = true;
    if (pthread_cond_broadcast(&m_cond))
    {
        pthread_mutex_unlock(&m_mutex);
        FIRTEX_THROW(SystemException, "cannot signal event");
    }
    pthread_mutex_unlock(&m_mutex);
}

inline void EventImpl::resetImpl()
{
    if (pthread_mutex_lock(&m_mutex))	
    {
        FIRTEX_THROW(SystemException, "cannot reset event");
    }
    m_bState = false;
    pthread_mutex_unlock(&m_mutex);
}

FX_NS_END

#endif 
