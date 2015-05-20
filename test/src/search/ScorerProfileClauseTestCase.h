#ifndef __FX_SCORERPROFILECLAUSETESTCASE_H
#define __FX_SCORERPROFILECLAUSETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(search);

class ScorerProfileClauseTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ScorerProfileClauseTestCase);
    CPPUNIT_TEST(testFromString);
    CPPUNIT_TEST_SUITE_END();
public:
    ScorerProfileClauseTestCase();
    ~ScorerProfileClauseTestCase();

    void setUp();
    void tearDown();

protected:
    void testFromString();

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_SCORERPROFILECLAUSETESTCASE_H
