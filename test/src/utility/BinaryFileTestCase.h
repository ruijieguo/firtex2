#ifndef __BINARYFILETESTCASE_H
#define __BINARYFILETESTCASE_H

#include "firtex/common/StdHeader.h"

#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(utility);

class BinaryFileTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( BinaryFileTestCase );
	CPPUNIT_TEST( testWrite );
	CPPUNIT_TEST( testRead );
	CPPUNIT_TEST( testSeek );
	CPPUNIT_TEST( testLargeFileWrite );
	CPPUNIT_TEST( testLargeFileSeekAndRead );
	CPPUNIT_TEST_SUITE_END();
public:
	BinaryFileTestCase(void);
	~BinaryFileTestCase(void);

public:
	void setUp();
	void tearDown();
protected:
	void testWrite();
	void testRead();
    void testSeek();
	void testLargeFileWrite();
	void testLargeFileSeekAndRead();
};

FX_NS_END

#endif
