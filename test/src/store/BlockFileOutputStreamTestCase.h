#ifndef __FX_BLOCKFILEOUTPUTSTREAMTESTCASE_H
#define __FX_BLOCKFILEOUTPUTSTREAMTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/store/BlockFileSystem.h"

FX_NS_DEF(store);

class BlockFileOutputStreamTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(BlockFileOutputStreamTestCase);
    CPPUNIT_TEST(testSeekAndWrite);
    CPPUNIT_TEST(testWriteFromInputStream);
    CPPUNIT_TEST_SUITE_END();
public:
    BlockFileOutputStreamTestCase();
    ~BlockFileOutputStreamTestCase();

    void setUp();
    void tearDown();

protected:
    void testSeekAndWrite();
    void testWriteFromInputStream();

private:
    BlockFileSystemPtr m_pBlockFileSystem;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BLOCKFILEOUTPUTSTREAMTESTCASE_H
