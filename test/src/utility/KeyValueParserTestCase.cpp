#include "KeyValueParserTestCase.h"
#include "firtex/utility/KeyValueParser.h"
#include <stdio.h>
#include "TestHelper.h"


FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(KeyValueParserTestCase);

KeyValueParserTestCase::KeyValueParserTestCase(void)
{	
}

KeyValueParserTestCase::~KeyValueParserTestCase(void)
{	
}


void KeyValueParserTestCase::setUp()
{	
}

void KeyValueParserTestCase::tearDown()
{	
}

void KeyValueParserTestCase::testParseKeyValue()
{
    KeyValueParser pp;
    CPPUNIT_ASSERT(pp.parse(_T("key1=value1;Key2=Value2")));
    CPPUNIT_ASSERT_EQUAL((size_t)2, pp.getKVCount());
    tstring value1;
    CPPUNIT_ASSERT(pp.getValue(_T("key1"), value1));
    CPPUNIT_ASSERT_EQUAL(tstring("value1"), value1);
    tstring value2;
    CPPUNIT_ASSERT(pp.getValue(_T("key2"), value2));
    CPPUNIT_ASSERT_EQUAL(tstring("Value2"), value2);
}

void KeyValueParserTestCase::testParseValue()
{
    KeyValueParser pp;
    CPPUNIT_ASSERT(pp.parseValue(_T("value1;Value2")));
    CPPUNIT_ASSERT_EQUAL((size_t)2, pp.getKVCount());
    tstring value1;
    CPPUNIT_ASSERT(pp.getValue(0, value1));
    CPPUNIT_ASSERT_EQUAL(tstring("value1"), value1);
    tstring value2;
    CPPUNIT_ASSERT(pp.getValue(1, value2));
    CPPUNIT_ASSERT_EQUAL(tstring("Value2"), value2);
}

FX_NS_END
