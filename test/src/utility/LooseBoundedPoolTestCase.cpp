#include "utility/LooseBoundedPoolTestCase.h"

using namespace std;

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, LooseBoundedPoolTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(LooseBoundedPoolTestCase);

LooseBoundedPoolTestCase::LooseBoundedPoolTestCase() 
{
}

LooseBoundedPoolTestCase::~LooseBoundedPoolTestCase() 
{
}

void LooseBoundedPoolTestCase::setUp()
{
}

void LooseBoundedPoolTestCase::tearDown()
{
    m_pPool.reset();
}

void LooseBoundedPoolTestCase::testAllocate()
{
    const static size_t POOL_SIZE = 1024;
    m_pPool.assign(new LooseBoundedPool(POOL_SIZE));
    void* p = m_pPool->allocate(512);
    CPPUNIT_ASSERT(p != NULL);
    
    p = m_pPool->allocate(512);
    CPPUNIT_ASSERT(p != NULL);

    CPPUNIT_ASSERT(!m_pPool->hasException());

    p = m_pPool->allocate(51);
    CPPUNIT_ASSERT(p != NULL);

    CPPUNIT_ASSERT(m_pPool->hasException());
}

void LooseBoundedPoolTestCase::testTryAllocate()
{
    const static size_t POOL_SIZE = 1024;
    m_pPool.assign(new LooseBoundedPool(POOL_SIZE));
    void* p = m_pPool->tryAllocate(512);
    CPPUNIT_ASSERT(p != NULL);
    
    p = m_pPool->tryAllocate(512);
    CPPUNIT_ASSERT(p != NULL);

    CPPUNIT_ASSERT(!m_pPool->hasException());

    p = m_pPool->tryAllocate(512);
    CPPUNIT_ASSERT(p == NULL);

    p = m_pPool->allocate(51);
    CPPUNIT_ASSERT(p != NULL);

    CPPUNIT_ASSERT(m_pPool->hasException());
}

FX_NS_END

