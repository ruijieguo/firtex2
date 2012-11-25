#ifndef __FX_TEXTTERMREADERTESTCASE_H
#define __FX_TEXTTERMREADERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/text/TextIndexer.h"
#include "firtex/index/text/TextTermReader.h"
#include "firtex/utility/HashMap.h"

FX_NS_DEF(index);

class TextTermReaderTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TextTermReaderTestCase);
    CPPUNIT_TEST(testNumTerms);
    CPPUNIT_TEST(testTermPosIterator);
    CPPUNIT_TEST(testTermIterator);
    CPPUNIT_TEST(testInMemTermIterator);
    CPPUNIT_TEST(testTermRangeIterator);
    CPPUNIT_TEST(testInMemTermRangeIterator);
    CPPUNIT_TEST(testSeek);
    CPPUNIT_TEST(testInMemSeek);
    CPPUNIT_TEST_SUITE_END();
public:
    TextTermReaderTestCase();
    ~TextTermReaderTestCase();

    void setUp();
    void tearDown();

protected:
    void testNumTerms();
    void testTermPosIterator();
    void testTermIterator();
    void testInMemTermIterator();
    void testTermRangeIterator();
    void testInMemTermRangeIterator();
    void testSeek();
    void testInMemSeek();

private:
    typedef std::map<uint64_t, TermMetaPtr> TermMap;
    typedef std::map<uint64_t, std::string> TermHashMap;

    void makeData();
    void checkIterator(TermIteratorPtr& pTermIt, const TermMap& answer);
    std::string getTestPath();

private:
    TextIndexerPtr m_pIndexer;
    TextTermReaderPtr m_pTermReader;
    FX_NS(document)::FieldSchemaPtr m_pFieldSchema;
    PostingPoolPtr m_pPool;
    TermMap m_answer;
    TermHashMap m_termHashMap;
    std::string m_sBarrel;
    FX_NS(store)::FileSystemPtr m_pFileSystem;
    FX_NS(store)::InputStreamPoolPtr m_pStreamPool;
    size_t m_nDataSize;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_TEXTTERMREADERTESTCASE_H
