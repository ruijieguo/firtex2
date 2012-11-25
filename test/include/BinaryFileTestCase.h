#ifndef __BINARYFILETESTCASE_H
#define __BINARYFILETESTCASE_H

#include "cppunit/extensions/HelperMacros.h"


class BinaryFileTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( BinaryFileTestCase );
	CPPUNIT_TEST( testWrite );
	CPPUNIT_TEST( testRead );
	CPPUNIT_TEST( testLargeFileWrite );
	CPPUNIT_TEST( testLargeFileRead );
	CPPUNIT_TEST_SUITE_END();
public:
	BinaryFileTestCase(void);
	~BinaryFileTestCase(void);
protected:
	void	testWrite();
	void	testRead();
	void	testLargeFileWrite();
	void	testLargeFileRead();
};

#endif