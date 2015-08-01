#ifndef __FX_REGEXTESTCASE_H
#define __FX_REGEXTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(util);

class RegexTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(RegexTestCase);
    CPPUNIT_TEST(testMatch);
    CPPUNIT_TEST_SUITE_END();
public:
    RegexTestCase();
    ~RegexTestCase();

    void setUp();
    void tearDown();

protected:
    void testMatch();

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_REGEXTESTCASE_H
