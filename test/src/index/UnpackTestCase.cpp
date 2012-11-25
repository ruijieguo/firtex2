#include "index/UnpackTestCase.h"
#include "firtex/index/compress/Pack.h"
#include "firtex/index/compress/Unpack.h"
#include <stdlib.h>
#include <assert.h>

using namespace std;

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, UnpackTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(UnpackTestCase);

UnpackTestCase::UnpackTestCase() 
{
}

UnpackTestCase::~UnpackTestCase() 
{
}

void UnpackTestCase::setUp()
{
}

void UnpackTestCase::tearDown()
{
}

void UnpackTestCase::createData(vector<uint32_t>& data, size_t n, uint64_t nMaxValue)
{
    data.resize(n);
    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < n; ++i)
    {
        if ((uint32_t)(nMaxValue + 1) == 0)//overflow
        {
            data[i] = (uint32_t)rand();
        }
        else 
        {
            data[i] = ((uint32_t)rand() % (uint32_t)(nMaxValue + 1));
        }
    }
}

void UnpackTestCase::testUnpack()
{
    for (size_t i = 1; i < 128; i++)
    {
        for (size_t frameBits = 1; frameBits < 31; ++frameBits)
        {
            vector<uint32_t> data;
            createData(data, i, (((uint32_t)1 << frameBits) - 1));

            vector<uint32_t> encode;
            encode.resize(i << 1);
            Pack::pack(frameBits, (uint32_t*)&(encode[0]), (const uint32_t*)&(data[0]), i);
            
            vector<uint32_t> dest;
            dest.resize(i);
            Unpack::unpack(frameBits, (uint32_t*)&(dest[0]), (const uint32_t*)&(encode[0]), i);

            for (uint32_t j = 0; j < i; ++j)
            {
                if (data[j] != dest[j])
                {
                    cout << "frameBits: " << frameBits << "i: " << i << endl;
                    assert(data[i] == dest[i]);
                }
                CPPUNIT_ASSERT_EQUAL(data[j], dest[j]);
            }
        }
    }
    
}

FX_NS_END

