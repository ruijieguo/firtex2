#include "store/BlockFileSystemTest.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/store/BlockFileSystem.h"
#include "TestHelper.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, BlockFileSystemTest);
CPPUNIT_TEST_SUITE_REGISTRATION(BlockFileSystemTest);

BlockFileSystemTest::BlockFileSystemTest() 
{
}

BlockFileSystemTest::~BlockFileSystemTest() 
{
}

void BlockFileSystemTest::setUp()
{
}

void BlockFileSystemTest::tearDown()
{
    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);
    if (File::exists(path))
    {
        File f(path);
        f.remove(true);
    }
}

void BlockFileSystemTest::testReadBlock()
{
    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);
    BlockFileSystem blockFS1;
    blockFS1.open(path, BlockFileSystem::CREATE);
    BlockFileSystem::FilePtr pFile1 = blockFS1.doCreateFile("testfile1");

    stringstream content;
    for (size_t i = 0; i < blockFS1.getBlockSize(); ++i)
    {
        content << (i % 10);
    }
    blockFS1.writeBlock(pFile1, 0, (const void*)content.str().c_str());
    pFile1->fileActualSize = content.str().size();
    blockFS1.closeFile(pFile1);
    blockFS1.close();

    BlockFileSystem blockFS2;
    blockFS2.open(path, BlockFileSystem::READ);
    BlockFileSystem::FilePtr pFile2 = blockFS2.doOpenFile("testfile1");

    string readCont;
    readCont.resize(blockFS2.getBlockSize());
    blockFS2.readBlock(pFile2, 0, (void*)readCont.c_str());
    blockFS2.closeFile(pFile2);

    CPPUNIT_ASSERT_EQUAL(content.str(), readCont);
}

void BlockFileSystemTest::testOpenAFileTwiceForRead()
{
    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);

    createFiles("testfile1");

    BlockFileSystem blockFS2;
    blockFS2.open(path, BlockFileSystem::READ);
    BlockFileSystem::FilePtr pFile2 = blockFS2.doOpenFile("testfile1");
    CPPUNIT_ASSERT(pFile2.isNotNull());

    BlockFileSystem::FilePtr pFile3 = blockFS2.doOpenFile("testfile1");
    CPPUNIT_ASSERT(pFile3.isNotNull());

    CPPUNIT_ASSERT_EQUAL((size_t)2, blockFS2.openedFiles());

    blockFS2.closeFile(pFile2);
    blockFS2.closeFile(pFile3);
}

void BlockFileSystemTest::testCreateAndOpenFile()
{
    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);
    createFiles("testfile1");

    BlockFileSystem blockFS2;
    blockFS2.open(path, BlockFileSystem::READ);
    BlockFileSystem::FilePtr pFile1 = blockFS2.doCreateFile("testfile1");
    CPPUNIT_ASSERT(pFile1.isNotNull());
    CPPUNIT_ASSERT(blockFS2.exists("testfile1"));
    CPPUNIT_ASSERT_EQUAL((size_t)1, blockFS2.openedFiles());

    bool bHasExcep = false;
    try
    {
        BlockFileSystem::FilePtr pFile2 = blockFS2.doOpenFile("testfile1");
    }
    catch(const FileIOException&)
    {
        bHasExcep = true;
    }
    CPPUNIT_ASSERT_EQUAL((size_t)1, blockFS2.openedFiles());

    CPPUNIT_ASSERT(bHasExcep);
    blockFS2.closeFile(pFile1);

    CPPUNIT_ASSERT_EQUAL((size_t)0, blockFS2.openedFiles());
    BlockFileSystem::FilePtr pFile2 = blockFS2.doOpenFile("testfile1");    
    CPPUNIT_ASSERT(pFile2.isNotNull());

    CPPUNIT_ASSERT_EQUAL((size_t)1, blockFS2.openedFiles());
    blockFS2.closeFile(pFile2);
    CPPUNIT_ASSERT_EQUAL((size_t)0, blockFS2.openedFiles());
}

void BlockFileSystemTest::testOpenAndCreateFile()
{
    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);
    createFiles("testfile1");

    BlockFileSystem blockFS2;
    blockFS2.open(path, BlockFileSystem::READ);
    BlockFileSystem::FilePtr pFile1 = blockFS2.doOpenFile("testfile1");
    CPPUNIT_ASSERT(pFile1.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)1, blockFS2.openedFiles());

    bool bHasExcep = false;
    try
    {
        BlockFileSystem::FilePtr pFile2 = blockFS2.doCreateFile("testfile1");
    }
    catch(const FileIOException&)
    {
        bHasExcep = true;
    }
    CPPUNIT_ASSERT_EQUAL((size_t)1, blockFS2.openedFiles());

    CPPUNIT_ASSERT(bHasExcep);
    blockFS2.closeFile(pFile1);

    CPPUNIT_ASSERT_EQUAL((size_t)0, blockFS2.openedFiles());

    BlockFileSystem::FilePtr pFile2 = blockFS2.doCreateFile("testfile1");    
    CPPUNIT_ASSERT(pFile2.isNotNull());

    CPPUNIT_ASSERT_EQUAL((size_t)1, blockFS2.openedFiles());
    blockFS2.closeFile(pFile2);
    CPPUNIT_ASSERT_EQUAL((size_t)0, blockFS2.openedFiles());
}

void BlockFileSystemTest::testDeleteFile()
{
	FX_LOG(INFO, "Delete files");
    createFiles("testfile1;testfile2");

    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);
    BlockFileSystem blockFS;
    blockFS.open(path, FileSystem::APPEND);
    
    CPPUNIT_ASSERT(blockFS.fileExists("testfile1"));
    CPPUNIT_ASSERT(blockFS.fileExists("testfile2"));

    blockFS.deleteFile("testfile1");

    CPPUNIT_ASSERT(!blockFS.fileExists("testfile1"));
    CPPUNIT_ASSERT(blockFS.fileExists("testfile2"));

    blockFS.deleteFile("testfile2");

    CPPUNIT_ASSERT(!blockFS.fileExists("testfile1"));
    CPPUNIT_ASSERT(!blockFS.fileExists("testfile2"));

    File f1(blockFS.getPath() + "testfile1");
    CPPUNIT_ASSERT(!f1.exists());
    File f2(blockFS.getPath() + "testfile2");
    CPPUNIT_ASSERT(!f2.exists());
}

void BlockFileSystemTest::testRenameFile()
{
    createFiles("testfile1;testfile2");
    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);

    BlockFileSystem blockFS;
    blockFS.open(path, FileSystem::READ);
}

void BlockFileSystemTest::createFiles(const string& files)
{
    tstring path = TestHelper::getTestDataPath(_T("block_fs_test"), true);
    BlockFileSystem blockFS;
    blockFS.open(path, BlockFileSystem::CREATE);
    
    StringTokenizer st(files, ";", StringTokenizer::TOKEN_TRIM | 
                       StringTokenizer::TOKEN_IGNORE_EMPTY);
    for (size_t i = 0; i < st.getNumTokens(); ++i)
    {
        BlockFileSystem::FilePtr pFile = blockFS.doCreateFile(st[i]);
        blockFS.closeFile(pFile);
    }

    blockFS.close();
}

FX_NS_END

