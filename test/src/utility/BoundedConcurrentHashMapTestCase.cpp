#include "utility/BoundedConcurrentHashMapTestCase.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/thread/Thread.h"
#include <time.h>
#include <vector>

using namespace std;
FX_NS_USE(thread);

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, BoundedConcurrentHashMapTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(BoundedConcurrentHashMapTestCase);

class MapOperator : public Runnable
{
public:
    MapOperator(BoundedConcurrentHashMap<int32_t, int32_t>& m,
                const vector<int32_t>& ops, int64_t nRunTime)
        : m_map(m)
        , m_ops(ops)
        , m_nTimeCost(0)
        , m_nRunningTime(nRunTime)
    {
    }

    int32_t getTimeCost() const
    {
        return m_nTimeCost;
    }

protected:
    BoundedConcurrentHashMap<int32_t, int32_t>& m_map;
    const vector<int32_t>& m_ops;

    int64_t m_nTimeCost;
    int64_t m_nRunningTime;
};

DEFINE_TYPED_PTR(MapOperator);

class Getter : public MapOperator
{
public:
    Getter(BoundedConcurrentHashMap<int32_t, int32_t>& m,
           const vector<int32_t>& ops, int64_t nRunTime)
        : MapOperator(m, ops, nRunTime)
    {
    }

public:
    void run()
    {
        Timestamp startTime;
        size_t nRuns = 0;
        size_t nTotalRuns = 0;

        while (startTime.elapsed() <= m_nRunningTime)
        {
            m_map.get(m_ops[nRuns]);
            ++nRuns;
            ++nTotalRuns;
            if (nRuns >= m_ops.size())
            {
                nRuns = 0;
            }
        }
        m_nTimeCost = startTime.elapsed();
    }
};

class Putter : public MapOperator
{
public:
    Putter(BoundedConcurrentHashMap<int32_t, int32_t>& m,
           const vector<int32_t>& ops, int64_t nRunTime)
        : MapOperator(m, ops, nRunTime)
    {
    }

public:
    void run()
    {
        Timestamp startTime;
        size_t nRuns = 0;
        size_t nTotalRuns = 0;

        while (startTime.elapsed() <= m_nRunningTime)
        {
            m_map.put(m_ops[nRuns], nRuns);
            ++nRuns;
            ++nTotalRuns;
            if (nRuns >= m_ops.size())
            {
                nRuns = 0;
            }
        }

        m_nTimeCost = startTime.elapsed();
    }
};


class Remover : public MapOperator
{
public:
    Remover(BoundedConcurrentHashMap<int32_t, int32_t>& m,
            const vector<int32_t>& ops, int64_t nRunTime)
        : MapOperator(m, ops, nRunTime)
    {
    }

public:
    void run()
    {
        Timestamp startTime;
        size_t nRuns = 0;
        size_t nTotalRuns = 0;

        while (startTime.elapsed() <= m_nRunningTime)
        {
            m_map.remove(m_ops[nRuns]);
            ++nRuns;
            ++nTotalRuns;
            if (nRuns >= m_ops.size())
            {
                nRuns = 0;
            }
        }
        m_nTimeCost = startTime.elapsed();
    }
};

BoundedConcurrentHashMapTestCase::BoundedConcurrentHashMapTestCase() 
{
}

BoundedConcurrentHashMapTestCase::~BoundedConcurrentHashMapTestCase() 
{
}

void BoundedConcurrentHashMapTestCase::setUp()
{
}

void BoundedConcurrentHashMapTestCase::tearDown()
{
}

void BoundedConcurrentHashMapTestCase::testPut()
{
    BoundedConcurrentHashMap<int64_t, int64_t>  m(2, 2, -1);
    m.put((const int64_t&)1, (const int64_t&)2);
    m.put((const int64_t&)2, (const int64_t&)1);
    CPPUNIT_ASSERT_EQUAL((size_t)2, m.size());
    CPPUNIT_ASSERT_EQUAL((int64_t)2, m.get(1));
    CPPUNIT_ASSERT_EQUAL((int64_t)1, m.get(2));

    m.put(3, 4);
    CPPUNIT_ASSERT_EQUAL((size_t)2, m.size());
    CPPUNIT_ASSERT_EQUAL((int64_t)4, m.get(3));
    CPPUNIT_ASSERT_EQUAL((int64_t)1, m.get(2));
    CPPUNIT_ASSERT_EQUAL((int64_t)-1, m.get(1));
}

void BoundedConcurrentHashMapTestCase::testPutWithLIRS()
{
    typedef BoundedConcurrentHashMap<int64_t, int64_t> LIRSMap;

    LIRSMap  m(2, 2, LIRSMap::EVICTION_LIRS, -1);
    m.put((const int64_t&)1, (const int64_t&)2);
    m.put((const int64_t&)2, (const int64_t&)1);
    CPPUNIT_ASSERT_EQUAL((size_t)2, m.size());
    CPPUNIT_ASSERT_EQUAL((int64_t)2, m.get(1));
    CPPUNIT_ASSERT_EQUAL((int64_t)1, m.get(2));

    m.put(3, 4);
    CPPUNIT_ASSERT_EQUAL((size_t)2, m.size());
    CPPUNIT_ASSERT_EQUAL((int64_t)4, m.get(3));
    CPPUNIT_ASSERT_EQUAL((int64_t)1, m.get(2));
    CPPUNIT_ASSERT_EQUAL((int64_t)-1, m.get(1));
}

void BoundedConcurrentHashMapTestCase::testManyPutWithLIRS()
{
    typedef BoundedConcurrentHashMap<int64_t, int64_t> LIRSMap;

    const static size_t TEST_TOTAL_COUNT = 17;
    const static size_t TEST_KEEP_COUNT = 16;

    LIRSMap  m(TEST_KEEP_COUNT, 8, LIRSMap::EVICTION_LIRS, -1);

    for (size_t i = 0; i < TEST_TOTAL_COUNT; ++i)
    {
        m.put(i, i + 1);
    }
//    CPPUNIT_ASSERT_EQUAL((size_t)TEST_KEEP_COUNT, m.size());
}

void BoundedConcurrentHashMapTestCase::testPerfOfLRU()
{
    const static size_t MAX_CAPACITY = 50 * 1000;
    typedef BoundedConcurrentHashMap<int32_t, int32_t> Map;
    Map m(MAX_CAPACITY, 8, Map::EVICTION_LIRS, -1);
    doStressTest("LRU BoundedConcurrentHashMap", m, 27, 3, 0, 10000000);
}

void BoundedConcurrentHashMapTestCase::doStressTest(const std::string& sMapName, 
        BoundedConcurrentHashMap<int32_t, int32_t>& map,
        size_t nNumReaders, size_t nNumWriters, size_t nNumRemovers, int64_t runningTimeout)
{
    std::vector<ThreadPtr> threads;
    threads.reserve(nNumReaders + nNumWriters + nNumWriters);
    std::vector<MapOperatorPtr> getters;
    getters.reserve(nNumReaders);
    std::vector<MapOperatorPtr> putters;
    putters.reserve(nNumWriters);
    std::vector<MapOperatorPtr> removers;
    removers.reserve(nNumWriters);

    std::vector<int32_t> readOps, writeOps, removeOps;
    generateDataForOps(readOps, writeOps, removeOps);
    
    for (size_t i = 0; i < nNumReaders; i++) 
    {
        ThreadPtr pGetThread = new Thread();
        MapOperatorPtr pRunner = new Getter(map, readOps, runningTimeout);
        getters.push_back(pRunner);
        threads.push_back(pGetThread);
        pGetThread->start(*pRunner);
    }

    for (size_t i = 0; i < nNumWriters; i++) 
    {
        ThreadPtr pGetThread = new Thread();
        MapOperatorPtr pRunner = new Putter(map, readOps, runningTimeout);
        putters.push_back(pRunner);
        threads.push_back(pGetThread);
        pGetThread->start(*pRunner);
    }

    for (size_t i = 0; i < nNumRemovers; i++) 
    {
        ThreadPtr pGetThread = new Thread();
        MapOperatorPtr pRunner = new Remover(map, readOps, runningTimeout);
        removers.push_back(pRunner);
        threads.push_back(pGetThread);
        pGetThread->start(*pRunner);
    }

    for (size_t i = 0; i < threads.size(); ++i)
    {
        threads[i]->join();
    }

    int64_t gets = 0;
    for (size_t i = 0; i < getters.size(); i++)
    {
        gets += getters[i]->getTimeCost();
    }

    int64_t puts = 0;
    for (size_t i = 0; i < putters.size(); i++)
    {
        puts += putters[i]->getTimeCost();
    }

    int64_t removes = 0;
    for (size_t i = 0; i < removers.size(); i++)
    {
        removes += removers[i]->getTimeCost();
    }

    cout << "Performance for map(" << sMapName << ")" << endl;
    cout << "Average get(ops/ms): " << gets / nNumReaders << endl;
    cout << "Average put(ops/ms): " << puts / nNumWriters << endl;
    cout << "Average remove(ops/ms): " << ( (nNumRemovers > 0) ? removes / nNumRemovers : 0 )<< endl;
    cout << "Size = " << map.size() << endl;
}

void BoundedConcurrentHashMapTestCase::generateDataForOps(
        vector<int32_t>& readOps, vector<int32_t>& writeOps, vector<int32_t>& removeOps)
{
    const static size_t LOOP_FACTOR = 20;
    const static size_t NUM_KEYS = 50 * 1000;

    for(size_t i = 0; i < NUM_KEYS * LOOP_FACTOR; i++) 
    {
        srand(time(NULL));
        readOps.push_back(rand() % NUM_KEYS);
        writeOps.push_back(rand() % NUM_KEYS);
        removeOps.push_back(rand() % NUM_KEYS);
    }
}

FX_NS_END

