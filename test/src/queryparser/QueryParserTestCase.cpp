#include "queryparser/QueryParserTestCase.h"
#include "firtex/analyzer/WhitespaceAnalyzer.h"

using namespace std;
FX_NS_USE(analyzer);
FX_NS_USE(search);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, QueryParserTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(QueryParserTestCase);

QueryParserTestCase::QueryParserTestCase() 
    : m_sDefField("def_field")
    , m_defOp(QueryParser::OP_AND)
{
}

QueryParserTestCase::~QueryParserTestCase() 
{
}

void QueryParserTestCase::setUp()
{
    m_pAnalyzerMapper.assign(new AnalyzerMapper());
    m_pAnalyzerMapper->map(0, "def_field", AnalyzerPtr(new WhitespaceAnalyzer));
    m_pAnalyzerMapper->map(1, "field1", AnalyzerPtr(new WhitespaceAnalyzer));
    m_pAnalyzerMapper->map(2, "field2", AnalyzerPtr(new WhitespaceAnalyzer));
    m_pAnalyzerMapper->map(2, "root.field", AnalyzerPtr(new WhitespaceAnalyzer));
    m_pAnalyzerMapper->map(2, "root_field", AnalyzerPtr(new WhitespaceAnalyzer));
}

void QueryParserTestCase::tearDown()
{
}

void QueryParserTestCase::testParseTermQuery()
{
    QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
    QueryPtr pQuery = parser.parse("field1: abc");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    CPPUNIT_ASSERT_EQUAL(std::string("TermQuery: [field1: abc]"),
                         pQuery->toString());
}

void QueryParserTestCase::testParseTermQueryWithDefField()
{
    QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
    QueryPtr pQuery = parser.parse("abc");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    
    CPPUNIT_ASSERT_EQUAL(std::string("TermQuery: [def_field: abc]"),
                         pQuery->toString());
}

void QueryParserTestCase::testParseQuery()
{
    QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
    QueryPtr pQuery = parser.parse("field1: \"abc def\"");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    CPPUNIT_ASSERT_EQUAL(std::string("PhraseQuery: [field1: abc, field1: def]"),
                         pQuery->toString());
}

void QueryParserTestCase::testParseQueryWithOneTerm()
{
    QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
    QueryPtr pQuery = parser.parse("field1: \"abcdef\"");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    CPPUNIT_ASSERT_EQUAL(std::string("TermQuery: [field1: abcdef]"),
                         pQuery->toString());
}

void QueryParserTestCase::testTermQueryWithMultiTerm()
{
    QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
    QueryPtr pQuery = parser.parse("field2: \'abc def\'");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    CPPUNIT_ASSERT_EQUAL(std::string("BooleanQuery: [+TermQuery: [field2: abc], "
                    "+TermQuery: [field2: def]]"),
                         pQuery->toString());
}

void QueryParserTestCase::testBooleanQuery()
{
    QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
    QueryPtr pQuery = parser.parse("field2: abc AND def\'");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    CPPUNIT_ASSERT_EQUAL(std::string("BooleanQuery: [+TermQuery: [field2: abc], "
                    "+TermQuery: [def_field: def]]"),
                         pQuery->toString());

    pQuery = parser.parse("field2: abc def\'");
    CPPUNIT_ASSERT(pQuery.isNotNull());
    CPPUNIT_ASSERT_EQUAL(std::string("BooleanQuery: [+TermQuery: [field2: abc], "
                    "+TermQuery: [def_field: def]]"),
                         pQuery->toString());
}

void QueryParserTestCase::testTermQueryWithSpecialFieldName()
{
    {
        QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
        QueryPtr pQuery = parser.parse("root.field: abc");
        CPPUNIT_ASSERT(pQuery.isNotNull());
        CPPUNIT_ASSERT_EQUAL(std::string("TermQuery: [root.field: abc]"),
                             pQuery->toString());
    }

    {
        QueryParser parser(m_pAnalyzerMapper.get(), m_sDefField, m_defOp);
        QueryPtr pQuery = parser.parse("root_field: abc");
        CPPUNIT_ASSERT(pQuery.isNotNull());
        CPPUNIT_ASSERT_EQUAL(std::string("TermQuery: [root_field: abc]"),
                             pQuery->toString());
    }
}

FX_NS_END

