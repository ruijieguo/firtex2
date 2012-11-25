#include "analyzer/DateTimeAnalyzerTestCase.h"
#include "firtex/analyzer/DateTimeAnalyzer.h"

using namespace std;

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, DateTimeAnalyzerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DateTimeAnalyzerTestCase);

DateTimeAnalyzerTestCase::DateTimeAnalyzerTestCase() 
{
}

DateTimeAnalyzerTestCase::~DateTimeAnalyzerTestCase() 
{
}

void DateTimeAnalyzerTestCase::setUp()
{
}

void DateTimeAnalyzerTestCase::tearDown()
{
}

void DateTimeAnalyzerTestCase::testParse()
{
    CPPUNIT_ASSERT_EQUAL((int64_t)makeTime(2008, 2, 1, 12, 1, 0),
                         DateTimeAnalyzer::parse("2008-02-01T12:01:00Z"));
    CPPUNIT_ASSERT_EQUAL((int64_t)makeTime(2008, 2, 1, 12, 1, 0),
                         DateTimeAnalyzer::parse("Sat, 1 Feb 2008 12:01:00 GMT"));
    CPPUNIT_ASSERT_EQUAL((int64_t)makeTime(2008, 2, 1, 12, 1, 0),
                         DateTimeAnalyzer::parse("Saturday, 1 Feb 2008 12:01:00 GMT"));
    CPPUNIT_ASSERT_EQUAL((int64_t)makeTime(2008, 2, 1, 12, 1, 0),
                         DateTimeAnalyzer::parse("2008/2/1 12:01:00"));
    CPPUNIT_ASSERT_EQUAL((int64_t)makeTime(2008, 2, 1, 12, 1, 0),
                         DateTimeAnalyzer::parse("2008-2-1 12:01:00"));
    CPPUNIT_ASSERT_EQUAL((int64_t)makeTime(2008, 2, 1, 12, 1, 0),
                         DateTimeAnalyzer::parse("Sat Feb 01 12:01:00 2008"));
    CPPUNIT_ASSERT_EQUAL((int64_t)makeTime(2008, 2, 1, 12, 1, 0),
                         DateTimeAnalyzer::parse("2008年2月1日 12点1分00秒"));
}

time_t DateTimeAnalyzerTestCase::makeTime(int32_t y, int32_t m, int32_t d, 
        int32_t h, int32_t min, int32_t s)
{
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(timeinfo));
    timeinfo.tm_year = y - 1900;
    timeinfo.tm_mon = m - 1;
    timeinfo.tm_mday = d;
    timeinfo.tm_hour = h;
    timeinfo.tm_min = min;
    timeinfo.tm_sec = s;
    return mktime(&timeinfo);
}

FX_NS_END

