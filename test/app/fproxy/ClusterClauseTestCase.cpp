#include "ClusterClauseTestCase.h"
#include "ClusterClause.h"

using namespace std;

FX_NS_DEF(app);

SETUP_STREAM_LOGGER(NONE, ClusterClauseTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(ClusterClauseTestCase);

ClusterClauseTestCase::ClusterClauseTestCase() 
{
}

ClusterClauseTestCase::~ClusterClauseTestCase() 
{
}

void ClusterClauseTestCase::setUp()
{
}

void ClusterClauseTestCase::tearDown()
{
}

void ClusterClauseTestCase::testFromString()
{
    ClusterClause clause("cluster", "cluster1,cluster2{!query=BODY:test},cluster3");
    
    ClusterClause::Iterator it = clause.iterator();
    CPPUNIT_ASSERT_EQUAL((size_t)3, it.size());
    
    CPPUNIT_ASSERT(it.hasNext());
    ClusterClause::ClausePair p = it.next();
    CPPUNIT_ASSERT_EQUAL(string("cluster1"), p->first);
    CPPUNIT_ASSERT(NULL == p->second);

    CPPUNIT_ASSERT(it.hasNext());
    p = it.next();
    CPPUNIT_ASSERT_EQUAL(string("cluster2"), p->first);
    CPPUNIT_ASSERT(NULL != p->second);

    CPPUNIT_ASSERT(it.hasNext());
    p = it.next();
    CPPUNIT_ASSERT_EQUAL(string("cluster3"), p->first);
    CPPUNIT_ASSERT(NULL == p->second);
}

FX_NS_END

