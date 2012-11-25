#include "index/S16CodingTestCase.h"

using namespace std;

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, S16CodingTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(S16CodingTestCase);

S16CodingTestCase::S16CodingTestCase() 
{
}

S16CodingTestCase::~S16CodingTestCase() 
{
}

void S16CodingTestCase::setUp()
{
}

void S16CodingTestCase::tearDown()
{
}

void S16CodingTestCase::testEncode()
{
    vector<uint32_t> orgData;
    size_t dataSize = 10000;
    orgData.resize(dataSize);
    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < dataSize; ++i)
    {
        uint32_t v = (uint32_t)rand();
        v = (v << 4) >> 4; //make sure less than or equal to 28 bits
        orgData[i] = v;
    }
    
    vector<uint32_t> encoded;
    encoded.resize(dataSize + dataSize/2);
    size_t ret = encode((uint32_t*)&(encoded[0]), dataSize + dataSize/2,
                        (const uint32_t*)&(orgData[0]), dataSize, true);
    
    vector<uint32_t> decoded;
    decoded.resize(dataSize);
    bool bSeal = true;
    size_t decodedNum = decode((uint32_t*)&(decoded[0]), dataSize,
                               (const uint32_t*)&(encoded[0]), ret, bSeal);
    CPPUNIT_ASSERT_EQUAL(decodedNum, dataSize);

    for (size_t i = 0; i < dataSize; ++i)
    {
        assert(orgData[i] == decoded[i]);
        CPPUNIT_ASSERT_EQUAL(orgData[i], decoded[i]);
    }
}

FX_NS_END

