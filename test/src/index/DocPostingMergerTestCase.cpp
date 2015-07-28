#include "index/DocPostingMergerTestCase.h"
#include "firtex/utility/Pool.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/DocPostingMerger.h"
#include "firtex/utility/LooseBoundedPool.h"
#include <vector>
#include <map>

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocPostingMergerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DocPostingMergerTestCase);

class FakeTermPostingIterator : public TermPostingIterator
{
public:
    FakeTermPostingIterator(df_t df, ctf_t ctf, DocPostingDecoderPtr& pDecoder)
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

struct PostingData
{
    PostingData(size_t dataSize)
        : m_nTestDataSize(dataSize)
        , maxDocId(0)
        , docFreq(dataSize)
        , totalTF(0)
    {
        setUpTestData();
    }

    ~PostingData()
    {
        tearDownTestData();
    }

    typedef std::pair<docid_t, tf_t> DocPair;
    typedef std::vector<DocPair> DocVector;

    void setUpTestData()
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

    void tearDownTestData()
    {
        m_answer.clear();
        m_pDecoder.reset();

        m_pDocInStream.reset();
        m_pDocOutStream.reset();

        m_pPostingWriter.reset();
        m_pPostingPool.reset();
    }

    void makeData(DocVector& answer, DocPostingWriterPtr& pWriter, size_t dataSize)
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
            tf_t tf = (tf_t)(i + 13) % 1981;
            totalTF += tf;
            answer[i].first = docId;
            answer[i].second = tf;
            for (tf_t j = 0; j < tf; ++j)
            {
                pWriter->addPosition(0);
            }
            pWriter->commitDocument(docId);
        }
    }

    PostingPoolPtr m_pPostingPool;
    DocPostingWriterPtr m_pPostingWriter;

    ByteSliceOutputStreamPtr m_pDocOutStream;
    ByteSliceOutputStreamPtr m_pPosOutStream;

    ByteSliceInputStreamPtr m_pDocInStream;
    ByteSliceInputStreamPtr m_pPosInStream;
    
    DocPostingDecoderPtr m_pDecoder;

    DocVector m_answer;
    size_t m_nTestDataSize;

    DocIdRecyclingPtr pDocIdRecycling;
    BitVectorPtr pDocFilter;

    df_t maxDocId;
    df_t docFreq;
    ctf_t totalTF;
};

DocPostingMergerTestCase::DocPostingMergerTestCase() 
    : m_nNumMergeTerms(5)
    , m_bHasDeletion(false)
{
}

DocPostingMergerTestCase::~DocPostingMergerTestCase() 
{
}

void DocPostingMergerTestCase::setUp()
{
    FX_TRACE("setUp");

    m_pBarrelsInfo = new BarrelsInfo();

    docid_t baseDocId = 0;
    docid_t newBaseDocId = 0;
    for (size_t i = 0; i < m_nNumMergeTerms; ++i)
    {
        df_t df = i * 134 + 1;
        FX_TRACE("doc count: %d, base docid: %d, new base docid: %d",
                        df, baseDocId, newBaseDocId);
        PostingDataPtr pData = new PostingData(df);
        m_postingHolder.push_back(pData);

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
        

    m_pPool = new LooseBoundedPool(10 * 1024 * 1024);
    m_pMergedStream = new ByteSliceOutputStream(m_pPool.get());
}

void DocPostingMergerTestCase::tearDown()
{
    FX_TRACE("tearDown");

    m_mergingTerms.clear();
    m_termsPtr.clear();
    m_postingHolder.clear();
    m_pBarrelsInfo.reset();

    m_pMergedStream.reset();
    m_pPool.reset();
}

void DocPostingMergerTestCase::testMerge()
{
    FX_TRACE("testMerge");

    tearDown();

    const static size_t TEST_TIMES = 10;
    for (m_nNumMergeTerms = 2; m_nNumMergeTerms < TEST_TIMES; ++m_nNumMergeTerms)
    {
        FX_TRACE("test merge %d terms", m_nNumMergeTerms);
        setUp();

        DocPostingMerger merger;
        merger.init(m_pMergedStream);
        offset_t nStartOff = merger.merge(m_mergingTerms);

        checkMergeResult(nStartOff);

        tearDown();
    }
}

void DocPostingMergerTestCase::testMergeWithDeletion()
{
    FX_TRACE("testMergeWithDeletion");

    tearDown();

    const static size_t TEST_TIMES = 10;//50
    for (m_nNumMergeTerms = 2; m_nNumMergeTerms < TEST_TIMES; ++m_nNumMergeTerms)
    {
        m_bHasDeletion = true;
        FX_TRACE("test merge %d terms", m_nNumMergeTerms);
        setUp();

        DocPostingMerger merger;
        merger.init(m_pMergedStream);
        offset_t nStartOff = merger.merge(m_mergingTerms);

        checkMergeResult(nStartOff);

        tearDown();
    }
}

void DocPostingMergerTestCase::checkMergeResult(offset_t nStartOff)
{
    PostingData::DocVector mergedAnswer;
    ctf_t totalTF = 0;
    size_t i = 0;
    docid_t baseDocId = 0;
    for (std::vector<PostingDataPtr>::const_iterator it = m_postingHolder.begin();
         it != m_postingHolder.end(); ++it)
    {
        PostingDataPtr pPostingData = *it;
        if (!pPostingData->pDocFilter)
        {
            baseDocId = (*m_pBarrelsInfo)[i].getBaseDocId();
        }

        const PostingData::DocVector& answer = pPostingData->m_answer;
        for (PostingData::DocVector::const_iterator it2 = answer.begin(); 
             it2 != answer.end(); ++it2)
        {
            tf_t tf = it2->second;
            docid_t docId = it2->first;
            if (pPostingData->pDocFilter && 
                (pPostingData->pDocFilter->test(docId)))
            {
                continue;
            }

            if (pPostingData->pDocIdRecycling &&
                pPostingData->pDocIdRecycling->hasDeletions())
            {
                docId = (*(pPostingData->pDocIdRecycling))[docId];
            }
            docId += baseDocId;
            mergedAnswer.push_back(make_pair(docId, tf));
            totalTF += tf;
        }
        if (pPostingData->pDocFilter)
        {
            baseDocId += (pPostingData->maxDocId + 1 - pPostingData->pDocFilter->count());
        }
        ++i;
    }

    ByteSliceInputStreamPtr pDocInStream = new ByteSliceInputStream(m_pMergedStream->getHeadSlice(), false);
    InputStreamPtr pDocInStreamPtr = pDocInStream.cast<InputStream>();
    pDocInStream->seek(nStartOff);

    DocPostingDecoder decoder;    
    decoder.init(pDocInStreamPtr, NULL, NULL);
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
            CPPUNIT_ASSERT_EQUAL(mergedAnswer[j + k].second, tfBuffer[k]);
        }
    }
}

FX_NS_END

