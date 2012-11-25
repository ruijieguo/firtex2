#include "index/OptPForDeltaCodingTestCase.h"
#include <assert.h>

using namespace std;

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, OptPForDeltaCodingTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(OptPForDeltaCodingTestCase);

OptPForDeltaCodingTestCase::OptPForDeltaCodingTestCase() 
{
}

OptPForDeltaCodingTestCase::~OptPForDeltaCodingTestCase() 
{
}

void OptPForDeltaCodingTestCase::setUp()
{
}

void OptPForDeltaCodingTestCase::tearDown()
{
}

void OptPForDeltaCodingTestCase::CheckFrameBits(const uint32_t* frame, size_t n,
        uint32_t nFrameBits, uint32_t nNumExceps)
{
    size_t exp = 0;
    for (size_t i = 0; i < n; ++i)
    {
        if (((uint64_t)frame[i] >> nFrameBits) != 0)
        {
            exp++;
        }
    }
    CPPUNIT_ASSERT_EQUAL((uint32_t)exp, nNumExceps);
}

void OptPForDeltaCodingTestCase::testGetHighBitOff()
{
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, getHighBitOff(0));
    for (uint32_t i = 0; i < 32; ++i)
    {
        uint32_t value = (1 << i);
        CPPUNIT_ASSERT_EQUAL(i + 1, getHighBitOff(value));
    }
}

void OptPForDeltaCodingTestCase::testGetHighBitArray()
{
    uint32_t frame[FRAME_SIZE] = { 0 };
    uint32_t bitArray[33] = { 0 };
    uint32_t maxBitOff = 0;
    getHighBitArray(bitArray, maxBitOff, frame, FRAME_SIZE);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, maxBitOff);
    CPPUNIT_ASSERT_EQUAL((uint32_t)FRAME_SIZE, bitArray[0]);

    for (size_t i = 0; i < 32; ++i)
    {
        uint32_t bitArray[33] = { 0 };
        uint32_t maxBitOff = 0;
        for (size_t j = 0; j < FRAME_SIZE; ++j)
        {
            frame[j] = (1 << i);
        }

        getHighBitArray(bitArray, maxBitOff, frame, FRAME_SIZE);

        CPPUNIT_ASSERT_EQUAL((uint32_t)BITS_PACK_MAP[i + 1], maxBitOff);
        CPPUNIT_ASSERT_EQUAL((uint32_t)FRAME_SIZE, bitArray[BITS_PACK_MAP[i + 1]]);
    }
}

void OptPForDeltaCodingTestCase::testDeterminFrameBitsWithManyException()
{
    uint32_t frame[FRAME_SIZE];
    for (size_t i = 0; i < 32; ++i)
    {
        for (size_t j = 0; j < FRAME_SIZE; ++j)
        {
            frame[j] = (1 << i) - 1;
        }
        frame[1] = (1 << i);
        frame[7] = (1 << i);
        frame[17] = (1 << i);
        uint32_t nNumExceps = (uint32_t)-1;
        uint32_t nFrameBits = determinFrameBits(frame, FRAME_SIZE, nNumExceps);
        CPPUNIT_ASSERT_EQUAL(BITS_PACK_MAP[i], nFrameBits);

        if (BITS_PACK_MAP[i] == 32)
        {
            CPPUNIT_ASSERT_EQUAL((uint32_t)0, nNumExceps);
        }
        else 
        {
            CPPUNIT_ASSERT_EQUAL((uint32_t)3, nNumExceps);
        }
        CheckFrameBits(frame, FRAME_SIZE, nFrameBits, nNumExceps);
    }
}

void OptPForDeltaCodingTestCase::testEncodeWithException()
{
    size_t dataSize = FRAME_SIZE;
    for (size_t i = 0; i < 32; ++i)
    {
        vector<uint32_t> orgData;
        orgData.resize(dataSize);

        for (size_t j = 0; j < dataSize; ++j)
        {
            orgData[j] = (1 << i) - 1;
        }
        orgData[1] = (1 << i);
        orgData[7] = (1 << i);
        orgData[17] = (1 << i);

        vector<uint32_t> encoded;
        encoded.resize(dataSize << 1);
        size_t ret = encode((uint32_t*)&(encoded[0]), dataSize << 1, 
                            (const uint32_t*)&(orgData[0]), dataSize, true);

        size_t frameSize = getFrameSize(encoded[0]);
        CPPUNIT_ASSERT_EQUAL(ret, frameSize);
    
        vector<uint32_t> decoded;
        decoded.resize(dataSize);
        bool bExhausted = false;
        size_t decodedNum = decode((uint32_t*)&(decoded[0]), dataSize,
                (const uint32_t*)&(encoded[0]), ret, bExhausted);
        CPPUNIT_ASSERT_EQUAL(decodedNum, dataSize);
        CPPUNIT_ASSERT(bExhausted == true);

        for (size_t j = 0; j < dataSize; ++j)
        {
            CPPUNIT_ASSERT_EQUAL(orgData[j], decoded[j]);
        }
    }
}

void OptPForDeltaCodingTestCase::testEncode()
{
    vector<uint32_t> orgData;
    size_t dataSize = 10000;
    orgData.resize(dataSize);
    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < dataSize; ++i)
    {
        orgData[i] = (uint32_t)rand();
    }
    
    vector<uint32_t> encoded;
    encoded.resize(dataSize + dataSize/2);
    size_t ret = encode((uint32_t*)&(encoded[0]), dataSize + dataSize/2,
                        (const uint32_t*)&(orgData[0]), dataSize, true);
    
    vector<uint32_t> decoded;
    decoded.resize(dataSize);
    bool bExhausted = false;
    size_t decodedNum = decode((uint32_t*)&(decoded[0]), dataSize, 
                               (const uint32_t*)&(encoded[0]), ret, bExhausted);
    CPPUNIT_ASSERT(bExhausted == true);
    CPPUNIT_ASSERT_EQUAL(decodedNum, dataSize);

    for (size_t i = 0; i < dataSize; ++i)
    {
        assert(orgData[i] == decoded[i]);
        CPPUNIT_ASSERT_EQUAL(orgData[i], decoded[i]);
    }
}

void OptPForDeltaCodingTestCase::testEncodeSpecialData()
{
    uint32_t orgData[] = {
        104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 
        128, 130, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26,
        28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56,
        58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 
        88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 
        114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 0, 2, 4, 
        6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 
        36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64,
        66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92
    };

    size_t dataSize = sizeof(orgData)/sizeof(orgData[0]);
    
    vector<uint32_t> encoded;
    encoded.resize(dataSize + dataSize/2);
    size_t ret = encode((uint32_t*)&(encoded[0]), dataSize + dataSize/2,
                        (const uint32_t*)orgData, dataSize, true);
    
    vector<uint32_t> decoded;
    decoded.resize(dataSize);
    bool bExhausted = false;
    size_t decodedNum = decode((uint32_t*)&(decoded[0]), dataSize, 
                               (const uint32_t*)&(encoded[0]), ret, bExhausted);
    CPPUNIT_ASSERT(bExhausted == true);
    CPPUNIT_ASSERT_EQUAL(decodedNum, dataSize);

    for (size_t i = 0; i < dataSize; ++i)
    {
        assert(orgData[i] == decoded[i]);
        CPPUNIT_ASSERT_EQUAL(orgData[i], decoded[i]);
    }
}



FX_NS_END

