#include "search/FieldSelectClauseTestCase.h"
#include "firtex/search/FieldSelectClause.h"

using namespace std;

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, FieldSelectClauseTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(FieldSelectClauseTestCase);

FieldSelectClauseTestCase::FieldSelectClauseTestCase() 
{
}

FieldSelectClauseTestCase::~FieldSelectClauseTestCase() 
{
}

void FieldSelectClauseTestCase::setUp()
{
}

void FieldSelectClauseTestCase::tearDown()
{
}

void FieldSelectClauseTestCase::testFromString()
{
    FieldSelectClause clause("field_select", "title,body:snippet"
                             "{!pre_tag=<em>,post_tag=</em>,separator=....},timestamp,"
                             "author:snippet");

    CPPUNIT_ASSERT_EQUAL((size_t)4, clause.getFieldCount());
    
    const FieldSelectClause::SnippetParam& p1 = clause.getField(0);
    CPPUNIT_ASSERT(!p1.snippet);
    CPPUNIT_ASSERT_EQUAL(string("title"), p1.field);
    CPPUNIT_ASSERT_EQUAL(string(""), p1.preTag);
    CPPUNIT_ASSERT_EQUAL(string(""), p1.postTag);
    CPPUNIT_ASSERT_EQUAL(string(""), p1.separator);

    const FieldSelectClause::SnippetParam& p2 = clause.getField(1);
    CPPUNIT_ASSERT(p2.snippet);
    CPPUNIT_ASSERT_EQUAL(string("body"), p2.field);
    CPPUNIT_ASSERT_EQUAL(string("<em>"), p2.preTag);
    CPPUNIT_ASSERT_EQUAL(string("</em>"), p2.postTag);
    CPPUNIT_ASSERT_EQUAL(string("...."), p2.separator);

    const FieldSelectClause::SnippetParam& p3 = clause.getField(2);
    CPPUNIT_ASSERT(!p3.snippet);
    CPPUNIT_ASSERT_EQUAL(string("timestamp"), p3.field);
    CPPUNIT_ASSERT_EQUAL(string(""), p3.preTag);
    CPPUNIT_ASSERT_EQUAL(string(""), p3.postTag);
    CPPUNIT_ASSERT_EQUAL(string(""), p3.separator);

    const FieldSelectClause::SnippetParam& p4 = clause.getField(3);
    CPPUNIT_ASSERT(p4.snippet);
    CPPUNIT_ASSERT_EQUAL(string("author"), p4.field);
    CPPUNIT_ASSERT_EQUAL(string(""), p4.preTag);
    CPPUNIT_ASSERT_EQUAL(string(""), p4.postTag);
    CPPUNIT_ASSERT_EQUAL(string(""), p4.separator);
}

FX_NS_END

