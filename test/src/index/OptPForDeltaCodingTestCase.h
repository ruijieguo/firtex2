#ifndef __FX_OPTPFORDELTACODINGTESTCASE_H
#define __FX_OPTPFORDELTACODINGTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/compress/OptPForDeltaCoding.h"

FX_NS_DEF(index);

class OptPForDeltaCodingTestCase : public CPPUNIT_NS::TestFixture,
                                   public OptPForDeltaCoding
{
    CPPUNIT_TEST_SUITE(OptPForDeltaCodingTestCase);
    CPPUNIT_TEST(testGetHighBitOff);
    CPPUNIT_TEST(testGetHighBitArray);
    CPPUNIT_TEST(testDeterminFrameBitsWithManyException);
    CPPUNIT_TEST(testEncodeWithException);
    CPPUNIT_TEST(testEncode);
    CPPUNIT_TEST(testEncodeSpecialData);
    CPPUNIT_TEST_SUITE_END();
public:
    OptPForDeltaCodingTestCase();
    ~OptPForDeltaCodingTestCase();

    void setUp();
    void tearDown();

protected:
    void testGetHighBitOff();
    void testGetHighBitArray();
    void testDeterminFrameBitsWithManyException();
    void testEncodeWithException();
    void testEncode();
    void testEncodeSpecialData();

protected:
    void CheckFrameBits(const uint32_t* frame, size_t n,
                        uint32_t nFrameBits, uint32_t nNumExceps);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_OPTPFORDELTACODINGTESTCASE_H
