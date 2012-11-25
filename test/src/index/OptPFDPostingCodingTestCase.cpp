#include "index/OptPFDPostingCodingTestCase.h"
#include "firtex/index/OptPFDPostingCoding.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/utility/LooseBoundedPool.h"
#include <vector>

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, OptPFDPostingCodingTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(OptPFDPostingCodingTestCase);

OptPFDPostingCodingTestCase::OptPFDPostingCodingTestCase() 
{
}

OptPFDPostingCodingTestCase::~OptPFDPostingCodingTestCase() 
{
}

void OptPFDPostingCodingTestCase::setUp()
{
}

void OptPFDPostingCodingTestCase::tearDown()
{
}

void OptPFDPostingCodingTestCase::testDecodeInt32()
{
    int32_t orgData[] = {
        46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 
        66, 68, 70, 72, 74, 76, 78, 80, 82, 84,
        86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 
        106, 108, 110, 112, 114, 116, 118, 120, 122, 124,
        126, 128, 0, 2, 4, 6, 8, 10, 12, 14,
        16, 18, 20, 22, 24, 26, 28, 30, 32, 34,
        36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 
        56, 58, 60, 62, 64, 66, 68, 70, 72, 74,
        76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 
        96, 98, 100, 102, 104, 106, 108, 110, 112, 114,
        116, 118, 120, 122, 124, 126, 128, 130, 0, 2,
        4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
        24, 26, 28, 30, 32, 34, 36, 38

    };
    size_t dataSize = sizeof(orgData)/sizeof(orgData[0]);
    OptPFDPostingCoding coding;
    LooseBoundedPool pool(1024 * 1024);
    ByteSliceOutputStream outStream(&pool);
    coding.encodeInt32(outStream, orgData, dataSize, false);

    ByteSliceInputStream inStream(outStream.getHeadSlice(), false);
    vector<int32_t> data;
    data.resize(dataSize);
    bool bExh = false;
    size_t ret = coding.decodeInt32((int32_t*)&(data[0]), data.size(), inStream, bExh);
    CPPUNIT_ASSERT_EQUAL(dataSize, ret);
    CPPUNIT_ASSERT(bExh == false);
    
    for (size_t i = 0; i < dataSize; ++i)
    {
        assert(orgData[i] == data[i]);
        CPPUNIT_ASSERT_EQUAL(orgData[i], data[i]);
    }
}

FX_NS_END

