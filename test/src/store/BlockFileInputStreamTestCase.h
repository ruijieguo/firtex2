#ifndef __FX_BLOCKFILEINPUTSTREAMTESTCASE_H
#define __FX_BLOCKFILEINPUTSTREAMTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/store/BlockFileSystem.h"

FX_NS_DEF(store);

class BlockFileInputStreamTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(BlockFileInputStreamTestCase);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST(testSeekAndRead);
    CPPUNIT_TEST(testSeekInBuffer);
    CPPUNIT_TEST(testReadInt);
    CPPUNIT_TEST_SUITE_END();

public:
    BlockFileInputStreamTestCase();
    ~BlockFileInputStreamTestCase();

    void setUp();
    void tearDown();

protected:
    void testRead();
    void testSeekAndRead();
    void testSeekInBuffer();
    void testReadInt();

private:
    BlockFileSystemPtr m_pBlockFileSystem;
};

FX_NS_END

#endif //__FX_BLOCKFILEINPUTSTREAMTESTCASE_H
