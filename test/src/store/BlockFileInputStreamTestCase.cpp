#include "store/BlockFileInputStreamTestCase.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/store/BlockFileOutputStream.h"
#include "firtex/store/BlockFileInputStream.h"
#include "TestHelper.h"
#include <assert.h>

using namespace std;

FX_NS_USE(utility);


FX_NS_DEF(store);

CPPUNIT_TEST_SUITE_REGISTRATION(BlockFileInputStreamTestCase);

BlockFileInputStreamTestCase::BlockFileInputStreamTestCase() 
{
}

BlockFileInputStreamTestCase::~BlockFileInputStreamTestCase() 
{
}

void BlockFileInputStreamTestCase::setUp()
{
    m_pBlockFileSystem.assign(new BlockFileSystem());

    tstring path = TestHelper::getTestDataPath(_T("block_fsinput_test"), true);
    m_pBlockFileSystem->open(path, BlockFileSystem::CREATE);
}

void BlockFileInputStreamTestCase::tearDown()
{
    m_pBlockFileSystem.reset();

    tstring path = TestHelper::getTestDataPath(_T("block_fsinput_test"), true);
    if (File::exists(path))
    {
        File f(path);
        f.remove(true);
    }
}

void BlockFileInputStreamTestCase::testRead()
{
    OutputStreamPtr pOutput = m_pBlockFileSystem->createFile("testfile1");

    size_t dataSize = 1000;
    stringstream content;
    for (size_t i = 0; i < dataSize; ++i)
    {
        content << (i % 10);
    }
    pOutput->write((const void*)content.str().c_str(), content.str().size());
    pOutput.reset();

    InputStreamPtr pInput = m_pBlockFileSystem->openFile("testfile1");
    CPPUNIT_ASSERT(pInput != NULL);   
    CPPUNIT_ASSERT_EQUAL(dataSize, (size_t)pInput->getSize());

    string readCont;
    readCont.resize(dataSize);
    pInput->read((void*)readCont.c_str(), dataSize);
    pInput.reset();

    CPPUNIT_ASSERT_EQUAL(content.str(), readCont);
}

void BlockFileInputStreamTestCase::testSeekAndRead()
{
    OutputStreamPtr pOutput = m_pBlockFileSystem->createFile("testfile1");

    size_t dataSize = 10000;
    stringstream content;
    for (size_t i = 0; i < dataSize; ++i)
    {
        content << (i % 10);
    }
    pOutput->write((const void*)content.str().c_str(), content.str().size());
    pOutput.reset();

    InputStreamPtr pInput = m_pBlockFileSystem->openFile("testfile1");
    CPPUNIT_ASSERT(pInput != NULL);   

    CPPUNIT_ASSERT_EQUAL(dataSize, (size_t)pInput->getSize());   

    for (size_t i = 0; i < dataSize - 1; ++i)
    {
        string readCont;
        readCont.resize(dataSize - i);
        if (i % 147 == 0)
        {
            pInput->seek(i % 789);
        }
        pInput->seekAndRead(i, (void*)readCont.c_str(), dataSize - i);

        string expStr = content.str().substr(i);
//        assert(expStr == readCont);
        CPPUNIT_ASSERT_EQUAL(expStr, readCont);
    }
    pInput.reset();
}

void BlockFileInputStreamTestCase::testSeekInBuffer()
{
    OutputStreamPtr pOutput = m_pBlockFileSystem->createFile("testfile1");

    size_t dataSize = 10000;
    stringstream content;
    for (size_t i = 0; i < dataSize; ++i)
    {
        content << (i % 10);
    }
    pOutput->write((const void*)content.str().c_str(), content.str().size());
    pOutput.reset();

    InputStreamPtr pInput = m_pBlockFileSystem->openFile("testfile1");
    CPPUNIT_ASSERT(pInput != NULL);   

    CPPUNIT_ASSERT_EQUAL(dataSize, (size_t)pInput->getSize());   
    off_t off = 100;
    string readCont;
    readCont.resize((size_t)off);
    pInput->read((void*)readCont.c_str(), (size_t)off);
    pInput->seek(off);

    size_t readSize = 1000;
    readCont.resize(readSize);
    pInput->read((void*)readCont.c_str(), readSize);
    
    string expStr = content.str().substr((size_t)off, readSize);
    CPPUNIT_ASSERT_EQUAL(expStr, readCont);

    pInput.reset();
}

void BlockFileInputStreamTestCase::testReadInt()
{
    OutputStreamPtr pOutput = m_pBlockFileSystem->createFile("testfile1");
    uint8_t ui8v =81;
    int32_t i32v = 321;
    int64_t i64v = ((int64_t)1 << 32) + 641;

    string str(129, '1');
    
    pOutput->writeByte(ui8v);
    pOutput->writeInt32(i32v);
    pOutput->write(str.c_str(), str.size());
    pOutput->writeVInt32(i32v);
    pOutput->writeInt64(i64v);
    pOutput->writeVInt64(i64v);
    pOutput.reset();

    InputStreamPtr pInput = m_pBlockFileSystem->openFile("testfile1");
    CPPUNIT_ASSERT(pInput.isNotNull());

    string expStr;
    expStr.resize(str.size());

    CPPUNIT_ASSERT_EQUAL(ui8v, pInput->readByte());
    CPPUNIT_ASSERT_EQUAL(i32v, pInput->readInt32());

    pInput->read((void*)expStr.c_str(), str.size());
    CPPUNIT_ASSERT_EQUAL(str, expStr);

    CPPUNIT_ASSERT_EQUAL(i32v, pInput->readVInt32());
    CPPUNIT_ASSERT_EQUAL(i64v, pInput->readInt64());
    CPPUNIT_ASSERT_EQUAL(i64v, pInput->readVInt64());
}

FX_NS_END

