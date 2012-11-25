#ifndef __THREADPOOLTESTCASE_H
#define __THREADPOOLTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"
#include "firtex/thread/ThreadPool.h"

FX_NS_DEF(thread);

class ThreadPoolTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( ThreadPoolTestCase );
    CPPUNIT_TEST(testThreadPool);
    CPPUNIT_TEST_SUITE_END();
public:
    ThreadPoolTestCase(void);
    ~ThreadPoolTestCase(void);
public:
    void setUp();
    void tearDown();
    void testThreadPool();
private:

};

FX_NS_END

#endif
