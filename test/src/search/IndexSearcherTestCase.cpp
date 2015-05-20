#include <algorithm>
#include <map>
#include "search/IndexSearcherTestCase.h"
#include "TestHelper.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/index/OptimizeMergePolicy.h"
#include "firtex/search/ScorerProfileFactory.h"
#include "../index/IndexBuildHelper.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(index);
FX_NS_USE(queryparser);
FX_NS_USE(document);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, IndexSearcherTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(IndexSearcherTestCase);

IndexSearcherTestCase::IndexSearcherTestCase() 
{
}

IndexSearcherTestCase::~IndexSearcherTestCase() 
{
}

void IndexSearcherTestCase::setUp()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void IndexSearcherTestCase::tearDown()
{
    m_pIndexReader.reset();
    m_pIndexSearcher.reset();
    m_pIndex.reset();
    m_pQueryParser.reset();

    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void IndexSearcherTestCase::testSearchTermQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test");

    QueryPtr pQuery = parseQuery("hello");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 1);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());
}

void IndexSearcherTestCase::testSearchPhraseQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test");

    QueryPtr pQuery = parseQuery("\"hello world\"");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)1, pHits->getTotalHits());

    pQuery = parseQuery("\"this is a smoke\"");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), 
            pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)1, pHits->getTotalHits());
}

void IndexSearcherTestCase::testSearchAnyQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test");

    QueryPtr pQuery = parseQuery("*:*");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(),
            pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());
}

void IndexSearcherTestCase::testSearchPhraseQueryAfterMerge()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, a b c");
    buildIndex(schema, "docid2, e f");

    {
        string str = getTestPath();
        Index index;
        index.open(str, Index::APPEND, NULL);
        IndexWriterPtr pIndexWriter = index.acquireWriter();
        MergePolicyPtr pMergePolicy(new OptimizeMergePolicy);
        pIndexWriter->setMergePolicy(pMergePolicy);
        pIndexWriter->mergeIndex();

        m_pIndex.assign(new Index);
        m_pIndex->open(str, Index::READ, NULL);
        m_pIndexReader = m_pIndex->acquireReader();
        m_pIndexSearcher.assign(new IndexSearcher(m_pIndexReader));

        m_pQueryParser.assign(new QueryParser(m_pIndexReader->getAnalyzerMapper(), 
                        "CONTENT", QueryParser::OP_AND));

    }

    QueryPtr pQuery = parseQuery("\"b c\"");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)1, pHits->getTotalHits());

    pQuery = parseQuery("\"e f\"");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), 
            pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)1, pHits->getTotalHits());
}

void IndexSearcherTestCase::testSearchPhraseQueryOfSWTEXT()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addField("CONTENT", "UTF8_SW_TEXT", false);

    buildIndex(schema, "docid1, hello world; docid2, I'm just testing out replacing a whole page with another page "
               "using JavaScript and I found this answer with document write. As to why document write");

    QueryPtr pQuery = parseQuery("\"As to why document\"");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), 
            pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)1, pHits->getTotalHits());
}

void IndexSearcherTestCase::testSearchPhraseQueryWithSlop()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, hello my world; "
               "docid3, hello my my my world");

    QueryPtr pQuery = parseQuery("\"hello world\"~2");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(),
            pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)1, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() > hitDoc2.getScore());
}

void IndexSearcherTestCase::testSearchAndQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test; "
               "docid3, hello hello my world");

    QueryPtr pQuery = parseQuery("hello AND world");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)2, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() > hitDoc2.getScore());
}

void IndexSearcherTestCase::testSearchAndQueryWithoutResult()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test; "
               "docid3, hello hello my world");

    QueryPtr pQuery = parseQuery("hello AND worlldd");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNull());
}

void IndexSearcherTestCase::testSearchOrQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test; "
               "docid3, hello");

    QueryPtr pQuery = parseQuery("hello OR world");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)2, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() > hitDoc2.getScore());
}

void IndexSearcherTestCase::testSearchNotQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, hello this is a smoke test; "
               "docid3, hello hello");

    QueryPtr pQuery = parseQuery("hello NOT world");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)2, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)1, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() > hitDoc2.getScore());
}

void IndexSearcherTestCase::testSearchReqOptQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test; "
               "docid3, hello");

    QueryPtr pQuery = parseQuery("+hello OR world");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)2, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() > hitDoc2.getScore());
}

void IndexSearcherTestCase::testSearchMixedAndOrQuery()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test; "
               "docid3, hello; docid4, not in result");

    QueryPtr pQuery = parseQuery("(hello AND world) OR smoke");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    QueryTracerPtr pNullTracer;
    QueryHitsPtr pHits = m_pIndexSearcher->search(pQuery, NULL, NULL,
            ScorerProfileFactory::instance()->getProfile(), pNullTracer, 0, 10);
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)1, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() > hitDoc2.getScore());
}

void IndexSearcherTestCase::testSearchByStatementString()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test; "
               "docid3, hello; docid4, not in result");

    QueryHitsPtr pHits = searchByStatement("query=(hello AND world) OR smoke");
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)1, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() > hitDoc2.getScore());
}

void IndexSearcherTestCase::testSortByDocId()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");

    buildIndex(schema, "docid1, hello world; docid2, this is a smoke test; "
               "docid3, hello and hello again; docid4, not in result");

    QueryHitsPtr pHits = searchByStatement("query=hello;sort_by=INDEXORDER:ASC");
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)2, hitDoc2.getDocId());
    CPPUNIT_ASSERT(hitDoc1.getScore() < hitDoc2.getScore());
}

void IndexSearcherTestCase::testSortByField()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");
    schema.addSortableField("modified", FieldType::INT64, true);

    buildIndex(schema, "docid1, hello world, 16; docid2, hello this is a smoke test, 4; "
               "docid3, hello and hello again, 36; docid4, not in result, 1");

    QueryHitsPtr pHits = searchByStatement("query=hello;sort_by=SQRT(modified) + modified");
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)3, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)3, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)2, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc2.getDocId());

    const QueryHits::HitDoc& hitDoc3 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)1, hitDoc3.getDocId());
}

void IndexSearcherTestCase::testFilterByField()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");
    schema.addSortableField("modified", FieldType::INT64, true);

    buildIndex(schema, "docid1, hello world, 16; docid2, hello this is a smoke test, 4; "
               "docid3, hello and hello again, 36; docid4, not in result, 1");

    QueryHitsPtr pHits = searchByStatement("query=hello;filter=modified<=16");
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)2, pHits->getTotalHits());

    QueryHits::Iterator it = pHits->iterator();
    const QueryHits::HitDoc& hitDoc1 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)0, hitDoc1.getDocId());

    const QueryHits::HitDoc& hitDoc2 = it.next();
    CPPUNIT_ASSERT_EQUAL((docid_t)1, hitDoc2.getDocId());
}

bool compare_pair(const std::pair<int64_t, docid_t>& i,
                  const std::pair<int64_t, docid_t>& j)
{
    return i.first > j.first;
}

void IndexSearcherTestCase::testSortByFieldWithMultiBarrels()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("CONTENT");
    schema.addSortableField("modified", FieldType::INT64, true);

    const size_t NUM_BARRELS = 3;
    const size_t NUM_DOCS = 300;

    std::vector<std::pair<int64_t, docid_t> > mod;

    size_t idx = 0;
    for (size_t i = 0; i < NUM_BARRELS; ++i)
    {
        stringstream ss;
        for (size_t j = 0; j < NUM_DOCS; ++j)
        {
            int64_t m = (idx * 13) % 7;
            ss << "docid" << idx << "," << "hello world, " << m << ";";
            mod.push_back(make_pair(m, (docid_t)idx));
            ++idx;
        }
        buildIndex(schema, ss.str());
    }

    stable_sort(mod.begin(), mod.end(), compare_pair);

    QueryHitsPtr pHits = searchByStatement("query=hello;sort_by=modified;"
            "parameter=start:0,top:900");
    CPPUNIT_ASSERT(pHits.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)900, pHits->size());
    CPPUNIT_ASSERT_EQUAL((uint64_t)NUM_DOCS * NUM_BARRELS, pHits->getTotalHits());

    std::vector<std::pair<int64_t, docid_t> >::const_iterator it = mod.begin();
    QueryHits::Iterator hitIt = pHits->iterator();
    while (hitIt.hasNext())
    {
        const QueryHits::HitDoc& hitDoc = hitIt.next();
        CPPUNIT_ASSERT_EQUAL(it->second, hitDoc.getDocId());
        ++it;
    }
}

QueryHitsPtr IndexSearcherTestCase::searchByStatement(const string& sStat)
{
    return m_pIndexSearcher->search(sStat, *m_pQueryParser);
}

QueryPtr IndexSearcherTestCase::parseQuery(const string& queryExpr)
{
    return m_pQueryParser->parse(queryExpr);
}

void IndexSearcherTestCase::buildIndex(const DocumentSchema& schema,
                                       const string& fieldsStr)
{
    GLOBAL_CONF().Build.buildThreadCount = 1;
    string str = getTestPath();
    IndexBuildHelper::buildIndex(str, schema, fieldsStr, true);
    
    m_pIndex.assign(new Index);
    m_pIndex->open(str, Index::READ, NULL);
    m_pIndexReader = m_pIndex->acquireReader();
    m_pIndexSearcher.assign(new IndexSearcher(m_pIndexReader));

    m_pQueryParser.assign(new QueryParser(m_pIndexReader->getAnalyzerMapper(), 
                    "CONTENT", QueryParser::OP_AND));
}

string IndexSearcherTestCase::getTestPath()
{
    string str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("/test_indexsearcher");
    return p.toString();
}

FX_NS_END

