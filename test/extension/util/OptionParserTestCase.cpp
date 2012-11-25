#include "OptionParserTestCase.h"
#include <memory>
#include "../src/TestHelper.h"
#include "firtex/extension/util/OptionException.h"

using namespace std;
FX_NS_DEF(util);

CPPUNIT_TEST_SUITE_REGISTRATION(OptionParserTestCase);

OptionParserTestCase::OptionParserTestCase(void)
{
    m_pParser = NULL;
    m_bOptionIsOk = false;
}

OptionParserTestCase::~OptionParserTestCase(void)
{
}

void OptionParserTestCase::setUp()
{
    m_pParser = new OptionParser();
    m_bOptionIsOk = false;
}

void OptionParserTestCase::tearDown()
{
    delete m_pParser;
    m_pParser = NULL;
}

void OptionParserTestCase::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull(_T("long_opt")))
    {
        m_bOptionIsOk = true;
    }
    else if (option.matchShort(_T("s")))
    {
        m_bOptionIsOk = true;
    }

    if (option.matchFull(_T("cmd-with-arg")))
    {
        m_sArgument = sValue;
    }
    else if (option.matchShort(_T("c")))
    {
        m_sArgument = sValue;
    }
}

void OptionParserTestCase::testParse()
{
    try
    {
        m_pParser->addOption(Option(_T("long_opt"), _T("s"), _T("option test"))
                             .required(true)
                             .repeatable(false)
                             .callback(OptionCallback<OptionParserTestCase>(
                                           this, &OptionParserTestCase::optionCallback)));
        const char* args[2] = {"test", "--long_opt"}; 
        m_pParser->parse(2, args);
        CPPUNIT_ASSERT(m_bOptionIsOk);

        m_bOptionIsOk = false;
        const char* shortArgs[2] = {"test", "-s"}; 
        m_pParser->parse(2, shortArgs);
        CPPUNIT_ASSERT(m_bOptionIsOk);
    }
    catch(const OptionException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

void OptionParserTestCase::testParseOptionWithArgument()
{
    try
    {
        m_pParser->addOption(Option(_T("cmd-with-arg"), _T("c"), _T("cmd with argument"))
                        .required(true)
                        .repeatable(false)
                        .argument("[ARGUMENT]", true)
                             .callback(OptionCallback<OptionParserTestCase>(
                                             this, &OptionParserTestCase::optionCallback)));

        m_sArgument = "";
        const char* args[2] = {"test", "--cmd-with-arg=argument"}; 
        m_pParser->parse(2, args);
        CPPUNIT_ASSERT_EQUAL(string("argument"), m_sArgument);

        m_sArgument = "";
        const char* shortArgs[3] = {"test", "-c", "argument"}; 
        m_pParser->parse(3, shortArgs);
        CPPUNIT_ASSERT_EQUAL(string("argument"), m_sArgument);
    }
    catch(const OptionException& )
    {
        CPPUNIT_ASSERT(false);
    }
}

FX_NS_END
