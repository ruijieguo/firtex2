#ifndef __COLLECTIONCONFIGURETESTCASE_H
#define __COLLECTIONCONFIGURETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/config/Configurator.h"

FX_NS_DEF(collection);

class CollectionConfigureTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( CollectionConfigureTestCase );
    CPPUNIT_TEST(testConfigure);
    CPPUNIT_TEST_SUITE_END();
public:
    CollectionConfigureTestCase(void);
    ~CollectionConfigureTestCase(void);
public:
    void setUp();
    void tearDown();
protected:
    void testConfigure();
    tstring toString(const FX_NS(config)::Configurator::ConfMap& confMap);
private:
};

FX_NS_END

#endif
