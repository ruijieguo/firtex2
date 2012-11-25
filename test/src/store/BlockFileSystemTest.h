#ifndef __FX_BLOCKFILESYSTEMTEST_H
#define __FX_BLOCKFILESYSTEMTEST_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(store);

class BlockFileSystemTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(BlockFileSystemTest);
    CPPUNIT_TEST(testReadBlock);
    CPPUNIT_TEST(testOpenAFileTwiceForRead);
    CPPUNIT_TEST(testCreateAndOpenFile);
    CPPUNIT_TEST(testOpenAndCreateFile);
    CPPUNIT_TEST(testDeleteFile);
    CPPUNIT_TEST(testRenameFile);
    CPPUNIT_TEST_SUITE_END();
public:
    BlockFileSystemTest();
    ~BlockFileSystemTest();

    void setUp();
    void tearDown();

protected:
    void testReadBlock();
    void testOpenAFileTwiceForRead();
    void testCreateAndOpenFile();
    void testOpenAndCreateFile();
    void testDeleteFile();
    void testRenameFile();
    
protected:
    void createFiles(const std::string& files);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BLOCKFILESYSTEMTEST_H
