#ifndef __THREADTESTCASE_H
#define __THREADTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/thread/Thread.h"

FX_NS_DEF(thread);

class ThreadTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ThreadTestCase );
    CPPUNIT_TEST(testThread);
    CPPUNIT_TEST(testNamedThread);
    CPPUNIT_TEST(testThreads);
    CPPUNIT_TEST(testJoin);
    CPPUNIT_TEST(testThreadStackSize);
	CPPUNIT_TEST_SUITE_END();
public:
	ThreadTestCase(void);   
	~ThreadTestCase(void);

public:
	void setUp();
	void tearDown();
public:
    void testThread();
    void testNamedThread();
    void testThreads();
    void testJoin();
    void testThreadStackSize();
private:
};

FX_NS_END

#endif
