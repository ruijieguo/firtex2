#ifndef __FX_BARRELTERMPOSTINGITERATORTESTCASE_H
#define __FX_BARRELTERMPOSTINGITERATORTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/PostingPool.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/index/DocPostingDecoder.h"
#include <vector>
#include <map>

FX_NS_DEF(index);

class BarrelTermPostingIteratorTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(BarrelTermPostingIteratorTestCase);
    CPPUNIT_TEST(testTermMeta);
    CPPUNIT_TEST(testSkipTo);
    CPPUNIT_TEST(testNextDocs);
    CPPUNIT_TEST(testSkipToAndNextDocs);
    CPPUNIT_TEST(testSkipToRandomly);
    CPPUNIT_TEST_SUITE_END();
public:
    BarrelTermPostingIteratorTestCase();
    ~BarrelTermPostingIteratorTestCase();

    void setUp();
    void tearDown();

    typedef std::pair<docid_t, tf_t> DocPair;
    typedef std::vector<DocPair> DocVector;

protected:
    void setUpTestData();
    void tearDownTestData();
    void makeData(DocVector& answer, DocPostingWriterPtr& pWriter, size_t dataSize);

protected:
    void testTermMeta();
    void testSkipTo();
    void testNextDocs();
    void testSkipToAndNextDocs();
    void testSkipToRandomly();

protected:
    PostingPoolPtr m_pPostingPool;
    DocPostingWriterPtr m_pPostingWriter;

    ByteSliceOutputStreamPtr m_pDocOutStream;
    ByteSliceOutputStreamPtr m_pPosOutStream;

    ByteSliceInputStreamPtr m_pDocInStream;
    ByteSliceInputStreamPtr m_pPosInStream;
    
    DocPostingDecoderPtr m_pDecoder;

    DocVector m_answer;
    size_t m_nTestDataSize;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BARRELTERMPOSTINGITERATORTESTCASE_H
