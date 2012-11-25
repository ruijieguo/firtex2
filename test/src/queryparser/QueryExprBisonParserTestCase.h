#ifndef __FX_BISONGRAMMARPARSERTESTCASE_H
#define __FX_BISONGRAMMARPARSERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include <string>

FX_NS_DEF(queryparser);

class QueryExprBisonParserTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(QueryExprBisonParserTestCase);
    CPPUNIT_TEST(testSimpleQueryExpr);
    CPPUNIT_TEST(testAndQueryExpr);
    CPPUNIT_TEST(testOrQueryExpr);
    CPPUNIT_TEST(testAndOrQueryExpr);
    CPPUNIT_TEST(testDefaultOpQueryExpr);
    CPPUNIT_TEST(testRangeQueryExprWithSingleValue);
    CPPUNIT_TEST(testRangeQueryExpr);
    CPPUNIT_TEST(testRangeQueryExprWithExclusive);
    CPPUNIT_TEST(testRangeQueryExprOneExclusive);
    CPPUNIT_TEST(testRequiredQueryExpr);
    CPPUNIT_TEST(testNotQueryExpr);
    CPPUNIT_TEST(testGroup);
    CPPUNIT_TEST(testFieldGroup);
    CPPUNIT_TEST(testPhraseExpr);
    CPPUNIT_TEST(testPhraseExprWithSlop);
    CPPUNIT_TEST(testSingleQuotedExpr);
    CPPUNIT_TEST_SUITE_END();
public:
    QueryExprBisonParserTestCase();
    ~QueryExprBisonParserTestCase();

    void setUp();
    void tearDown();
private:
    std::string parseExpr(const std::string& exprStr);

protected:
    void testSimpleQueryExpr();
    void testAndQueryExpr();
    void testOrQueryExpr();
    void testAndOrQueryExpr();
    void testDefaultOpQueryExpr();
    void testRangeQueryExprWithSingleValue();
    void testRangeQueryExpr();
    void testRangeQueryExprWithExclusive();
    void testRangeQueryExprOneExclusive();
    void testRequiredQueryExpr();
    void testNotQueryExpr();
    void testGroup();
    void testFieldGroup();
    void testPhraseExpr();
    void testPhraseExprWithSlop();
    void testSingleQuotedExpr();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BISONGRAMMARPARSERTESTCASE_H
