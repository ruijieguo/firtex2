#include "SlicedVectorTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include <stdio.h>
#include "TestHelper.h"

using namespace std;

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(SlicedVectorTestCase);

SlicedVectorTestCase::SlicedVectorTestCase(void)
{	
}

SlicedVectorTestCase::~SlicedVectorTestCase(void)
{	
}


void SlicedVectorTestCase::setUp()
{	
}

void SlicedVectorTestCase::tearDown()
{	
}

void SlicedVectorTestCase::testPushBackInt32()
{
    const size_t ELEM_COUNT = 10000;
    vector<int32_t> data;
    data.resize(ELEM_COUNT);
    for (size_t i = 0; i < ELEM_COUNT; ++i)
    {
        data[i] = (int32_t)i;
    }
    doTestPushBack<int32_t>(data, 1024, 2000);
}

void SlicedVectorTestCase::testSetInt32()
{
    const size_t ELEM_COUNT = 10000;
    vector<int32_t> data;
    data.resize(ELEM_COUNT);
    for (size_t i = 0; i < ELEM_COUNT; ++i)
    {
        data[i] = (int32_t)i;
    }
    doTestSet<int32_t>(data, 1000, 2000);
}

void SlicedVectorTestCase::testSetInt64()
{
    const size_t ELEM_COUNT = 10000;
    vector<int64_t> data;
    data.resize(ELEM_COUNT);
    for (size_t i = 0; i < ELEM_COUNT; ++i)
    {
        data[i] = (int64_t)i;
    }
    doTestSet<int64_t>(data, 1024, 2000);
}

void SlicedVectorTestCase::testSetInt8()
{
    const size_t ELEM_COUNT = 10000;
    vector<int8_t> data;
    data.resize(ELEM_COUNT);
    for (size_t i = 0; i < ELEM_COUNT; ++i)
    {
        data[i] = (int8_t)(i % 128);
    }
    doTestSet<int8_t>(data, 1024, 2000);
}

void SlicedVectorTestCase::testSliceSizeNotAligned()
{
    SlicedVector<int32_t> sv(1000, 100);
    CPPUNIT_ASSERT_EQUAL((size_t)1024, sv.getSliceSize());

    SlicedVector<int32_t> sv1(100, 100);
    CPPUNIT_ASSERT_EQUAL((size_t)128, sv1.getSliceSize());

    SlicedVector<int32_t> sv2(10240 - 1, 100);
    CPPUNIT_ASSERT_EQUAL((size_t)16384, sv2.getSliceSize());
}

FX_NS_END
