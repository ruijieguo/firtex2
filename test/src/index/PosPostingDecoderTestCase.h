#ifndef __FX_POSPOSTINGDECODERTESTCASE_H
#define __FX_POSPOSTINGDECODERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/Pool.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/index/PosPostingDecoder.h"
#include <vector>
#include <map>

FX_NS_DEF(index);

class PosPostingDecoderTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(PosPostingDecoderTestCase);
    CPPUNIT_TEST(testDecodePosRecord);
    CPPUNIT_TEST(testDecodePosRecordManyTimes);
    CPPUNIT_TEST(testDecodePosRecordManyTimesInMem);
    CPPUNIT_TEST_SUITE_END();
public:
    PosPostingDecoderTestCase();
    ~PosPostingDecoderTestCase();

    void setUp();
    void tearDown();

    typedef std::pair<docid_t, std::vector<loc_t> > DocPair;
    typedef std::vector<DocPair> DocVector;

    typedef std::vector<size_t> InputVector;

protected:
    void testDecodePosRecord();
    void testDecodePosRecordManyTimes();
    void testDecodePosRecordManyTimesInMem();

protected:
    void setUpTestData();
    void tearDownTestData();
    void makeData(DocVector& answer, PosPostingWriterPtr& writer, size_t dataSize);
    void getCTF(const DocVector& answer, size_t startOff, size_t endOff, ctf_t& ctf);
    void generateInputData(InputVector& input);


protected:
    PostingPoolPtr m_pPool;
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

#endif //__FX_POSPOSTINGDECODERTESTCASE_H
