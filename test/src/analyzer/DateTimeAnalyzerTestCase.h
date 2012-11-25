#ifndef __FX_DATETIMEANALYZERTESTCASE_H
#define __FX_DATETIMEANALYZERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(analyzer);

class DateTimeAnalyzerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DateTimeAnalyzerTestCase);
    CPPUNIT_TEST(testParse);
    CPPUNIT_TEST_SUITE_END();
public:
    DateTimeAnalyzerTestCase();
    ~DateTimeAnalyzerTestCase();

    void setUp();
    void tearDown();

protected:
    void testParse();

private:
    time_t makeTime(int32_t y, int32_t m, int32_t d, 
                    int32_t h, int32_t min, int32_t s);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DATETIMEANALYZERTESTCASE_H
