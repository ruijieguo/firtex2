#ifndef __FX_SEARCHRESOURCETESTCASE_H
#define __FX_SEARCHRESOURCETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(app);

class SearchResourceTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(SearchResourceTestCase);
    CPPUNIT_TEST(testLoadClusters);
    CPPUNIT_TEST(testReloadTypes);
    CPPUNIT_TEST(testReloadClusters);
    CPPUNIT_TEST(testReloadGeneration);
    CPPUNIT_TEST_SUITE_END();
public:
    SearchResourceTestCase();
    ~SearchResourceTestCase();

    void setUp();
    void tearDown();

protected:
    void testLoadClusters();
    void testReloadTypes();
    void testReloadClusters();
    void testReloadGeneration();

protected:
    std::string getTestPath(const std::string& sCluster, 
                            const std::string& sType,
                            const std::string& sGen);

    void buildIndex(const std::string& sCluster,
                    const std::string& sType,
                    const std::string& sGen);
    void buildIndex(const std::string& sCluster,
                    const std::string& sType,
                    const std::string& sGen,
                    const std::string& sDocs,
                    bool bInc);
private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_SEARCHRESOURCETESTCASE_H
