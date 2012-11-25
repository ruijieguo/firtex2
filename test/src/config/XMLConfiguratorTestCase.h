#ifndef __XMLCONFIGURATORTESTCASE_H
#define __XMLCONFIGURATORTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"
#include "firtex/config/Configurator.h"
#include "firtex/common/Any.h"
#include <map>

FX_NS_DEF(config);

class XMLConfiguratorTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( XMLConfiguratorTestCase );
    CPPUNIT_TEST(testLoad);
    CPPUNIT_TEST_SUITE_END();
public:
    XMLConfiguratorTestCase(void);
    ~XMLConfiguratorTestCase(void);

public:
    void setUp();
    void tearDown();

protected:
    void testLoad();
    tstring toXML(const Configurator::ConfMap& confMap);
    tstring toXML(const std::multimap<tstring, FX_NS(common)::Any>& confMap);
};

FX_NS_END

#endif
