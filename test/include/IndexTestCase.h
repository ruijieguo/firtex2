#ifndef __INDEXTESTCASE_H
#define __INDEXTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"

class IndexTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( IndexTestCase );
//	CPPUNIT_TEST( testIndex );
//	CPPUNIT_TEST( testCTF );
	CPPUNIT_TEST( testPosting );
	CPPUNIT_TEST_SUITE_END();
public:
	IndexTestCase(void);
	~IndexTestCase(void);
protected:
	void testIndex();
	void testCTF();	
	void testPosting();
};

#endif
