#include "RegexTestCase.h"
#include "firtex/extension/util/Regex.h"

using namespace std;

FX_NS_DEF(util);

SETUP_LOGGER(NONE, RegexTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(RegexTestCase);

RegexTestCase::RegexTestCase() 
{
}

RegexTestCase::~RegexTestCase() 
{
}

void RegexTestCase::setUp()
{
}

void RegexTestCase::tearDown()
{
}

void RegexTestCase::testMatch()
{
    Regex regex;
    CPPUNIT_ASSERT(regex.compile((const char*)"^/_test"));
    CPPUNIT_ASSERT(regex.match("/_test") == Regex::MATCH_OK);

    CPPUNIT_ASSERT(regex.compile("^/([\\w,]{1,})(/[\\w,]{1,}){0,1}/_search$"));
    CPPUNIT_ASSERT(regex.match("/example/type1/_search") == Regex::MATCH_OK);
}

FX_NS_END

