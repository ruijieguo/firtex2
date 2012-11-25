#ifndef __KEYVALUEPARSER_TESTCASE_H
#define __KEYVALUEPARSER_TESTCASE_H

#include "firtex/common/StdHeader.h"

#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(utility);

class KeyValueParserTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( KeyValueParserTestCase );
	CPPUNIT_TEST(testParseKeyValue);
	CPPUNIT_TEST(testParseValue);

	CPPUNIT_TEST_SUITE_END();
public:
	KeyValueParserTestCase(void);
	~KeyValueParserTestCase(void);

public:
	void setUp();
	void tearDown();

protected:
	void testParseKeyValue();
    void testParseValue();
};

FX_NS_END

#endif
