#ifndef __FX_BARRELTERMPOSITIONITERATORTESTCASE_H
#define __FX_BARRELTERMPOSITIONITERATORTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/PostingPool.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/index/PosPostingDecoder.h"
#include <vector>
#include <map>

FX_NS_DEF(index);

class BarrelTermPositionIteratorTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(BarrelTermPositionIteratorTestCase);
    CPPUNIT_TEST(testTermMeta);
    CPPUNIT_TEST(testSkipTo);
    CPPUNIT_TEST(testNextDocs);
    CPPUNIT_TEST(testSkipToAndNextDocs);
    CPPUNIT_TEST(testSkipToWithSpecialSeq);
    CPPUNIT_TEST(testSkipToRandomly);
    CPPUNIT_TEST(testSkipToPosition);
    CPPUNIT_TEST(testSkipToPositionRandomly);
    CPPUNIT_TEST_SUITE_END();
public:
    BarrelTermPositionIteratorTestCase();
    ~BarrelTermPositionIteratorTestCase();

    void setUp();
    void tearDown();

    typedef std::pair<docid_t, std::vector<loc_t> > DocPair;
    typedef std::vector<DocPair> DocVector;

    typedef std::pair<size_t, std::vector<size_t> >InputPair;
    typedef std::vector<InputPair> InputVector;

protected:
    void setUpTestData();
    void tearDownTestData();
    void makeData(DocVector& answer, PosPostingWriterPtr& pWriter, size_t dataSize);
    void getCTF(const DocVector& answer, size_t startOff, size_t endOff, ctf_t& ctf);

protected:
    void testTermMeta();
    void testSkipTo();
    void testNextDocs();
    void testSkipToAndNextDocs();
    void testSkipToWithSpecialSeq();
    void testSkipToRandomly();
    void testSkipToPosition();
    void testSkipToPositionRandomly();

    void generateInputData(InputVector& input);

protected:
    PostingPoolPtr m_pPostingPool;
    PosPostingWriterPtr m_pPostingWriter;

    ByteSliceOutputStreamPtr m_pDocOutStream;
    ByteSliceOutputStreamPtr m_pPosOutStream;

    ByteSliceInputStreamPtr m_pDocInStream;
    ByteSliceInputStreamPtr m_pPosInStream;
    
    PosPostingDecoderPtr m_pDecoder;

    DocVector m_answer;
    size_t m_nTestDataSize;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_BARRELTERMPOSITIONITERATORTESTCASE_H
