#include "HashMapTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/HashMap.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/Hash.h"
#include "firtex/store/BlockFileSystem.h"
#include <stdio.h>
#include <vector>
#include <map>
#include <limits>
#include "TestHelper.h"

using namespace std;

FX_NS_USE(store);
FX_NS_DEF(utility);


CPPUNIT_TEST_SUITE_REGISTRATION(HashMapTestCase);

HashMapTestCase::HashMapTestCase(void)
{	
}

HashMapTestCase::~HashMapTestCase(void)
{	
}


void HashMapTestCase::setUp()
{	
}

void HashMapTestCase::tearDown()
{
    File f(getTestPath());
    if (f.exists())
    {
        f.remove(true);	
    }
}

void HashMapTestCase::testLookupInt32()
{
    HashMap<int32_t, int32_t> m(0, -1);
    m.insert(1, 1);
    m.insert(24, 2);

    CPPUNIT_ASSERT_EQUAL(int32_t(1), m[1]);
    CPPUNIT_ASSERT_EQUAL(int32_t(2), m[24]);
}

void HashMapTestCase::testManyInsert()
{
    HashMap<int32_t, int64_t> m(-1, -1);

    map<int32_t, int64_t> ans;
    int32_t key;
    for (int32_t i = 0; i < 12345; i++)
    {
        srand((unsigned int)time(NULL));
        key = (int32_t)rand();
        int64_t value = (int64_t)rand();
        m.insert(key, value);
        ans.insert(make_pair(key, value));
    }
    
    CPPUNIT_ASSERT_EQUAL((size_t)12345, m.size());

    for (map<int32_t, int64_t>::const_iterator it = ans.begin();
         it != ans.end(); ++it)
    {
        CPPUNIT_ASSERT_EQUAL(it->second, m[it->first]);
    }
}

void HashMapTestCase::testIterator()
{
    typedef HashMap<int64_t, int32_t> Int64Map;
    Int64Map m(-1, -1);

    map<int64_t, int32_t> ans;
    int64_t key;
    for (int32_t i = 0; i < 10000; i++)
    {
        srand((unsigned int)time(NULL));
        key = (int64_t)rand();
        int32_t value = (int32_t)rand();
        m.insert(key, value);
        ans.insert(make_pair(key, value));
    }

    Int64Map::Iterator it = m.iterator();
    while (it.hasNext())
    {
        Int64Map::KeyValueType& kv = it.next();
        map<int64_t, int32_t>::const_iterator it2 = ans.find(kv.first);
        CPPUNIT_ASSERT(it2 != ans.end());
        CPPUNIT_ASSERT_EQUAL(it2->second, kv.second);
    }
}

void HashMapTestCase::testIteratorOfUInt64ToInt32()
{
    typedef HashMap<uint64_t, int32_t> UInt64Map;
    UInt64Map m(std::numeric_limits<uint64_t>::max(), -1);

    map<uint64_t, int32_t> ans;
    uint64_t key;
    for (int32_t i = 0; i < 100; i++)
    {
        stringstream ss;
        ss << "abc" << i;
        key = Hash::hashString64(ss.str().c_str());
        m.insert(key, i);
        ans.insert(make_pair(key, i));
    }

    UInt64Map::Iterator it = m.iterator();
    while (it.hasNext())
    {
        UInt64Map::KeyValueType& kv = it.next();
        map<uint64_t, int32_t>::const_iterator it2 = ans.find(kv.first);
        CPPUNIT_ASSERT(it2 != ans.end());
        CPPUNIT_ASSERT_EQUAL(it2->second, kv.second);
    }
}

void HashMapTestCase::testLoad()
{
    BlockFileSystem fileSys;
    fileSys.open(getTestPath(), BlockFileSystem::CREATE);

    map<uint64_t, int32_t> ans;
    {
        HashMap<uint64_t, int32_t> m(numeric_limits<uint64_t>::max(), -1);
        for (int32_t i = 0; i < 10000; i++)
        {
            stringstream ss1;
            ss1 << "abc" << i;
            uint64_t key = Hash::hashString64(ss1.str().c_str());
            int32_t value = i;
            m.insert(key, value);
            ans.insert(make_pair(key, value));
        }
        OutputStreamPtr pOutput = fileSys.createFile("hashmap.map");
        m.save(pOutput);
    }

    HashMap<uint64_t, int32_t> m(numeric_limits<uint64_t>::max(), -1);
    InputStreamPtr pInput = fileSys.openFile("hashmap.map");
    m.load(pInput);
        
    for (map<uint64_t, int32_t>::const_iterator it = ans.begin();
         it != ans.end(); ++it)
    {
        CPPUNIT_ASSERT_EQUAL(it->second, m[it->first]);
        CPPUNIT_ASSERT_EQUAL(it->second, m.find(it->first));
    }    
}

string HashMapTestCase::getTestPath()
{
    tstring str = TestHelper::getTestDataPath();
    Path p(str);
    p.makeDirectory();
    p.pushDirectory("hashmap_test");
    return p.toString();
}

FX_NS_END
