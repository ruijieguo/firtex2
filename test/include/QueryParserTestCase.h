#ifndef __QUERYPARSERTESTCASE_H
#define __QUERYPARSERTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"

class QueryParserTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( QueryParserTestCase );
	CPPUNIT_TEST( testParser );
	CPPUNIT_TEST_SUITE_END();
public:
	QueryParserTestCase(void);
	~QueryParserTestCase(void);
public:
	void testParser();
};

#endif
