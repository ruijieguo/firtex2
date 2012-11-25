#ifndef __FX_EXPRPARSERTESTCASE_H
#define __FX_EXPRPARSERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(queryparser);

class ExprParserTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ExprParserTestCase);
    CPPUNIT_TEST(testParseIntBinaryExpr);
    CPPUNIT_TEST(testParseFloatBinaryExpr);
    CPPUNIT_TEST(testParseFloatUnaryExpr);
    CPPUNIT_TEST(testComplexExpr);
    CPPUNIT_TEST(testParseDateExpr);
    CPPUNIT_TEST_SUITE_END();
public:
    ExprParserTestCase();
    ~ExprParserTestCase();

    void setUp();
    void tearDown();

protected:
    void testParseIntBinaryExpr();
    void testParseFloatBinaryExpr();
    void testParseFloatUnaryExpr();
    void testComplexExpr();
    void testParseDateExpr();

protected:
    void parseAndAssertInt32(const std::string& sExpr, int32_t iExpectVal);
    void parseAndAssertInt64(const std::string& sExpr, int64_t iExpectVal);
    void parseAndAssertFloat(const std::string& sExpr, float fExpectVal);
    void parseAndAssertDouble(const std::string& sExpr, double fExpectVal);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_EXPRPARSERTESTCASE_H
