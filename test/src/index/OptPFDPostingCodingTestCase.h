#ifndef __FX_OPTPFDPOSTINGCODINGTESTCASE_H
#define __FX_OPTPFDPOSTINGCODINGTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class OptPFDPostingCodingTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(OptPFDPostingCodingTestCase);
    CPPUNIT_TEST(testDecodeInt32);
    CPPUNIT_TEST_SUITE_END();
public:
    OptPFDPostingCodingTestCase();
    ~OptPFDPostingCodingTestCase();

    void setUp();
    void tearDown();

protected:
    void testDecodeInt32();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_OPTPFDPOSTINGCODINGTESTCASE_H
