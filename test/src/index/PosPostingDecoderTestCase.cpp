#include "index/PosPostingDecoderTestCase.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/index/PosPostingDecoder.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/utility/LooseBoundedPool.h"
#include <vector>
#include <map>


using namespace std;

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosPostingDecoderTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(PosPostingDecoderTestCase);

PosPostingDecoderTestCase::PosPostingDecoderTestCase() 
{
}

PosPostingDecoderTestCase::~PosPostingDecoderTestCase() 
{
}

void PosPostingDecoderTestCase::setUp()
{
}

void PosPostingDecoderTestCase::tearDown()
{
    tearDownTestData();
}

void PosPostingDecoderTestCase::setUpTestData()
{
    m_pPool = new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                              new LooseBoundedPool(10 * 1024 * 1024));

    m_pPostingWriter = new PosPostingWriter();
    m_pPostingWriter->init(m_pPool.get());

    makeData(m_answer, m_pPostingWriter, m_nTestDataSize);

    m_pDocOutStream = new ByteSliceOutputStream(m_pPool->postingPool);
    m_pPosOutStream = new ByteSliceOutputStream(m_pPool->postingPool);

    OutputStreamPtr pDocStreamPtr = m_pDocOutStream.cast<OutputStream>();
    OutputStreamPtr pPosStreamPtr = m_pPosOutStream.cast<OutputStream>();

    offset_t startOff = m_pPostingWriter->commit(pDocStreamPtr, pPosStreamPtr);

    m_pDocInStream = new ByteSliceInputStream(m_pDocOutStream->getHeadSlice(), false);
    m_pPosInStream = new ByteSliceInputStream(m_pPosOutStream->getHeadSlice(), false);
    InputStreamPtr pDocInStreamPtr = m_pDocInStream.cast<InputStream>();
    InputStreamPtr pPosInStreamPtr = m_pPosInStream.cast<InputStream>();
    m_pDocInStream->seek(startOff);
    
    m_pDecoder = new PosPostingDecoder();
    m_pDecoder->init(pDocInStreamPtr, pPosInStreamPtr, NULL);
}

void PosPostingDecoderTestCase::tearDownTestData()
{
    m_answer.clear();
    m_pDecoder.reset();

    m_pDocInStream.reset();
    m_pPosInStream.reset();

    m_pDocOutStream.reset();
    m_pPosOutStream.reset();

    m_pPostingWriter.reset();
    m_pPool.reset();
}

void PosPostingDecoderTestCase::makeData(
        DocVector& answer, PosPostingWriterPtr& writer, size_t dataSize)
{
    ctf_t ctf = 0;
    answer.resize(dataSize);
    for (size_t i = 0; i < dataSize; ++i)
    {
        docid_t docId = (docid_t)(i * 13 + (i % 12));
        tf_t tf = (tf_t)(i + 13) % 131 + 1;
        ctf += tf;
        answer[i].first = docId;
        answer[i].second.resize(tf);
        for (tf_t j = 0; j < tf; ++j)
        {
            loc_t pos = (loc_t) (j * j + j);
            answer[i].second[j] = pos;
            writer->addPosition(pos);
        }
        writer->commitDocument(docId);
    }
    FX_TRACE("Make data: df: %d, ctf: %lld", dataSize, ctf);
}

void PosPostingDecoderTestCase::getCTF(const DocVector& answer, size_t startOff, size_t endOff, ctf_t& ctf)
{
    for (size_t i = startOff; i < endOff; ++i)
    {
        ctf += answer[i].second.size();
    }
}

void PosPostingDecoderTestCase::testDecodePosRecord()
{
    FX_TRACE("testDecodePosRecord");

    m_nTestDataSize = 1000;
    setUpTestData();

    docid_t docBuffer[RECORD_SIZE];
    tf_t tfBuffer[RECORD_SIZE];
    loc_t posBuffer[RECORD_SIZE];
    docid_t docId = 0;
    uint32_t ret = m_pDecoder->decodeDocRecord(docBuffer, docId);
    uint32_t ret2 = m_pDecoder->decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)RECORD_SIZE, ret);
    CPPUNIT_ASSERT_EQUAL((uint32_t)ret, ret2);
    
    size_t i = 0;
    for (; i < RECORD_SIZE; ++i)
    {
        CPPUNIT_ASSERT_EQUAL(m_answer[i].first, docBuffer[i]);
        CPPUNIT_ASSERT_EQUAL(m_answer[i].second.size(), (size_t)tfBuffer[i]);
    }

    ctf_t ctf = 0;
    getCTF(m_answer, 0, 15, ctf);
    uint32_t startOffInBuf = 0;
    uint32_t ret3 = m_pDecoder->decodePosRecord(posBuffer, ctf, startOffInBuf);
    CPPUNIT_ASSERT_EQUAL((uint32_t)128, ret3);
    ret3 -= startOffInBuf;
    ret3 = (ret3 > m_answer[15].second.size()) ? m_answer[15].second.size() : ret3;
    CPPUNIT_ASSERT_EQUAL(m_answer[15].second[0], posBuffer[startOffInBuf]);
    for (size_t k = 1; k < ret3; ++k)
    {
        loc_t pos = (loc_t)(m_answer[15].second[k] - m_answer[15].second[k - 1]);
        CPPUNIT_ASSERT_EQUAL(pos, posBuffer[startOffInBuf + k]);
    }
    
    docId = m_answer[260].first;
    ret = m_pDecoder->decodeDocRecord(docBuffer, docId);
    ret2 = m_pDecoder->decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)RECORD_SIZE, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);

    i = 256;
    for (size_t j = 0; j < RECORD_SIZE; ++i, ++j)
    {
        CPPUNIT_ASSERT_EQUAL(m_answer[i].first, docBuffer[j]);
        CPPUNIT_ASSERT_EQUAL(m_answer[i].second.size(), (size_t)tfBuffer[j]);
    }

    getCTF(m_answer, 15, 139, ctf);
    ret3 = m_pDecoder->decodePosRecord(posBuffer, ctf, startOffInBuf);
    CPPUNIT_ASSERT_EQUAL((uint32_t)128, ret3);
    ret3 -= startOffInBuf;
    ret3 = (ret3 > m_answer[139].second.size()) ? m_answer[139].second.size() : ret3;

    CPPUNIT_ASSERT_EQUAL(m_answer[139].second[0], posBuffer[startOffInBuf]);
    for (size_t k = 1; k < ret3; ++k)
    {
        loc_t pos = (loc_t)(m_answer[139].second[k] - m_answer[139].second[k - 1]);
        CPPUNIT_ASSERT_EQUAL(pos, posBuffer[startOffInBuf + k]);
    }

    docId = m_answer[953].first;
    ret = m_pDecoder->decodeDocRecord(docBuffer, docId);
    ret2 = m_pDecoder->decodeTfRecord(tfBuffer);
    CPPUNIT_ASSERT_EQUAL((uint32_t)104, ret);
    CPPUNIT_ASSERT_EQUAL(ret, ret2);

    i = 896;
    for (size_t j = 0; j < 104; ++i, ++j)
    {
        CPPUNIT_ASSERT_EQUAL(m_answer[i].first, docBuffer[j]);
        CPPUNIT_ASSERT_EQUAL(m_answer[i].second.size(), (size_t)tfBuffer[j]);
    }

    getCTF(m_answer, 139, 899, ctf);
    ret3 = m_pDecoder->decodePosRecord(posBuffer, ctf, startOffInBuf);
    CPPUNIT_ASSERT_EQUAL((uint32_t)128, ret3);
    ret3 -= startOffInBuf;
    ret3 = (ret3 > m_answer[899].second.size()) ? m_answer[899].second.size() : ret3;
    CPPUNIT_ASSERT_EQUAL(m_answer[899].second[0], posBuffer[startOffInBuf]);
    for (size_t k = 1; k < ret3; ++k)
    {
        loc_t pos = (loc_t)(m_answer[899].second[k] - m_answer[899].second[k - 1]);
        CPPUNIT_ASSERT_EQUAL(pos, posBuffer[startOffInBuf + k]);
        //assert(pos == posBuffer[k]);
    }

    ret = m_pDecoder->decodeDocRecord(docBuffer, docId);
    CPPUNIT_ASSERT_EQUAL((uint32_t)0, ret);
}

void PosPostingDecoderTestCase::testDecodePosRecordManyTimes()
{
    FX_TRACE("testDecodePosRecordManyTimes");

    const static size_t TEST_TIMES = 10000;
    for (size_t dataSize = 1; dataSize < TEST_TIMES; dataSize += 123)
    {
//        size_t dataSize = 2092;
        FX_TRACE("test data size: %d", dataSize);
        m_nTestDataSize = dataSize;
        setUpTestData();

        InputVector inputVect;
        generateInputData(inputVect);

        // std::cout << "input: " << std::endl;
        // for (size_t j = 0; j < inputVect.size(); ++j)
        // {
        //     std::cout << inputVect[j] << ", ";
        // }
        // std::cout << "end input " << std::endl;

        for (size_t j = 0; j < inputVect.size(); ++j)
        {
            docid_t docBuffer[RECORD_SIZE];
            tf_t tfBuffer[RECORD_SIZE];
            loc_t posBuffer[RECORD_SIZE];

            size_t testDoc = inputVect[j];
            docid_t docId = m_answer[testDoc].first - 1;

            uint32_t ret = m_pDecoder->decodeDocRecord(docBuffer, docId);
            uint32_t ret2 = m_pDecoder->decodeTfRecord(tfBuffer);
            size_t leftData = m_nTestDataSize - testDoc;
            if (leftData >= RECORD_SIZE)
            {
                leftData = RECORD_SIZE;
            }

            //CPPUNIT_ASSERT_EQUAL((uint32_t)leftData, ret);
            CPPUNIT_ASSERT_EQUAL((uint32_t)ret, ret2);
    
            size_t i = testDoc;
            for (; i < leftData; ++i)
            {
                CPPUNIT_ASSERT_EQUAL(m_answer[i].first, docBuffer[i]);
                CPPUNIT_ASSERT_EQUAL(m_answer[i].second.size(), (size_t)tfBuffer[i]);
            }

            ctf_t ctf = 0;
            getCTF(m_answer, 0, testDoc, ctf);
            uint32_t startOffInBuf = 0;
            uint32_t ret3 = m_pDecoder->decodePosRecord(posBuffer, ctf, startOffInBuf);
            //CPPUNIT_ASSERT_EQUAL((uint32_t)128, ret3);
            ret3 -= startOffInBuf;
            ret3 = (ret3 > m_answer[testDoc].second.size()) ? m_answer[testDoc].second.size() : ret3;
            CPPUNIT_ASSERT_EQUAL(m_answer[testDoc].second[0], posBuffer[startOffInBuf]);
            for (size_t k = 1; k < ret3; ++k)
            {
                loc_t pos = (loc_t)(m_answer[testDoc].second[k] - m_answer[testDoc].second[k - 1]);
                CPPUNIT_ASSERT_EQUAL(pos, posBuffer[startOffInBuf + k]);
            }
        }
    }
}

void PosPostingDecoderTestCase::testDecodePosRecordManyTimesInMem()
{
    FX_TRACE("testDecodePosRecordManyTimes");

    const static size_t TEST_TIMES = 10000;
    for (size_t dataSize = 1; dataSize < TEST_TIMES; dataSize += 123)
    {
        FX_TRACE("test data size: %d", dataSize);
        m_nTestDataSize = dataSize;

        m_pPool = new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                new LooseBoundedPool(10 * 1024 * 1024));

        m_pPostingWriter = new PosPostingWriter();
        m_pPostingWriter->init(m_pPool.get());

        makeData(m_answer, m_pPostingWriter, m_nTestDataSize);

        m_pPostingWriter->seal();
        PostingDecoderPtr pDecoder = m_pPostingWriter->createDecoder();

        InputVector inputVect;
        generateInputData(inputVect);

        for (size_t j = 0; j < inputVect.size(); ++j)
        {
            docid_t docBuffer[RECORD_SIZE];
            tf_t tfBuffer[RECORD_SIZE];
            loc_t posBuffer[RECORD_SIZE];

            size_t testDoc = inputVect[j];
            docid_t docId = m_answer[testDoc].first - 1;

            uint32_t ret = pDecoder->decodeDocRecord(docBuffer, docId);
            uint32_t ret2 = pDecoder->decodeTfRecord(tfBuffer);
            size_t leftData = m_nTestDataSize - testDoc;
            if (leftData >= RECORD_SIZE)
            {
                leftData = RECORD_SIZE;
            }

            //CPPUNIT_ASSERT_EQUAL((uint32_t)leftData, ret);
            CPPUNIT_ASSERT_EQUAL((uint32_t)ret, ret2);
    
            size_t i = testDoc;
            for (; i < leftData; ++i)
            {
                CPPUNIT_ASSERT_EQUAL(m_answer[i].first, docBuffer[i]);
                CPPUNIT_ASSERT_EQUAL(m_answer[i].second.size(), (size_t)tfBuffer[i]);
            }

            ctf_t ctf = 0;
            getCTF(m_answer, 0, testDoc, ctf);
            uint32_t startOffInBuf = 0;
            uint32_t ret3 = pDecoder->decodePosRecord(posBuffer, ctf, startOffInBuf);
            //CPPUNIT_ASSERT_EQUAL((uint32_t)128, ret3);
            ret3 -= startOffInBuf;
            ret3 = (ret3 > m_answer[testDoc].second.size()) ? m_answer[testDoc].second.size() : ret3;
            CPPUNIT_ASSERT_EQUAL(m_answer[testDoc].second[0], posBuffer[startOffInBuf]);
            for (size_t k = 1; k < ret3; ++k)
            {
                loc_t pos = (loc_t)(m_answer[testDoc].second[k] - m_answer[testDoc].second[k - 1]);
                CPPUNIT_ASSERT_EQUAL(pos, posBuffer[startOffInBuf + k]);
            }
        }
    }
}

void PosPostingDecoderTestCase::generateInputData(InputVector& input)
{
//    size_t inData[] = {139, 279, 419, 559, 699, 839, 979, 1119, 1259, 1399, 1539, 1679, 1819, 1959};
//    input.assign(inData, inData + sizeof(inData)/sizeof(inData[0]));
//    return ;

    size_t testDoc = 0;
    while (testDoc < m_nTestDataSize)
    {
        srand((unsigned int)time(NULL));
        testDoc += rand() % 100 + 128;
        if (testDoc >= m_nTestDataSize)
        {
            testDoc = m_nTestDataSize - 1;
            if (input.size() == 0)
            {
                input.push_back(testDoc);
            }
        }
        else
        {
            input.push_back(testDoc);
        }

        ++testDoc;
    }
}

FX_NS_END

