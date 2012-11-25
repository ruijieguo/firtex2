#include "index/PosSkipListReaderTestCase.h"
#include "firtex/index/PosSkipListWriter.h"
#include "firtex/index/PosSkipListReader.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/LooseBoundedPool.h"
#include "TestHelper.h"

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosSkipListReaderTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(PosSkipListReaderTestCase);

PosSkipListReaderTestCase::PosSkipListReaderTestCase() 
{
}

PosSkipListReaderTestCase::~PosSkipListReaderTestCase() 
{
}

void PosSkipListReaderTestCase::setUp()
{
}

void PosSkipListReaderTestCase::tearDown()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void makeData(vector<offset_t>& answer, PosSkipListWriter& skipWriter, size_t dataSize)
{
    for (size_t i = 0; i < dataSize; ++i)
    {
        offset_t off = (offset_t)i + 1;
        answer.push_back((offset_t)off);
        skipWriter.addSkipPoint(off);
    }
}

string PosSkipListReaderTestCase::getTestPath()
{
    tstring str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("/test_posskip");
    return p.toString();
}

void PosSkipListReaderTestCase::testSkipToWithTwoLevel()
{
    LooseBoundedPool pool(10 * 1024 * 1024);
    vector<offset_t> answer;
    PosSkipListWriter skipWriter(2, &pool, &pool);
    
    const static size_t TEST_DATA_SIZE = 390;
    makeData(answer, skipWriter, TEST_DATA_SIZE);

    BlockFileSystem blockFS;
    blockFS.open(getTestPath(), FileSystem::CREATE);
    
    OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
    skipWriter.commit(pOutStream);
    pOutStream.reset();

    PosSkipListReader skipReader;

    InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
    skipReader.init(pInSteram);

    size_t skipPoints[] = {5 * SKIP_INTERVAL, 260 * SKIP_INTERVAL + 7,
                           289 * SKIP_INTERVAL + 1, 290 * SKIP_INTERVAL,
                           391 * SKIP_INTERVAL};

    for (size_t i = 0; i < sizeof(skipPoints)/sizeof(skipPoints[0]); ++i)
    {
        offset_t off = skipReader.skipTo(skipPoints[i]);
        CPPUNIT_ASSERT_EQUAL(off, skipReader.getCurPoint());

        uint32_t skipCount = skipPoints[i]/SKIP_INTERVAL;
        if (skipCount > TEST_DATA_SIZE)
        {
            skipCount = TEST_DATA_SIZE;
        }
        CPPUNIT_ASSERT_EQUAL(answer[skipCount - 1], off);
        CPPUNIT_ASSERT_EQUAL((uint32_t)skipCount * SKIP_INTERVAL, skipReader.getNumSkipped());
    }
}

void PosSkipListReaderTestCase::testSkipToWithSpecialSequence()
{
    for (uint32_t level = 1; level < 8; ++level)
    {
//        std::cout << "level: " << level << std::endl;
        LooseBoundedPool pool(10 * 1024 * 1024);
        vector<offset_t> answer;
        PosSkipListWriter skipWriter(level, &pool, &pool);
    
        const static size_t TEST_DATA_SIZE = 100000;
        makeData(answer, skipWriter, TEST_DATA_SIZE);

        BlockFileSystem blockFS;
        blockFS.open(getTestPath(), FileSystem::CREATE);
    
        OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
        skipWriter.commit(pOutStream);
        pOutStream.reset();

        PosSkipListReader skipReader;

        InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
        skipReader.init(pInSteram);

        size_t skipSeq[] = { 12747419, 12777770, 12800000};

        for (size_t i = 0; i < sizeof(skipSeq)/sizeof(skipSeq[0]); ++i)
        {
            offset_t off = skipReader.skipTo((ctf_t)skipSeq[i]);
            CPPUNIT_ASSERT_EQUAL(off, skipReader.getCurPoint());

            size_t nRecords = skipSeq[i] / SKIP_INTERVAL;
            if (nRecords == 0)
            {
                CPPUNIT_ASSERT_EQUAL((offset_t)0, off);
            }
            else 
            {
                CPPUNIT_ASSERT_EQUAL(answer[nRecords - 1], off);
            }
        }
        
//        offset_t off = skipReader.skipTo(TEST_DATA_SIZE + 1);
//        CPPUNIT_ASSERT_EQUAL(answer[TEST_DATA_SIZE - 1], off);
    }    
}

void PosSkipListReaderTestCase::testSkipTo()
{
    for (uint32_t level = 3; level < 4; ++level)
    {
//        std::cout << "level: " << level << std::endl;
        LooseBoundedPool pool(10 * 1024 * 1024);
        vector<offset_t> answer;
        PosSkipListWriter skipWriter(level, &pool, &pool);
    
        const static size_t TEST_DATA_SIZE = 100000;
        makeData(answer, skipWriter, TEST_DATA_SIZE);

        BlockFileSystem blockFS;
        blockFS.open(getTestPath(), FileSystem::CREATE);
    
        OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
        skipWriter.commit(pOutStream);
        pOutStream.reset();

        PosSkipListReader skipReader;

        InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
        skipReader.init(pInSteram);

        for (size_t i = 0; i < TEST_DATA_SIZE * SKIP_INTERVAL; ++i)
        {
            srand((unsigned int)time(NULL));
            i += rand() % 50000;

            if (i > TEST_DATA_SIZE * SKIP_INTERVAL)
            {
                i = TEST_DATA_SIZE * SKIP_INTERVAL;
            }
            offset_t off = skipReader.skipTo((ctf_t)i);
            CPPUNIT_ASSERT_EQUAL(off, skipReader.getCurPoint());

            size_t nRecords = i / SKIP_INTERVAL;
            if (nRecords == 0)
            {
                CPPUNIT_ASSERT_EQUAL((offset_t)0, off);
            }
            else 
            {
                CPPUNIT_ASSERT_EQUAL(answer[nRecords - 1], off);
            }
        }
        
        offset_t off = skipReader.skipTo(TEST_DATA_SIZE + 1);
        CPPUNIT_ASSERT_EQUAL(answer[TEST_DATA_SIZE - 1], off);
    }    
}

void PosSkipListReaderTestCase::testSkiptoWithThreeLevel()
{
    LooseBoundedPool pool(10 * 1024 * 1024);
    vector<offset_t> answer;
    PosSkipListWriter skipWriter(3, &pool, &pool);
    
    const static size_t TEST_DATA_SIZE = 100000;
    makeData(answer, skipWriter, TEST_DATA_SIZE);

    BlockFileSystem blockFS;
    blockFS.open(getTestPath(), FileSystem::CREATE);
    
    OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
    skipWriter.commit(pOutStream);
    pOutStream.reset();

    PosSkipListReader skipReader;

    InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
    skipReader.init(pInSteram);
    
    ctf_t i = 675;
    offset_t off = skipReader.skipTo(i);
    CPPUNIT_ASSERT_EQUAL(answer[(size_t)i/SKIP_INTERVAL - 1], off);
    i = 1351;
    off = skipReader.skipTo(i);
    CPPUNIT_ASSERT_EQUAL(answer[(size_t)i/SKIP_INTERVAL - 1], off);

    i = 2027;
    off = skipReader.skipTo(i);
    CPPUNIT_ASSERT_EQUAL(answer[(size_t)i/SKIP_INTERVAL - 1], off);

    i = 2703;
    off = skipReader.skipTo(i);
    CPPUNIT_ASSERT_EQUAL(answer[(size_t)i/SKIP_INTERVAL - 1], off);
}

FX_NS_END

