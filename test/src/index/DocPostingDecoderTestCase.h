#ifndef __FX_DOCPOSTINGDECODERTESTCASE_H
#define __FX_DOCPOSTINGDECODERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class DocPostingDecoderTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DocPostingDecoderTestCase);
    CPPUNIT_TEST(testDecodeDocRecord);
    CPPUNIT_TEST(testDecodeDocRecordRandom);
    CPPUNIT_TEST(testDecodeDocRecordWithSpecialSeq);
    CPPUNIT_TEST(testDecodeDocRecordInMem);
    CPPUNIT_TEST(testDecodeDocRecordRandomInMem);
    CPPUNIT_TEST_SUITE_END();
public:
    DocPostingDecoderTestCase();
    ~DocPostingDecoderTestCase();

    void setUp();
    void tearDown();

protected:
    void testDecodeDocRecord();
    void testDecodeDocRecordRandom();
    void testDecodeDocRecordWithSpecialSeq();
    void testDecodeDocRecordInMem();
    void testDecodeDocRecordRandomInMem();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DOCPOSTINGDECODERTESTCASE_H
