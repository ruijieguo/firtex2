#include "BinaryFileTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/BinaryFile.h"
#include "firtex/utility/File.h"
#include <stdio.h>
#include "TestHelper.h"

//#define TEST_LARGE_FILE 1

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(BinaryFileTestCase);

BinaryFileTestCase::BinaryFileTestCase(void)
{	
}

BinaryFileTestCase::~BinaryFileTestCase(void)
{	
	File f(TestHelper::getTestDataPath(_T("BinaryFileTestWriteFile"), false));
	if (f.exists())
	{
		f.remove();
	}

#ifdef TEST_LARGE_FILE
    File f2(TestHelper::getTestDataPath(_T("BinaryFileTestLargeFile"), false));
	if (f2.exists())
	{
		f2.remove();
	}
#endif	
}


void BinaryFileTestCase::setUp()
{	
}

void BinaryFileTestCase::tearDown()
{	
}

void BinaryFileTestCase::testWrite()
{
	tstring str = TestHelper::getTestDataPath(_T("BinaryFileTestWriteFile"), false);
	BinaryFile bf;
	bf.open(str.c_str(), BinaryFile::CRTR);
	CPPUNIT_ASSERT( bf.isFileOpen() );
	int32_t nData;	
	for(size_t i = 0; i < 10000; i += 1000)
	{
		nData = (int32_t)i;
		bf.seek(i);
		bf.write(&nData,sizeof(int32_t));
	}
	bf.close();
}

void BinaryFileTestCase::testRead()
{
	tstring str = TestHelper::getTestDataPath(_T("BinaryFileTestWriteFile"), false);
	BinaryFile bf;
	bf.open(str.c_str(),BinaryFile::READ);
	CPPUNIT_ASSERT( bf.isFileOpen() );
	int32_t nData;
	for(size_t i = 0; i < 10000;i += 1000)
	{
		bf.seek(i);
		bf.read(&nData,sizeof(int32_t));
		CPPUNIT_ASSERT_EQUAL(nData,(int32_t)i);
	}
	bf.close();
}

void BinaryFileTestCase::testSeek()
{
	tstring str = TestHelper::getTestDataPath(_T("BinaryFileTestSeek"), false);
	BinaryFile fileWriter;
    fileWriter.open(str, BinaryFile::CREATE);
	CPPUNIT_ASSERT( fileWriter.isFileOpen() );
	int32_t nData;
	for(size_t i = 0; i < 1000; ++i)
	{
        nData = (int32_t)i;
		fileWriter.write(&nData, sizeof(int32_t));
	}
	fileWriter.close();

	BinaryFile fileReader;
    fileReader.open(str, BinaryFile::READ);
	CPPUNIT_ASSERT( fileReader.isFileOpen() );
	CPPUNIT_ASSERT((int64_t)1000 * sizeof(int32_t) == fileReader.getLength());

	CPPUNIT_ASSERT_EQUAL((int64_t)0, fileReader.getPos());
    fileReader.seek(10);
	CPPUNIT_ASSERT_EQUAL((int64_t)10, fileReader.getPos());

    fileReader.close();
    
    //Remove file
    File f(str);
    f.remove();
}

void BinaryFileTestCase::testLargeFileWrite()
{
#ifdef TEST_LARGE_FILE
    BinaryFile bf;
	tstring str = TestHelper::getTestDataPath(_T("BinaryFileTestLargeFile"), false);
	bf.open(str.c_str(),BinaryFile::CRTR);
	CPPUNIT_ASSERT( bf.isFileOpen() );	
	int64_t nPos = 1000000000;//1 GB
	nPos <<= 4;//16 GB	
	bf.seek(nPos);
	int64_t nData = 123456789;
	bf.write(&nData,sizeof(int64_t));	
	bf.close();
#endif
}

void BinaryFileTestCase::testLargeFileSeekAndRead()
{
#ifdef TEST_LARGE_FILE
	tstring str = TestHelper::getTestDataPath(_T("BinaryFileTestLargeFile"), false);
	BinaryFile bf;
	bf.open(str.c_str(), BinaryFile::READ);
	CPPUNIT_ASSERT( bf.isFileOpen() );
	int64_t nPos = 1000000000;//1 GB
	nPos <<= 4;//16 GB
	int64_t nData = 123456789;
	bf.seek(nPos);
	bf.read(&nData,sizeof(int64_t));
	CPPUNIT_ASSERT_EQUAL(nData,(int64_t)123456789);
#endif
}

FX_NS_END
