#include "index/PosPostingMergerTestCase.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/index/PosPostingDecoder.h"
#include "firtex/index/PosPostingMerger.h"
#include "firtex/utility/LooseBoundedPool.h"
#include <vector>
#include <map>

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosPostingMergerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(PosPostingMergerTestCase);

class FakeTermPostingIterator : public TermPostingIterator
{
public:
    FakeTermPostingIterator(df_t df, ctf_t ctf, PosPostingDecoderPtr& pDecoder)
        : m_termMeta(df, ctf)
        , m_pPostingDecoder(pDecoder)
    {
    }

public:
    const TermMeta& getTermMeta() const {return m_termMeta;}
    df_t nextDocs(docid_t*& docs, tf_t*& freqs) {return 0;}
    docid_t skipTo(docid_t docId) { return INVALID_DOCID;}
    docid_t doc() const { return INVALID_DOCID;}
    tf_t freq() {return 0;}
    tf_t nextPositions(loc_t*& positions) { return 0;}
    loc_t skipToPosition(loc_t pos) { return INVALID_POS;};

    PostingDecoderPtr getPostingDecoder() {return m_pPostingDecoder;}

protected:
    TermMeta m_termMeta;
    PostingDecoderPtr m_pPostingDecoder;
};

class FakeTermIterator : public TermIterator
{
public:
    FakeTermIterator(const TermPostingIteratorPtr& it)
        : m_pPostIterator(it)
    {
    }

public:

    bool hasNext() {return true;}
    TermEntry next() { return TermEntry(NULL, m_pPostIterator);}
    int64_t size() const { return 1;}

protected:
    TermPostingIteratorPtr  m_pPostIterator;
};

class FakeTermReader : public TermReader
{
public:
    FakeTermReader(TermIteratorPtr& it)
        : m_pTermIterator(it)
    {
    }

public:
    void reopen(const tstring& sBarrel, 
                FX_NS(store)::FileSystemPtr& pFileSystem) {}

    TermIteratorPtr termIterator(const tstring& sField) const
    {
        return m_pTermIterator;
    }

    TermIteratorPtr termIterator(const Term* pLowerTerm,
                                 const Term* pUpperTerm) const 
    {
        return TermIteratorPtr();
    }

    TermPostingIteratorPtr seek(const Term* pTerm) const
    {
        return TermPostingIteratorPtr();
    }

    TermReader* clone() const {return NULL;}

protected:
    TermIteratorPtr m_pTermIterator;
};

struct PosPostingData
{
    PosPostingData(size_t dataSize)
        : m_nTestDataSize(dataSize)
        , maxDocId(0)
        , docFreq(dataSize)
        , totalTF(0)
    {
        setUpTestData();
    }

    ~PosPostingData()
    {
        tearDownTestData();
    }

    typedef std::pair<docid_t, std::vector<loc_t> > DocPair;
    typedef std::vector<DocPair> DocVector;

    void setUpTestData()
    {
        m_pPool.assign(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                        new LooseBoundedPool(10 * 1024 * 1024)));

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

    void tearDownTestData()
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

    void makeData(DocVector& answer, PosPostingWriterPtr& pWriter, size_t dataSize)
    {
        totalTF = 0;
        answer.resize(dataSize);
        for (size_t i = 0; i < dataSize; ++i)
        {
            docid_t docId = (docid_t)(i * 13 + (i % 12));
            if (docId > maxDocId)
            {
                maxDocId = docId;
            }
            tf_t tf = (tf_t)(i + 13) % 191 + 1;
            totalTF += tf;
            answer[i].first = docId;
            answer[i].second.resize(tf);
            for (tf_t j = 0; j < tf; ++j)
            {
                loc_t pos = (loc_t) (j * j + j);
                answer[i].second[j] = pos;
                pWriter->addPosition(pos);
            }
            pWriter->commitDocument(docId);
        }
    }

    PostingPoolPtr m_pPool;
    PosPostingWriterPtr m_pPostingWriter;

    ByteSliceOutputStreamPtr m_pDocOutStream;
    ByteSliceOutputStreamPtr m_pPosOutStream;

    ByteSliceInputStreamPtr m_pDocInStream;
    ByteSliceInputStreamPtr m_pPosInStream;
    
    PosPostingDecoderPtr m_pDecoder;

    DocVector m_answer;
    size_t m_nTestDataSize;

    DocIdRecyclingPtr pDocIdRecycling;
    BitVectorPtr pDocFilter;

    df_t maxDocId;
    df_t docFreq;
    ctf_t totalTF;
};



PosPostingMergerTestCase::PosPostingMergerTestCase() 
    : m_nNumMergeTerms(5)
    , m_bHasDeletion(false)
{
}

PosPostingMergerTestCase::~PosPostingMergerTestCase() 
{
}

void PosPostingMergerTestCase::setUp()
{
    FX_TRACE("setUp");

    m_pBarrelsInfo = new BarrelsInfo();

    docid_t baseDocId = 0;
    docid_t newBaseDocId = 0;
    for (size_t i = 0; i < m_nNumMergeTerms; ++i)
    {
        df_t df = i * 134 + 1;
        PosPostingDataPtr pData = new PosPostingData(df);
        m_postingHolder.push_back(pData);

        FX_TRACE("doc count: %d, pos count %lld, base docid: %d, new base docid: %d",
                        df, pData->totalTF, baseDocId, newBaseDocId);

        FakeTermPostingIterator* pFakePostIt = new FakeTermPostingIterator(
                pData->docFreq, pData->totalTF, pData->m_pDecoder);

        TermPostingIteratorPtr pTmpPtr(pFakePostIt);
        FakeTermIterator* pFakeIt = new FakeTermIterator(pTmpPtr);
        TermIteratorPtr pTermIterator(pFakeIt);
        TermReaderPtr pTermReader = new FakeTermReader(pTermIterator);

        BarrelInfo& barrelInfo = m_pBarrelsInfo->newBarrel();
        barrelInfo.setBaseDocId(baseDocId);
        barrelInfo.setDocCount(pData->maxDocId + 1);
        MergingTermPtr mergingTerm;
        if (!m_bHasDeletion)
        {
            DocIdRecyclingPtr pDocIdMap(new DocIdRecycling());
            pDocIdMap->init(pData->maxDocId + 1, NULL);

            IndexMergeInfo mergeFieldInfo(barrelInfo, pDocIdMap, 
                    newBaseDocId, pTermReader);

            mergingTerm.assign(new MergingTerm(mergeFieldInfo));

            newBaseDocId += (docid_t)pData->maxDocId + 1;
        }
        else
        {
            BitVectorPtr delDocs = new BitVector();
            DocIdRecyclingPtr pDocIdMap(new DocIdRecycling());
            pDocIdMap->init(pData->maxDocId + 1, delDocs.get());

            pData->pDocIdRecycling = pDocIdMap;
            pData->pDocFilter = delDocs;

            IndexMergeInfo mergeFieldInfo(barrelInfo, pDocIdMap, 
                    newBaseDocId, pTermReader);
            mergingTerm.assign(new MergingTerm(mergeFieldInfo));

            newBaseDocId += ((docid_t)pData->maxDocId + 1 - delDocs->count());
        }
        m_termsPtr.push_back(mergingTerm);

        mergingTerm->next();
        m_mergingTerms.push_back(mergingTerm.get());

        baseDocId += (docid_t)pData->maxDocId + 1;
    }

    m_pPool = new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                              new LooseBoundedPool(10 * 1024 * 1024));

    m_pMergedDocStream = new ByteSliceOutputStream(m_pPool->postingPool);
    m_pMergedPosStream = new ByteSliceOutputStream(m_pPool->postingPool);
}

void PosPostingMergerTestCase::tearDown()
{
    FX_TRACE("tearDown");

    m_mergingTerms.clear();
    m_termsPtr.clear();
    m_postingHolder.clear();
    m_pBarrelsInfo.reset();

    m_pMergedDocStream.reset();
    m_pMergedPosStream.reset();
    m_pPool.reset();
}

void PosPostingMergerTestCase::testMerge()
{
    FX_TRACE("testMerge");

    tearDown();

    const static size_t TEST_TIMES = 10;
    for (m_nNumMergeTerms = 2; m_nNumMergeTerms < TEST_TIMES; 
         ++m_nNumMergeTerms)
         //m_nNumMergeTerms += 6)
    {
        FX_TRACE("test merge %d terms", m_nNumMergeTerms);
        setUp();

        PosPostingMerger merger;
        merger.init(m_pMergedDocStream, m_pMergedPosStream);

        offset_t nStartOff = merger.merge(m_mergingTerms);
        checkMergeResult(nStartOff);

        tearDown();
    }
}

void PosPostingMergerTestCase::testMergeWithDeletion()
{
    FX_TRACE("testMergeWithDeletion");

    tearDown();

    const static size_t TEST_TIMES = 8;
    for (m_nNumMergeTerms = 2; m_nNumMergeTerms < TEST_TIMES;
         ++m_nNumMergeTerms)
         //m_nNumMergeTerms += 7)
    {
        m_bHasDeletion = true;
        FX_TRACE("test merge %d terms", m_nNumMergeTerms);
        setUp();

        PosPostingMerger merger;
        merger.init(m_pMergedDocStream, m_pMergedPosStream);
        offset_t nStartOff = merger.merge(m_mergingTerms);
        checkMergeResult(nStartOff);
        tearDown();
    }
}

void PosPostingMergerTestCase::checkMergeResult(offset_t nStartOff)
{
    PosPostingData::DocVector mergedAnswer;
    ctf_t totalTF = 0;
    size_t i = 0;
    docid_t baseDocId = 0;
    for (std::vector<PosPostingDataPtr>::const_iterator it = m_postingHolder.begin();
         it != m_postingHolder.end(); ++it)
    {
        PosPostingDataPtr pPosPostingData = *it;
        if (pPosPostingData->pDocFilter.isNull())
        {
            baseDocId = (*m_pBarrelsInfo)[i].getBaseDocId();
        }

        const PosPostingData::DocVector& answer = pPosPostingData->m_answer;
        for (PosPostingData::DocVector::const_iterator it2 = answer.begin(); 
             it2 != answer.end(); ++it2)
        {
            tf_t tf = (tf_t)it2->second.size();
            docid_t docId = it2->first;
            if (pPosPostingData->pDocFilter.isNotNull() && 
                (pPosPostingData->pDocFilter->test(docId)))
            {
                continue;
            }

            if (pPosPostingData->pDocIdRecycling.isNotNull() && 
                pPosPostingData->pDocIdRecycling->hasDeletions())
            {
                docId = (*(pPosPostingData->pDocIdRecycling))[docId];
            }
            docId += baseDocId;
            mergedAnswer.push_back(make_pair(docId, it2->second));
            totalTF += tf;
        }
        if (pPosPostingData->pDocFilter.isNotNull())
        {
            baseDocId += (pPosPostingData->maxDocId + 1 -
                          pPosPostingData->pDocFilter->count());
        }
        ++i;
    }

    ByteSliceInputStreamPtr pDocInStream = new ByteSliceInputStream(
            m_pMergedDocStream->getHeadSlice(), false);
    ByteSliceInputStreamPtr pPosInStream = new ByteSliceInputStream(
            m_pMergedPosStream->getHeadSlice(), false);
    InputStreamPtr pDocInStreamPtr = pDocInStream.cast<InputStream>();
    InputStreamPtr pPosInStreamPtr = pPosInStream.cast<InputStream>();
    pDocInStream->seek(nStartOff);

    PosPostingDecoder decoder;    
    decoder.init(pDocInStreamPtr, pPosInStreamPtr, NULL);
    const TermMeta& termMeta = decoder.getTermMeta();
    CPPUNIT_ASSERT_EQUAL(mergedAnswer.size(), (size_t)termMeta.getDocFreq());
    CPPUNIT_ASSERT_EQUAL(totalTF, termMeta.getCTF());

    docid_t docBuffer[RECORD_SIZE];
    tf_t tfBuffer[RECORD_SIZE];
    for (size_t j = 0; j < mergedAnswer.size(); j += 128)
    {
        size_t dataSize = (mergedAnswer.size() - j) > 128 ? 128 : (mergedAnswer.size() - j);
        docid_t docId = mergedAnswer[j].first;
        size_t ret = decoder.decodeDocRecord(docBuffer, docId);
        CPPUNIT_ASSERT_EQUAL(dataSize, ret);
        size_t ret2 = decoder.decodeTfRecord(tfBuffer);
        CPPUNIT_ASSERT_EQUAL(dataSize, ret2);

        for (size_t k = 0; k < dataSize; ++k)
        {
            assert(mergedAnswer[j + k].first == docBuffer[k]);
            CPPUNIT_ASSERT_EQUAL(mergedAnswer[j + k].first, docBuffer[k]);
            CPPUNIT_ASSERT_EQUAL((tf_t)mergedAnswer[j + k].second.size(), tfBuffer[k]);
        }
    }

    loc_t posBuffer[RECORD_SIZE];
    size_t curDoc = 0;
    size_t curPos = 0;
    loc_t lastPos = 0;
    for (size_t pos = 0; pos < (size_t)totalTF; pos += RECORD_SIZE)
    {
        uint32_t startOff = 0;
        size_t ret3 = decoder.decodePosRecord(posBuffer, pos, startOff);
        if (pos + RECORD_SIZE <= (size_t)totalTF)
        {
            CPPUNIT_ASSERT_EQUAL((size_t)RECORD_SIZE, ret3);
        }
        else 
        {
            CPPUNIT_ASSERT_EQUAL((size_t)(totalTF - pos), ret3);
        }
        for (size_t k = 0; k < ret3; ++k)
        {
            if (curPos >= mergedAnswer[curDoc].second.size())
            {
                ++curDoc;
                curPos = 0;
                lastPos = 0;
            }
            lastPos += posBuffer[k];
            CPPUNIT_ASSERT_EQUAL(mergedAnswer[curDoc].second[curPos++], lastPos);
        }
    }
}

FX_NS_END

