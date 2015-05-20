#include "firtex/search/ScorerProfileClause.h"
#include "search/ScorerProfileClauseTestCase.h"

using namespace std;

FX_NS_DEF(search);

SETUP_LOGGER(search, ScorerProfileClauseTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(ScorerProfileClauseTestCase);

ScorerProfileClauseTestCase::ScorerProfileClauseTestCase() 
{
}

ScorerProfileClauseTestCase::~ScorerProfileClauseTestCase() 
{
}

void ScorerProfileClauseTestCase::setUp()
{
}

void ScorerProfileClauseTestCase::tearDown()
{
}

void ScorerProfileClauseTestCase::testFromString()
{
    ScorerProfileClause c("scorer_profile", 
                          "default");
    CPPUNIT_ASSERT_EQUAL(string("default"), c.getProfileName());

    ScorerProfileClause c2("scorer_profile", 
                           "rank_formula{!formula=a+b}");
    CPPUNIT_ASSERT_EQUAL(string("rank_formula"), c2.getProfileName());
    
    const ScorerProfileClause::KeyValueMap& params = c2.getParameters();
    CPPUNIT_ASSERT(params.size() == 1);
    ScorerProfileClause::KeyValueMap::const_iterator it = params.find("formula");
    CPPUNIT_ASSERT(it != params.end());
    CPPUNIT_ASSERT_EQUAL(string("formula"), it->first);
    CPPUNIT_ASSERT_EQUAL(string("a+b"), it->second);
}

FX_NS_END

