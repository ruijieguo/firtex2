#include "index/ByteSliceInputStreamTestCase.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/LooseBoundedPool.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, ByteSliceInputStreamTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(ByteSliceInputStreamTestCase);

ByteSliceInputStreamTestCase::ByteSliceInputStreamTestCase() 
{
}

ByteSliceInputStreamTestCase::~ByteSliceInputStreamTestCase() 
{
}

void ByteSliceInputStreamTestCase::setUp()
{
    m_pMemPool = new LooseBoundedPool(10 * 1024 * 1024);
}

void ByteSliceInputStreamTestCase::tearDown()
{
    m_pMemPool.reset();
}

void ByteSliceInputStreamTestCase::makeData(string& answer, 
        ByteSliceOutputStream& outStream, size_t dataSize)
{
    answer.reserve(dataSize);
    for (size_t j = 0; j < dataSize; ++j)
    {
        NumberFormatter::append(answer, (int32_t)(j % 10));
    }
    outStream.write((const void*)answer.c_str(), answer.size());
}

void ByteSliceInputStreamTestCase::testSeekAndReadLargeBytes()
{
    const static size_t DATA_SIZE = 29442868;
    const static size_t SEEK_POS = 29264452;

    ByteSliceOutputStream outStream(m_pMemPool.get());
    string answer;
    makeData(answer, outStream, DATA_SIZE);

    ByteSliceInputStream inStream(outStream.getHeadSlice(), false);

    string expStr;
    expStr.resize(DATA_SIZE - SEEK_POS);
    inStream.seek(SEEK_POS);
    inStream.read((void*)expStr.c_str(), DATA_SIZE - SEEK_POS);
    CPPUNIT_ASSERT_EQUAL(answer.substr(SEEK_POS, DATA_SIZE - SEEK_POS), expStr);
}

void ByteSliceInputStreamTestCase::testSeekAndReadVInt64()
{
    const static size_t SEEK_POS = 29264452;

    ByteSliceOutputStream outStream(m_pMemPool.get());
    string answer;
    makeData(answer, outStream, SEEK_POS);
    int64_t int64V = 29264452;
    outStream.writeVInt64(int64V);

    ByteSliceInputStream inStream(outStream.getHeadSlice(), false);
    inStream.seek(SEEK_POS);
    int64_t ret = inStream.readVInt64();
    CPPUNIT_ASSERT_EQUAL(int64V, ret);
}

FX_NS_END

