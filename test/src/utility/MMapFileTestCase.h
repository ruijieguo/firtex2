#ifndef __MMAPFILETESTCASE_H
#define __MMAPFILETESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/MMapFile.h"

FX_NS_DEF(utility);

class MMapFileTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( MMapFileTestCase );
	CPPUNIT_TEST( testReadWrite );
	CPPUNIT_TEST( testCreate );
	CPPUNIT_TEST( testCreateFromFile );
	CPPUNIT_TEST_SUITE_END();
public:
	MMapFileTestCase(void);
	~MMapFileTestCase(void);

public:
	void setUp();
	void tearDown();
protected:
	void testReadWrite();
	void testCreate();
	void testCreateFromFile();	
};

FX_NS_END

#endif
