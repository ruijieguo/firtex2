#ifndef __FX_QUERYPARSERTESTCASE_H
#define __FX_QUERYPARSERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/queryparser/QueryParser.h"

FX_NS_DEF(queryparser);

class QueryParserTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(QueryParserTestCase);
    CPPUNIT_TEST(testParseTermQuery);
    CPPUNIT_TEST(testParseTermQueryWithDefField);
    CPPUNIT_TEST(testParseQuery);
    CPPUNIT_TEST(testParseQueryWithOneTerm);
    CPPUNIT_TEST(testTermQueryWithMultiTerm);
    CPPUNIT_TEST(testBooleanQuery);
    CPPUNIT_TEST(testTermQueryWithSpecialFieldName);
    CPPUNIT_TEST_SUITE_END();
public:
    QueryParserTestCase();
    ~QueryParserTestCase();

    void setUp();
    void tearDown();

protected:
    void testParseTermQuery();
    void testParseTermQueryWithDefField();
    void testParseQuery();
    void testParseQueryWithOneTerm();
    void testTermQueryWithMultiTerm();
    void testBooleanQuery();
    void testTermQueryWithSpecialFieldName();

private:
    FX_NS(analyzer)::AnalyzerMapperPtr m_pAnalyzerMapper;
    std::string m_sDefField;
    QueryParser::OpType m_defOp;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_QUERYPARSERTESTCASE_H
