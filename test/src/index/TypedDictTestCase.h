#ifndef __FX_TYPEDDICTTESTCASE_H
#define __FX_TYPEDDICTTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include <string>

FX_NS_DEF(index);

class TypedDictTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TypedDictTestCase);
    CPPUNIT_TEST(testFind);
    CPPUNIT_TEST(testIterator);
    CPPUNIT_TEST(testRangeIterator);
    CPPUNIT_TEST(testSaveAndLoad);
    CPPUNIT_TEST_SUITE_END();
public:
    TypedDictTestCase();
    ~TypedDictTestCase();

    void setUp();
    void tearDown();

protected:
    void testFind();
    void testIterator();
    void testRangeIterator();
    void testSaveAndLoad();

protected:
    std::string getTestPath();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_TYPEDDICTTESTCASE_H
