#include "search/DateTimeSearchTestCase.h"
#include "TestHelper.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "../index/IndexBuildHelper.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(document);
FX_NS_USE(index);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, DateTimeSearchTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DateTimeSearchTestCase);

DateTimeSearchTestCase::DateTimeSearchTestCase() 
{
}

DateTimeSearchTestCase::~DateTimeSearchTestCase() 
{
}

void DateTimeSearchTestCase::setUp()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

void DateTimeSearchTestCase::tearDown()
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

void DateTimeSearchTestCase::testSearch()
{
    DocumentSchema schema;
    schema.addUnIndexedField("docid");
    schema.addTextField("TITLE");
    schema.addField("modified", "DATETIME64", true);

    buildIndex(schema, "doc1, hello, 2011-12-27 21:51:1; "
               "doc2, hello, 2050-1-1 1:51:12");

    QueryHitsPtr pHits = m_pIndexSearcher->search("query=TITLE:hello;"
            "filter=modified=DATE(\'2011-12-27 21:51:1\')", *m_pQueryParser);
    CPPUNIT_ASSERT(pHits);
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());

    pHits = m_pIndexSearcher->search("query=TITLE:hello;"
            "filter=NOW()>modified", *m_pQueryParser);
    CPPUNIT_ASSERT(pHits);
    CPPUNIT_ASSERT_EQUAL((size_t)1, pHits->size());
}

void DateTimeSearchTestCase::buildIndex(const DocumentSchema& schema,
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
                    "modified", QueryParser::OP_AND));
}

string DateTimeSearchTestCase::getTestPath()
{
    string str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("test_indexsearcher");
    return p.toString();
}

FX_NS_END

