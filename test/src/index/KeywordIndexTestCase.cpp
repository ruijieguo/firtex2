#include "KeywordIndexTestCase.h"
#include "firtex/index/Index.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/TermIterator.h"
#include "firtex/search/IndexSearcher.h"
#include "TestHelper.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/Hash.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/index/TypedTerm.h"
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;
FX_NS_USE(index);
FX_NS_USE(search);
FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(utility);

FX_NS_DEF(index);

CPPUNIT_TEST_SUITE_REGISTRATION(KeywordIndexTestCase);

KeywordIndexTestCase::KeywordIndexTestCase(void)
{
}

KeywordIndexTestCase::~KeywordIndexTestCase(void)
{
}

void KeywordIndexTestCase::setUp()
{
    TestHelper::removeTestFile("test_keywordindex", true);
}

void KeywordIndexTestCase::tearDown()
{
    TestHelper::removeTestFile("test_keywordindex", true);
}

void KeywordIndexTestCase::testCTF()
{
    GLOBAL_CONF().Build.buildThreadCount = 2;
    buildKeywordIndex("1, 2; 3, 4; 5, 6; 7, 8; 9, 10; "
                      "1, 2; 3, 4; 5, 6; 7, 8; 9, 10");

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    TermReaderPtr pTermReader = pReader->termReader();
    CPPUNIT_ASSERT(pTermReader);
    TermIteratorPtr pTermIterator = pTermReader->termIterator("Keyword1");
    CPPUNIT_ASSERT(pTermIterator != NULL);

    try 
    {
        while (pTermIterator->hasNext())
        {
            TermIterator::TermEntry entry = pTermIterator->next();
            const TermMeta& termMeta = entry.postingIterator->getTermMeta();
            CPPUNIT_ASSERT_EQUAL((df_t)2, termMeta.getDocFreq());
            CPPUNIT_ASSERT_EQUAL((ctf_t)2, termMeta.getCTF());
        }
    }
    catch(const FirteXException& e)
    {
        cout << "ERROR:" << e.what() << endl;
        CPPUNIT_ASSERT(false);
    } 
}

struct HashAndDoc
{
    uint64_t nHash;
    docid_t docId;
};

bool hashLess(const HashAndDoc& h1, const HashAndDoc& h2)
{
    return h1.nHash < h2.nHash;
}

void KeywordIndexTestCase::testPosting()
{
    const size_t NUM_DOCS = 100;

    vector<HashAndDoc> hashVec;
    stringstream ss;
    size_t i;
    for (i = 0; i < NUM_DOCS; ++i)
    {
        stringstream ss1;
        ss1 << "abc" << i;
        ss << ss1.str() << ", " << "abc" << ";";
        HashAndDoc hd;
        hd.nHash = Hash::hashString64(ss1.str().c_str());
        hd.docId = (docid_t)i;
        hashVec.push_back(hd);
    }
    GLOBAL_CONF().Build.buildThreadCount = 1;
    buildKeywordIndex(ss.str());

    Index index;
    index.open(getIndexPath(), Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    TermReaderPtr pTermReader = pReader->termReader();
    TermIteratorPtr pIterator = pTermReader->termIterator("Keyword1");
    CPPUNIT_ASSERT(pIterator);

    sort(hashVec.begin(), hashVec.end(), hashLess);
    int32_t j = 0;
    while (pIterator->hasNext())
    {
        TermIterator::TermEntry entry = pIterator->next();

        const Term* pTerm = entry.term;
        const UInt64Term* pTermX = pTerm->cast<uint64_t>();
        uint64_t nCurTerm = pTermX->getValue();
        uint64_t nExpTerm = hashVec[j].nHash;
        CPPUNIT_ASSERT_EQUAL(nExpTerm, nCurTerm);

        TermPostingIteratorPtr pPostingIter = entry.postingIterator;
        CPPUNIT_ASSERT(pPostingIter != NULL);
        docid_t docId = pPostingIter->skipTo(0);
        CPPUNIT_ASSERT_EQUAL(hashVec[j].docId, docId);
        docId = pPostingIter->skipTo(docId + 1);
        CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);
        j++;
    } 

    //test abc term
    TermIteratorPtr pIterator2 = pTermReader->termIterator("Keyword2");
    CPPUNIT_ASSERT(pIterator2);
    i = 0;
    while (pIterator2->hasNext())
    {
        TermIterator::TermEntry entry = pIterator2->next();

        const Term* pTerm = entry.term;
        const UInt64Term* pTermX = pTerm->cast<uint64_t>();
        uint64_t nCurTerm = pTermX->getValue();
        uint64_t nExpTerm = Hash::hashString64("abc");
        CPPUNIT_ASSERT_EQUAL(nExpTerm, nCurTerm);

        TermPostingIteratorPtr pPostingIter = entry.postingIterator;
        CPPUNIT_ASSERT(pPostingIter != NULL);
        docid_t docId = 0;
        docid_t docIdRet = 0;
        while ((docIdRet = pPostingIter->skipTo(docId)) != INVALID_DOCID)
        {
            CPPUNIT_ASSERT_EQUAL(docId++, docIdRet);
        }
        CPPUNIT_ASSERT_EQUAL((docid_t)100, docId);
        i++;
    } 
    CPPUNIT_ASSERT_EQUAL((size_t)1, i);
}

string KeywordIndexTestCase::getIndexPath()
{
    return TestHelper::getTestDataPath("test_keywordindex", true);
}

void KeywordIndexTestCase::buildKeywordIndex(const string& sDocs)
{
    try
    {
        DocumentSchema schema;
        schema.addField("Keyword1", "KEYWORD", true);
        schema.addField("Keyword2", "KEYWORD", true);

        Index index;
        index.open(getIndexPath(), Index::WRITE, &schema);
        IndexWriterPtr pIndexWriter = index.acquireWriter();

        StringTokenizer st(sDocs, ";", StringTokenizer::TOKEN_TRIM |
                           StringTokenizer::TOKEN_IGNORE_EMPTY);

        for (StringTokenizer::Iterator it = st.begin();
             it != st.end(); ++it)
        {
            DocumentPtr pDoc = new Document(pIndexWriter->getDocSchema());
            StringTokenizer st2(*it, ",", StringTokenizer::TOKEN_TRIM |
                    StringTokenizer::TOKEN_IGNORE_EMPTY);
            pDoc->addField(0, st2[0].c_str());
            pDoc->addField(1, st2[1].c_str());

            pIndexWriter->addDocument(pDoc);
        }
        pIndexWriter->close();
    }
    catch (const FirteXException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

FX_NS_END
