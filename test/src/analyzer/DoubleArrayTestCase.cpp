#include <map>
#include "analyzer/DoubleArrayTestCase.h"
#include "firtex/analyzer/DoubleArrayTrie.h"

using namespace std;

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, DoubleArrayTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DoubleArrayTestCase);

DoubleArrayTestCase::DoubleArrayTestCase() 
{
}

DoubleArrayTestCase::~DoubleArrayTestCase() 
{
}

void DoubleArrayTestCase::setUp()
{
}

void DoubleArrayTestCase::tearDown()
{
}

void DoubleArrayTestCase::testInsert()
{
    typedef std::map<std::string, int32_t> AnswerMap;
    const static size_t TEST_DATA_SIZE = 10000;

    DoubleArrayTrie da(TEST_DATA_SIZE);

    AnswerMap ans;
    for (size_t i = 0; i < TEST_DATA_SIZE; ++i)
    {
        stringstream ss;
        ss << "key" << i;
        da.insert(ss.str().c_str(), i);
        ans.insert(make_pair(ss.str(), i));
    }

    for (AnswerMap::const_iterator it = ans.begin();
         it != ans.end(); ++it)
    {
        CPPUNIT_ASSERT_EQUAL(it->second, (int32_t)da.search(it->first.c_str()));
    }
}

void DoubleArrayTestCase::testMaxForwardSearch()
{
    const static size_t TEST_DATA_SIZE = 10;
    DoubleArrayTrie da(TEST_DATA_SIZE);
    da.insert("信息", 1);
    da.insert("信息检索", 2);
    da.insert("北京", 3);
    da.insert("信息检索系统", 4);
    da.insert("中国上海", 5);

    const char* p = NULL;
    string str = "信息检索";

    CPPUNIT_ASSERT_EQUAL((int32_t)2, (int32_t)da.search(str.c_str(), &p));
    CPPUNIT_ASSERT(p == str.c_str() + str.length());

    string str2 = "信息检索工程";
    CPPUNIT_ASSERT_EQUAL((int32_t)2, (int32_t)da.search(str2.c_str(), &p));
    CPPUNIT_ASSERT(p == str2.c_str() + str.length());

    CPPUNIT_ASSERT_EQUAL((int32_t)2, (int32_t)da.search(str2.c_str(), str2.length(), &p));
    CPPUNIT_ASSERT(p == str2.c_str() + str.length());

    string str3 = "中国";
    CPPUNIT_ASSERT_EQUAL((int32_t)0, (int32_t)da.search(str3.c_str(), &p));
    CPPUNIT_ASSERT(p == str3.c_str());

    CPPUNIT_ASSERT_EQUAL((int32_t)0, (int32_t)da.search(str3.c_str(), str3.length(), &p));
    CPPUNIT_ASSERT(p == str3.c_str());

    string str4 = "上海";
    CPPUNIT_ASSERT_EQUAL((int32_t)0, (int32_t)da.search(str4.c_str(), &p));
    CPPUNIT_ASSERT(p == str4.c_str());

    CPPUNIT_ASSERT_EQUAL((int32_t)0, (int32_t)da.search(str4.c_str(), str4.length(), &p));
    CPPUNIT_ASSERT(p == str4.c_str());

    string str5 = "信息检索北京";
    CPPUNIT_ASSERT_EQUAL((int32_t)2, (int32_t)da.search(str5.c_str(), &p));
    CPPUNIT_ASSERT_EQUAL((int32_t)3, (int32_t)da.search(p, &p));

    CPPUNIT_ASSERT_EQUAL((int32_t)2, (int32_t)da.search(str5.c_str(), str5.length(), &p));
    CPPUNIT_ASSERT_EQUAL((int32_t)3, (int32_t)da.search(p, &p));
}


class TraversalHelper
{
public:
    typedef std::map<std::string, int32_t> AnswerMap;

    void invoke(const char* szKey, int value)
    {
        answer.insert(make_pair(szKey, (int32_t)value));
    }
    AnswerMap answer;
};

void DoubleArrayTestCase::testTraversal()
{
    typedef std::map<std::string, int32_t> AnswerMap;
    const static size_t TEST_DATA_SIZE = 10000;

    DoubleArrayTrie da(TEST_DATA_SIZE);

    AnswerMap ans;
    for (size_t i = 0; i < TEST_DATA_SIZE; ++i)
    {
        stringstream ss;
        ss << "key" << i;
        da.insert(ss.str().c_str(), i);
        ans.insert(make_pair(ss.str(), i));
    }


    TraversalHelper traHelper;

    da.setTraversalCallback<TraversalHelper>(&traHelper, &TraversalHelper::invoke);
    da.traversal();

    CPPUNIT_ASSERT_EQUAL(traHelper.answer.size(), ans.size());
    AnswerMap::const_iterator it1 = traHelper.answer.begin();
    AnswerMap::const_iterator it2 = ans.begin();
    for (; it1 != traHelper.answer.end(); ++it1, ++it2)
    {
        CPPUNIT_ASSERT_EQUAL(it1->first, it2->first);
        CPPUNIT_ASSERT_EQUAL(it1->second, it2->second);
    }
}

FX_NS_END

