#ifndef __FX_STANDARDTOKENIZERTESTCASE_H
#define __FX_STANDARDTOKENIZERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(analyzer);

class StandardTokenizerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(StandardTokenizerTestCase);
    CPPUNIT_TEST(testTokenzieUTF8);
    CPPUNIT_TEST(testTokenzieGBK);
    CPPUNIT_TEST(testTokenizeGBKFile);
    CPPUNIT_TEST(testTokenizeUTF8File);
    CPPUNIT_TEST_SUITE_END();
public:
    StandardTokenizerTestCase();
    ~StandardTokenizerTestCase();

    void setUp();
    void tearDown();

protected:
    void testTokenzieUTF8();
    void testTokenzieGBK();
    void testTokenizeGBKFile();
    void testTokenizeUTF8File();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_STANDARDTOKENIZERTESTCASE_H
