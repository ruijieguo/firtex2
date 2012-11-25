#ifndef __FX_POSSKIPLISTREADERTESTCASE_H
#define __FX_POSSKIPLISTREADERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class PosSkipListReaderTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(PosSkipListReaderTestCase);
    CPPUNIT_TEST(testSkipToWithTwoLevel);
    CPPUNIT_TEST(testSkipToWithSpecialSequence);
    CPPUNIT_TEST(testSkipTo);
    CPPUNIT_TEST(testSkiptoWithThreeLevel);
    CPPUNIT_TEST_SUITE_END();
public:
    PosSkipListReaderTestCase();
    ~PosSkipListReaderTestCase();

    void setUp();
    void tearDown();

protected:
    void testSkipToWithTwoLevel();
    void testSkipToWithSpecialSequence();
    void testSkipTo();
    void testSkiptoWithThreeLevel();

    std::string getTestPath();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_POSSKIPLISTREADERTESTCASE_H
