#ifndef __SEARCHTESTCASE_H
#define __SEARCHTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"

class SearchTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( SearchTestCase );
	CPPUNIT_TEST( testSearch );
	CPPUNIT_TEST_SUITE_END();
public:
	SearchTestCase(void);
	~SearchTestCase(void);
public:
	void testSearch();
};

#endif
