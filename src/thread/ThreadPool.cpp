#include "firtex/thread/ThreadPool.h"
#include "firtex/thread/Event.h"
#include "firtex/utility/Exception.h"
//#include "firtex/thread/ThreadLocal.h"
#include <sstream>
#include <ctime>

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(thread);

class PooledThread: public Runnable
{
public:
    PooledThread(ThreadPool& pool, const string& sName, size_t nStackSize = 0);
    ~PooledThread();

    void start();
    void start(Thread::Priority priority, Runnable& target);
    void start(Thread::Priority priority, Runnable& target, const string& sName);
    bool idle() const;
    size_t idleTime();
    void join(bool bWaitThreadFinish);
    void activate();
    void release();
    void run();

private:
    volatile bool m_bIdle;
    volatile time_t m_idleTime;
    Runnable* m_pTarget;
    string m_sName;
    Thread m_thread;
    Event m_targetReady;
    Event m_targetCompleted;
    Event m_started;
    FastMutex m_mutex;

    ThreadPool& m_pool;
};

PooledThread::PooledThread(ThreadPool& pool, const string& sName, size_t nStackSize)
    : m_bIdle(true)
    , m_idleTime(0)
    , m_pTarget(0)
    , m_sName(sName)
    , m_thread(sName)
    , m_targetCompleted(false)
    , m_pool(pool)
{
    FIRTEX_ASSERT2(nStackSize >= 0);
    m_thread.setStackSize(nStackSize);
    m_idleTime = time(NULL);
}

PooledThread::~PooledThread()
{
    join(true);
}

void PooledThread::start()
{
    m_thread.start(*this);
    m_started.wait();
}

void PooledThread::start(Thread::Priority priority, Runnable& target)
{
    FastMutex::Guard lock(m_mutex);
	
    FIRTEX_ASSERT2(m_pTarget == 0);

    m_pTarget = &target;
    m_thread.setPriority(priority);
    m_targetReady.set();
}

void PooledThread::start(Thread::Priority priority, Runnable& target, const string& sName)
{
    FastMutex::Guard lock(m_mutex);

    string fullName(sName);
    if (sName.empty())
    {
        fullName = m_sName;
    }
    else
    {
        fullName.append(" (");
        fullName.append(m_sName);
        fullName.append(")");
    }
    m_thread.setName(fullName);
    m_thread.setPriority(priority);
	
    FIRTEX_ASSERT2(m_pTarget == 0);

    m_pTarget = &target;
    m_targetReady.set();
}

inline bool PooledThread::idle() const
{
    return m_bIdle;
}

size_t PooledThread::idleTime()
{
    FastMutex::Guard lock(m_mutex);

    return (size_t) (time(NULL) - m_idleTime);
}

void PooledThread::join(bool bWaitThreadFinish)
{
    m_mutex.lock();
    Runnable* pTarget = m_pTarget;
    m_mutex.unlock();
    if (pTarget || !idle())
    {
        m_targetCompleted.wait();
    }
//     if (bWaitThreadFinish && m_thread.isRunning())
//     {
//         m_thread.join();
//     }
}

void PooledThread::activate()
{
    FastMutex::Guard lock(m_mutex);
    FIRTEX_ASSERT2(m_bIdle);
    m_bIdle = false;
    m_targetCompleted.reset();
}

void PooledThread::release()
{
    m_mutex.lock();
    m_pTarget = NULL;
    m_mutex.unlock();

    m_targetReady.set();
    m_thread.join();

    // In case of a statically allocated thread pool (such
    // as the default thread pool), Windows may have already
    // terminated the thread before we got here.
    // if (m_thread.isRunning()) 
    // {
    //     m_targetReady.set();
    // }
}

void PooledThread::run()
{
    m_started.set();
    for (;;)
    {
        m_targetReady.wait();
        m_mutex.lock();
        if (m_pTarget) // a NULL target means kill yourself
        {
            m_mutex.unlock();
            try
            {
                m_pTarget->run();
            }
            catch (const FirteXException&)
            {
                throw;
            }
            catch (const exception& exc)
            {
                FIRTEX_THROW(RuntimeException, "%s", exc.what());
            }
            catch (...)
            {
                FIRTEX_THROW(RuntimeException, "Unknow exception.");
            }
            FastMutex::Guard lock(m_mutex);
            m_pTarget = NULL;
            m_idleTime = time(NULL);
            m_bIdle = true;
            m_targetCompleted.set();
//	    ThreadLocalStorage::clear();
            m_thread.setName(m_sName);
            m_thread.setPriority(Thread::PRIO_NORMAL);

            m_pool.broadcast();
        }
        else
        {
            m_bIdle = true;
            m_mutex.unlock();
            break;
        }
    }
}

///////////////////////////////////////////////////////
///ThreadPool

ThreadPool::ThreadPool(size_t nMinCapacity, size_t nMaxCapacity,
                       size_t nIdleTime, size_t nStackSize)
    : m_nMinCapacity(nMinCapacity)
    , m_nMaxCapacity(nMaxCapacity)
    , m_nIdleTime(nIdleTime)
    , m_nSerial(0)
    , m_nAge(0)
    , m_nStackSize(nStackSize)
    , m_bStopped(false)
{
    FIRTEX_ASSERT2(nMinCapacity >= 1 && nMaxCapacity >= nMinCapacity && nIdleTime > 0);

    for (size_t i = 0; i < m_nMinCapacity; i++)
    {
        PooledThread* pThread = createThread();
        m_threads.push_back(pThread);
        pThread->start();
    }
}

ThreadPool::ThreadPool(const string& sName, size_t nMinCapacity,
                       size_t nMaxCapacity, size_t nIdleTime,
                       size_t nStackSize)
    : m_sName(sName)
    , m_nMinCapacity(nMinCapacity)
    , m_nMaxCapacity(nMaxCapacity)
    , m_nIdleTime(nIdleTime)
    , m_nSerial(0)
    , m_nAge(0)
    , m_nStackSize(nStackSize)
    , m_bStopped(false)
{
    FIRTEX_ASSERT2 (nMinCapacity >= 1 && nMaxCapacity >= nMinCapacity && nIdleTime > 0);

    for (size_t i = 0; i < m_nMinCapacity; i++)
    {
        PooledThread* pThread = createThread();
        m_threads.push_back(pThread);
        pThread->start();
    }
}

ThreadPool::~ThreadPool()
{
    stopAll();
    joinAll();
}

void ThreadPool::addCapacity(int32_t n)
{
    FastMutex::Guard lock(m_mutex);

    FIRTEX_ASSERT2 ((int32_t)m_nMaxCapacity + n >= (int32_t)m_nMinCapacity);
    m_nMaxCapacity += n;
    housekeep();
}

size_t ThreadPool::capacity() const
{
    FastMutex::Guard lock(m_mutex);
    return m_nMaxCapacity;
}

size_t ThreadPool::numAvailableThreads()
{
    FastMutex::Guard lock(m_mutex);

    size_t count = 0;
    for (ThreadVec::iterator it = m_threads.begin();
         it != m_threads.end(); ++it)
    {
        if ((*it)->idle()) ++count;
    }
    return (size_t) (count + m_nMaxCapacity - m_threads.size());
}

size_t ThreadPool::numUsedThreads()
{
    FastMutex::Guard lock(m_mutex);

    size_t count = 0;
    for (ThreadVec::const_iterator it = m_threads.begin();
         it != m_threads.end(); ++it)
    {
        if (!(*it)->idle()) ++count;
    }
    return count;
}

size_t ThreadPool::numAllocatedThreads() const
{
    FastMutex::Guard lock(m_mutex);

    return size_t(m_threads.size());
}

void ThreadPool::start(Runnable& target, bool bBlock)
{
    getThread(bBlock)->start(Thread::PRIO_NORMAL, target);
} 

void ThreadPool::start(Runnable& target, const string& sName)
{
    getThread(false)->start(Thread::PRIO_NORMAL, target, sName);
}

void ThreadPool::startWithPriority(Thread::Priority priority, Runnable& target)
{
    getThread(false)->start(priority, target);
}

void ThreadPool::startWithPriority(Thread::Priority priority, Runnable& target, const string& sName)
{
    getThread(false)->start(priority, target, sName);
}

void ThreadPool::stopAll()
{
    FastMutex::Guard lock(m_mutex);

    for (ThreadVec::iterator it = m_threads.begin();
         it != m_threads.end(); ++it)
    {
        (*it)->release();
    }
    m_bStopped = true;
}

void ThreadPool::joinAll()
{
    FastMutex::Guard lock(m_mutex);

    for (ThreadVec::iterator it = m_threads.begin(); 
         it != m_threads.end(); ++it)
    {
        (*it)->join(m_bStopped);
    }
    if (m_bStopped)
    {
        m_threads.clear();
    }
    else
    {
        housekeep();
    }
}

void ThreadPool::collect()
{
    FastMutex::Guard lock(m_mutex);
    housekeep();
}

void ThreadPool::housekeep()
{
    m_nAge = 0;
    if (m_threads.size() <= m_nMinCapacity)
        return;

    ThreadVec idleThreads;
    ThreadVec expiredThreads;
    ThreadVec activeThreads;
    idleThreads.reserve(m_threads.size());
    activeThreads.reserve(m_threads.size());
	
    for (ThreadVec::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
    {
        if ((*it)->idle())
        {
            if ((*it)->idleTime() < m_nIdleTime)
                idleThreads.push_back(*it);
            else 
                expiredThreads.push_back(*it);	
        }
        else activeThreads.push_back(*it);
    }
    size_t n = (size_t) activeThreads.size();
    size_t limit = (size_t) idleThreads.size() + n;
    if (limit < m_nMinCapacity) limit = m_nMinCapacity;
    idleThreads.insert(idleThreads.end(), expiredThreads.begin(), expiredThreads.end());
    m_threads.clear();

    for (ThreadVec::iterator it = idleThreads.begin(); it != idleThreads.end(); ++it)
    {
        if (n < limit)
        {
            m_threads.push_back(*it);
            ++n;
        }
        else (*it)->release();
    }
    m_threads.insert(m_threads.end(), activeThreads.begin(), activeThreads.end());
}

PooledThreadPtr ThreadPool::getThread(bool bBlock)
{
    FastMutex::Guard lock(m_mutex);

    if (++m_nAge == 32)
        housekeep();

    do 
    {
        PooledThreadPtr pThread;
        for (ThreadVec::iterator it = m_threads.begin();
             !pThread && it != m_threads.end(); ++it)
        {
            if ((*it)->idle())
            {
                pThread = *it;
            }
        }

        if (pThread.isNull())
        {
            if (m_threads.size() < m_nMaxCapacity)
            {
                pThread = createThread();
                m_threads.push_back(pThread);
                pThread->start();
            }
            else
            {
                if(bBlock)
                {
                    FastMutex::Guard lock2(m_condMutex);
                    m_cond.wait(m_condMutex);
                    continue;
                }
                else
                {
                    FIRTEX_THROW(NoThreadAvailableException, "No thread available");
                }
            }
        }
        pThread->activate();
        return pThread;
    } while (bBlock);

    FIRTEX_THROW(NoThreadAvailableException, "No thread available");
    return PooledThreadPtr();
}

void ThreadPool::broadcast()
{
    FastMutex::Guard lock(m_condMutex);
    m_cond.broadcast();
}

PooledThread* ThreadPool::createThread()
{
    ostringstream name;
    name << m_sName << "[#" << ++m_nSerial << "]";
    return new PooledThread(*this, name.str(), m_nStackSize);
}

class ThreadPoolSingletonHolder
{
public:
    ThreadPoolSingletonHolder()
    {
        m_pPool = 0;
    }
    ~ThreadPoolSingletonHolder()
    {
        delete m_pPool;
    }
    ThreadPool* pool()
    {
        FastMutex::Guard lock(m_mutex);
		
        if (!m_pPool)
        {
            m_pPool = new ThreadPool("default");
        }
        return m_pPool;
    }
	
private:
    ThreadPool* m_pPool;
    FastMutex m_mutex;
};

ThreadPool& ThreadPool::defaultPool()
{
    static ThreadPoolSingletonHolder sh;
    return *sh.pool();
}

FX_NS_END
