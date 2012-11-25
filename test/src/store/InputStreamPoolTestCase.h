#ifndef __FX_INPUTSTREAMPOOLTESTCASE_H
#define __FX_INPUTSTREAMPOOLTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(store);

class InputStreamPoolTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(InputStreamPoolTestCase);
    CPPUNIT_TEST(testGetInputStream);
    CPPUNIT_TEST_SUITE_END();
public:
    InputStreamPoolTestCase();
    ~InputStreamPoolTestCase();

    void setUp();
    void tearDown();

protected:
    void testGetInputStream();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_INPUTSTREAMPOOLTESTCASE_H
