#include "firtex/thread/Thread.h"
#include "firtex/thread/Mutex.h"
#include "firtex/utility/Exception.h"
#include "firtex/thread/FastMutex.h"
#include <set>
//#include "firtex/utility/ThreadLocal.h"
#include <sstream>


#if defined(FX_WINDOWS)
#include "Thread_WIN32.cpp"
#else
#include "Thread_POSIX.cpp"
#endif

FX_NS_USE(utility);
FX_NS_DEF(thread);

class ThreadRefer : public FX_NS(utility)::Singleton<ThreadRefer>
{
public:
    ThreadRefer(){}
    ~ThreadRefer() 
    {
        for (ThreadSet::iterator it = m_threads.begin();
             it != m_threads.end(); ++it)
        {
            delete *it;
        }
        m_threads.clear();
    }

    void add(Thread* pThread)
    {
        FastMutex::Guard g(m_mutex);
        m_threads.insert(pThread);
    }

private:
    typedef std::set<Thread*> ThreadSet;
    
    FastMutex m_mutex;
    ThreadSet m_threads;
};

Thread::Thread()
    : m_nId(uniqueId())
    , m_sName(makeName())
    , m_pTLS(NULL)
{
}

Thread::Thread(const std::string& name)
    : m_nId(uniqueId())
    , m_sName(name)
    , m_pTLS(NULL)
{
}

Thread::~Thread()
{
    clearTLS();
}

void Thread::setPriority(Priority prio)
{
    setPriorityImpl(prio);
}

Thread::Priority Thread::getPriority() const
{
    return Priority(getPriorityImpl());
}

void Thread::start(Runnable& target)
{
    startImpl(target);
}

void Thread::join()
{
    joinImpl();
}

void Thread::join(long milliseconds)
{
    if (!joinImpl(milliseconds))
    {
        FIRTEX_THROW(TimeoutException, "Join timeout");
    }
}

bool Thread::tryJoin(long milliseconds)
{
    return joinImpl(milliseconds);
}

ThreadLocalStorage& Thread::tls()
{
    if (!m_pTLS)
    {
        m_pTLS = new ThreadLocalStorage();
    }
    return *m_pTLS;
}

void Thread::clearTLS()
{
    if (m_pTLS)
    {
        delete m_pTLS;
        m_pTLS = NULL;
    }
}

std::string Thread::makeName()
{
    std::ostringstream name;
    name << '#' << m_nId;
    return name.str();
}


int Thread::uniqueId()
{
    static FastMutex mtx;
    FastMutex::Guard lock(mtx);

    static unsigned count = 0;
    ++count;
    return count;
}


void Thread::setName(const std::string& name)
{
    FastMutex::Guard lock(m_mutex);

    m_sName = name;
}

Thread* Thread::current()
{
    ThreadImpl* pImp = currentImpl();
    if (pImp)
    {
        return static_cast<Thread*>(currentImpl());
    }
    else
    {
        Thread* pThread = new Thread();
        m_threadMap.set(pThread);
        ThreadRefer::instance()->add(pThread);
        return pThread;
    }
}


FX_NS_END
