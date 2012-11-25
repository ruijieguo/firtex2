#ifndef __NUMBERPARSERTESTCASE_H
#define __NUMBERPARSERTESTCASE_H

#include "firtex/common/StdHeader.h"

#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(utility);

class NumberParserTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( NumberParserTestCase );
	CPPUNIT_TEST( testParseInt32 );
	CPPUNIT_TEST( testParseInt64 );
	CPPUNIT_TEST_SUITE_END();
public:
	NumberParserTestCase(void);
	~NumberParserTestCase(void);

public:
	void setUp();
	void tearDown();

protected:
    void testParseInt32();
    void testParseInt64();
};

FX_NS_END

#endif
