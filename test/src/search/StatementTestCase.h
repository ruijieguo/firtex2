#ifndef __FX_STATEMENTTESTCASE_H
#define __FX_STATEMENTTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(search);

class StatementTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(StatementTestCase);
    CPPUNIT_TEST(testParseAllClause);
    CPPUNIT_TEST(testParseClauseWithSkipBlock);
    CPPUNIT_TEST(testParseAllClauseWithSkipBlock);
    CPPUNIT_TEST(testParseClauseWithSeperatorTail);
    CPPUNIT_TEST(testToString);
    CPPUNIT_TEST_SUITE_END();
public:
    StatementTestCase();
    ~StatementTestCase();

    void setUp();
    void tearDown();

protected:
    void testParseAllClause();
    void testParseClauseWithSkipBlock();
    void testParseAllClauseWithSkipBlock();
    void testParseClauseWithSeperatorTail();
    void testToString();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_STATEMENTTESTCASE_H
