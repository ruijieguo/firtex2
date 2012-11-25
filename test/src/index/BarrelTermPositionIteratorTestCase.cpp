#include "index/BarrelTermPositionIteratorTestCase.h"
#include "firtex/index/BarrelTermPositionIterator.h"
#include "firtex/utility/LooseBoundedPool.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, BarrelTermPositionIteratorTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(BarrelTermPositionIteratorTestCase);

BarrelTermPositionIteratorTestCase::BarrelTermPositionIteratorTestCase() 
    : m_nTestDataSize(1000)
{
}

BarrelTermPositionIteratorTestCase::~BarrelTermPositionIteratorTestCase() 
{
}

void BarrelTermPositionIteratorTestCase::setUp()
{

}

void BarrelTermPositionIteratorTestCase::tearDown()
{
    tearDownTestData();
}

void BarrelTermPositionIteratorTestCase::setUpTestData()
{
    m_pPostingPool = new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
            new LooseBoundedPool(10 * 1024 * 1024));
    m_pPostingWriter = new PosPostingWriter();
    m_pPostingWriter->init(m_pPostingPool.get());

    makeData(m_answer, m_pPostingWriter, m_nTestDataSize);

    m_pDocOutStream = new ByteSliceOutputStream(m_pPostingPool->postingPool);
    m_pPosOutStream = new ByteSliceOutputStream(m_pPostingPool->postingPool);

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

void BarrelTermPositionIteratorTestCase::tearDownTestData()
{
    m_answer.clear();
    m_pDecoder.reset();

    m_pDocInStream.reset();
    m_pPosInStream.reset();

    m_pDocOutStream.reset();
    m_pPosOutStream.reset();

    m_pPostingWriter.reset();
    m_pPostingPool.reset();
}

void BarrelTermPositionIteratorTestCase::makeData(
        DocVector& answer, PosPostingWriterPtr& writer, size_t dataSize)
{
    ctf_t ctf = 0;
    for (size_t i = 0; i < dataSize; ++i)
    {
        docid_t docId = (docid_t)(i * 13 + (i % 12));
        tf_t tf = (tf_t)(i + 13) % 131 + 1;
        ctf += tf;
        answer.push_back(std::make_pair(docId, tf));
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

void BarrelTermPositionIteratorTestCase::testTermMeta()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPositionIterator iterator;
    iterator.init(m_pDecoder, NULL);

    const TermMeta& termMeta = iterator.getTermMeta();
    CPPUNIT_ASSERT_EQUAL((df_t)1000, termMeta.getDocFreq());
    ctf_t ctf = 0;
    for (size_t i = 0; i < m_answer.size(); ++i)
    {
        ctf += m_answer[i].second.size();
    }
    CPPUNIT_ASSERT_EQUAL(ctf, termMeta.getCTF());
}

void BarrelTermPositionIteratorTestCase::testSkipTo()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPositionIterator iterator;
    iterator.init(m_pDecoder, NULL);

    docid_t docId = 0;
    size_t testElems[] = {0, 125, 255, 789, 998, 999};
    for (size_t i = 0; i < sizeof(testElems) / sizeof(testElems[0]); ++i)
    {
        size_t testPos = testElems[i];
        docId = m_answer[testPos].first - 3;
        if (docId < 0)
        {
            docId = 0;
        }
        docId = iterator.skipTo(docId);
        CPPUNIT_ASSERT_EQUAL(m_answer[testPos].first, docId);
        CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[testPos].second.size(), iterator.freq());
    }

    docId = m_answer[999].first + 3;
    docId = iterator.skipTo(docId);
    CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);
}

void BarrelTermPositionIteratorTestCase::testNextDocs()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPositionIterator iterator;
    iterator.init(m_pDecoder, NULL);

    docid_t* docs;
    tf_t* freqs;
    df_t dataSize = 0;
    while (dataSize < (df_t)m_nTestDataSize)
    {
        df_t ret = iterator.nextDocs(docs, freqs);
        for (df_t i = 0; i < ret; ++i)
        {
            CPPUNIT_ASSERT_EQUAL(m_answer[dataSize].first, docs[i]);
            CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[dataSize].second.size(), freqs[i]);
            ++dataSize;
        }
    }
    CPPUNIT_ASSERT_EQUAL((df_t)m_nTestDataSize, dataSize);
    df_t ret = iterator.nextDocs(docs, freqs);
    CPPUNIT_ASSERT_EQUAL((df_t)0, ret);
}

void BarrelTermPositionIteratorTestCase::testSkipToAndNextDocs()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPositionIterator iterator;
    iterator.init(m_pDecoder, NULL);

    docid_t* pDocs;
    tf_t* pFreqs;
    docid_t docId = 0;
    size_t testElems[] = {0, 128, 131, 125, 256, 128, 998, 2};
    for (size_t i = 0; i < sizeof(testElems) / sizeof(testElems[0]); i += 2)
    {
        size_t testPos = testElems[i];
        docId = m_answer[testPos].first - 3;
        if (docId < 0)
        {
            docId = 0;
        }
        docId = iterator.skipTo(docId);
        CPPUNIT_ASSERT_EQUAL(m_answer[testPos].first, docId);
        CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[testPos].second.size(), iterator.freq());

        size_t answerPos = testPos;
        uint32_t ret = iterator.nextDocs(pDocs, pFreqs);
        CPPUNIT_ASSERT_EQUAL((uint32_t)testElems[i + 1], ret);
        for (uint32_t j = 0; j < ret; ++j)
        {
            CPPUNIT_ASSERT_EQUAL(m_answer[answerPos].first, pDocs[j]);
            CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[answerPos].second.size(), pFreqs[j]);
            ++answerPos;
        }
    }

    docId = m_answer[999].first + 3;
    docId = iterator.skipTo(docId);
    CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);    
}

void BarrelTermPositionIteratorTestCase::testSkipToWithSpecialSeq()
{
    FX_TRACE("Begin testSkipToWithSpecialSeq");

    m_nTestDataSize = 7750;

    setUpTestData();

    BarrelTermPositionIterator iterator;
    iterator.init(m_pDecoder, NULL);

    size_t testSeq[] = {37, 75, 113, 151, 189, 227, 265, 303, 341, 379, 417, 455, 493, 531, 569, 607, 645, 683, 721, 759, 797, 835, 873, 911, 949, 987, 1025, 1063, 1101, 1139, 1177, 1215, 1253, 1291, 1329, 1367, 1405, 1443, 1481, 1519, 1557, 1595, 1633, 1671, 1709, 1747, 1785, 1823, 1861, 1899, 1937, 1975, 2013, 2051, 2089, 2127, 2165, 2203, 2241, 2279, 2317, 2355, 2393, 2431, 2469, 2507, 2545, 2583, 2621, 2659, 2697, 2735, 2773, 2811, 2849, 2887, 2925, 2963, 3001, 3039, 3077, 3115, 3153, 3191, 3229, 3267, 3305, 3343, 3381, 3419, 3457, 3495, 3533, 3571, 3609, 3647, 3685, 3723, 3761, 3799, 3837, 3875, 3913, 3951, 3989, 4027, 4065, 4103, 4141, 4179, 4217, 4255, 4293, 4331, 4369, 4407, 4445, 4483, 4521, 4559, 4597, 4635, 4673, 4711, 4749, 4787, 4825, 4863, 4901, 4939, 4977, 5015, 5053, 5091, 5129, 5167, 5205, 5243, 5281, 5319, 5357, 5395, 5433, 5471, 5509, 5547, 5585, 5623, 5661, 5699, 5737, 5775, 5813, 5851, 5889, 5927, 5965, 6003, 6041, 6079, 6117, 6155, 6193
    };

    docid_t docId = 0;
    for (size_t i = 0; i < sizeof(testSeq)/sizeof(testSeq[0]); ++i)
    {
        size_t testPos = testSeq[i];
        docId = m_answer[testPos].first - 1;
        if (docId < 0)
        {
            docId = 0;
        }
        docId = iterator.skipTo(docId);
        //CPPUNIT_ASSERT_EQUAL(m_answer[testPos].first, docId);
        assert(m_answer[testPos].first ==  docId);
        CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[testPos].second.size(), iterator.freq());
        ++testPos;
    }

    tearDownTestData();
}

void BarrelTermPositionIteratorTestCase::testSkipToRandomly()
{
    FX_TRACE("Begin testSkipToRandomly");

    const static size_t TEST_TIMES = 10000;
    for (size_t i = 1; i < TEST_TIMES; i += 123)
    {
        m_nTestDataSize = i;

        FX_TRACE("Test data size: %u", (uint32_t)m_nTestDataSize);
        setUpTestData();

        BarrelTermPositionIterator iterator;
        iterator.init(m_pDecoder, NULL);

        docid_t docId = 0;
        size_t testPos = 0;
        for (; testPos < m_nTestDataSize;)
        {
            srand((unsigned int)time(NULL));
            testPos += rand() % 100;
            if (testPos >= m_nTestDataSize)
            {
                testPos = m_nTestDataSize - 1;
            }
            docId = m_answer[testPos].first - 1;
            if (docId < 0)
            {
                docId = 0;
            }
            docId = iterator.skipTo(docId);
            //CPPUNIT_ASSERT_EQUAL(m_answer[testPos].first, docId);
            assert(m_answer[testPos].first ==  docId);
            CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[testPos].second.size(), iterator.freq());
            ++testPos;
        }

        docId = m_answer[m_nTestDataSize - 1].first + 3;
        docId = iterator.skipTo(docId);
        CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);

        tearDownTestData();
    }
}

void BarrelTermPositionIteratorTestCase::getCTF(const DocVector& answer,
        size_t startOff, size_t endOff, ctf_t& ctf)
{
    for (size_t i = startOff; i < endOff; ++i)
    {
        ctf += answer[i].second.size();
    }
}

void BarrelTermPositionIteratorTestCase::testSkipToPosition()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPositionIterator iterator;
    iterator.init(m_pDecoder, NULL);

    docid_t docId = 0;
    size_t testElems[] = {0, 125, 255, 789, 998, 999};
    size_t testPos[] = {
        1, 0, //tf: 14
        2, 1, 7, //tf: 8
        2, 4, 6, //tf: 7
        3, 1, 10, 15, //tf: 17
        4, 2, 50, 60, 90, //tf: 95
        6, 2, 50, 60, 90, 92, 95//tf: 96
    };

    size_t cur = 0;    
    for (size_t i = 0; i < sizeof(testElems) / sizeof(testElems[0]); ++i)
    {
        size_t testDoc = testElems[i];
        docId = m_answer[testDoc].first - 3;
        if (docId < 0)
        {
            docId = 0;
        }
        docId = iterator.skipTo(docId);
        CPPUNIT_ASSERT_EQUAL(m_answer[testDoc].first, docId);
        CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[testDoc].second.size(), iterator.freq());
        
        ctf_t ctf = 0;
        getCTF(m_answer, 0, testDoc, ctf);
        FX_TRACE("ctf: %lld", ctf);

        size_t count = testPos[cur];
        ++cur;
        for (size_t k = 0; k < count; ++k)
        {
            loc_t t = m_answer[testDoc].second[testPos[cur + k]] - (k % 2);
            loc_t pos = iterator.skipToPosition(t);
            CPPUNIT_ASSERT_EQUAL(m_answer[testDoc].second[testPos[cur + k]], pos);
        }
        cur += count;
    }

    docId = m_answer[999].first + 3;
    docId = iterator.skipTo(docId);
    CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);    
}

void BarrelTermPositionIteratorTestCase::generateInputData(InputVector& input)
{
    size_t testDoc = 0;
    while (testDoc < m_nTestDataSize)
    {
        InputPair inPair;
        srand((unsigned int)time(NULL));
        testDoc += rand() % 100;
        if (testDoc >= m_nTestDataSize)
        {
            testDoc = m_nTestDataSize - 1;
        }

        inPair.first = testDoc;

        ctf_t ctf = 0;
        getCTF(m_answer, 0, testDoc, ctf);

        size_t maxCount = (rand() % m_answer[testDoc].second.size()) + 1;
        size_t p = 0;
        for (size_t k = 0; k < maxCount && p < maxCount - 1; ++k)
        {
            p += rand() % maxCount;
            if (p >= maxCount)
            {
                p = maxCount - 1;
            }
            inPair.second.push_back(p);
        }

        ++testDoc;
    }
}

void BarrelTermPositionIteratorTestCase::testSkipToPositionRandomly()
{
    FX_TRACE("Begin testSkipToPositionRandomly");

    const static size_t TEST_TIMES = 10000;
    for (size_t dataSize = 1; dataSize < TEST_TIMES; dataSize += 123)
    {
        m_nTestDataSize = dataSize;

        FX_TRACE("Test data size: %u", (uint32_t)m_nTestDataSize);
        setUpTestData();
        
        InputVector input;
        generateInputData(input);

        BarrelTermPositionIterator iterator;
        iterator.init(m_pDecoder, NULL);

        docid_t docId = 0;
        for (size_t i = 0; i < input.size(); ++i)
        {
            size_t testDoc = input[i].first;
            docId = m_answer[testDoc].first - 1;
            if (docId < 0)
            {
                docId = 0;
            }
            docId = iterator.skipTo(docId);
            //CPPUNIT_ASSERT_EQUAL(m_answer[testDoc].first, docId);
            assert(m_answer[testDoc].first ==  docId);
            CPPUNIT_ASSERT_EQUAL((tf_t)m_answer[testDoc].second.size(), iterator.freq());

            ctf_t ctf = 0;
            getCTF(m_answer, 0, testDoc, ctf);
            FX_TRACE("ctf: %lld", ctf);
            
            for (size_t k = 0; k < input[i].second.size(); ++k)
            {
                size_t testPos = input[i].second[k];
                loc_t t = m_answer[testDoc].second[testPos] - (k % 2);
                loc_t pos = iterator.skipToPosition(t);
                CPPUNIT_ASSERT_EQUAL(m_answer[testDoc].second[testPos], pos);
//                assert(m_answer[testDoc].second[p] == pos);
            }
        }

        docId = m_answer[m_nTestDataSize - 1].first + 3;
        docId = iterator.skipTo(docId);
        CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);

        tearDownTestData();
    }
}

FX_NS_END

