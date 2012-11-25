#include "search/ClauseTestCase.h"
#include "firtex/search/Clause.h"

using namespace std;

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, ClauseTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(ClauseTestCase);

ClauseTestCase::ClauseTestCase() 
{
}

ClauseTestCase::~ClauseTestCase() 
{
}

void ClauseTestCase::setUp()
{
}

void ClauseTestCase::tearDown()
{
}

void ClauseTestCase::testParseClauseValue()
{
    //single value
    string str = "k1:v1{!s1}";
    checkResult(str, 0, "k1", "v1", "s1", str.size());

    str = "k1:v1";
    checkResult(str, 0, "k1", "v1", "", str.size());
    
    str = "k1";
    checkResult(str, 0, "k1", "", "", str.size());

    str = "k1{!s1}";
    checkResult(str, 0, "k1", "", "s1", str.size());

    //multi value
    str = "k1,k2";
    checkResult(str, 0, "k1", "", "", 3);
    str = "k1:v1,k2:v2";
    checkResult(str, 0, "k1", "v1", "", 6);

    str = "k1:v1{!s1},k2:v2";
    checkResult(str, 0, "k1", "v1", "s1", 11);

    //
    str = "k1:v2:v3";
    checkResult(str, 0, "k1", "v2:v3", "", 8);
 
    str = "k1:v1{!s1}{!s2}";
    checkResult(str, 0, "k1", "v1", "s2", str.size());

    str = "k1,k2:v2{!s2}";
    checkResult(str, 3, "k2", "v2", "s2", str.size());
}

void ClauseTestCase::checkResult(const string& sClause, size_t off, 
                                 const string& sExpKey, const string& sExpValue,
                                 const string& sExpSkip, size_t offAfterParse)
{
    string sKey, sValue, sSkip;
    CPPUNIT_ASSERT_EQUAL(offAfterParse, Clause::parseClauseValue(
                    sKey, sValue, sSkip, sClause, off));
    CPPUNIT_ASSERT_EQUAL(sExpKey, sKey);
    CPPUNIT_ASSERT_EQUAL(sExpValue, sValue);
    CPPUNIT_ASSERT_EQUAL(sExpSkip, sSkip);
}

FX_NS_END

