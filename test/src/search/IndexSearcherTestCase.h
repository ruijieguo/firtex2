#ifndef __FX_INDEXSEARCHERTESTCASE_H
#define __FX_INDEXSEARCHERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/Index.h"
#include "firtex/index/IndexReader.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/queryparser/QueryParser.h"

FX_NS_DEF(search);

class IndexSearcherTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(IndexSearcherTestCase);
    CPPUNIT_TEST(testSearchTermQuery);
    CPPUNIT_TEST(testSearchPhraseQuery);
    CPPUNIT_TEST(testSearchPhraseQueryAfterMerge);
    CPPUNIT_TEST(testSearchPhraseQueryOfSWTEXT);
    CPPUNIT_TEST(testSearchPhraseQueryWithSlop);
    CPPUNIT_TEST(testSearchAndQuery);
    CPPUNIT_TEST(testSearchAndQueryWithoutResult);
    CPPUNIT_TEST(testSearchOrQuery);
    CPPUNIT_TEST(testSearchNotQuery);
    CPPUNIT_TEST(testSearchReqOptQuery);
    CPPUNIT_TEST(testSearchMixedAndOrQuery);
    CPPUNIT_TEST(testSearchByStatementString);
    CPPUNIT_TEST(testSortByDocId);
    CPPUNIT_TEST(testSortByField);
    CPPUNIT_TEST(testFilterByField);
    CPPUNIT_TEST(testSortByFieldWithMultiBarrels);
    CPPUNIT_TEST_SUITE_END();
public:
    IndexSearcherTestCase();
    ~IndexSearcherTestCase();

    void setUp();
    void tearDown();

protected:
    void testSearchTermQuery();
    void testSearchPhraseQuery();
    void testSearchPhraseQueryAfterMerge();
    void testSearchPhraseQueryOfSWTEXT();
    void testSearchPhraseQueryWithSlop();
    void testSearchAndQuery();
    void testSearchAndQueryWithoutResult();
    void testSearchOrQuery();
    void testSearchNotQuery();
    void testSearchReqOptQuery();
    void testSearchMixedAndOrQuery();
    void testSearchByStatementString();
    void testSortByDocId();
    void testSortByField();
    void testFilterByField();
    void testSortByFieldWithMultiBarrels();

protected:
    std::string getTestPath();
    void buildIndex(const FX_NS(document)::DocumentSchema& schema,
                    const std::string& fieldsStr);
    QueryPtr parseQuery(const std::string& queryExpr);
    QueryHitsPtr searchByStatement(const std::string& sStat);

private:
    FX_NS(index)::IndexPtr m_pIndex;
    FX_NS(index)::IndexReaderPtr m_pIndexReader;
    FX_NS(queryparser)::QueryParserPtr m_pQueryParser;
    IndexSearcherPtr m_pIndexSearcher;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_INDEXSEARCHERTESTCASE_H
