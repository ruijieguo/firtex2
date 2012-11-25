#ifndef __PRIMARYKEYINDEXTESTCASE_H
#define __PRIMARYKEYINDEXTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/index/IndexWriter.h"

#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class PrimaryKeyIndexTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(PrimaryKeyIndexTestCase);
    CPPUNIT_TEST(testBuildAndSearch);
//    CPPUNIT_TEST(testSearchInMem);
    CPPUNIT_TEST_SUITE_END();
public:
    PrimaryKeyIndexTestCase(void);
    ~PrimaryKeyIndexTestCase(void);

public:
    void tearDown();

protected:
    void testBuildAndSearch();
    void testSearchInMem();

private:
    void buildIndex(const std::string& sDocs, bool bInc);
    void buildIndex(const string& sDocs, IndexWriterPtr& pIndexWriter);
    std::string getIndexPath();
};

FX_NS_END

#endif
