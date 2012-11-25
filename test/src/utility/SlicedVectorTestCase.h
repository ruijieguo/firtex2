#ifndef __SLICEDVECTORTESTCASE_H
#define __SLICEDVECTORTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/SlicedVector.h"
#include <vector>
#include <time.h>

FX_NS_DEF(utility);

class SlicedVectorTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( SlicedVectorTestCase );
	CPPUNIT_TEST(testPushBackInt32);
	CPPUNIT_TEST(testSetInt32);
	CPPUNIT_TEST(testSetInt64);
	CPPUNIT_TEST(testSetInt8);
	CPPUNIT_TEST(testSliceSizeNotAligned);
	CPPUNIT_TEST_SUITE_END();
public:
	SlicedVectorTestCase(void);
	~SlicedVectorTestCase(void);

public:
	void setUp();
	void tearDown();

protected:
    void testPushBackInt32();
    void testSetInt32();
    void testSetInt64();
    void testSetInt8();
    void testSliceSizeNotAligned();

    template<typename T>
    void doTestPushBack(const std::vector<T>& data, size_t nSliceSize, size_t nNumSlices);

    template<typename T>
    void doTestSet(std::vector<T>& data, size_t nSliceSize, size_t nNumSlices);
};

template<typename T>
void SlicedVectorTestCase::doTestPushBack(const std::vector<T>& data, size_t nSliceSize, size_t nNumSlices)
{
    SlicedVector<T> sv(nSliceSize, nNumSlices);
    CPPUNIT_ASSERT_EQUAL((size_t)0, sv.size());

    for (size_t i = 0; i < data.size(); ++i)
    {
        sv.pushBack(data[i]);
    }

    CPPUNIT_ASSERT_EQUAL(data.size(), sv.size());
    for (size_t j = 0; j < sv.size(); ++j)
    {
        CPPUNIT_ASSERT_EQUAL(data[j], sv[j]);
    }
}

template<typename T>
void SlicedVectorTestCase::doTestSet(std::vector<T>& data, size_t nSliceSize, size_t nNumSlices)
{
    SlicedVector<T> sv(nSliceSize, nNumSlices);
    CPPUNIT_ASSERT_EQUAL((size_t)0, sv.size());
    for (size_t i = 0; i < data.size(); ++i)
    {
        sv.set(i, data[i]);
    }

    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < data.size() / 10 + 1; ++i)
    {
        int r = rand() % (int32_t)data.size();
        data[r] += 1;
        sv.set(r, data[r]);
    }

    CPPUNIT_ASSERT_EQUAL(data.size(), sv.size());
    for (size_t j = 0; j < sv.size(); ++j)
    {
        CPPUNIT_ASSERT_EQUAL(data[j], sv[j]);
    }
}

FX_NS_END

#endif
