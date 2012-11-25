#ifndef __FX_CONCURRENTLINKEDQUEUETESTCASE_H
#define __FX_CONCURRENTLINKEDQUEUETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(utility);

class ConcurrentLinkedQueueTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ConcurrentLinkedQueueTestCase);
    CPPUNIT_TEST(testEnqueue);
    CPPUNIT_TEST(testRemove);
    CPPUNIT_TEST_SUITE_END();
public:
    ConcurrentLinkedQueueTestCase();
    ~ConcurrentLinkedQueueTestCase();

    void setUp();
    void tearDown();

protected:
    void testEnqueue();
    void testRemove();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_CONCURRENTLINKEDQUEUETESTCASE_H
