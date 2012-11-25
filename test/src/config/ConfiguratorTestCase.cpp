#include "ConfiguratorTestCase.h"
#include "firtex/config/Configurator.h"
#include "firtex/utility/Number.h"
#include <memory>
#include <sstream>
#include "TestHelper.h"

using namespace std;
using namespace firtex;

FX_NS_USE(common);
FX_NS_USE(utility);

FX_NS_DEF(config);

CPPUNIT_TEST_SUITE_REGISTRATION(ConfiguratorTestCase);

ConfiguratorTestCase::ConfiguratorTestCase(void)
{
}

ConfiguratorTestCase::~ConfiguratorTestCase(void)
{
}

void ConfiguratorTestCase::setUp()
{
}

void ConfiguratorTestCase::tearDown()
{
}

void ConfiguratorTestCase::testConfigureString()
{
    Configurator conf;
    tstring v1("value1");
    tstring v2("value2");
    conf.configure("key1", v1, "default1");
    conf.configure("key2", v2, "default2");    
    Configurator::ConfMap confMap = conf.getMap();

    int i = 0;
    for (Configurator::ConfMap::const_iterator it = confMap.begin(); 
         it != confMap.end(); ++it)
    {
        stringstream ss;
        ss << "key" << i + 1;
        CPPUNIT_ASSERT_EQUAL(ss.str(), it->first);

        tstring value = AnyCast<tstring>(it->second);
        stringstream ss1;
        ss1 << "value" << i + 1;
        CPPUNIT_ASSERT_EQUAL(ss1.str(), value);
        i++;
    }
}

void ConfiguratorTestCase::testConfigureInt()
{
    Configurator conf;
    int32_t v1 = 11;
    int32_t v2 = 12;
    conf.configure("key1", v1);
    conf.configure("key2", v2);    
    Configurator::ConfMap confMap = conf.getMap();

    int i = 0;
    for (Configurator::ConfMap::const_iterator it = confMap.begin(); 
         it != confMap.end(); ++it)
    {
        stringstream ss;
        ss << "key" << i + 1;
        CPPUNIT_ASSERT_EQUAL(ss.str(), it->first);

        tstring value = AnyCast<tstring>(it->second);
        int32_t v = NumberParser::parseInt32(value);
        if (i == 0)
        {
          CPPUNIT_ASSERT_EQUAL((int32_t)11, v);
        }
        else 
        {
          CPPUNIT_ASSERT_EQUAL((int32_t)12, v);    
        }
        i++;
    }
}

void ConfiguratorTestCase::testFindConf()
{
    Configurator conf;
	string str1 = "value1", str2 = "value2";
    conf.configure("key1", str1, "");
    conf.configure("key2", str2, ""); 
    
    CPPUNIT_ASSERT(conf.findConf("key1").hasNext());
    CPPUNIT_ASSERT(conf.findConf("key2").hasNext());
    CPPUNIT_ASSERT(!conf.findConf("key3").hasNext());
}


FX_NS_END
