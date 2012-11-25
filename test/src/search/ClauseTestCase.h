#ifndef __FX_CLAUSETESTCASE_H
#define __FX_CLAUSETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(search);

class ClauseTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ClauseTestCase);
    CPPUNIT_TEST(testParseClauseValue);
    CPPUNIT_TEST_SUITE_END();
public:
    ClauseTestCase();
    ~ClauseTestCase();

    void setUp();
    void tearDown();

protected:
    void testParseClauseValue();

    void checkResult(const std::string& sClause, size_t off, 
                     const std::string& sExpKey, const std::string& sExpValue,
                     const std::string& sExpSkip, size_t offAfterParse);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_CLAUSETESTCASE_H
