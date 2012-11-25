#include "firtex/thread/Event_POSIX.h"
#include "firtex/utility/Exception.h"
#include <sys/time.h>

FX_NS_USE(utility);
FX_NS_DEF(thread);

EventImpl::EventImpl(bool bAutoReset): m_bAuto(bAutoReset), m_bState(false)
{
    if (pthread_mutex_init(&m_mutex, NULL))
        FIRTEX_THROW(SystemException, "cannot create event (mutex)");
    if (pthread_cond_init(&m_cond, NULL))
        FIRTEX_THROW(SystemException, "cannot create event (condition)");
}

EventImpl::~EventImpl()
{
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

void EventImpl::waitImpl()
{
    if (pthread_mutex_lock(&m_mutex))
    {
        FIRTEX_THROW(SystemException, "wait for event failed (lock)"); 
    }
    while (!m_bState) 
    {
        if (pthread_cond_wait(&m_cond, &m_mutex))
        {
            pthread_mutex_unlock(&m_mutex);
            FIRTEX_THROW(SystemException, "wait for event failed");
        }
    }
    if (m_bAuto)
        m_bState = false;
    pthread_mutex_unlock(&m_mutex);
}

bool EventImpl::waitImpl(int64_t milliseconds)
{
    int rc = 0;
    struct timespec abstime;

#if defined(__VMS)
    struct timespec delta;
    delta.tv_sec  = milliseconds / 1000;
    delta.tv_nsec = (milliseconds % 1000)*1000000;
    pthread_get_expiration_np(&delta, &abstime);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
    abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
    if (abstime.tv_nsec >= 1000000000)
    {
        abstime.tv_nsec -= 1000000000;
        abstime.tv_sec++;
    }
#endif

    if (pthread_mutex_lock(&m_mutex) != 0)
        FIRTEX_THROW(SystemException, "wait for event failed (lock)"); 
    while (!m_bState) 
    {
        if ((rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
        {
            if (rc == ETIMEDOUT) break;
            pthread_mutex_unlock(&m_mutex);
            FIRTEX_THROW(SystemException, "cannot wait for event");
        }
    }
    if (rc == 0 && m_bAuto) m_bState = false;
    pthread_mutex_unlock(&m_mutex);
    return rc == 0;
}

FX_NS_END
