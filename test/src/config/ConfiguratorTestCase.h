#ifndef __CONFIGURATORTESTCASE_H
#define __CONFIGURATORTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"


FX_NS_DEF(config);

class ConfiguratorTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ConfiguratorTestCase );
    CPPUNIT_TEST(testConfigureString);
    CPPUNIT_TEST(testConfigureInt);
    CPPUNIT_TEST(testFindConf);
	CPPUNIT_TEST_SUITE_END();
public:
	ConfiguratorTestCase(void);
	~ConfiguratorTestCase(void);

public:
	void setUp();
	void tearDown();

protected:
    void testConfigureString();
    void testConfigureInt();
    void testFindConf();
};

FX_NS_END

#endif
