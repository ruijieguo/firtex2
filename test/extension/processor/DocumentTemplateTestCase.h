#ifndef __DOCUMENTTEMPLATETESTCASE_H
#define __DOCUMENTTEMPLATETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/config/Configurator.h"

FX_NS_DEF(processor);

class DocumentTemplateTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( DocumentTemplateTestCase );
    CPPUNIT_TEST(testToConf);
    CPPUNIT_TEST(testFromConf);
    CPPUNIT_TEST_SUITE_END();
public:
    DocumentTemplateTestCase(void);
    ~DocumentTemplateTestCase(void);
public:
    void setUp();
    void tearDown();
protected:
    void testToConf();
    void testFromConf();
    tstring toString(const FX_NS(config)::Configurator::ConfMap& confMap);
private:
};

FX_NS_END

#endif
