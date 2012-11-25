#ifndef __FX_S16CODINGTESTCASE_H
#define __FX_S16CODINGTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/compress/S16Coding.h"

FX_NS_DEF(index);

class S16CodingTestCase : public CPPUNIT_NS::TestFixture, public S16Coding
{
    CPPUNIT_TEST_SUITE(S16CodingTestCase);
    CPPUNIT_TEST(testEncode);
    CPPUNIT_TEST_SUITE_END();
public:
    S16CodingTestCase();
    ~S16CodingTestCase();

    void setUp();
    void tearDown();

protected:
    void testEncode();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_S16CODINGTESTCASE_H
