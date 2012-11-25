#include "XMLConfiguratorTestCase.h"
#include "firtex/config/XMLConfigurator.h"
#include <memory>
#include <sstream>
#include <typeinfo>
#include "TestHelper.h"

using namespace std;

FX_NS_USE(common);
FX_NS_DEF(config);

CPPUNIT_TEST_SUITE_REGISTRATION(XMLConfiguratorTestCase);

XMLConfiguratorTestCase::XMLConfiguratorTestCase(void)
{
}

XMLConfiguratorTestCase::~XMLConfiguratorTestCase(void)
{
}

void XMLConfiguratorTestCase::setUp()
{
}

void XMLConfiguratorTestCase::tearDown()
{
}

void XMLConfiguratorTestCase::testLoad()
{
    tstring str = TestHelper::getTestDataPath();
    str += _T("/xmlconfiguratortest.xml");
    XMLConfigurator conf;
    conf.load(str);
    Configurator::ConfMap rootMap = conf.getMap();

    tstring strXML = toXML(rootMap);
    tstring expectXML = "<sub_item1>";
    expectXML += "<sub_item11>item11</sub_item11>";
    expectXML += "</sub_item1>";
    expectXML += "<sub_item1>";
    expectXML += "<sub_item11>item12</sub_item11>";
    expectXML += "</sub_item1>";
    expectXML += "<sub_item2>";
    expectXML += "<sub_item21>";
    expectXML += "<sub_item211>";
    expectXML += "<sub_item2111>item2111</sub_item2111>";
    expectXML += "</sub_item211>";
    expectXML += "</sub_item21>";
    expectXML += "</sub_item2>";
    CPPUNIT_ASSERT_EQUAL(expectXML, strXML);
}

tstring XMLConfiguratorTestCase::toXML(const Configurator::ConfMap& confMap)
{
    stringstream ss;
    for (Configurator::ConfMap::const_iterator it = confMap.begin();
         it != confMap.end(); ++it)
    {
        ss << "<" << it->first << ">";
        if (it->second.getType() == typeid(tstring))
        {
            ss << AnyCast<tstring>(it->second);
        }
        else if (it->second.getType() == typeid(Configurator::ConfMap))
        {
            Configurator::ConfMap confMap2 = AnyCast<Configurator::ConfMap>(it->second);
            ss << toXML(confMap2);
        }
        else if (it->second.getType() == typeid(multimap<tstring, Any>))
        {
            multimap<tstring, Any> confMap2 = AnyCast<multimap<tstring, Any> >(it->second);
            ss << toXML(confMap2);
        }
        ss << "</" << it->first << ">";
    }
    return ss.str();
}

tstring XMLConfiguratorTestCase::toXML(const multimap<tstring, Any>& confMap)
{
    stringstream ss;
    for (multimap<tstring, Any>::const_iterator it = confMap.begin();
         it != confMap.end(); ++it)
    {
        ss << "<" << it->first << ">";
        if (it->second.getType() == typeid(tstring))
        {
            ss << AnyCast<tstring>(it->second);
        }
        else if (it->second.getType() == typeid(Configurator::ConfMap))
        {
            Configurator::ConfMap confMap2 = AnyCast<Configurator::ConfMap>(it->second);
            ss << toXML(confMap2);
        }
        else if (it->second.getType() == typeid(multimap<tstring, Any>))
        {
            multimap<tstring, Any> confMap2 = AnyCast<multimap<tstring, Any> >(it->second);
            ss << toXML(confMap2);
        }
        ss << "</" << it->first << ">";
    }
    return ss.str();
}

FX_NS_END
