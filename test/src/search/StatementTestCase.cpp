#include "search/StatementTestCase.h"
#include "firtex/search/Statement.h"
#include "firtex/utility/NumberFormatter.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, StatementTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(StatementTestCase);

StatementTestCase::StatementTestCase() 
{
}

StatementTestCase::~StatementTestCase() 
{
}

void StatementTestCase::setUp()
{
}

void StatementTestCase::tearDown()
{
}

void StatementTestCase::testParseAllClause()
{
    Statement stat;
    stat.fromString("query=abc;parameter=start:10,top:20");
    QueryClausePtr pQClause = stat.getQueryClause();
    CPPUNIT_ASSERT(pQClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL(string("abc"), pQClause->getQueryString());

    ParameterClausePtr pParamClause = stat.getParameterClause();
    CPPUNIT_ASSERT(pParamClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)10, pParamClause->getStartDoc());
    CPPUNIT_ASSERT_EQUAL((size_t)20, pParamClause->getTopDocs());
}

void StatementTestCase::testParseClauseWithSkipBlock()
{
    Statement stat;
    stat.fromString("query=abc;cluster=cluster1:{!query=abc;parameter=top:10}");
    QueryClausePtr pQClause = stat.getQueryClause();
    CPPUNIT_ASSERT(pQClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL(string("abc"), pQClause->getQueryString());

    ClausePtr pClusterClause = stat.getClause("cluster");
    CPPUNIT_ASSERT(pClusterClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL(string("cluster1:{!query=abc;parameter=top:10}"),
                         pClusterClause->getValue());
}

void StatementTestCase::testParseAllClauseWithSkipBlock()
{
    Statement stat;
    stat.fromString("query=abc;cluster=cluster1:{!query=abc;parameter=top:10};parameter=start:10,top:20");
    QueryClausePtr pQClause = stat.getQueryClause();
    CPPUNIT_ASSERT(pQClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL(string("abc"), pQClause->getQueryString());

    ParameterClausePtr pParamClause = stat.getParameterClause();
    CPPUNIT_ASSERT(pParamClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)10, pParamClause->getStartDoc());
    CPPUNIT_ASSERT_EQUAL((size_t)20, pParamClause->getTopDocs());

    ClausePtr pClusterClause = stat.getClause("cluster");
    CPPUNIT_ASSERT(pClusterClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL(string("cluster1:{!query=abc;parameter=top:10}"),
                         pClusterClause->getValue());
}

void StatementTestCase::testParseClauseWithSeperatorTail()
{
    Statement stat;
    stat.fromString("query=abc;field_select=TITLE,BODY:snippet;");
    QueryClausePtr pQClause = stat.getQueryClause();
    CPPUNIT_ASSERT(pQClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL(string("abc"), pQClause->getQueryString());

    FieldSelectClausePtr pFieldClause = stat.getFieldSelectClause();
    CPPUNIT_ASSERT(pFieldClause.isNotNull());
    CPPUNIT_ASSERT_EQUAL((size_t)2, pFieldClause->getFieldCount());
    
    const FieldSelectClause::SnippetParam& p = pFieldClause->getField(0);
    CPPUNIT_ASSERT(!p.snippet);
    CPPUNIT_ASSERT_EQUAL(string("TITLE"), p.field);

    const FieldSelectClause::SnippetParam& p2 = pFieldClause->getField(1);
    CPPUNIT_ASSERT(p2.snippet);
    CPPUNIT_ASSERT_EQUAL(string("BODY"), p2.field);
}

void StatementTestCase::testToString()
{
    std::string sPhaseParam;
    NumberFormatter::append(sPhaseParam, (int32_t)2);
    Statement stat;
    stat.setParameter("phase", sPhaseParam);

    std::stringstream ss;
    stat.toString(ss);
    CPPUNIT_ASSERT_EQUAL(string("parameter=start:0,top:10,phase:2;"), ss.str());
}

FX_NS_END

