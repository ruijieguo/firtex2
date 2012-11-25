#include <vector>
#include <map>
#include "utility/LinkedHashMapTestCase.h"
#include "firtex/utility/LinkedHashMap.h"

using namespace std;

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, LinkedHashMapTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(LinkedHashMapTestCase);

LinkedHashMapTestCase::LinkedHashMapTestCase() 
{
}

LinkedHashMapTestCase::~LinkedHashMapTestCase() 
{
}

void LinkedHashMapTestCase::setUp()
{
}

void LinkedHashMapTestCase::tearDown()
{
}

void LinkedHashMapTestCase::testPut()
{
    typedef LinkedHashMap<int64_t, int64_t> Map;
    Map lm(-1);

    lm.put(1, 2);
    lm.put(2, 3);
    lm.put(3, 4);

    CPPUNIT_ASSERT_EQUAL((size_t)3, lm.size());

    CPPUNIT_ASSERT(lm.containsKey(1));
    CPPUNIT_ASSERT(lm.containsKey(2));
    CPPUNIT_ASSERT(lm.containsKey(3));
    CPPUNIT_ASSERT(!lm.containsKey(4));

    CPPUNIT_ASSERT_EQUAL((int64_t)2, lm.get(1));
    CPPUNIT_ASSERT_EQUAL((int64_t)3, lm.get(2));
    CPPUNIT_ASSERT_EQUAL((int64_t)4, lm.get(3));
    CPPUNIT_ASSERT_EQUAL((int64_t)-1, lm.get(5));

    Map::Iterator it = lm.iterator();

    CPPUNIT_ASSERT(it.hasNext());
    Map::Entry* e = it.nextEntry();
    CPPUNIT_ASSERT(e != NULL);
    CPPUNIT_ASSERT_EQUAL((int64_t)1, e->kv.first);
    CPPUNIT_ASSERT_EQUAL((int64_t)2, e->kv.second);

    CPPUNIT_ASSERT(it.hasNext());
    e = it.nextEntry();
    CPPUNIT_ASSERT(e != NULL);
    CPPUNIT_ASSERT_EQUAL((int64_t)2, e->kv.first);
    CPPUNIT_ASSERT_EQUAL((int64_t)3, e->kv.second);

    CPPUNIT_ASSERT(it.hasNext());
    e = it.nextEntry();
    CPPUNIT_ASSERT(e != NULL);
    CPPUNIT_ASSERT_EQUAL((int64_t)3, e->kv.first);
    CPPUNIT_ASSERT_EQUAL((int64_t)4, e->kv.second);

    CPPUNIT_ASSERT(!it.hasNext());
}

void LinkedHashMapTestCase::testManyPut()
{
    typedef LinkedHashMap<int64_t, int64_t> Map;
    typedef std::map<int64_t, int64_t> AnswerMap;
    typedef std::vector<std::pair<int64_t, int64_t> > AnswerVect;

    const static size_t TEST_ELEM_COUNT = 10000;
    
    Map lm(-1);
    AnswerVect aVect;
    for (size_t i = 0; i < TEST_ELEM_COUNT; ++i)
    {
        int64_t k = i;
        int64_t v = ((i * 100) % 13) + 23;
        aVect.push_back(std::make_pair(k, v));
        lm.put(k, v);
    }

    CPPUNIT_ASSERT_EQUAL((size_t)TEST_ELEM_COUNT, lm.size());

    Map::Iterator it = lm.iterator();
    for (size_t j = 0; j < aVect.size(); ++j)
    {
        CPPUNIT_ASSERT(it.hasNext());
        Map::Entry* e = it.nextEntry();
        CPPUNIT_ASSERT(e != NULL);
        CPPUNIT_ASSERT_EQUAL(aVect[j].first, e->kv.first);
        CPPUNIT_ASSERT_EQUAL(aVect[j].second, e->kv.second);

        CPPUNIT_ASSERT(lm.containsKey(aVect[j].first));
        CPPUNIT_ASSERT_EQUAL(aVect[j].second, lm.get(aVect[j].first));
    }

    CPPUNIT_ASSERT(!it.hasNext());
}

FX_NS_END

