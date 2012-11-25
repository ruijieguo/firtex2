#include "utility/ConcurrentLinkedQueueTestCase.h"
#include "firtex/utility/ConcurrentLinkedQueue.h"

using namespace std;

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, ConcurrentLinkedQueueTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(ConcurrentLinkedQueueTestCase);

ConcurrentLinkedQueueTestCase::ConcurrentLinkedQueueTestCase() 
{
}

ConcurrentLinkedQueueTestCase::~ConcurrentLinkedQueueTestCase() 
{
}

void ConcurrentLinkedQueueTestCase::setUp()
{
}

void ConcurrentLinkedQueueTestCase::tearDown()
{
}

void ConcurrentLinkedQueueTestCase::testEnqueue()
{
    ConcurrentLinkedQueue<int32_t> q;
    q.enqueue(1);
    q.enqueue(2);
    CPPUNIT_ASSERT_EQUAL((size_t)2, q.size());
    int32_t v = 0;
    bool ret = q.dequeue(v);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL((int32_t)1, v);

    ret = q.dequeue(v);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL((int32_t)2, v);
}

void ConcurrentLinkedQueueTestCase::testRemove()
{
    ConcurrentLinkedQueue<int32_t> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    CPPUNIT_ASSERT_EQUAL((size_t)3, q.size());
    CPPUNIT_ASSERT(q.remove(2));
    CPPUNIT_ASSERT_EQUAL((size_t)2, q.size());

    CPPUNIT_ASSERT(q.remove(3));
    CPPUNIT_ASSERT_EQUAL((size_t)1, q.size());

    CPPUNIT_ASSERT(q.remove(1));
    CPPUNIT_ASSERT_EQUAL((size_t)0, q.size());
}


FX_NS_END

