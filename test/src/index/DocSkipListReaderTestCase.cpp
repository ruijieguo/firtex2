#include "index/DocSkipListReaderTestCase.h"
#include "firtex/index/DocSkipListWriter.h"
#include "firtex/index/DocSkipListReader.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/LooseBoundedPool.h"
#include "TestHelper.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(store);
FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocSkipListReaderTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DocSkipListReaderTestCase);

DocSkipListReaderTestCase::DocSkipListReaderTestCase() 
{
}

DocSkipListReaderTestCase::~DocSkipListReaderTestCase() 
{
}

void DocSkipListReaderTestCase::setUp()
{
	File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void DocSkipListReaderTestCase::tearDown()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void makeData(vector<DocSkipPoint>& answer, DocSkipListWriter& skipWriter, size_t dataSize)
{
    for (size_t i = 0; i < dataSize; ++i)
    {
        docid_t docId = (docid_t)(i * 2 + i); 
        ctf_t cs = (ctf_t)(i * 3 + 31);
        offset_t off = (offset_t)i;
        answer.push_back(DocSkipPoint(docId, cs, off));
        skipWriter.addSkipPoint(docId, cs, off);
    }
}

string DocSkipListReaderTestCase::getTestPath()
{
    tstring str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("test_docskip");
    return p.toString();
}

void DocSkipListReaderTestCase::testSkipToWithTwoLevel()
{
    LooseBoundedPool pool(10 * 1024 * 1024);
    vector<DocSkipPoint> answer;
    DocSkipListWriter skipWriter(2, &pool, &pool);
    
    const static size_t TEST_DATA_SIZE = 390;
    makeData(answer, skipWriter, TEST_DATA_SIZE);

    BlockFileSystem blockFS;
    blockFS.open(getTestPath(), FileSystem::CREATE);
    
    OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
    skipWriter.commit(pOutStream);
    pOutStream.reset();

    DocSkipListReader skipReader;

    InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
    skipReader.init(pInSteram);

    size_t skipPoints[] = {5, 260, 289, 290};

    for (size_t i = 0; i < sizeof(skipPoints)/sizeof(skipPoints[0]); ++i)
    {
        docid_t docId = skipReader.skipTo(answer[skipPoints[i]].docId + 1);
        const DocSkipPoint& sp = skipReader.getCurPoint();
        CPPUNIT_ASSERT_EQUAL(answer[skipPoints[i]].docId, docId);
        CPPUNIT_ASSERT_EQUAL(answer[skipPoints[i]].cumSumTf, sp.cumSumTf);
        CPPUNIT_ASSERT_EQUAL(answer[skipPoints[i]].docOff, sp.docOff);
        CPPUNIT_ASSERT_EQUAL((uint32_t)(skipPoints[i] + 1)* SKIP_INTERVAL, skipReader.getNumSkipped());
    }
}

void DocSkipListReaderTestCase::testSkipToWithThreeLevel()
{
    LooseBoundedPool pool(10 * 1024 * 1024);
    vector<DocSkipPoint> answer;
    DocSkipListWriter skipWriter(3, &pool, &pool);
    
    const static size_t TEST_DATA_SIZE = 128 * 128 * 6;
    makeData(answer, skipWriter, TEST_DATA_SIZE);

    BlockFileSystem blockFS;
    blockFS.open(getTestPath(), FileSystem::CREATE);
    
    OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
    skipWriter.commit(pOutStream);
    pOutStream.reset();

    DocSkipListReader skipReader;

    InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
    skipReader.init(pInSteram);

    size_t skipPoints[] = {5, 129, 128 * 128 - 1, 128 * 128 + 11,
                           128 * 128 * 3 + 127};

    for (size_t i = 0; i < sizeof(skipPoints)/sizeof(skipPoints[0]); ++i)
    {
        size_t pos = skipPoints[i];
        docid_t docId = skipReader.skipTo(answer[pos].docId + 1);
        const DocSkipPoint& sp = skipReader.getCurPoint();

        CPPUNIT_ASSERT_EQUAL(answer[pos].docId, docId);
        CPPUNIT_ASSERT_EQUAL(answer[pos].cumSumTf, sp.cumSumTf);
        CPPUNIT_ASSERT_EQUAL(answer[pos].docOff, sp.docOff);
        CPPUNIT_ASSERT_EQUAL((uint32_t)(pos + 1)* SKIP_INTERVAL, skipReader.getNumSkipped());
    }
}

void DocSkipListReaderTestCase::testSkipTo()
{
    for (uint32_t level = 1; level < 8; ++level)
    {
//        std::cout << "level: " << level << std::endl;
        LooseBoundedPool pool(10 * 1024 * 1024);
        vector<DocSkipPoint> answer;
        DocSkipListWriter skipWriter(level, &pool, &pool);
    
        const static size_t TEST_DATA_SIZE = 100000;
        makeData(answer, skipWriter, TEST_DATA_SIZE);

        BlockFileSystem blockFS;
        blockFS.open(getTestPath(), FileSystem::CREATE);
    
        OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
        skipWriter.commit(pOutStream);
        pOutStream.reset();

        DocSkipListReader skipReader;

        InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
        skipReader.init(pInSteram);

        size_t i = 0;
        docid_t docId = INVALID_DOCID;
        for (; i < 134; i += 2)
        {
            docId = skipReader.skipTo(++docId);
            const DocSkipPoint& sp = skipReader.getCurPoint();
            if (i == 0)
            {
//                std::cout << i << ";" << std::endl;
                CPPUNIT_ASSERT_EQUAL((docid_t)0, docId);
                CPPUNIT_ASSERT_EQUAL((ctf_t)0, sp.cumSumTf);
                CPPUNIT_ASSERT_EQUAL((offset_t)0, sp.docOff);
                CPPUNIT_ASSERT_EQUAL((uint32_t)0, skipReader.getNumSkipped());
                docId = answer[0].docId;
            }
            else 
            {
//                std::cout << i - 1 << ";" << std::endl;
                CPPUNIT_ASSERT_EQUAL(answer[i - 1].docId, docId);
                CPPUNIT_ASSERT_EQUAL(answer[i - 1].cumSumTf, sp.cumSumTf);
                CPPUNIT_ASSERT_EQUAL(answer[i - 1].docOff, sp.docOff);
                CPPUNIT_ASSERT_EQUAL((uint32_t)i * SKIP_INTERVAL, skipReader.getNumSkipped());
                docId = answer[i].docId;
            }
            docId = skipReader.skipTo(++docId);
//            std::cout << i << ";" << std::endl;
            CPPUNIT_ASSERT_EQUAL(answer[i].docId, docId);
            CPPUNIT_ASSERT_EQUAL(answer[i].cumSumTf, sp.cumSumTf);
            CPPUNIT_ASSERT_EQUAL(answer[i].docOff, sp.docOff);
            CPPUNIT_ASSERT_EQUAL((uint32_t)(i + 1) * SKIP_INTERVAL, skipReader.getNumSkipped());
            docId = answer[i + 1].docId;
        }

        for (; i < TEST_DATA_SIZE; ++i)
        {
            srand((unsigned int)time(NULL));
            i += rand() % 1000;
            if (i >= TEST_DATA_SIZE)
            {
                i = TEST_DATA_SIZE - 1;
            }
            docId = answer[i].docId + rand() % i;
            if (i < TEST_DATA_SIZE - 1 && docId > answer[i + 1].docId)
            {
                docId = answer[i + 1].docId - 1;
            }
            if (docId == answer[i].docId)
            {
                --i;
            }
            docId = skipReader.skipTo(docId);
//            std::cout << i << ";" << std::endl;
            const DocSkipPoint& sp = skipReader.getCurPoint();
            CPPUNIT_ASSERT_EQUAL(answer[i].docId, docId);
            CPPUNIT_ASSERT_EQUAL(answer[i].cumSumTf, sp.cumSumTf);
            CPPUNIT_ASSERT_EQUAL(answer[i].docOff, sp.docOff);
            CPPUNIT_ASSERT_EQUAL((uint32_t)(i + 1) * SKIP_INTERVAL, skipReader.getNumSkipped());
        }
        
        docId = skipReader.skipTo(docId);
        CPPUNIT_ASSERT_EQUAL(answer[TEST_DATA_SIZE - 1].docId, docId);
    }
}

void DocSkipListReaderTestCase::testSkipToWithSpecialSequence()
{
    struct SkipPoint
    {
        docid_t docId;
        size_t i;
    };

    SkipPoint skipSeq[] = {    
        {0, (size_t)-1},
        {1, 0}, {4, 1}, {7, 2}, {10, 3}, {13, 4}, {16, 5},
        {19, 6}, {22, 7}, {25, 8}, {28, 9}, {31, 10}, 
        {34, 11}, {37, 12}, {40, 13}, {43, 14},{46, 15}, 
        {49, 16}, {52, 17}, {55, 18}, {58, 19}, {61, 20}, 
        {64, 21}, {67, 22}, {70, 23}, {73, 24}, {76, 25}, 
        {79, 26}, {82, 27}, {85, 28}, {88, 29}, {91, 30}, 
        {94, 31}, {97, 32}, {100, 33}, {103, 34}, {106, 35}, 
        {109, 36}, {112, 37}, {115, 38}, {118, 39}, {121, 40}, 
        {124, 41}, {127, 42}, {130, 43}, {133, 44}, {136, 45}, 
        {139, 46}, {142, 47}, {145, 48}, {148, 49}, {151, 50}, 
        {154, 51}, {157, 52}, {160, 53}, {163, 54}, {166, 55}, 
        {169, 56}, {172, 57}, {175, 58}, {178, 59}, {181, 60}, 
        {184, 61}, {187, 62}, {190, 63}, {193, 64}, {196, 65},
        {199, 66}, {202, 67}, {205, 68}, {208, 69}, {211, 70}, 
        {214, 71}, {217, 72}, {220, 73}, {223, 74}, {226, 75}, 
        {229, 76}, {232, 77}, {235, 78}, {238, 79}, {241, 80}, 
        {244, 81}, {247, 82}, {250, 83}, {253, 84}, {256, 85}, 
        {259, 86}, {262, 87}, {265, 88}, {268, 89}, {271, 90}, 
        {274, 91}, {277, 92}, {280, 93}, {283, 94}, {286, 95}, 
        {289, 96}, {292, 97}, {295, 98}, {298, 99}, {301, 100}, 
        {304, 101}, {307, 102}, {310, 103}, {313, 104}, {316, 105}, 
        {319, 106}, {322, 107}, {325, 108}, {328, 109}, {331, 110}, 
        {334, 111}, {337, 112}, {340, 113}, {343, 114}, {346, 115}, 
        {349, 116}, {352, 117}, {355, 118}, {358, 119}, {361, 120}, 
        {14471, 4823}, {17285, 5761}, {20099, 6699}, {22913, 7637}, 
        {25727, 8575}, {28541, 9513}, {31355, 10451}, {34169, 11389}, 
        {36983, 12327}, {39797, 13265}, {42611, 14203}, {45425, 15141}, 
        {48239, 16079}}; 

    LooseBoundedPool pool(10 * 1024 * 1024);
    vector<DocSkipPoint> answer;
    DocSkipListWriter skipWriter(2, &pool, &pool);
    
    const static size_t TEST_DATA_SIZE = 100000;
    makeData(answer, skipWriter, TEST_DATA_SIZE);

    BlockFileSystem blockFS;
    blockFS.open(getTestPath(), FileSystem::CREATE);
    
    OutputStreamPtr pOutStream = blockFS.createFile("testskipfile");
    skipWriter.commit(pOutStream);
    pOutStream.reset();

    DocSkipListReader skipReader;

    InputStreamPtr pInSteram = blockFS.openFile("testskipfile");
    skipReader.init(pInSteram);

    for (size_t i = 0; i < sizeof(skipSeq)/sizeof(skipSeq[0]); ++i)
    {
        docid_t docId = skipReader.skipTo(skipSeq[i].docId);
        const DocSkipPoint& sp = skipReader.getCurPoint();
        if (skipSeq[i].i == (size_t)-1)
        {
            CPPUNIT_ASSERT_EQUAL((docid_t)0, docId);
            CPPUNIT_ASSERT_EQUAL((ctf_t)0, sp.cumSumTf);
            CPPUNIT_ASSERT_EQUAL((offset_t)0, sp.docOff);
        }
        else
        {
            CPPUNIT_ASSERT_EQUAL(answer[skipSeq[i].i].docId, docId);
            CPPUNIT_ASSERT_EQUAL(answer[skipSeq[i].i].cumSumTf, sp.cumSumTf);
            CPPUNIT_ASSERT_EQUAL(answer[skipSeq[i].i].docOff, sp.docOff);
        }
    }
}

FX_NS_END

