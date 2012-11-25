#ifndef __FX_DOCSKIPLISTREADERTESTCASE_H
#define __FX_DOCSKIPLISTREADERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class DocSkipListReaderTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DocSkipListReaderTestCase);
    CPPUNIT_TEST(testSkipToWithTwoLevel);
    CPPUNIT_TEST(testSkipToWithThreeLevel);
    CPPUNIT_TEST(testSkipTo);
    CPPUNIT_TEST(testSkipToWithSpecialSequence);
    CPPUNIT_TEST_SUITE_END();
public:
    DocSkipListReaderTestCase();
    ~DocSkipListReaderTestCase();

    void setUp();
    void tearDown();

protected:
    void testSkipToWithTwoLevel();
    void testSkipToWithThreeLevel();
    void testSkipTo();
    void testSkipToWithSpecialSequence();

protected:
    std::string getTestPath();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DOCSKIPLISTREADERTESTCASE_H
