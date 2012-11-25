#ifndef __FX_SINGLEWORDANALYZERTESTCASE_H
#define __FX_SINGLEWORDANALYZERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(analyzer);

class SingleWordAnalyzerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(SingleWordAnalyzerTestCase);
    CPPUNIT_TEST(testTokenize);
    CPPUNIT_TEST_SUITE_END();
public:
    SingleWordAnalyzerTestCase();
    ~SingleWordAnalyzerTestCase();

    void setUp();
    void tearDown();

protected:
    void testTokenize();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_SINGLEWORDANALYZERTESTCASE_H
