#include "store/BlockFileOutputStreamTestCase.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/StringTokenizer.h"
#include "TestHelper.h"
#include <assert.h>

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, BlockFileOutputStreamTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(BlockFileOutputStreamTestCase);

BlockFileOutputStreamTestCase::BlockFileOutputStreamTestCase() 
{
}

BlockFileOutputStreamTestCase::~BlockFileOutputStreamTestCase() 
{
}

void BlockFileOutputStreamTestCase::setUp()
{
    m_pBlockFileSystem.assign(new BlockFileSystem());

    tstring path = TestHelper::getTestDataPath(_T("block_fsoutput_test"), true);
    m_pBlockFileSystem->open(path, BlockFileSystem::CREATE);
}

void BlockFileOutputStreamTestCase::tearDown()
{
    m_pBlockFileSystem.reset();

    tstring path = TestHelper::getTestDataPath(_T("block_fsoutput_test"), true);
    if (File::exists(path))
    {
        File f(path);
        f.remove(true);
    }
}

void BlockFileOutputStreamTestCase::testSeekAndWrite()
{
    size_t dataSize = 10000;
    stringstream content;
    for (size_t i = 0; i < dataSize; ++i)
    {
        content << (i % 10);
    }

    for (size_t i = 0; i < dataSize - 1; ++i)
    {
        OutputStreamPtr pOutput = m_pBlockFileSystem->createFile("testfile1");
        pOutput->write((const void*) content.str().c_str(), i);
        if (i % 137)
        {
            pOutput->write((const void*) content.str().c_str(), i % dataSize);
        }
        pOutput->seek(i);
        pOutput->write((const void*) (content.str().c_str() + i), dataSize - i);
        pOutput->close();

        InputStreamPtr pInput = m_pBlockFileSystem->openFile("testfile1");
        string readCont;
        readCont.resize(dataSize);
        pInput->read((void*)readCont.c_str(), dataSize);

        CPPUNIT_ASSERT_EQUAL(content.str(), readCont);
    }
}

void makeData(string& answer, OutputStream& outStream, size_t dataSize)
{
    answer.reserve(dataSize);
    for (size_t j = 0; j < dataSize; ++j)
    {
        NumberFormatter::append(answer, (int32_t)(j % 10));
    }
    outStream.write((const void*)answer.c_str(), answer.size());
}

void BlockFileOutputStreamTestCase::testWriteFromInputStream()
{
    const static size_t MAX_TEST_DATA = 10000;
    for (size_t i = 1; i < MAX_TEST_DATA; i += 31)
    {
        OutputStreamPtr pOutput = m_pBlockFileSystem->createFile("testfile1");

        string answer;
        makeData(answer, *pOutput, i);
        pOutput.reset();
        
        InputStreamPtr pInput = m_pBlockFileSystem->openFile("testfile1");

        pOutput = m_pBlockFileSystem->createFile("testfile2");
        pOutput->write(*pInput);
        pOutput.reset();

        pInput = m_pBlockFileSystem->openFile("testfile2");
        
        string expStr;
        expStr.resize(i);
        pInput->read((void*)expStr.c_str(), i);
        assert(answer==expStr);        
        CPPUNIT_ASSERT_EQUAL(answer, expStr);
    }
}

FX_NS_END

