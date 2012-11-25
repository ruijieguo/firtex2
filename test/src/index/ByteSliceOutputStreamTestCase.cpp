#include "index/ByteSliceOutputStreamTestCase.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/LooseBoundedPool.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, ByteSliceOutputStreamTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(ByteSliceOutputStreamTestCase);

ByteSliceOutputStreamTestCase::ByteSliceOutputStreamTestCase() 
{
}

ByteSliceOutputStreamTestCase::~ByteSliceOutputStreamTestCase() 
{
}

void ByteSliceOutputStreamTestCase::setUp()
{
    m_pMemPool = new LooseBoundedPool(10 * 1024 * 1024);
}

void ByteSliceOutputStreamTestCase::tearDown()
{
    m_pMemPool.reset();
}

void ByteSliceOutputStreamTestCase::makeData(string& answer, 
        ByteSliceOutputStream& outStream, size_t dataSize)
{
    answer.reserve(dataSize);
    for (size_t j = 0; j < dataSize; ++j)
    {
        NumberFormatter::append(answer, (int32_t)(j % 10));
    }
    outStream.write((const void*)answer.c_str(), answer.size());
}

void ByteSliceOutputStreamTestCase::testWrite()
{
    const static size_t MAX_TEST_DATA = 10000;
    for (size_t i = 1; i < MAX_TEST_DATA; i += 31)
    {
        ByteSliceOutputStream outStream(m_pMemPool.get());
        string answer;
        makeData(answer, outStream, i);

        ByteSliceInputStream inStream(outStream.getHeadSlice(), 
                false, (size_t)outStream.tell());
        
        string expStr;
        expStr.resize(i);
        inStream.read((void*)expStr.c_str(), i);
        CPPUNIT_ASSERT_EQUAL(answer, expStr);
    }
}

void ByteSliceOutputStreamTestCase::testCopyTo()
{
    const static size_t MAX_TEST_DATA = 10000;
    for (size_t i = 1; i < MAX_TEST_DATA; i += 31)
    {
        ByteSliceOutputStream outStream(m_pMemPool.get());
        string answer;
        makeData(answer, outStream, i);

        ByteSliceOutputStream outStream2(m_pMemPool.get());
        outStream.copyTo(outStream2);

        ByteSliceInputStream inStream(outStream2.getHeadSlice(), 
                false, (size_t)outStream2.tell());
        
        string expStr;
        expStr.resize(i);
        inStream.read((void*)expStr.c_str(), i);
        CPPUNIT_ASSERT_EQUAL(answer, expStr);
    }
}

void ByteSliceOutputStreamTestCase::testWriteFromInputStream()
{
    const static size_t MAX_TEST_DATA = 10000;
    for (size_t i = 1; i < MAX_TEST_DATA; i += 31)
    {
        ByteSliceOutputStream outStream(m_pMemPool.get());
        string answer;
        makeData(answer, outStream, i);

        ByteSliceInputStream inStream(outStream.getHeadSlice(), 
                false, (size_t)outStream.tell());

        ByteSliceOutputStream outStream2(m_pMemPool.get());
        outStream2.write(inStream);

        ByteSliceInputStream inStream2(outStream2.getHeadSlice(), 
                false, (size_t)outStream2.tell());
        
        string expStr;
        expStr.resize(i);
        inStream2.read((void*)expStr.c_str(), i);
        CPPUNIT_ASSERT_EQUAL(answer, expStr);
    }
}

FX_NS_END

