#include "index/DocPostingDecoderTestCase.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/utility/LooseBoundedPool.h"
#include <vector>
#include <map>

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocPostingDecoderTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DocPostingDecoderTestCase);

DocPostingDecoderTestCase::DocPostingDecoderTestCase() 
{
}

DocPostingDecoderTestCase::~DocPostingDecoderTestCase() 
{
}

void DocPostingDecoderTestCase::setUp()
{
}

void DocPostingDecoderTestCase::tearDown()
{
}

typedef std::pair<docid_t, tf_t> DocPair;
typedef std::vector<DocPair> DocVector;

void makeData(DocVector& answer, DocPostingWriter& writer, size_t dataSize)
{
    for (size_t i = 0; i < dataSize; ++i)
    {
        docid_t docId = (docid_t)(i * 13 + (i % 12));
        tf_t tf = (tf_t)(i + 13) % 1981;
        answer.push_back(std::make_pair(docId, tf));
        for (tf_t j = 0; j < tf; ++j)
        {
            writer.addPosition(0);
        }
        writer.commitDocument(docId);
    }
}

void DocPostingDecoderTestCase::testDecodeDocRecord()
{
    FX_TRACE("testDecodeDocRecord");

    PostingPoolPtr pPostingPool(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                    new LooseBoundedPool(10 * 1024 * 1024)));
    DocPostingWriter postWriter;
    postWriter.init(pPostingPool.get());
    DocVector answer;
    const static size_t TEST_DATA_SIZE = 1000;
    
    makeData(answer, postWriter, TEST_DATA_SIZE);

    ByteSliceOutputStream* pOutStream = new ByteSliceOutputStream(pPostingPool->postingPool);
    OutputStreamPtr pOStream(pOutStream);

    offset_t startOff = postWriter.commit(pOStream);

    ByteSliceInputStream* pInputStream = new ByteSliceInputStream(pOutStream->getHeadSlice(), false);
    InputStreamPtr pInStream(pInputStream);
    pInStream->seek(startOff);
    
    DocPostingDecoder decoder;
    decoder.init(pInStream, NULL, NULL);

    docid_t docBuffer[RECORD_SIZE];
    tf_t tfBuffer[RECORD_SIZE];
    docid_t docId = 0;
    uint32_t ret = decoder.decodeDocRecord(docBuffer, docId);
    uint32_t ret2 = decoder.decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)RECORD_SIZE, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);
    
    size_t i = 0;
    for (; i < RECORD_SIZE; ++i)
    {
        CPPUNIT_ASSERT_EQUAL(answer[i].first, docBuffer[i]);
        CPPUNIT_ASSERT_EQUAL(answer[i].second, tfBuffer[i]);
    }
    
    docId = answer[260].first;
    ret = decoder.decodeDocRecord(docBuffer, docId);
    ret2 = decoder.decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)RECORD_SIZE, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);

    i = 256;
    for (size_t j = 0; j < RECORD_SIZE; ++i, ++j)
    {
        CPPUNIT_ASSERT_EQUAL(answer[i].first, docBuffer[j]);
        CPPUNIT_ASSERT_EQUAL(answer[i].second, tfBuffer[j]);
    }

    docId = answer[953].first;
    ret = decoder.decodeDocRecord(docBuffer, docId);
    ret2 = decoder.decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)104, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);

    i = 896;
    for (size_t j = 0; j < 104; ++i, ++j)
    {
        CPPUNIT_ASSERT_EQUAL(answer[i].first, docBuffer[j]);
        CPPUNIT_ASSERT_EQUAL(answer[i].second, tfBuffer[j]);
    }

    ret = decoder.decodeDocRecord(docBuffer, docId);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, ret);
}

void DocPostingDecoderTestCase::testDecodeDocRecordRandom()
{
    FX_TRACE("testDecodeDocRecordRandom");

    const static size_t TEST_DATA_SIZE = 10000;
    size_t i = 1;
    while (i < TEST_DATA_SIZE)
    {
        FX_TRACE("Begin test: size: %d", i);
        PostingPoolPtr pPostingPool(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                        new LooseBoundedPool(10 * 1024 * 1024)));

        DocPostingWriter postWriter;
        postWriter.init(pPostingPool.get());
        DocVector answer;
        makeData(answer, postWriter, i);

        ByteSliceOutputStream* pOutStream = new ByteSliceOutputStream(pPostingPool->postingPool);
        OutputStreamPtr pOStream(pOutStream);

        offset_t startOff = postWriter.commit(pOStream);

        ByteSliceInputStream* pInputStream = new ByteSliceInputStream(pOutStream->getHeadSlice(), false);
        InputStreamPtr pInStream(pInputStream);
        pInStream->seek(startOff);
    
        DocPostingDecoder decoder;
        decoder.init(pInStream, NULL, NULL);

        docid_t docBuffer[RECORD_SIZE];
        tf_t tfBuffer[RECORD_SIZE];
        size_t testPos = 0;
        docid_t docId = 0;

        do
        {
            srand((unsigned int)time(NULL));
            size_t oldTestPos = testPos;
            testPos += rand() % 500;
            while (testPos - oldTestPos < RECORD_SIZE)
            {
                testPos += rand() % 500;                
            }
            if (testPos >= i)
            {
                testPos = i - 1;
            }

            docId = answer[testPos].first + rand() % 13;
            if (testPos < i - 1 && docId >= answer[testPos + 1].first)
            {
                docId = answer[testPos + 1].first - 1;
            }
            FX_TRACE("%d, %d", testPos, docId);

            uint32_t ret = decoder.decodeDocRecord(docBuffer, docId);
            uint32_t ret2 = 0;
            if (ret > 0)
            {
                ret2 = decoder.decodeTfRecord(tfBuffer);
            }
            CPPUNIT_ASSERT_EQUAL((uint32_t)ret, ret2);
    
            size_t j = (testPos / RECORD_SIZE) * RECORD_SIZE;
            if ((j + RECORD_SIZE) < (i - 1) && docBuffer[0] >= answer[j + RECORD_SIZE].first)
            {
                j += RECORD_SIZE;
            }
            for (size_t k = 0; k < ret; ++k, ++j)
            {
                CPPUNIT_ASSERT_EQUAL(answer[j].first, docBuffer[k]);
                CPPUNIT_ASSERT_EQUAL(answer[j].second, tfBuffer[k]);
            }
        } while (testPos < i - 1);

        uint32_t ret = decoder.decodeDocRecord(docBuffer, docId);
        CPPUNIT_ASSERT_EQUAL((uint32_t)0, ret);

        FX_TRACE("End test: size: %d", i);
        srand((unsigned int)time(NULL));
        i += rand() % 123;
    }
}

void DocPostingDecoderTestCase::testDecodeDocRecordWithSpecialSeq()
{
    FX_TRACE("\ntestDecodeDocRecordWithSpecialSeq");

    PostingPoolPtr pPostingPool(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                    new LooseBoundedPool(10 * 1024 * 1024)));

    DocPostingWriter postWriter;
    postWriter.init(pPostingPool.get());
    DocVector answer;
    const static size_t TEST_DATA_SIZE = 121; //1255;//487;//4689;//4982;
    
    makeData(answer, postWriter, TEST_DATA_SIZE);

    ByteSliceOutputStream* pOutStream = new ByteSliceOutputStream(pPostingPool->postingPool);
    OutputStreamPtr pOStream(pOutStream);

    offset_t startOff = postWriter.commit(pOStream);

    ByteSliceInputStream* pInputStream = new ByteSliceInputStream(pOutStream->getHeadSlice(), false);
    InputStreamPtr pInStream(pInputStream);
    pInStream->seek(startOff);
    
    DocPostingDecoder decoder;
    decoder.init(pInStream, NULL, NULL);

    docid_t docBuffer[RECORD_SIZE];
    tf_t tfBuffer[RECORD_SIZE];
    struct DocPair
    {
        size_t i;
        docid_t docId;
    };
    DocPair decodeSeq[] = {
        {49, 640},
        {98, 1278}
    };

    for (size_t i = 0; i < sizeof(decodeSeq)/sizeof(decodeSeq[0]); ++i)
    {
        uint32_t ret = decoder.decodeDocRecord(docBuffer, decodeSeq[i].docId);
        uint32_t ret2 = 0;
        if (ret > 0)
        {
            ret2 = decoder.decodeTfRecord(tfBuffer);
        }
        CPPUNIT_ASSERT_EQUAL(ret, ret2);
    
        size_t j = (decodeSeq[i].i/ RECORD_SIZE) * RECORD_SIZE;
        if (j + RECORD_SIZE < (TEST_DATA_SIZE - 1) && docBuffer[0] >= answer[j + RECORD_SIZE].first)
        {
            j += RECORD_SIZE;
        }
        for (size_t k = 0; k < ret; ++j, ++k)
        {
            CPPUNIT_ASSERT_EQUAL(answer[j].first, docBuffer[k]);
            CPPUNIT_ASSERT_EQUAL(answer[j].second, tfBuffer[k]);
        }
    }
}

void DocPostingDecoderTestCase::testDecodeDocRecordInMem()
{
    FX_TRACE("testDecodeDocRecordInMem");

    PostingPoolPtr pPostingPool(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                    new LooseBoundedPool(10 * 1024 * 1024)));
    DocPostingWriter postWriter;
    postWriter.init(pPostingPool.get());
    DocVector answer;
    const static size_t TEST_DATA_SIZE = 1000;
    
    makeData(answer, postWriter, TEST_DATA_SIZE);

    postWriter.seal();
    PostingDecoderPtr pDecoder = postWriter.createDecoder();

    docid_t docBuffer[RECORD_SIZE];
    tf_t tfBuffer[RECORD_SIZE];
    docid_t docId = 0;
    uint32_t ret = pDecoder->decodeDocRecord(docBuffer, docId);
    uint32_t ret2 = pDecoder->decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)RECORD_SIZE, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);
    
    size_t i = 0;
    for (; i < RECORD_SIZE; ++i)
    {
        CPPUNIT_ASSERT_EQUAL(answer[i].first, docBuffer[i]);
        CPPUNIT_ASSERT_EQUAL(answer[i].second, tfBuffer[i]);
    }
    
    docId = answer[260].first;
    ret = pDecoder->decodeDocRecord(docBuffer, docId);
    ret2 = pDecoder->decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)RECORD_SIZE, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);

    i = 256;
    for (size_t j = 0; j < RECORD_SIZE; ++i, ++j)
    {
        CPPUNIT_ASSERT_EQUAL(answer[i].first, docBuffer[j]);
        CPPUNIT_ASSERT_EQUAL(answer[i].second, tfBuffer[j]);
    }

    docId = answer[953].first;
    ret = pDecoder->decodeDocRecord(docBuffer, docId);
    ret2 = pDecoder->decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)104, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);

    i = 896;
    for (size_t j = 0; j < 104; ++i, ++j)
    {
        CPPUNIT_ASSERT_EQUAL(answer[i].first, docBuffer[j]);
        CPPUNIT_ASSERT_EQUAL(answer[i].second, tfBuffer[j]);
    }

    ret = pDecoder->decodeDocRecord(docBuffer, docId);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, ret);
}

void DocPostingDecoderTestCase::testDecodeDocRecordRandomInMem()
{
    FX_TRACE("testDecodeDocRecordRandom");

    const static size_t TEST_DATA_SIZE = 10000;
    size_t i = 1;
    while (i < TEST_DATA_SIZE)
    {
        FX_TRACE("Begin test: size: %d", i);
        PostingPoolPtr pPostingPool(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                        new LooseBoundedPool(10 * 1024 * 1024)));

        DocPostingWriter postWriter;
        postWriter.init(pPostingPool.get());
        DocVector answer;
        makeData(answer, postWriter, i);

        postWriter.seal();
        PostingDecoderPtr pDecoder = postWriter.createDecoder();

        docid_t docBuffer[RECORD_SIZE];
        tf_t tfBuffer[RECORD_SIZE];
        size_t testPos = 0;
        docid_t docId = 0;

        do
        {
            srand((unsigned int)time(NULL));
            size_t oldTestPos = testPos;
            testPos += rand() % 500;
            while (testPos - oldTestPos < RECORD_SIZE)
            {
                testPos += rand() % 500;                
            }
            if (testPos >= i)
            {
                testPos = i - 1;
            }

            docId = answer[testPos].first + rand() % 13;
            if (testPos < i - 1 && docId >= answer[testPos + 1].first)
            {
                docId = answer[testPos + 1].first - 1;
            }
            FX_TRACE("%d, %d", testPos, docId);

            uint32_t ret = pDecoder->decodeDocRecord(docBuffer, docId);
            uint32_t ret2 = 0;
            if (ret > 0)
            {
                ret2 = pDecoder->decodeTfRecord(tfBuffer);
            }
            CPPUNIT_ASSERT_EQUAL((uint32_t)ret, ret2);
    
            size_t j = (testPos / RECORD_SIZE) * RECORD_SIZE;
            if ((j + RECORD_SIZE) < (i - 1) && docBuffer[0] >= answer[j + RECORD_SIZE].first)
            {
                j += RECORD_SIZE;
            }
            for (size_t k = 0; k < ret; ++k, ++j)
            {
                CPPUNIT_ASSERT_EQUAL(answer[j].first, docBuffer[k]);
                CPPUNIT_ASSERT_EQUAL(answer[j].second, tfBuffer[k]);
            }
        } while (testPos < i - 1);

        uint32_t ret = pDecoder->decodeDocRecord(docBuffer, docId);
        CPPUNIT_ASSERT_EQUAL((uint32_t)0, ret);

        FX_TRACE("End test: size: %d", i);
        srand((unsigned int)time(NULL));
        i += rand() % 123;
    }
}

FX_NS_END

