#ifndef __NUMBERINDEXTESTCASE_H
#define __NUMBERINDEXTESTCASE_H

#include "firtex/common/StdHeader.h"

#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class NumberIndexTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( NumberIndexTestCase );
	CPPUNIT_TEST( testCTF );
	CPPUNIT_TEST( testPosting );
    CPPUNIT_TEST( testRangeQuery );
	CPPUNIT_TEST_SUITE_END();

public:
	NumberIndexTestCase(void);
	~NumberIndexTestCase(void);

	void setUp();
	void tearDown();

protected:
	void testCTF();	
	void testPosting();
    void testRangeQuery();

private:
	void buildNumberIndex(const std::string& sDocs);
    std::string getIndexPath();
};

FX_NS_END

#endif
