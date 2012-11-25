#ifndef __PATHTESTCASE_H
#define __PATHTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/Path.h"

FX_NS_DEF(utility);

class PathTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( PathTestCase );
	CPPUNIT_TEST( testToString );
	CPPUNIT_TEST( testRelative );
	CPPUNIT_TEST_SUITE_END();
public:
	PathTestCase(void);
	~PathTestCase(void);

public:
	void setUp();
	void tearDown();
protected:
	void testToString();
    void testRelative();
private:
    FX_NS(utility)::Path* m_pPath;
};

FX_NS_END

#endif
