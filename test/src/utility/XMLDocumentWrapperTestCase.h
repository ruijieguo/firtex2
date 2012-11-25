#ifndef __FX_XMLDOCUMENTWRAPPERTESTCASE_H
#define __FX_XMLDOCUMENTWRAPPERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(utility);

class XMLDocumentWrapperTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(XMLDocumentWrapperTestCase);
    CPPUNIT_TEST(testParse);
    CPPUNIT_TEST(testPrint);
    CPPUNIT_TEST(testClearAndReuse);
    CPPUNIT_TEST_SUITE_END();
public:
    XMLDocumentWrapperTestCase();
    ~XMLDocumentWrapperTestCase();

    void setUp();
    void tearDown();

protected:
    void testParse();
    void testPrint();
    void testClearAndReuse();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_XMLDOCUMENTWRAPPERTESTCASE_H
