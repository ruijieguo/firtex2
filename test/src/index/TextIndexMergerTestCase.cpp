#include "index/TextIndexMergerTestCase.h"
#include "firtex/index/text/TextTermIterator.h"
#include "firtex/utility/File.h"
#include "TestHelper.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/analyzer/TokenView.h"
#include <limits>
#include <vector>

using namespace std;

FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(store);

FX_NS_DEF(index);

class FakeFieldComponentCreator : public FieldComponentCreator
{		
public:
    FakeFieldComponentCreator(void) {}

public:
    FieldIndexer* createIndexer(const FieldSchema* pFieldSchema) const {return NULL;}
    FieldMerger* createMerger() const { return NULL;}
    TermReader* createTermReader() const {return new TextTermReader();}
};

static FakeFieldComponentCreator FAKE_COMP_CREATOR; 

SETUP_STREAM_LOGGER(index, TextIndexMergerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(TextIndexMergerTestCase);

TextIndexMergerTestCase::MergeEntry::MergeEntry()
{
    pFieldSchema.assign(new FieldSchema("testfield", NULL, false));
    pBarrelInfo.assign(new BarrelInfo());
    pBarrelInfo->getIndexMeta().fieldMeta("testfield").totalTermCount = 0;

    pPool.assign(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                          new LooseBoundedPool(10 * 1024 * 1024)));

    pIndexer = new TextIndexer(pFieldSchema.get());
    pIndexer->init(pPool);
    pTermReader = new TextTermReader();
}

TextIndexMergerTestCase::MergeEntry::~MergeEntry()
{
    pBarrelInfo.reset();
    pFieldSchema.reset();
    pIndexer.reset();
    pPool.reset();
    pTermReader.reset();

    pFileSystem.reset();
}

void TextIndexMergerTestCase::MergeEntry::makeData()
{
    for (size_t i = 0; i < nDataSize; ++i)
    {
        int32_t nPosInc = 0;
        TokenViewPtr pTokenView = new TokenView(i + 1, Token::TT_CWORD);
        for (size_t j = 0; j < i + 1; ++j)
        {
            stringstream ss;
            ss << "abc" << (i + j) % 100;
            pTokenView->addToken(ss.str().c_str(), nPosInc, 
                    0, 0, Token::TT_CWORD);
            nPosInc = 1;

            uint64_t hash = Hash::hashString64(ss.str().c_str());
            TermMap::iterator it = answer.find(hash);
            if (it != answer.end())
            {
                DocVector& docVect = it->second;
                if (docVect[docVect.size() - 1].first == (docid_t)i)
                {
                    docVect[docVect.size() - 1].second.push_back(j);
                }
                else 
                {
                    std::vector<loc_t> pos;
                    pos.push_back(j);
                    docVect.push_back(make_pair(i, pos));
                }
            }
            else
            {
                DocVector docVect;
                docVect.resize(1);
                docVect[0].first = i;
                docVect[0].second.push_back(j);
                answer.insert(make_pair(hash, docVect));
            }
        }
        //pFieldInfo->getIndexInfo().totalTerms += pTokenView->getNumTokens();
        AnalyzedFieldPtr pField = new AnalyzedField(NULL, pTokenView);
        pIndexer->addField(pField.get());
        pIndexer->commitDocument((docid_t)i);

        if (pDocFilter)
        {
            if (i % 5 == 0 || i == (nDataSize - 1))
            {
                pDocFilter->set(i);
            }
        }
    }
    //pFieldInfo->getIndexInfo().distinctTerms = nDataSize;
    BarrelDirectory::createBarrel(pFileSystem, pBarrelInfo->getSuffix());
    pIndexer->commit(pFileSystem, pBarrelInfo->getSuffix());

    if (pDocFilter)
    {
        pDocIdRemapper = new DocIdRecycling();
        pDocIdRemapper->init(nDataSize, pDocFilter.get());
    }
}

TextIndexMergerTestCase::TextIndexMergerTestCase() 
    : m_nNumEntries(2)
    , m_bHasDeletions(false)
{
}

TextIndexMergerTestCase::~TextIndexMergerTestCase() 
{
}

void TextIndexMergerTestCase::setUp()
{
    m_pFileSystem = new BlockFileSystem();
    string sFilePath = TestHelper::getTestDataPath("text_term_merge_test", true);
    m_pFileSystem->open(sFilePath, FileSystem::CREATE);
    m_pStreamPool = new InputStreamPool(m_pFileSystem);

    m_fieldSchema.setName("testfield");
    m_pTextMerger = new TextIndexMerger();

    m_pTextMerger->init(m_pFileSystem, &m_fieldSchema);
}

void TextIndexMergerTestCase::tearDown()
{
    m_mergeEntries.clear();
    m_pTextMerger.reset();
    m_pStreamPool.reset();
    m_pFileSystem.reset();
    string sFilePath = TestHelper::getTestDataPath("text_term_merge_test", true);
    File f(sFilePath);
    if (f.exists())
    {
        f.remove(true);
    }
}

void TextIndexMergerTestCase::makeData()
{
    docid_t docId = 0;
    for (size_t i = 0; i < m_nNumEntries; ++i)
    {
        MergeEntryPtr pEntry = new MergeEntry();

        FX_TRACE("Make data: base docid: %d, doc count: %d",
                        docId, (i + 1) * 56);
        pEntry->nDataSize = (i + 1) * 56;

        pEntry->pFileSystem = m_pFileSystem;
        pEntry->pStreamPool = new InputStreamPool(m_pFileSystem);
        pEntry->pBarrelInfo->setBaseDocId(docId);
        pEntry->pBarrelInfo->setDocCount(pEntry->nDataSize);
        pEntry->pBarrelInfo->setCommitId((commitid_t)i);
        pEntry->newBaseDocId = docId;
        if (m_bHasDeletions)
        {
            pEntry->pDocFilter = new BitVector();
        }
        pEntry->makeData();
        m_mergeEntries.push_back(pEntry);

        docId += (docid_t)pEntry->nDataSize;
        if (pEntry->pDocFilter)
        {
            docId -= pEntry->pDocFilter->count();
        }
    }
}

void TextIndexMergerTestCase::makeMergedAnswer(TermMap& mergedAnswer)
{
    for (size_t j = 0; j < m_nNumEntries; ++j)
    {
        TermMap& ans = m_mergeEntries[j]->answer;
        for (TermMap::const_iterator it2 = ans.begin(); 
             it2 != ans.end(); ++it2)
        {
            for (size_t l = 0; l < it2->second.size(); ++l)
            {
                if (m_mergeEntries[j]->pDocIdRemapper)
                {
                    docid_t docId = it2->second[l].first;
                    if (!m_mergeEntries[j]->pDocFilter->test((size_t)docId))
                    {
                        docId = (*(m_mergeEntries[j]->pDocIdRemapper))[docId];
                        TermMap::iterator it = mergedAnswer.find(it2->first);
                        if (it == mergedAnswer.end())
                        {
                            DocVector docVect;
                            mergedAnswer.insert(make_pair(it2->first, docVect));
                            it = mergedAnswer.find(it2->first);
                        }

                        it->second.push_back(make_pair(docId + m_mergeEntries[j]->newBaseDocId,
                                        it2->second[l].second));
                    }
                }
                else
                {
                    TermMap::iterator it = mergedAnswer.find(it2->first);
                    if (it == mergedAnswer.end())
                    {
                        DocVector docVect;
                        mergedAnswer.insert(make_pair(it2->first, docVect));
                        it = mergedAnswer.find(it2->first);
                    }

                    it->second.push_back(make_pair(it2->second[l].first + 
                                    m_mergeEntries[j]->newBaseDocId,
                                    it2->second[l].second));
                }
//                FX_TRACE("==term: %llu, doc: %d, first pos: %d", it2->first, it2->second[l].first, 
//                                it2->second[l].second[0]);
            }
        }
    }
}

void TextIndexMergerTestCase::testMerge()
{
    m_nNumEntries = 1;
    m_bHasDeletions = false;
    makeData();

    IndexMergeInfos infos("result_barrel");
    BarrelDirectory::createBarrel(m_mergeEntries[0]->pFileSystem, "result_barrel");
    for (size_t i = 0; i < m_nNumEntries; ++i)
    {
        DocIdRecyclingPtr pDocIdRemapper(new DocIdRecycling());
        pDocIdRemapper->init(m_mergeEntries[i]->pBarrelInfo->getDocCount(), NULL);
        
        TermReaderPtr pTermReader(new TextTermReader());
        pTermReader->open(m_mergeEntries[i]->pFileSystem, 
                          m_mergeEntries[i]->pStreamPool, 
                          m_mergeEntries[i]->pBarrelInfo->getSuffix(),
                          m_mergeEntries[i]->pFieldSchema.get(), NULL);

        IndexMergeInfo mergeFieldInfo(*(m_mergeEntries[i]->pBarrelInfo),
                pDocIdRemapper, m_mergeEntries[i]->newBaseDocId, pTermReader);
        infos.pushBack(mergeFieldInfo);
    }

    TermMap mergedAnswer;
    makeMergedAnswer(mergedAnswer);
    
    FieldMeta fieldMeta;
    m_pTextMerger->merge(infos, fieldMeta);
    TextTermReader termReader;
    termReader.open(m_pFileSystem, m_pStreamPool, "result_barrel", 
                    m_mergeEntries[0]->pFieldSchema.get(), NULL);
    TermIteratorPtr pTermIt = termReader.termIterator(
            m_mergeEntries[0]->pFieldSchema->getName());

    // printPositngAnswer(mergedAnswer);
    // TermIteratorPtr pTermIt2 = termReader.termIterator(m_mergeEntries[0]->pFieldSchema->getName());
    // printPositng(pTermIt2);

    checkMergedResult(pTermIt, mergedAnswer);
}

void TextIndexMergerTestCase::testMergeWithDeletions()
{
    m_nNumEntries = 4;
    m_bHasDeletions = true;
    makeData();

    IndexMergeInfos infos("result_barrel");
    BarrelDirectory::createBarrel(m_mergeEntries[0]->pFileSystem, "result_barrel");
    for (size_t i = 0; i < m_nNumEntries; ++i)
    {
        DocIdRecyclingPtr pDocIdRemapper(new DocIdRecycling());
        pDocIdRemapper->init(m_mergeEntries[i]->pBarrelInfo->getDocCount(),
                             m_mergeEntries[i]->pDocFilter.get());

        TermReaderPtr pTermReader(new TextTermReader());
        pTermReader->open(m_mergeEntries[i]->pFileSystem, 
                          m_mergeEntries[i]->pStreamPool, 
                          m_mergeEntries[i]->pBarrelInfo->getSuffix(),
                          m_mergeEntries[i]->pFieldSchema.get(), NULL);

        IndexMergeInfo mergeFieldInfo(*(m_mergeEntries[i]->pBarrelInfo),
                pDocIdRemapper, m_mergeEntries[i]->newBaseDocId, pTermReader);
        infos.pushBack(mergeFieldInfo);
    }

    TermMap mergedAnswer;
    makeMergedAnswer(mergedAnswer);
    
    FieldMeta fieldMeta;
    m_pTextMerger->merge(infos, fieldMeta);
    TextTermReader termReader;
    termReader.open(m_pFileSystem, m_pStreamPool, "result_barrel",
                    m_mergeEntries[0]->pFieldSchema.get(), NULL);
    TermIteratorPtr pTermIt = termReader.termIterator(
            m_mergeEntries[0]->pFieldSchema->getName());
        
    // printPositngAnswer(mergedAnswer);
    // TermIteratorPtr pTermIt2 = termReader.termIterator(m_mergeEntries[0]->pFieldSchema->getName());
    // printPositng(pTermIt2);

    checkMergedResult(pTermIt, mergedAnswer);
}

void TextIndexMergerTestCase::checkMergedResult(TermIteratorPtr& pTermIt, TermMap& answer)
{
    CPPUNIT_ASSERT(pTermIt);

    CPPUNIT_ASSERT_EQUAL((int64_t)answer.size(), pTermIt->size());
    size_t termCount = 0;
    while (pTermIt->hasNext())
    {
        TermIterator::TermEntry entry = pTermIt->next();
        const TextTermIterator::TermType* pTerm =
            dynamic_cast<const TextTermIterator::TermType*>(entry.term);
        CPPUNIT_ASSERT(pTerm != NULL);
        uint64_t hash = pTerm->getValue();
        FX_TRACE("Term hash: %llu", hash);
        TermMap::const_iterator it = answer.find(hash);
        CPPUNIT_ASSERT(it != answer.end());
        CPPUNIT_ASSERT_EQUAL(it->first, hash);
        
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        size_t df = it->second.size();
        CPPUNIT_ASSERT_EQUAL((df_t)df, termMeta.getDocFreq());

        ctf_t ctf = 0;

        for (size_t i = 0; i < df; ++i)
        {
            docid_t docId = entry.postingIterator->skipTo(it->second[i].first);
            CPPUNIT_ASSERT_EQUAL(it->second[i].first, docId);
            tf_t tf = it->second[i].second.size();
            CPPUNIT_ASSERT_EQUAL(tf, entry.postingIterator->freq());

            for (size_t j = 0; j < (size_t)tf; ++j)
            {
                loc_t posExp = it->second[i].second[j];
                loc_t pos = entry.postingIterator->skipToPosition(posExp);
                CPPUNIT_ASSERT_EQUAL(posExp, pos);
            }
            ctf += it->second[i].second.size();
        }

        CPPUNIT_ASSERT_EQUAL(ctf, termMeta.getCTF());

        termCount++;
    }
    CPPUNIT_ASSERT_EQUAL(answer.size(), termCount);
}

void TextIndexMergerTestCase::printPositng(TermIteratorPtr& pTermIt)
{
    while (pTermIt->hasNext())
    {
        TermIterator::TermEntry entry = pTermIt->next();
        const TextTermIterator::TermType* pTerm =
            dynamic_cast<const TextTermIterator::TermType*>(entry.term);
        CPPUNIT_ASSERT(pTerm != NULL);
        uint64_t hash = pTerm->getValue();
        std::cout << "Term hash: " << hash << std::endl;
        
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        std::cout << "Term meta: df: " << termMeta.getDocFreq()
                  << ", ctf: " << termMeta.getCTF() << std::endl;
        PostingDecoderPtr pDecoder = entry.postingIterator->getPostingDecoder();
        docid_t docBuffer[RECORD_SIZE];
        tf_t tfBuffer[RECORD_SIZE];

        uint32_t nDecoded = 0;
        docid_t lastDocId = 0;
        while ((nDecoded = pDecoder->decodeDocRecord(docBuffer, lastDocId)) > 0)
        {
            if (pDecoder->decodeTfRecord(tfBuffer) != nDecoded)
            {
                FIRTEX_THROW(IndexCollapseException, "Doc and Tf record is inconsistant.");                
            }

            std::cout << "doc list: " << std::endl;
            for (size_t i = 0;i < nDecoded; ++i)
            {
                std::cout << docBuffer[i] << ", ";
            }
            std::cout << "end doc list: " << std::endl;

            std::cout << "tf list: " << std::endl;
            for (size_t i = 0;i < nDecoded; ++i)
            {
                std::cout << tfBuffer[i] << ", ";
            }
            std::cout << "end tf list: " << std::endl;

            lastDocId = docBuffer[nDecoded - 1] + 1;
        }

        loc_t posBuffer[RECORD_SIZE];
        uint32_t startOff = 0;
        uint32_t nDecodedPos = 0;
        uint32_t nTotalDecodedPos = 0;
        while ((nDecodedPos = pDecoder->decodePosRecord(posBuffer,
                                nTotalDecodedPos, startOff)) > 0)
        {
            std::cout << "pos list: " << std::endl;
            for (size_t i = 0;i < nDecodedPos; ++i)
            {
                std::cout << posBuffer[i] << ", ";
            }
            std::cout << "end pos list: " << std::endl;
            nTotalDecodedPos += nDecodedPos;
        }
    }
}

void TextIndexMergerTestCase::printPositngAnswer(const TermMap& answer)
{
    for (TermMap::const_iterator it = answer.begin(); 
         it != answer.end(); ++it)
    {
        std::cout << "Anser term hash: " << it->first << std::endl;

        std::cout << "anser doc list: " << std::endl;
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            std::cout << it->second[i].first << ", ";
        }
        std::cout << "end anser doc list: " << std::endl;

        ctf_t ctf = 0;
        std::cout << "anser tf list: " << std::endl;
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            ctf += it->second[i].second.size();
            std::cout << it->second[i].second.size() << ", ";
        }
        std::cout << "end anser tf list: " << std::endl;

        std::cout << "anser pos list: " << std::endl;
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            for (size_t j = 0; j < it->second[i].second.size(); ++j)
            {
                std::cout << it->second[i].second[j] << ", ";
            }
        }
        std::cout << "end anser pos list: " << std::endl;
        std::cout << "Anser term meta: df: " << it->second.size()
                  << ", ctf: " << ctf << std::endl;
    }
}

FX_NS_END

