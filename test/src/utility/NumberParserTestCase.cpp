#include "NumberParserTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/File.h"
#include <stdio.h>
#include "TestHelper.h"

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(NumberParserTestCase);

NumberParserTestCase::NumberParserTestCase(void)
{	
}

NumberParserTestCase::~NumberParserTestCase(void)
{	
}

void NumberParserTestCase::setUp()
{	
}

void NumberParserTestCase::tearDown()
{	
}

void NumberParserTestCase::testParseInt32()
{
    int32_t value = NumberParser::parseInt32("1");
    CPPUNIT_ASSERT_EQUAL((int32_t)1, value);
    bool ret = NumberParser::tryParseInt32("1abc", value);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL((int32_t)1, value);
}

void NumberParserTestCase::testParseInt64()
{
    int64_t value = NumberParser::parseInt64("10000");
    CPPUNIT_ASSERT_EQUAL((int64_t)10000, value);
    bool ret = NumberParser::tryParseInt64("1000abc", value);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL((int64_t)1000, value);
}

FX_NS_END
