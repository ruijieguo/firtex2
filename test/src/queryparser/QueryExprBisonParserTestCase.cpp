#include "queryparser/QueryExprBisonParserTestCase.h"
#include "firtex/queryparser/QueryExprParser.h"

using namespace std;

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, QueryExprBisonParserTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(QueryExprBisonParserTestCase);

QueryExprBisonParserTestCase::QueryExprBisonParserTestCase() 
{
}

QueryExprBisonParserTestCase::~QueryExprBisonParserTestCase() 
{
}

void QueryExprBisonParserTestCase::setUp()
{
}

void QueryExprBisonParserTestCase::tearDown()
{
}

void QueryExprBisonParserTestCase::testSimpleQueryExpr()
{
    string str = parseExpr("field:single_term");
    CPPUNIT_ASSERT_EQUAL(string("field: single_term"), str);
}

void QueryExprBisonParserTestCase::testAndQueryExpr()
{
    string str = parseExpr("field1:q1 AND field2:q2");
    CPPUNIT_ASSERT_EQUAL(string("<field1: q1> AND <field2: q2>"), str);
}

void QueryExprBisonParserTestCase::testOrQueryExpr()
{
    string str = parseExpr("field1:q1 OR field2:q2");
    CPPUNIT_ASSERT_EQUAL(string("<field1: q1> OR <field2: q2>"), str);
}

void QueryExprBisonParserTestCase::testAndOrQueryExpr()
{
    string str = parseExpr("field1:q1 AND field2:q2 OR field3:q3");
    CPPUNIT_ASSERT_EQUAL(string("<<field1: q1> AND <field2: q2>> OR <field3: q3>"), str);
}

void QueryExprBisonParserTestCase::testRangeQueryExprWithSingleValue()
{
    string str = parseExpr("field1:123");
    CPPUNIT_ASSERT_EQUAL(string("field1: 123"), str);
}

void QueryExprBisonParserTestCase::testRangeQueryExpr()
{
    string str = parseExpr("field1:[123 TO 1000]");
    CPPUNIT_ASSERT_EQUAL(string("field1: [123 TO 1000]"), str);

    str = parseExpr("field1:[abc TO def]");
    CPPUNIT_ASSERT_EQUAL(string("field1: [abc TO def]"), str);
}

void QueryExprBisonParserTestCase::testRangeQueryExprWithExclusive()
{
    string str = parseExpr("field1:{123 TO 1000}");
    CPPUNIT_ASSERT_EQUAL(string("field1: {123 TO 1000}"), str);

    str = parseExpr("field1:{abc TO def}");
    CPPUNIT_ASSERT_EQUAL(string("field1: {abc TO def}"), str);
}

void QueryExprBisonParserTestCase::testRangeQueryExprOneExclusive()
{
    string str = parseExpr("field1:[123 TO 1000}");
    CPPUNIT_ASSERT_EQUAL(string("field1: [123 TO 1000}"), str);

    str = parseExpr("field1:{123 TO 1000]");
    CPPUNIT_ASSERT_EQUAL(string("field1: {123 TO 1000]"), str);

    str = parseExpr("field1:[abc TO def}");
    CPPUNIT_ASSERT_EQUAL(string("field1: [abc TO def}"), str);

    str = parseExpr("field1:{abc TO def]");
    CPPUNIT_ASSERT_EQUAL(string("field1: {abc TO def]"), str);
}

void QueryExprBisonParserTestCase::testRequiredQueryExpr()
{
    string str = parseExpr("+field1:abc");
    CPPUNIT_ASSERT_EQUAL(string("+field1: abc"), str);
}

void QueryExprBisonParserTestCase::testNotQueryExpr()
{
    string str = parseExpr("NOT field1:abc");
    CPPUNIT_ASSERT_EQUAL(string("-field1: abc"), str);

    str = parseExpr("-field1:abc");
    CPPUNIT_ASSERT_EQUAL(string("-field1: abc"), str);
}

void QueryExprBisonParserTestCase::testDefaultOpQueryExpr()
{
    string str = parseExpr("field1:q1 field2:q2");
    CPPUNIT_ASSERT_EQUAL(string("<field1: q1> AND <field2: q2>"), str);
}

void QueryExprBisonParserTestCase::testGroup()
{
    string str = parseExpr("field1:q1 AND (field2:q2 OR field3:q3)");
    CPPUNIT_ASSERT_EQUAL(string("<field1: q1> AND <<field2: q2> OR <field3: q3>>"), str);

    str = parseExpr("(field1:q1 AND field2:q2) OR field3:q3");
    CPPUNIT_ASSERT_EQUAL(string("<<field1: q1> AND <field2: q2>> OR <field3: q3>"), str);

    str = parseExpr("NOT (field1:q1 AND field2:q2) AND field3:q3");
    CPPUNIT_ASSERT_EQUAL(string("<-<<field1: q1> AND <field2: q2>>> AND <field3: q3>"), str);
}

void QueryExprBisonParserTestCase::testFieldGroup()
{
    string str = parseExpr("field1: (q1 q2 q3)");
    CPPUNIT_ASSERT_EQUAL(string("<<field1: q1> AND <field1: q2>> AND <field1: q3>"), str);

    str = parseExpr("field1: (+q1 -q2)");
    CPPUNIT_ASSERT_EQUAL(string("<+field1: q1> AND <-field1: q2>"), str);

    str = parseExpr("field1: (q1 AND q2 OR q3)");
    CPPUNIT_ASSERT_EQUAL(string("<<field1: q1> AND <field1: q2>> OR <field1: q3>"), str);

    str = parseExpr("field1: (q1 AND field2: q2 OR q3)");
    CPPUNIT_ASSERT_EQUAL(string("<<field1: q1> AND <field1: q2>> OR <field1: q3>"), str);
}

void QueryExprBisonParserTestCase::testPhraseExpr()
{
    string str = parseExpr("field1:\"q1 q2 q3\"");
    CPPUNIT_ASSERT_EQUAL(string("field1: \"q1 q2 q3\""), str);
}

void QueryExprBisonParserTestCase::testPhraseExprWithSlop()
{
    string str = parseExpr("field1:\"q1 q2 q3\"~10");
    CPPUNIT_ASSERT_EQUAL(string("field1: \"q1 q2 q3\"~10"), str);
}

void QueryExprBisonParserTestCase::testSingleQuotedExpr()
{
    string str = parseExpr("field1:\'(1+1=2)?$#\'");
    CPPUNIT_ASSERT_EQUAL(string("field1: (1+1=2)?$#"), str);
}

string QueryExprBisonParserTestCase::parseExpr(const string& exprStr)
{
    QueryExprParser parser("def_field");
    QueryExprPtr pExpr = parser.parse(exprStr);
    CPPUNIT_ASSERT(pExpr.isNotNull());
    return pExpr->toString();
}

FX_NS_END

