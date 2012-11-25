#include "index/BarrelTermPostingIteratorTestCase.h"
#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/utility/LooseBoundedPool.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, BarrelTermPostingIteratorTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(BarrelTermPostingIteratorTestCase);

BarrelTermPostingIteratorTestCase::BarrelTermPostingIteratorTestCase() 
    : m_nTestDataSize(1000)
{
}

BarrelTermPostingIteratorTestCase::~BarrelTermPostingIteratorTestCase() 
{
}

void BarrelTermPostingIteratorTestCase::setUp()
{

}

void BarrelTermPostingIteratorTestCase::tearDown()
{
    tearDownTestData();
}

void BarrelTermPostingIteratorTestCase::setUpTestData()
{
    m_pPostingPool = new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
            new LooseBoundedPool(10 * 1024 * 1024));

    m_pPostingWriter = new DocPostingWriter();
    m_pPostingWriter->init(m_pPostingPool.get());

    makeData(m_answer, m_pPostingWriter, m_nTestDataSize);

    m_pDocOutStream = new ByteSliceOutputStream(m_pPostingPool->postingPool);

    OutputStreamPtr pDocStreamPtr = m_pDocOutStream.cast<OutputStream>();

    offset_t startOff = m_pPostingWriter->commit(pDocStreamPtr);

    m_pDocInStream = new ByteSliceInputStream(m_pDocOutStream->getHeadSlice(), false);
    InputStreamPtr pDocInStreamPtr = m_pDocInStream.cast<InputStream>();
    m_pDocInStream->seek(startOff);
    
    m_pDecoder = new DocPostingDecoder();
    m_pDecoder->init(pDocInStreamPtr, NULL, NULL);
}

void BarrelTermPostingIteratorTestCase::tearDownTestData()
{
    m_answer.clear();
    m_pDecoder.reset();

    m_pDocInStream.reset();
    m_pDocOutStream.reset();

    m_pPostingWriter.reset();
    m_pPostingPool.reset();
}

void BarrelTermPostingIteratorTestCase::makeData(
        DocVector& answer, DocPostingWriterPtr& pWriter, size_t dataSize)
{
    for (size_t i = 0; i < dataSize; ++i)
    {
        docid_t docId = (docid_t)(i * 13 + (i % 12));
        tf_t tf = (tf_t)(i + 13) % 1981;
        answer.push_back(std::make_pair(docId, tf));
        for (tf_t j = 0; j < tf; ++j)
        {
            pWriter->addPosition(0);
        }
        pWriter->commitDocument(docId);
    }
}

void BarrelTermPostingIteratorTestCase::testTermMeta()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPostingIterator iterator;
    iterator.init(m_pDecoder, NULL);

    const TermMeta& termMeta = iterator.getTermMeta();
    CPPUNIT_ASSERT_EQUAL((df_t)1000, termMeta.getDocFreq());
    ctf_t ctf = 0;
    for (size_t i = 0; i < m_answer.size(); ++i)
    {
        ctf += m_answer[i].second;
    }
    CPPUNIT_ASSERT_EQUAL(ctf, termMeta.getCTF());
}

void BarrelTermPostingIteratorTestCase::testSkipTo()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPostingIterator iterator;
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
        CPPUNIT_ASSERT_EQUAL(m_answer[testPos].second, iterator.freq());
    }

    docId = m_answer[999].first + 3;
    docId = iterator.skipTo(docId);
    CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);
}

void BarrelTermPostingIteratorTestCase::testNextDocs()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPostingIterator iterator;
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
            CPPUNIT_ASSERT_EQUAL(m_answer[dataSize].second, freqs[i]);
            ++dataSize;
        }
    }
    CPPUNIT_ASSERT_EQUAL((df_t)m_nTestDataSize, dataSize);
    df_t ret = iterator.nextDocs(docs, freqs);
    CPPUNIT_ASSERT_EQUAL((df_t)0, ret);
}

void BarrelTermPostingIteratorTestCase::testSkipToAndNextDocs()
{
    m_nTestDataSize = 1000;
    setUpTestData();

    BarrelTermPostingIterator iterator;
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
        CPPUNIT_ASSERT_EQUAL(m_answer[testPos].second, iterator.freq());

        size_t answerPos = testPos;
        uint32_t ret = iterator.nextDocs(pDocs, pFreqs);
        CPPUNIT_ASSERT_EQUAL((uint32_t)testElems[i + 1], ret);
        for (uint32_t j = 0; j < ret; ++j)
        {
            CPPUNIT_ASSERT_EQUAL(m_answer[answerPos].first, pDocs[j]);
            CPPUNIT_ASSERT_EQUAL(m_answer[answerPos].second, pFreqs[j]);
            ++answerPos;
        }
    }

    docId = m_answer[999].first + 3;
    docId = iterator.skipTo(docId);
    CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);    
}

void BarrelTermPostingIteratorTestCase::testSkipToRandomly()
{
    const static size_t TEST_TIMES = 10000;
    for (size_t i = 1; i < TEST_TIMES; i += 123)
    {
        m_nTestDataSize = i;
        setUpTestData();

        BarrelTermPostingIterator iterator;
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
            CPPUNIT_ASSERT_EQUAL(m_answer[testPos].second, iterator.freq());
            ++testPos;
        }

        docId = m_answer[m_nTestDataSize - 1].first + 3;
        docId = iterator.skipTo(docId);
        CPPUNIT_ASSERT_EQUAL(INVALID_DOCID, docId);

        tearDownTestData();
    }
}

FX_NS_END

