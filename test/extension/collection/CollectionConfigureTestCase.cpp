#include "CollectionConfigureTestCase.h"
#include <memory>
#include "../src/TestHelper.h"
#include "firtex/extension/collection/Collection.h"

#include "firtex/common/Any.h"

FX_NS_USE(config);
FX_NS_USE(common);

FX_NS_DEF(collection);

CPPUNIT_TEST_SUITE_REGISTRATION(CollectionConfigureTestCase);


CollectionConfigureTestCase::CollectionConfigureTestCase(void)
{
}

CollectionConfigureTestCase::~CollectionConfigureTestCase(void)
{
}

void CollectionConfigureTestCase::setUp()
{

}

void CollectionConfigureTestCase::tearDown()
{

}

void CollectionConfigureTestCase::testConfigure()
{
    CollectionConfigure conf;

    conf.Collection.name = "name_value";
    conf.Collection.identifier = "identifier_value";

    conf.Collection.source = "source_value";
    conf.Collection.target = "indexLocation_value";
    conf.Collection.buildMode = "accessMode_value";

    conf.Collection.autoCommit.maxDocs = 1;
    conf.Collection.autoCommit.maxTime = 2;
    conf.Collection.encoding = "encoding_value";
    conf.Collection.docTypes = "docTypes_value";
    conf.Collection.parameter = "parameter_value";

    Configurator c;
    conf.configure(c);

    CollectionConfigure conf2;
    c.setMode(Configurator::FROM_CONF);
    conf2.configure(c);
    
    CPPUNIT_ASSERT_EQUAL(conf.Collection.source, conf2.Collection.source);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.name, conf2.Collection.name);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.identifier, conf2.Collection.identifier);

    CPPUNIT_ASSERT_EQUAL(conf.Collection.source, conf2.Collection.source);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.target, conf2.Collection.target);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.buildMode, conf2.Collection.buildMode);

    CPPUNIT_ASSERT_EQUAL(conf.Collection.autoCommit.maxDocs, conf2.Collection.autoCommit.maxDocs);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.autoCommit.maxTime, conf2.Collection.autoCommit.maxTime);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.encoding, conf2.Collection.encoding);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.docTypes, conf2.Collection.docTypes);
    CPPUNIT_ASSERT_EQUAL(conf.Collection.parameter, conf2.Collection.parameter);
}

std::string CollectionConfigureTestCase::toString(const Configurator::ConfMap& confMap)
{
    std::string str;
    Configurator c(confMap);
    Configurator::Iterator it = c.iterator();
    while (it.hasNext())
    {
        Configurator::KeyValuePair kv = it.next();
        str += "   ";
        str += kv.first;
        str += ": \n";
        Configurator::ConfMap subMap = AnyCast<Configurator::ConfMap>(kv.second);
        for (Configurator::ConfMap::const_iterator it2 = subMap.begin();
             it2 != subMap.end(); ++it2)
        {
            str += "      ";
            str += it2->first;
            str += ": ";
            str += AnyCast<std::string>(it2->second);
            str += "\n";
        }        
    }
    return str;
}

FX_NS_END
