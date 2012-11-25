#ifndef __INDEXTESTCASE_H
#define __INDEXTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"
#include "firtex/index/Index.h"
#include <string>

FX_NS_DEF(document);
class DocumentSchema;
FX_NS_END

FX_NS_DEF(index);

class IndexTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(IndexTestCase);
    CPPUNIT_TEST(testTextIndex);
    CPPUNIT_TEST(testKeywordIndex);
    CPPUNIT_TEST(testPrimaryKeyIndex);
    CPPUNIT_TEST(testInt32ForwardIndex);
    CPPUNIT_TEST(testInt32Index);
    CPPUNIT_TEST(testInt32_IF);
    CPPUNIT_TEST(testIncrementalIndex);
    CPPUNIT_TEST(testIndexOptimize);
    CPPUNIT_TEST(testRefreshIndex);
    CPPUNIT_TEST(testRefreshIndexAfterOptimize);
    CPPUNIT_TEST(testRefreshIndexWithMultiBarrel);
    CPPUNIT_TEST(testDocumentDeletion);
    CPPUNIT_TEST(testIndexWithEmptyField);
    CPPUNIT_TEST_SUITE_END();
public:
    IndexTestCase(void);
    ~IndexTestCase(void);

public:
    void setUp();
    void tearDown();

protected:
    void buildIndex(const FX_NS(document)::DocumentSchema& schema,
                    const std::string& fieldsStr, bool bInc = false);

protected:
    void testTextIndex();
    void testKeywordIndex();
    void testPrimaryKeyIndex();
    void testInt32ForwardIndex();
    void testInt32Index();
    void testInt32_IF();
    void testIncrementalIndex();
    void testIndexOptimize();
    void testRefreshIndex();
    void testRefreshIndexAfterOptimize();
    void testRefreshIndexWithMultiBarrel();
    void testDocumentDeletion();
    void testIndexWithEmptyField();

protected:
    std::string getTestPath();
    void checkDocFreq(IndexReaderPtr& pIndexReader,
                      const tstring& sField, 
                      const tstring& sTerm,
                      df_t expDf);
};

FX_NS_END

#endif
