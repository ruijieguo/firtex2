#ifndef __FX_BYTESLICEINPUTSTREAMTESTCASE_H
#define __FX_BYTESLICEINPUTSTREAMTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Pool.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/ByteSliceOutputStream.h"

FX_NS_DEF(index);

class ByteSliceInputStreamTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ByteSliceInputStreamTestCase);
    CPPUNIT_TEST(testSeekAndReadLargeBytes);
    CPPUNIT_TEST(testSeekAndReadVInt64);
    CPPUNIT_TEST_SUITE_END();
public:
    ByteSliceInputStreamTestCase();
    ~ByteSliceInputStreamTestCase();

    void setUp();
    void tearDown();

protected:
    void testSeekAndReadLargeBytes();
    void testSeekAndReadVInt64();

protected:
    void makeData(std::string& answer, ByteSliceOutputStream& outStream,
                  size_t dataSize);
protected:
    FX_NS(utility)::PoolPtr m_pMemPool;
    
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BYTESLICEINPUTSTREAMTESTCASE_H
