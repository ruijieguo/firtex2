#include "IndexTestCase.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/TermIterator.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexIterator.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberParser.h"
#include "TestHelper.h"
#include "firtex/analyzer/StandardAnalyzer.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/common/SharedPtr.h"
#include "IndexBuildHelper.h"
#include <sstream>

using namespace std;
using namespace firtex;

FX_NS_USE(analyzer);
FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(document);

FX_NS_DEF(index);

CPPUNIT_TEST_SUITE_REGISTRATION(IndexTestCase);

IndexTestCase::IndexTestCase(void)
{
}

IndexTestCase::~IndexTestCase(void)
{
}

void IndexTestCase::setUp()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void IndexTestCase::tearDown()
{
    return;
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void IndexTestCase::buildIndex(const DocumentSchema& schema,
                               const string& fieldsStr, bool bInc)
{
    tstring str = getTestPath();
    IndexBuildHelper::buildIndex(str, schema, fieldsStr, bInc);
}

void IndexTestCase::testTextIndex()
{
    DocumentSchema schema;
    schema.addUnIndexedField("PATH");
    schema.addTextField("CONTENT");

    buildIndex(schema, "file1.txt, hello world.");

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    TermReaderPtr pTermReader = pReader->termReader();
    CPPUNIT_ASSERT(pTermReader);

    TermIteratorPtr pTermIterator = pTermReader->termIterator("CONTENT");
    CPPUNIT_ASSERT(pTermIterator != NULL);

    while (pTermIterator->hasNext())
    {
        TermIterator::TermEntry entry = pTermIterator->next();
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL((df_t)1, termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL((ctf_t)1, termMeta.getCTF());
    }

    Term term("CONTENT", "hello");
    TermPostingIteratorPtr pPost = pTermReader->seek(&term);

    CPPUNIT_ASSERT(pPost.isNotNull());
    docid_t docId = pPost->skipTo(0);
    CPPUNIT_ASSERT_EQUAL((docid_t)0, docId);
    docId = pPost->skipTo(++docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);

    StoredFieldsReaderPtr pDocReader = pReader->createStoredFieldsReader();
    CPPUNIT_ASSERT(pDocReader.isNotNull());
    FieldSelector selector(pReader->getDocSchema(), true, false);
    ResultDoc resultDoc(0);
    bool ret = pDocReader->getDocument(selector, resultDoc);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT(resultDoc.size() > 0);
}

void IndexTestCase::testKeywordIndex()
{
    DocumentSchema schema;
    schema.addUnIndexedField("PATH");
    schema.addField("Keyword", "KEYWORD", false);

    buildIndex(schema, "file1.txt, hello world.");

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    TermReaderPtr pTermReader = pReader->termReader();
    CPPUNIT_ASSERT(pTermReader);
    TermIteratorPtr pTermIterator = pTermReader->termIterator("Keyword");
    CPPUNIT_ASSERT(pTermIterator != NULL);

    while (pTermIterator->hasNext())
    {
        TermIterator::TermEntry entry = pTermIterator->next();
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL((df_t)1, termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL((ctf_t)1, termMeta.getCTF());
    }

    Term term("Keyword", "hello world.");
    TermPostingIteratorPtr pPost = pTermReader->seek(&term);
    CPPUNIT_ASSERT(pPost.isNotNull());
    docid_t docId = pPost->skipTo(0);
    CPPUNIT_ASSERT_EQUAL((docid_t)0, docId);
    docId = pPost->skipTo(++docId);
    CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);
}

void IndexTestCase::testPrimaryKeyIndex()
{
    DocumentSchema schema;
    schema.addField("PK", "PRIMARY_KEY", false);

    const static size_t NUM_DOCS = 1000;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << i << ";";
    }

    GLOBAL_CONF().Build.buildThreadCount = 1;

    buildIndex(schema, ss.str());

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    TermReaderPtr pTermReader = pReader->termReader();
    CPPUNIT_ASSERT(pTermReader);
    TermIteratorPtr pTermIterator = pTermReader->termIterator("PK");
    CPPUNIT_ASSERT(pTermIterator != NULL);

    while (pTermIterator->hasNext())
    {
        TermIterator::TermEntry entry = pTermIterator->next();
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL((df_t)1, termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL((ctf_t)1, termMeta.getCTF());
    }

    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        stringstream ss2;
        ss2 << i;
        Term  term("PK", ss2.str());
        TermPostingIteratorPtr pPost = pTermReader->seek(&term);
        CPPUNIT_ASSERT(pPost.isNotNull());
        docid_t docId = pPost->skipTo(0);
        CPPUNIT_ASSERT_EQUAL((docid_t)i, docId);
        docId = pPost->skipTo(++docId);
        CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);
    }
}

void IndexTestCase::testInt32Index()
{
    DocumentSchema schema;
    schema.addField("Int32", "INT32_I", false);

    const static size_t NUM_DOCS = 1000;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << (i % 100) << ";";
    }

    GLOBAL_CONF().Build.buildThreadCount = 1;
    buildIndex(schema, ss.str());

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    TermReaderPtr pTermReader = pReader->termReader();
    CPPUNIT_ASSERT(pTermReader);
    TermIteratorPtr pTermIterator = pTermReader->termIterator("Int32");
    CPPUNIT_ASSERT(pTermIterator != NULL);

    while (pTermIterator->hasNext())
    {
        TermIterator::TermEntry entry = pTermIterator->next();
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL((df_t)10, termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL((ctf_t)10, termMeta.getCTF());
    }

    Term term("Int32", "0");
    TermPostingIteratorPtr pPost = pTermReader->seek(&term);
    CPPUNIT_ASSERT(pPost.isNotNull());
    docid_t docId = pPost->skipTo(0);
    CPPUNIT_ASSERT_EQUAL((docid_t)0, docId);
    docId = pPost->skipTo(901);
    CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);
}

void IndexTestCase::testInt32_IF()
{
    DocumentSchema schema;
    schema.addField("Int32", "INT32_IF", false);

    const static size_t NUM_DOCS = 1000;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << (i % 100) << ";";
    }

    GLOBAL_CONF().Build.buildThreadCount = 1;
    buildIndex(schema, ss.str());

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    TermReaderPtr pTermReader = pReader->termReader();
    CPPUNIT_ASSERT(pTermReader);
    TermIteratorPtr pTermIterator = pTermReader->termIterator("Int32");
    CPPUNIT_ASSERT(pTermIterator != NULL);

    while (pTermIterator->hasNext())
    {
        TermIterator::TermEntry entry = pTermIterator->next();
        const TermMeta& termMeta = entry.postingIterator->getTermMeta();
        CPPUNIT_ASSERT_EQUAL((df_t)10, termMeta.getDocFreq());
        CPPUNIT_ASSERT_EQUAL((ctf_t)10, termMeta.getCTF());
    }

    Term term("Int32", "0");
    TermPostingIteratorPtr pPost = pTermReader->seek(&term);
    CPPUNIT_ASSERT(pPost.isNotNull());
    docid_t docId = pPost->skipTo(0);
    CPPUNIT_ASSERT_EQUAL((docid_t)0, docId);
    docId = pPost->skipTo(901);
    CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);

    ForwardIndexIteratorPtr pForIndexIt = pReader->forwardIndexReader("Int32");
    CPPUNIT_ASSERT(pForIndexIt != NULL);
    Int32ForwardIndexIteratorPtr pInt32ForIndexIt =
        pForIndexIt.cast<Int32ForwardIndexIterator>();
    CPPUNIT_ASSERT(pInt32ForIndexIt != NULL);
    
    int32_t value = 0;
    docId = 0;
    for (; docId < (docid_t)NUM_DOCS; ++docId)
    {
        CPPUNIT_ASSERT(pInt32ForIndexIt->seek(docId, value));
        CPPUNIT_ASSERT_EQUAL((int32_t)(docId % 100), value);
    }
    CPPUNIT_ASSERT(!pInt32ForIndexIt->seek(docId, value));
}

void IndexTestCase::testInt32ForwardIndex()
{
    DocumentSchema schema;
    schema.addSortableField("Int32Id", FieldType::INT32, false);

    const static size_t NUM_DOCS = 1000;
    stringstream ss;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << i << ";";
    }

    GLOBAL_CONF().Build.buildThreadCount = 1;
    buildIndex(schema, ss.str());

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pReader = index.acquireReader();
    CPPUNIT_ASSERT(pReader != NULL);
    ForwardIndexIteratorPtr pForIndexIt = pReader->forwardIndexReader("Int32Id");
    CPPUNIT_ASSERT(pForIndexIt != NULL);
    Int32ForwardIndexIteratorPtr pInt32ForIndexIt =
        pForIndexIt.cast<Int32ForwardIndexIterator>();
    CPPUNIT_ASSERT(pInt32ForIndexIt != NULL);
    
    int32_t value = 0;
    docid_t docId = 0;
    for (; docId < (docid_t)NUM_DOCS; ++docId)
    {
        CPPUNIT_ASSERT(pInt32ForIndexIt->seek(docId, value));
        CPPUNIT_ASSERT_EQUAL((int32_t)docId, value);
    }
    CPPUNIT_ASSERT(!pInt32ForIndexIt->seek(docId, value));
}

void IndexTestCase::testIncrementalIndex()
{
    DocumentSchema schema;
    schema.addTextField("BODY");
    
    stringstream ss;
    const size_t NUM_DOCS = 1000;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << "body" << i << " hot;";
    }
    GLOBAL_CONF().Build.buildThreadCount = 8;
    buildIndex(schema, ss.str());
    buildIndex(schema, ss.str(), true);

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pIndexReader = index.acquireReader();

    checkDocFreq(pIndexReader, "BODY", "hot", 2000);
}

void IndexTestCase::testIndexOptimize()
{
    GLOBAL_CONF().Merge.maxAllowedOpenFiles = 16;
    DocumentSchema schema;
    schema.addTextField("BODY");
    
    stringstream ss;
    const size_t NUM_DOCS = 1000;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << "body" << i << " hot;";
    }

    try
    {
        buildIndex(schema, ss.str());
        buildIndex(schema, ss.str(), true);
    }
    catch (const FirteXException& e)
    {
        cout << "ERROR: " << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }

    tstring str = getTestPath();

    {
        Index index;
        index.open(str, Index::APPEND, NULL); 
        IndexWriterPtr pIndexWriter = index.acquireWriter();
        CPPUNIT_ASSERT(pIndexWriter != NULL);
    
        try
        {
            pIndexWriter->optimizeIndex();
        }
        catch (const FirteXException& e)
        {
            cout << "ERROR: " << e.what() << endl;
            CPPUNIT_ASSERT(false);
        }
    }
    Index index;
    try
    {
        index.open(str, Index::READ, NULL);
        IndexReaderPtr pIndexReader = index.acquireReader();
        checkDocFreq(pIndexReader, "BODY", "hot", 2 * NUM_DOCS);
    }
    catch (const FirteXException& e)
    {
        cout << "ERROR: " << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }
}

void IndexTestCase::testRefreshIndex()
{
    DocumentSchema schema;
    schema.addTextField("BODY");
    
    stringstream ss;
    const size_t NUM_DOCS = 1000;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << "body" << i << " hot;";
    }
    buildIndex(schema, ss.str());

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);

    IndexReaderPtr pIndexReader1 = index.acquireReader();
    checkDocFreq(pIndexReader1, "BODY", "hot", NUM_DOCS);

    IndexBarrelKeeperPtr pKeeper = index.getIndexBarrelKeeper();
    CPPUNIT_ASSERT_EQUAL((size_t)1, pKeeper->getHeldCommitCount());

    buildIndex(schema, ss.str(), true);
    IndexReaderPtr pIndexReader2 = index.acquireReader(true);
    CPPUNIT_ASSERT(pIndexReader1 != pIndexReader2);

    checkDocFreq(pIndexReader2, "BODY", "hot", 2 * NUM_DOCS);

    CPPUNIT_ASSERT_EQUAL((size_t)2, pKeeper->getHeldCommitCount());

    pIndexReader1.reset();
    pIndexReader2.reset();
    buildIndex(schema, ss.str(), true);

    IndexReaderPtr pIndexReader3 = index.acquireReader(true);
    CPPUNIT_ASSERT_EQUAL((size_t)1, pKeeper->getHeldCommitCount());
}

void IndexTestCase::testRefreshIndexAfterOptimize()
{
    DocumentSchema schema;
    schema.addTextField("BODY");
    
    stringstream ss;
    const size_t NUM_DOCS = 1000;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << "body" << i << " hot;";
    }
    buildIndex(schema, ss.str());

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pIndexReader1 = index.acquireReader();
    checkDocFreq(pIndexReader1, "BODY", "hot", NUM_DOCS);

    buildIndex(schema, ss.str(), true);

    //Optimize index
    {
        Index index2;
        index2.open(str, Index::APPEND, NULL); 
        IndexWriterPtr pIndexWriter = index2.acquireWriter();
        pIndexWriter->optimizeIndex();
    }

    IndexReaderPtr pIndexReader2 = index.acquireReader(true);
    CPPUNIT_ASSERT(pIndexReader1 != pIndexReader2);
    
    checkDocFreq(pIndexReader2, "BODY", "hot", 2 * NUM_DOCS);
}

void IndexTestCase::testRefreshIndexWithMultiBarrel()
{
    DocumentSchema schema;
    schema.addTextField("BODY");
    
    stringstream ss;
    const size_t NUM_DOCS = 1000;
    for (size_t i = 0; i < NUM_DOCS; ++i)
    {
        ss << "body" << i << " hot;";
    }
    buildIndex(schema, ss.str());
    buildIndex(schema, ss.str(), true);

    tstring str = getTestPath();
    Index index;
    index.open(str, Index::READ, NULL);
    IndexReaderPtr pIndexReader1 = index.acquireReader();
    checkDocFreq(pIndexReader1, "BODY", "hot", 2 * NUM_DOCS);

    buildIndex(schema, ss.str(), true);
    IndexReaderPtr pIndexReader2 = index.acquireReader(true);
    CPPUNIT_ASSERT(pIndexReader1 != pIndexReader2);
    
    checkDocFreq(pIndexReader2, "BODY", "hot", 3 * NUM_DOCS);
}

void IndexTestCase::testDocumentDeletion()
{
    DocumentSchema schema;
    schema.addField("URL", "PRIMARY_KEY", true);
    schema.addTextField("BODY");
    schema.addField("MODIFIED", "INT64", true);
    
    stringstream ss1;
    const size_t NUM_DOCS = 1000;
    size_t i = 0;
    for (; i < NUM_DOCS; ++i)
    {
        ss1 << "url" << i << ", body" << i << " hot," 
            << (i * 100) % 1000 << ";";
    }
    buildIndex(schema, ss1.str());

    stringstream ss2;
    for (; i < 2 * NUM_DOCS; ++i)
    {
        ss2 << "url" << i << ", body" << i << " hot," 
            << (i * 100) % 1000 << ";";
    }

    buildIndex(schema, ss2.str(), true);

    StandardAnalyzerPtr sa(new StandardAnalyzer());
    sa->init();

    TokenViewPtr pTokens = sa->tokenize("hot", 3);
    CPPUNIT_ASSERT(pTokens.isNull() != true);
    CPPUNIT_ASSERT(pTokens->getNumTokens() == 1);
    TokenView::Iterator it = pTokens->iterator();
    TermPtr pTerm(new Term("BODY", it.next().getTextValue()));
    
    tstring str = getTestPath();
    
    std::set<docid_t> answer;

    {
        Index index;
        index.open(str, Index::RDWR, NULL); 
        IndexWriterPtr pIndexWriter = index.acquireWriter();
        CPPUNIT_ASSERT(pIndexWriter != NULL);

        IndexReaderPtr pIndexReader = index.acquireReader();
        CPPUNIT_ASSERT(pIndexReader != NULL);

        for (size_t i = 0; i < 2 * NUM_DOCS; ++i)
        {
            stringstream ss;
            ss << "url" << i;
            if (i == 1000 || i == 1500 || i == 1505 || i == 1999)
            {
                pIndexWriter->deleteDocument(ss.str());
            }
            else
            {
                TermReaderPtr pTermReader = pIndexReader->termReader();
                TermPtr pTerm(new Term("URL", ss.str()));
                TermPostingIteratorPtr pIt = pTermReader->seek(pTerm.get());
                docid_t docId = pIt->skipTo(0);
                answer.insert(docId);
            }
        }

        TermReaderPtr pTermReader = pIndexReader->termReader();
        TermPostingIteratorPtr pDocFreqs = pTermReader->seek(pTerm.get());
        CPPUNIT_ASSERT(!pDocFreqs.isNull());

        CPPUNIT_ASSERT_EQUAL((df_t)NUM_DOCS * 2, pDocFreqs->getTermMeta().getDocFreq());

        std::set<docid_t>::const_iterator it = answer.begin();
        for (docid_t i = 0; i < (docid_t)(2 * NUM_DOCS); )
        {        
            docid_t docId = pDocFreqs->skipTo((docid_t)i);
            i = docId + 1;
            if (docId == INVALID_DOCID)
            {
                break;
            }
            CPPUNIT_ASSERT_EQUAL(*it, docId);
            ++it;
        }
        CPPUNIT_ASSERT(it == answer.end());
    }

    {
        Index index;
        index.open(str, Index::READ, NULL); 
        IndexReaderPtr pIndexReader = index.acquireReader();
        CPPUNIT_ASSERT(pIndexReader != NULL);

        TermReaderPtr pTermReader = pIndexReader->termReader();
        TermPostingIteratorPtr pDocFreqs = pTermReader->seek(pTerm.get());
        CPPUNIT_ASSERT(!pDocFreqs.isNull());

        CPPUNIT_ASSERT_EQUAL((df_t)(2 * NUM_DOCS), pDocFreqs->getTermMeta().getDocFreq());
        std::set<docid_t>::const_iterator it = answer.begin();
        for (docid_t i = 0; i < (docid_t)(2 * NUM_DOCS); )
        {        
            docid_t docId = pDocFreqs->skipTo((docid_t)i);
            i = docId + 1;
            if (docId == INVALID_DOCID)
            {
                break;
            }
            CPPUNIT_ASSERT_EQUAL(*it, docId);
            ++it;
        }
        CPPUNIT_ASSERT(it == answer.end());

        // for (std::set<docid_t>::const_iterator it = answer.begin();
        //      it != answer.end(); ++it)
        // {
        //     docid_t docId = pDocFreqs->skipTo(*it);
        //     CPPUNIT_ASSERT_EQUAL(*it, docId);
        // }

        // docid_t docId = pDocFreqs->skipTo(NUM_DOCS + 0);
        // CPPUNIT_ASSERT_EQUAL((docid_t)NUM_DOCS + 1, docId);
        // docId = pDocFreqs->skipTo(NUM_DOCS + 500);
        // CPPUNIT_ASSERT_EQUAL((docid_t)NUM_DOCS + 501, docId);
        // docId = pDocFreqs->skipTo(NUM_DOCS + 505);
        // CPPUNIT_ASSERT_EQUAL((docid_t)NUM_DOCS + 506, docId);
        // docId = pDocFreqs->skipTo(2 * NUM_DOCS - 1);
        // CPPUNIT_ASSERT_EQUAL((docid_t)INVALID_DOCID, docId);
    }
}

void IndexTestCase::testIndexWithEmptyField()
{
    DocumentSchema schema;
    schema.addUnIndexedField("PATH");
    schema.addTextField("TITLE");
    schema.addTextField("CONTENT");

    buildIndex(schema, "file1.txt, hello world;file2.txt, hello world");

    string str = getTestPath();
    Index index;

    try
    {
        index.open(str, Index::READ, NULL);
    }
    catch(const FirteXException& e)
    {
        cout << "===ERROR: " << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }
}

string IndexTestCase::getTestPath()
{
    tstring str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("/test_index");
    return p.toString();
}

void IndexTestCase::checkDocFreq(IndexReaderPtr& pIndexReader,
                                 const tstring& sField, 
                                 const tstring& sTerm,
                                 df_t expDf)
{
    TermReaderPtr pTermReader = pIndexReader->termReader();
    CPPUNIT_ASSERT(pTermReader);    

    StandardAnalyzer sa;
    sa.init();

    TokenViewPtr pTokens = sa.tokenize(sTerm.c_str(), sTerm.length());
    CPPUNIT_ASSERT(pTokens.isNull() != true);
    CPPUNIT_ASSERT(pTokens->getNumTokens() == 1);
    TokenView::Iterator it = pTokens->iterator();
    TermPtr pTerm(new Term(sField, it.next().getTextValue()));

    TermPostingIteratorPtr pPost = pTermReader->seek(pTerm.get());
    CPPUNIT_ASSERT(!pPost.isNull());    
    const TermMeta& termMeta = pPost->getTermMeta();
    CPPUNIT_ASSERT_EQUAL(expDf, termMeta.getDocFreq());
}

FX_NS_END
