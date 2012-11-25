#ifndef __HASHTESTCASE_H
#define __HASHTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"

FX_NS_DEF(utility);

class HashTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HashTestCase );
	CPPUNIT_TEST(testHashString64);
	CPPUNIT_TEST_SUITE_END();
public:
	HashTestCase(void);
	~HashTestCase(void);

public:
	void setUp();
	void tearDown();
protected:
	void testHashString64();
};

FX_NS_END

#endif
