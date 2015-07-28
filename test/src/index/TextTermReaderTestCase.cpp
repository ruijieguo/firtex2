#include "index/TextTermReaderTestCase.h"
#include "firtex/index/text/TextTermIterator.h"
#include "firtex/utility/File.h"
#include "firtex/index/Index.h"
#include "TestHelper.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/utility/LooseBoundedPool.h"
#include "IndexBuildHelper.h"
#include <limits>
#include <vector>

using namespace std;

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, TextTermReaderTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(TextTermReaderTestCase);

TextTermReaderTestCase::TextTermReaderTestCase() 
    : m_sBarrel("test_barrel")
    , m_nDataSize(100)
{
}

TextTermReaderTestCase::~TextTermReaderTestCase() 
{
}

void TextTermReaderTestCase::setUp()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }

    m_pFieldSchema.assign(new FieldSchema("testfield", NULL, false));
    
    m_pPool.assign(new PostingPool(new LooseBoundedPool(10 * 1024 * 1024),
                                   new LooseBoundedPool(10 * 1024 * 1024)));

    m_pIndexer = new TextIndexer(m_pFieldSchema.get());
    m_pIndexer->init(m_pPool);
    m_pTermReader = new TextTermReader();
    m_pFileSystem = new BlockFileSystem();
    string sFilePath = getTestPath();
    m_pFileSystem->open(sFilePath, FileSystem::CREATE);
    m_pStreamPool = new InputStreamPool(m_pFileSystem);
}

void TextTermReaderTestCase::tearDown()
{
    m_pFieldSchema.reset();
    m_pPool.reset();
    m_pIndexer.reset();
    m_pTermReader.reset();
    m_pStreamPool.reset();
    m_pFileSystem.reset();

    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void TextTermReaderTestCase::makeData()
{
    std::vector<TermMetaPtr> terms;
    for (size_t i = 0; i < m_nDataSize; ++i)
    {
        TokenViewPtr pTokenView = new TokenView(i + 1, Token::TT_CWORD);
        for (size_t j = 0; j < i + 1; ++j)
        {
            stringstream ss;
            ss << "abc" << j;
            pTokenView->addToken(ss.str().c_str());

            uint64_t hash = Hash::hashString64(ss.str().c_str());
            TermMap::iterator it = m_answer.find(hash);
            if (it != m_answer.end())
            {
                terms.push_back(it->second);
                it->second->getCTF()++;
            }
            else
            {
                m_answer.insert(make_pair(hash, TermMetaPtr(new TermMeta(1, 1))));
                m_termHashMap.insert(make_pair(hash, ss.str()));
            }
        }
        AnalyzedFieldPtr pField = new AnalyzedField(NULL, pTokenView);
        m_pIndexer->addField(pField.get());
        m_pIndexer->commitDocument((docid_t)i);

        for (std::vector<TermMetaPtr>::iterator it2 = terms.begin();
             it2 != terms.end(); ++it2)
        {
            (*it2)->getDocFreq()++;
        }
        terms.clear();
    }
    BarrelDirectory::createBarrel(m_pFileSystem, "test_barrel");
    m_pIndexer->commit(m_pFileSystem, "test_barrel");
}

void TextTermReaderTestCase::testNumTerms()
{
    m_nDataSize = 100;
    makeData();
    CPPUNIT_ASSERT_EQUAL((uint64_t)m_answer.size(), m_pIndexer->distinctNumTerms());
    uint64_t totalTerms = 0;
    for (TermMap::const_iterator it = m_answer.begin(); it != m_answer.end(); ++it)
    {
        totalTerms += it->second->getCTF();
    }
    CPPUNIT_ASSERT_EQUAL(totalTerms, m_pIndexer->totalTerms());
}

void TextTermReaderTestCase::testTermPosIterator()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    GLOBAL_CONF().Build.buildThreadCount = 1;
    string str = getTestPath();
    IndexBuildHelper::buildIndex(str, schema, "docid1, hello world; docid2, hello my world; "
                                 "docid3, hello my my my world", true);
    
    IndexPtr pIndex = new Index;
    pIndex->open(str, Index::READ, NULL);
    IndexReaderPtr pIndexReader = pIndex->acquireReader();
    TermReaderPtr pTermReader = pIndexReader->termReader();

    Term term("CONTENT", "hello");
    TermPostingIteratorPtr pPostIt = pTermReader->seek(&term);
    CPPUNIT_ASSERT(pPostIt);

    const TermMeta& termMeta = pPostIt->getTermMeta();
    CPPUNIT_ASSERT_EQUAL((df_t)3, termMeta.getDocFreq());
    CPPUNIT_ASSERT_EQUAL((ctf_t)3, termMeta.getCTF());

    docid_t docId = INVALID_DOCID;
    docId = pPostIt->skipTo(docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)0, docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)1, pPostIt->freq());
    loc_t pos = INVALID_POS;
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)0, pos);
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)INVALID_POS, pos);
    
    ++docId;
    docId = pPostIt->skipTo(docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)1, docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)1, pPostIt->freq());

    ++docId;    
    docId = pPostIt->skipTo(docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)2, docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)1, pPostIt->freq());

    Term term2("CONTENT", "my");
    pPostIt = pTermReader->seek(&term2);
    CPPUNIT_ASSERT(pPostIt);

    const TermMeta& termMeta2 = pPostIt->getTermMeta();
    CPPUNIT_ASSERT_EQUAL((df_t)2, termMeta2.getDocFreq());
    CPPUNIT_ASSERT_EQUAL((ctf_t)4, termMeta2.getCTF());

    docId = INVALID_DOCID;
    docId = pPostIt->skipTo(docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)1, docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)1, pPostIt->freq());
    pos = INVALID_POS;
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)1, pos);
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)INVALID_POS, pos);
    
    ++docId;
    docId = pPostIt->skipTo(docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)2, docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)3, pPostIt->freq());
    pos = INVALID_POS;
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)1, pos);
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)2, pos);
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)3, pos);
    pos = pPostIt->skipToPosition(++pos);
    CPPUNIT_ASSERT_EQUAL((loc_t)INVALID_POS, pos);

    ++docId;    
    docId = pPostIt->skipTo(docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);
}

void TextTermReaderTestCase::testTermIterator()
{
    m_nDataSize = 1000;
    makeData();

    m_pTermReader->open(m_pFileSystem, m_pStreamPool, m_sBarrel,
                        m_pFieldSchema.get(), NULL);
    TermIteratorPtr pTermIt = m_pTermReader->termIterator(
            m_pFieldSchema->getName());
    CPPUNIT_ASSERT(pTermIt);

    checkIterator(pTermIt, m_answer);
}

void TextTermReaderTestCase::testInMemTermIterator()
{
    m_nDataSize = 1000;
    makeData();

    TermReaderPtr pTermReader = m_pIndexer->termReader();
    CPPUNIT_ASSERT(pTermReader);
    TermIteratorPtr pTermIt = pTermReader->termIterator(
            m_pFieldSchema->getName());
    CPPUNIT_ASSERT(pTermIt);

    checkIterator(pTermIt, m_answer);
}

void TextTermReaderTestCase::testTermRangeIterator()
{
    m_nDataSize = 1000;
    makeData();

    m_pTermReader->open(m_pFileSystem, m_pStreamPool, m_sBarrel,
                        m_pFieldSchema.get(), NULL);

    uint64_t testTerms[] = {
        2532843299785153037ULL, 2538691235596585163ULL,
        2545119675759083127ULL, 2550767367222976046ULL,
        2599609793921200729ULL, 2610290482603386691ULL,
        2621683998420677351ULL, 2633479129038327396ULL
    };

    size_t numTerms = sizeof(testTerms)/sizeof(testTerms[0]);

    TextTermIterator::TermType lowerTerm("testfield", testTerms[0], Term::UINT64);
    TextTermIterator::TermType upperTerm("testfield", testTerms[numTerms - 1], Term::UINT64);

    TermIteratorPtr pTermIt = m_pTermReader->termIterator(&lowerTerm, &upperTerm);
    CPPUNIT_ASSERT(pTermIt);

    CPPUNIT_ASSERT_EQUAL((int64_t)numTerms, pTermIt->size());
    size_t termCount = 0;
    while (pTermIt->hasNext())
    {
        TermIterator::TermEntry entry = pTermIt->next();
        const TextTermIterator::TermType* pTerm =
            dynamic_cast<const TextTermIterator::TermType*>(entry.term);
        CPPUNIT_ASSERT(pTerm != NULL);
        uint64_t hash = pTerm->getValue();

        TermMap::const_iterator it = m_answer.find(hash);
        CPPUNIT_ASSERT(it != m_answer.end());
        CPPUNIT_ASSERT_EQUAL(it->first, hash);
        CPPUNIT_ASSERT_EQUAL(testTerms[termCount], hash);

        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL(it->second->getDocFreq(), termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL(it->second->getCTF(), termMeta.getCTF());

        termCount++;
    }
    CPPUNIT_ASSERT_EQUAL(numTerms, termCount);
}

void TextTermReaderTestCase::testInMemTermRangeIterator()
{
    m_nDataSize = 1000;
    makeData();

    TermReaderPtr pTermReader = m_pIndexer->termReader();
    CPPUNIT_ASSERT(pTermReader);

    uint64_t testTerms[] = {
        2532843299785153037ULL, 2538691235596585163ULL,
        2545119675759083127ULL, 2550767367222976046ULL,
        2599609793921200729ULL, 2610290482603386691ULL,
        2621683998420677351ULL, 2633479129038327396ULL
    };

    size_t numTerms = sizeof(testTerms)/sizeof(testTerms[0]);

    TextTermIterator::TermType lowerTerm("testfield", testTerms[0], Term::UINT64);
    TextTermIterator::TermType upperTerm("testfield", testTerms[numTerms - 1], Term::UINT64);

    TermIteratorPtr pTermIt = pTermReader->termIterator(&lowerTerm, &upperTerm);
    CPPUNIT_ASSERT(pTermIt);

    CPPUNIT_ASSERT_EQUAL((int64_t)numTerms, pTermIt->size());
    size_t termCount = 0;
    while (pTermIt->hasNext())
    {
        TermIterator::TermEntry entry = pTermIt->next();
        const TextTermIterator::TermType* pTerm =
            dynamic_cast<const TextTermIterator::TermType*>(entry.term);
        CPPUNIT_ASSERT(pTerm != NULL);
        uint64_t hash = pTerm->getValue();

        TermMap::const_iterator it = m_answer.find(hash);
        CPPUNIT_ASSERT(it != m_answer.end());
        CPPUNIT_ASSERT_EQUAL(it->first, hash);
        CPPUNIT_ASSERT_EQUAL(testTerms[termCount], hash);

        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL(it->second->getDocFreq(), termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL(it->second->getCTF(), termMeta.getCTF());

        termCount++;
    }
    CPPUNIT_ASSERT_EQUAL(numTerms, termCount);
}


void TextTermReaderTestCase::testSeek()
{
    FX_TRACE("Begin testSeek()");
    m_nDataSize = 1000;
    makeData();

    m_pTermReader->open(m_pFileSystem, m_pStreamPool, m_sBarrel,
                        m_pFieldSchema.get(), NULL);

    for (TermMap::const_iterator it = m_answer.begin();
         it != m_answer.end(); ++it)
    {
        TermHashMap::const_iterator it2 = m_termHashMap.find(it->first);
        Term term("testfield", it2->second);
        TermPostingIteratorPtr pPostIt = m_pTermReader->seek(&term);
        CPPUNIT_ASSERT(pPostIt);

        const TermMeta& termMeta = pPostIt->getTermMeta();
        CPPUNIT_ASSERT_EQUAL(it->second->getDocFreq(), termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL(it->second->getCTF(), termMeta.getCTF());
    }
}

void TextTermReaderTestCase::testInMemSeek()
{
    FX_TRACE("Begin testSeek()");
    m_nDataSize = 1000;
    makeData();

    TermReaderPtr pTermReader = m_pIndexer->termReader();

    for (TermMap::const_iterator it = m_answer.begin();
         it != m_answer.end(); ++it)
    {
        TermHashMap::const_iterator it2 = m_termHashMap.find(it->first);
        Term term("testfield", it2->second);
        TermPostingIteratorPtr pPostIt = pTermReader->seek(&term);
        CPPUNIT_ASSERT(pPostIt);

        const TermMeta& termMeta = pPostIt->getTermMeta();
        CPPUNIT_ASSERT_EQUAL(it->second->getDocFreq(), termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL(it->second->getCTF(), termMeta.getCTF());
    }
}

void TextTermReaderTestCase::checkIterator(TermIteratorPtr& pTermIt, const TermMap& answer)
{
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
        CPPUNIT_ASSERT_EQUAL(it->second->getDocFreq(), termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL(it->second->getCTF(), termMeta.getCTF());

        termCount++;
    }
    CPPUNIT_ASSERT_EQUAL(answer.size(), termCount);
}

string TextTermReaderTestCase::getTestPath()
{
    return TestHelper::getTestDataPath("test_termreader", true);
}


FX_NS_END

