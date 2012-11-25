#include "ThreadPoolTestCase.h"
#include "../src/TestHelper.h"
#include "firtex/thread/ThreadPool.h"
#include "firtex/thread/Condition.h"
#include "firtex/thread/FastMutex.h"

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(thread);

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadPoolTestCase);

class TestRunner : public Runnable
{
public:
    TestRunner() : m_nCount(0) {}
public:
    void run()
    {        
        {
            FastMutex::Guard g(m_mutex);
            m_cond.wait(m_mutex);
        }
        for (int i = 0; i < 10000; ++i)
        {
            m_mutex.lock();
            ++m_nCount;
            m_mutex.unlock();
        }
    }
    size_t count() const {return m_nCount;}
    void setCount(size_t nCount) {m_nCount = nCount;}
    void broadcast() 
    {
        FastMutex::Guard g(m_mutex);
        m_cond.broadcast();
    }

private:
    FastMutex m_mutex;
    Condition m_cond;
    size_t m_nCount;
};

ThreadPoolTestCase::ThreadPoolTestCase(void)
{
}

ThreadPoolTestCase::~ThreadPoolTestCase(void)
{
}

void ThreadPoolTestCase::setUp()
{
}

void ThreadPoolTestCase::tearDown()
{
}

void ThreadPoolTestCase::testThreadPool()
{
    ThreadPool pool(2, 3, 2);
    pool.setStackSize(1);

    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)0, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)3, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)3, pool.numAvailableThreads());
    pool.addCapacity(1);
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)0, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.numAvailableThreads());

    TestRunner runner;
    pool.start(runner);
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)3, pool.numAvailableThreads());

    pool.start(runner);
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAvailableThreads());

    pool.start(runner);
    CPPUNIT_ASSERT_EQUAL((size_t)3, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)3, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pool.numAvailableThreads());

    pool.start(runner);
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)0, pool.numAvailableThreads());

    try
    {
        pool.start(runner);
        CPPUNIT_ASSERT(false);//thread pool exhausted - must throw exception
    }
    catch (const NoThreadAvailableException&)
    {
    }
    catch (...)
    {
        CPPUNIT_ASSERT(false);//thread pool exhausted - must throw exception
    }
    
    runner.broadcast(); // go
    pool.joinAll();
    CPPUNIT_ASSERT_EQUAL((size_t)40000, runner.count());
    
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)0, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.numAvailableThreads());
    
    Thread::sleep(3000);

    pool.collect();
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)0, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.numAvailableThreads());

    runner.setCount(0);
    pool.start(runner);
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)3, pool.numAvailableThreads());

    pool.start(runner);
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pool.numAvailableThreads());

    Thread::sleep(1000);
    runner.broadcast(); 
    pool.stopAll();
    pool.joinAll();

    CPPUNIT_ASSERT_EQUAL((size_t)20000, runner.count());
    
    CPPUNIT_ASSERT_EQUAL((size_t)0, pool.numAllocatedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)0, pool.numUsedThreads());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.capacity());
    CPPUNIT_ASSERT_EQUAL((size_t)4, pool.numAvailableThreads());
}

FX_NS_END
