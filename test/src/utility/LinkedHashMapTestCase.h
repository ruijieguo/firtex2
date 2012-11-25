#ifndef __FX_LINKEDHASHMAPTESTCASE_H
#define __FX_LINKEDHASHMAPTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(utility);

class LinkedHashMapTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(LinkedHashMapTestCase);
    CPPUNIT_TEST(testPut);
    CPPUNIT_TEST(testManyPut);
    CPPUNIT_TEST_SUITE_END();
public:
    LinkedHashMapTestCase();
    ~LinkedHashMapTestCase();

    void setUp();
    void tearDown();

protected:
    void testPut();
    void testManyPut();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_LINKEDHASHMAPTESTCASE_H
