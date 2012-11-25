#ifndef __HASHMAPTESTCASE_H
#define __HASHMAPTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"
#include <string>

FX_NS_DEF(utility);

class HashMapTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HashMapTestCase );
	CPPUNIT_TEST(testLookupInt32);
	CPPUNIT_TEST(testManyInsert);
	CPPUNIT_TEST(testIterator);
	CPPUNIT_TEST(testIteratorOfUInt64ToInt32);
	CPPUNIT_TEST(testLoad);
	CPPUNIT_TEST_SUITE_END();

public:
	HashMapTestCase(void);
	~HashMapTestCase(void);

public:
	void setUp();
	void tearDown();

protected:
	void testLookupInt32();
    void testManyInsert();
    void testIterator();
    void testIteratorOfUInt64ToInt32();
    void testLoad();

    std::string getTestPath();
};

FX_NS_END

#endif
