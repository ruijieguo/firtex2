#ifndef __OPTIONPARSERTESTCASE_H
#define __OPTIONPARSERTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/OptionParser.h"

FX_NS_DEF(util);

class OptionParserTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( OptionParserTestCase );
    CPPUNIT_TEST(testParse);
    CPPUNIT_TEST(testParseOptionWithArgument);
    CPPUNIT_TEST_SUITE_END();

public:
    OptionParserTestCase(void);
    ~OptionParserTestCase(void);

public:
    void setUp();
    void tearDown();

public:
    void testParse();
    void testParseOptionWithArgument();
protected:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);

private:
    FX_NS(util)::OptionParser* m_pParser;
    bool m_bOptionIsOk;
    std::string m_sArgument;
};

FX_NS_END

#endif
