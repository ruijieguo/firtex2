#ifndef __FX_DATETIMESEARCHTESTCASE_H
#define __FX_DATETIMESEARCHTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/Index.h"
#include "firtex/index/IndexReader.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/queryparser/QueryParser.h"

FX_NS_DEF(search);

class DateTimeSearchTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DateTimeSearchTestCase);
    CPPUNIT_TEST(testSearch);
    CPPUNIT_TEST_SUITE_END();
public:
    DateTimeSearchTestCase();
    ~DateTimeSearchTestCase();

    void setUp();
    void tearDown();

protected:
    void testSearch();

private:
    void buildIndex(const FX_NS(document)::DocumentSchema& schema,
                    const std::string& fieldsStr);
    std::string getTestPath();

private:
    FX_NS(index)::IndexPtr m_pIndex;
    FX_NS(index)::IndexReaderPtr m_pIndexReader;
    FX_NS(queryparser)::QueryParserPtr m_pQueryParser;
    IndexSearcherPtr m_pIndexSearcher;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DATETIMESEARCHTESTCASE_H
