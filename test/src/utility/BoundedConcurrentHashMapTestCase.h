#ifndef __FX_BOUNDEDCONCURRENTHASHMAPTESTCASE_H
#define __FX_BOUNDEDCONCURRENTHASHMAPTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/BoundedConcurrentHashMap.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(utility);

class BoundedConcurrentHashMapTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(BoundedConcurrentHashMapTestCase);
    CPPUNIT_TEST(testPut);
    CPPUNIT_TEST(testPutWithLIRS);
    CPPUNIT_TEST(testManyPutWithLIRS);
    CPPUNIT_TEST(testPerfOfLRU);
    CPPUNIT_TEST_SUITE_END();
public:
    BoundedConcurrentHashMapTestCase();
    ~BoundedConcurrentHashMapTestCase();

    void setUp();
    void tearDown();

    void doStressTest(const std::string& sMapName, 
                      BoundedConcurrentHashMap<int32_t, int32_t>& map,
                      size_t nNumReaders, size_t nNumWriters,
                      size_t nNumRemovers, int64_t runningTimeout);
    void generateDataForOps(std::vector<int32_t>& readOps,
                            std::vector<int32_t>& writeOps,
                            std::vector<int32_t>& removeOps);

protected:
    void testPut();
    void testPutWithLIRS();
    void testManyPutWithLIRS();
    void testPerfOfLRU();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BOUNDEDCONCURRENTHASHMAPTESTCASE_H
