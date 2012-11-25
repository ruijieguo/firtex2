#include "index/TypedDictTestCase.h"
#include "firtex/index/TypedDict.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "TestHelper.h"

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, TypedDictTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(TypedDictTestCase);

TypedDictTestCase::TypedDictTestCase() 
{
}

TypedDictTestCase::~TypedDictTestCase() 
{
}

void TypedDictTestCase::setUp()
{
}

void TypedDictTestCase::tearDown()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);
    }
}

template<typename K, typename V>
void testFindTemp(const vector<pair<K, V> >& answer, TypedDict<K, V>& dict)
{
    for (size_t i = 0; i < answer.size(); ++i)
    {
        V v;
        CPPUNIT_ASSERT(dict.find(answer[i].first, v));
        CPPUNIT_ASSERT_EQUAL(answer[i].second, v);
    }
}

template<typename K, typename V>
void testIteratorTemp(const vector<pair<K, V> >& answer, TypedDict<K, V>& dict)
{
    typename TypedDict<K, V>::Iterator it = dict.iterator();
    CPPUNIT_ASSERT_EQUAL(answer.size(), it.size());
    for (size_t i = 0; i < answer.size(); ++i)
    {
        CPPUNIT_ASSERT(it.hasNext());
        typename TypedDict<K, V>::KeyValueType kv = it.next();
        CPPUNIT_ASSERT_EQUAL(answer[i].first, kv.first);
        CPPUNIT_ASSERT_EQUAL(answer[i].second, kv.second);
    }
}

template<typename K, typename V>
void testRangeIteratorTemp(const vector<pair<K, V> >& answer, TypedDict<K, V>& dict)
{
    for (size_t i = 0; i < answer.size(); i += answer.size()/20)
    {
        for (size_t j = i + 1; j < answer.size(); j += answer.size()/20)
        {
            typename TypedDict<K, V>::Iterator it = dict.rangeIterator(answer[i].first, answer[j].first);
            
            for (size_t k = i; k <= j; ++k)
            {
                CPPUNIT_ASSERT(it.hasNext());
                typename TypedDict<K, V>::KeyValueType kv = it.next();
                CPPUNIT_ASSERT_EQUAL(answer[k].first, kv.first);
                CPPUNIT_ASSERT_EQUAL(answer[k].second, kv.second);
            }
        }
    }
}

template<typename K, typename V>
void makeData(TypedDict<K, V>& dict, vector<pair<K, V> >& answer, size_t dataSize)
{
    answer.reserve(dataSize);
    dict.reserve(dataSize);
    for (size_t i = 0; i < dataSize; ++i)
    {
        answer.push_back(make_pair((uint64_t)(i * 1381 + (i % 189)), 
                        (int64_t)(i * 1831 + i % 981)));
        dict.pushBack(answer[i].first, answer[i].second);
    }
}

string TypedDictTestCase::getTestPath()
{
    tstring str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("/test_typeddict");
    return p.toString();
}

void TypedDictTestCase::testFind()
{
    const static size_t TEST_DATA_SIZE = 10000;
    vector<pair<uint64_t, int64_t> > answer;
    TypedDict<uint64_t, int64_t> dict;
    makeData(dict, answer, TEST_DATA_SIZE);

    testFindTemp<uint64_t, int64_t>(answer, dict);
}

void TypedDictTestCase::testIterator()
{
    const static size_t TEST_DATA_SIZE = 10000;
    vector<pair<uint64_t, int64_t> > answer;
    TypedDict<uint64_t, int64_t> dict;
    makeData(dict, answer, TEST_DATA_SIZE);

    testIteratorTemp<uint64_t, int64_t>(answer, dict);
}

void TypedDictTestCase::testRangeIterator()
{
    const static size_t TEST_DATA_SIZE = 10000;
    vector<pair<uint64_t, int64_t> > answer;
    TypedDict<uint64_t, int64_t> dict;
    makeData(dict, answer, TEST_DATA_SIZE);

    testRangeIteratorTemp<uint64_t, int64_t>(answer, dict);
}

void TypedDictTestCase::testSaveAndLoad()
{
    BlockFileSystem bfs;
    bfs.open(getTestPath(), FileSystem::CREATE);
    
    OutputStreamPtr pOutStream = bfs.createFile("test_dict");

    const static size_t TEST_DATA_SIZE = 10000;    
    vector<pair<uint64_t, int64_t> > answer;
    TypedDict<uint64_t, int64_t> dict;
    makeData(dict, answer, TEST_DATA_SIZE);

    dict.save(pOutStream);
    pOutStream->close();

    InputStreamPtr pInStream = bfs.openFile("test_dict");
    CPPUNIT_ASSERT(pInStream.isNotNull());
    
    TypedDict<uint64_t, int64_t> dict2;
    dict2.load(pInStream);
    
    pInStream->close();
    
    testFindTemp<uint64_t, int64_t>(answer, dict2);
    testIteratorTemp<uint64_t, int64_t>(answer, dict2);
    testRangeIteratorTemp<uint64_t, int64_t>(answer, dict2);
}


FX_NS_END

