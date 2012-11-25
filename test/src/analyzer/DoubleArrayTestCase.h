#ifndef __FX_DOUBLEARRAYTESTCASE_H
#define __FX_DOUBLEARRAYTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(analyzer);

class DoubleArrayTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DoubleArrayTestCase);
    CPPUNIT_TEST(testInsert);
    CPPUNIT_TEST(testMaxForwardSearch);
    CPPUNIT_TEST(testTraversal);
    CPPUNIT_TEST_SUITE_END();

public:
    DoubleArrayTestCase();
    ~DoubleArrayTestCase();

    void setUp();
    void tearDown();

protected:
    void testInsert();
    void testMaxForwardSearch();
    void testTraversal();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DOUBLEARRAYTESTCASE_H
