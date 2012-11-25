#ifndef __FX_BYTESLICEOUTPUTSTREAMTESTCASE_H
#define __FX_BYTESLICEOUTPUTSTREAMTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Pool.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/ByteSliceOutputStream.h"

FX_NS_DEF(index);

class ByteSliceOutputStreamTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ByteSliceOutputStreamTestCase);
    CPPUNIT_TEST(testWrite);
    CPPUNIT_TEST(testCopyTo);
    CPPUNIT_TEST(testWriteFromInputStream);
    CPPUNIT_TEST_SUITE_END();
public:
    ByteSliceOutputStreamTestCase();
    ~ByteSliceOutputStreamTestCase();

    void setUp();
    void tearDown();

protected:
    void testWrite();
    void testCopyTo();
    void testWriteFromInputStream();

protected:
    void makeData(std::string& answer, ByteSliceOutputStream& outStream,
                  size_t dataSize);
protected:
    FX_NS(utility)::PoolPtr m_pMemPool;
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BYTESLICEOUTPUTSTREAMTESTCASE_H
