#include "MMapFileTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/MMapFile.h"
#include "firtex/utility/File.h"
#include <stdio.h>
#include "TestHelper.h"

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(MMapFileTestCase);

MMapFileTestCase::MMapFileTestCase(void)
{		
}

MMapFileTestCase::~MMapFileTestCase(void)
{		
}


void MMapFileTestCase::setUp()
{	
	TestHelper::removeTestFile("MMapFileTest", false);
}

void MMapFileTestCase::tearDown()
{	
	TestHelper::removeTestFile("MMapFileTest", false);
}

void MMapFileTestCase::testReadWrite()
{
	MMapFile writeFile;
	writeFile.open(_T("share"), 4096, MMapFile::AM_WRITE, NULL, false);
	writeFile.begin()[0] = 'a';
	writeFile.end()[-1] = 'z';

	MMapFile mf;
	mf.open(_T("share"), 4096, MMapFile::AM_READ, NULL, false);
	CPPUNIT_ASSERT_EQUAL((int64_t)4096, (int64_t)(mf.end() - mf.begin()));
	CPPUNIT_ASSERT_EQUAL('a', mf.begin()[0]);
	CPPUNIT_ASSERT_EQUAL('z', mf.end()[-1]);

	writeFile.begin()[0] = 'A';
	writeFile.end()[-1] = 'Z';

	CPPUNIT_ASSERT_EQUAL('A', mf.begin()[0]);
	CPPUNIT_ASSERT_EQUAL('Z', mf.end()[-1]);

    mf.close();
    writeFile.close();
}

void MMapFileTestCase::testCreate()
{
	MMapFile mf;
	mf.open("mmapfile", 4096, MMapFile::AM_WRITE, NULL, false);
	CPPUNIT_ASSERT(mf.end() - mf.begin() == 4096);
	mf.begin()[0] = 'A';
	mf.end()[-1] = 'Z';

    mf.close();
}

void MMapFileTestCase::testCreateFromFile()
{
	tstring str = TestHelper::getTestDataPath("MMapFileTest", false);
	FILE* f = fopen(str.c_str(), "w+b");
	CPPUNIT_ASSERT(f);
	char ch = 'A';
	fwrite((const void*)&ch, sizeof(char), 1, f);
	fseek(f, 4096, SEEK_SET);
	ch = 'Z';
	fwrite((const void*)&ch, sizeof(char), 1, f);
	fclose(f);

	MMapFile mf;
	mf.open(str, MMapFile::AM_READ);
	CPPUNIT_ASSERT(mf.end() > mf.begin());
	CPPUNIT_ASSERT_EQUAL(mf.length(), (int64_t)(mf.end() - mf.begin()));
	CPPUNIT_ASSERT_EQUAL('A', mf.begin()[0]);
	CPPUNIT_ASSERT_EQUAL('Z', mf.begin()[4096]);
	CPPUNIT_ASSERT_EQUAL('Z', mf.end()[-1]);
	mf.close();

	TestHelper::removeTestFile("MMapFileTest", false);
}

FX_NS_END
