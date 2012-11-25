#include "queryparser/ExprParserTestCase.h"
#include "firtex/queryparser/ExprParser.h"
#include "firtex/search/ScoredDoc.h"
#include "firtex/index/IndexReader.h"
#include "firtex/analyzer/DateTimeAnalyzer.h"
#include <math.h>

using namespace std;

FX_NS_USE(search);
FX_NS_USE(index);
FX_NS_USE(analyzer);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, ExprParserTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(ExprParserTestCase);

ExprParserTestCase::ExprParserTestCase() 
{
}

ExprParserTestCase::~ExprParserTestCase() 
{
}

void ExprParserTestCase::setUp()
{
}

void ExprParserTestCase::tearDown()
{
}

void ExprParserTestCase::parseAndAssertInt32(
        const string& sExpr, int32_t iExpectVal)
{
    IndexReaderPtr pIndexReader;
    ExprParser parser(pIndexReader);
    ExprEvaluatorPtr pEval = parser.parse(sExpr);
    CPPUNIT_ASSERT(pEval.isNotNull());
    ScoredDoc doc(0, 0.0f);
    CPPUNIT_ASSERT_EQUAL(iExpectVal, pEval->evaluateInt32(doc));
}

void ExprParserTestCase::parseAndAssertInt64(
        const string& sExpr, int64_t iExpectVal)
{
    IndexReaderPtr pIndexReader;
    ExprParser parser(pIndexReader);
    ExprEvaluatorPtr pEval = parser.parse(sExpr);
    CPPUNIT_ASSERT(pEval.isNotNull());
    ScoredDoc doc(0, 0.0f);
    CPPUNIT_ASSERT_EQUAL(iExpectVal, pEval->evaluateInt64(doc));
}

void ExprParserTestCase::parseAndAssertFloat(
        const string& sExpr, float fExpectVal)
{
    IndexReaderPtr pIndexReader;
    ExprParser parser(pIndexReader);
    ExprEvaluatorPtr pEval = parser.parse(sExpr);
    CPPUNIT_ASSERT(pEval.isNotNull());
    ScoredDoc doc(0, 0.0f);
    CPPUNIT_ASSERT_EQUAL(fExpectVal, pEval->evaluateFloat(doc));
}

void ExprParserTestCase::parseAndAssertDouble(
        const string& sExpr, double dbExpectVal)
{
    IndexReaderPtr pIndexReader;
    ExprParser parser(pIndexReader);
    ExprEvaluatorPtr pEval = parser.parse(sExpr);
    CPPUNIT_ASSERT(pEval.isNotNull());
    ScoredDoc doc(0, 0.0f);
    CPPUNIT_ASSERT_EQUAL(dbExpectVal, pEval->evaluateDouble(doc));
}

void ExprParserTestCase::testParseIntBinaryExpr()
{
    struct Expr2Ans
    {
        char expr[32];
        int32_t ans;
    };
    Expr2Ans testData[] = 
    {
        {"1 + 2", 3},
        {"1 - 2", -1},
        {"1 * 2", 2},
        {"3 / 2", 1},
        {"3 & 2", 3 & 2},
        {"3 | 2", 3 | 2},
        {"3 < 2", 0},
        {"3 > 2", 1},
        {"3 <= 2", 0},
        {"3 >= 2", 1},
        {"3 = 2", 0},
        {"3 <> 2", 1},
        {"MIN(3, 2)", 2},
        {"MAX(3, 2)", 3},
        {"POW(3, 2)", (int32_t)(pow((float)3, (float)2))},
        {"3 AND 2", 1},
        {"0 AND 2", 0},
        {"3 OR 2", 1},
        {"1 OR 0", 1},
    };

    for (size_t i = 0; i < sizeof(testData)/sizeof(testData[0]); ++i)
    {
        parseAndAssertInt32(testData[i].expr, testData[i].ans);
        parseAndAssertInt64(testData[i].expr, testData[i].ans);
    }
}

void ExprParserTestCase::testParseFloatBinaryExpr()
{
    struct Expr2Ans
    {
        char expr[32];
        float ans;
    };
    Expr2Ans testData[] = 
    {
        {"1.0 + 2.0", 3.0},
        {"1.0 - 2.0", -1.0},
        {"1.0 * 2.0", 2.0},
        {"3.0 / 2.0", 1.5},
        {"3.0 < 2.0", 0},
        {"3.0 > 2.0", 1.0},
        {"3.0 <= 2.0", 0},
        {"3.0 >= 2.0", 1.0},
        {"3.0 = 2.0", 0},
        {"3.0 <> 2.0", 1.0},
        {"MIN(3.0, 2.0)", 2.0},
        {"MAX(3.0, 2.0)", 3.0},
        {"POW(3.0, 2.0)", (float)(pow((float)3.0, (float)2.0))},
    };

    for (size_t i = 0; i < sizeof(testData)/sizeof(testData[0]); ++i)
    {
        parseAndAssertFloat(testData[i].expr, testData[i].ans);
        parseAndAssertDouble(testData[i].expr, (double)testData[i].ans);
    }
}

void ExprParserTestCase::testParseFloatUnaryExpr()
{
    struct Expr2Ans
    {
        char expr[32];
        float ans;
    };
    Expr2Ans testData[] = 
    {
        {"SQRT(4.0)", sqrt(4.0f)},
        {"CEIL(4.0)", ceil(4.0f)},
        {"LN(4.0)", log(4.0f)},
    };

    for (size_t i = 0; i < sizeof(testData)/sizeof(testData[0]); ++i)
    {
        parseAndAssertFloat(testData[i].expr, (float)testData[i].ans);
        parseAndAssertDouble(testData[i].expr, (double)testData[i].ans);
    }
}

void ExprParserTestCase::testComplexExpr()
{
    struct Expr2Ans
    {
        char expr[32];
        double ans;
    };
    Expr2Ans testData[] = 
    {
        {"SQRT(4.0) + 4.0 * POW(2.0, 3.0)", sqrt(4.0) + 4.0 * pow(2.0, 3.0)},
    };

    for (size_t i = 0; i < sizeof(testData)/sizeof(testData[0]); ++i)
    {
        parseAndAssertFloat(testData[i].expr, (float)testData[i].ans);
        parseAndAssertDouble(testData[i].expr, (double)testData[i].ans);
    }
}

void ExprParserTestCase::testParseDateExpr()
{
    string str = "2010-12-28 22:10:10";
    string strDateExpr = "DATE(\'" + str + "\')";
    parseAndAssertInt64(strDateExpr, DateTimeAnalyzer::parse(str));

    strDateExpr = "NOW() > DATE(\'" + str + "\')";    
    parseAndAssertInt64(strDateExpr, 1);
}

FX_NS_END

