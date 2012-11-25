#ifndef __FX_LOOSEBOUNDEDPOOLTESTCASE_H
#define __FX_LOOSEBOUNDEDPOOLTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/LooseBoundedPool.h"

FX_NS_DEF(utility);

class LooseBoundedPoolTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(LooseBoundedPoolTestCase);
    CPPUNIT_TEST(testAllocate);
    CPPUNIT_TEST(testTryAllocate);
    CPPUNIT_TEST_SUITE_END();
public:
    LooseBoundedPoolTestCase();
    ~LooseBoundedPoolTestCase();

    void setUp();
    void tearDown();

protected:
    void testAllocate();
    void testTryAllocate();

private:
    LooseBoundedPoolPtr m_pPool;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_LOOSEBOUNDEDPOOLTESTCASE_H
