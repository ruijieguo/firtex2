#ifndef __FILETESTCASE_H
#define __FILETESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/File.h"

FX_NS_DEF(utility);

class FileTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( FileTestCase );
	CPPUNIT_TEST( testCreateFile );
    CPPUNIT_TEST(testCreateDirectory);
    CPPUNIT_TEST(testRemoveEmptyDirectory);
	CPPUNIT_TEST_SUITE_END();
public:
	FileTestCase(void);
	~FileTestCase(void);

public:
	void setUp();
	void tearDown();
protected:
    void testCreateFile();
    void testCreateDirectory();
    void testRemoveEmptyDirectory();
private:
    FX_NS(utility)::File* m_pFile;
};

FX_NS_END

#endif
