#ifndef __STRINGTOKENIZERTESTCASE_H
#define __STRINGTOKENIZERTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"

#include "firtex/common/StdHeader.h"

FX_NS_DEF(utility);

class StringTokenizerTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( StringTokenizerTestCase );
    CPPUNIT_TEST( testTokenizeNormal );
    CPPUNIT_TEST( testTokenizeSpaceSep );
    CPPUNIT_TEST( testTokenizeWithSpace );
    CPPUNIT_TEST( testTokenizeIgnoreEmpty );
    CPPUNIT_TEST( testTokenizeLeaveAs );
    CPPUNIT_TEST( testTokenizeTrimIgnoreEmpty );
    CPPUNIT_TEST( testTokenizeWithSpaceSep );
    CPPUNIT_TEST( testTokenizeWithSpaceSepAndTrim );
    CPPUNIT_TEST( testTokenizeWithTwoCharSep );
    CPPUNIT_TEST( testTokenizeWithSpaceSepAndTrimIgnoreEmpty );
	CPPUNIT_TEST_SUITE_END();
public:
	StringTokenizerTestCase(void);
	~StringTokenizerTestCase(void);

public:
	void setUp();
	void tearDown();
    void testTokenizeNormal();
    void testTokenizeSpaceSep();
    void testTokenizeWithSpace();
    void testTokenizeIgnoreEmpty();
    void testTokenizeLeaveAs();
    void testTokenizeTrimIgnoreEmpty();
    void testTokenizeWithTwoCharSep();
    void testTokenizeWithSpaceSepAndTrim();
    void testTokenizeWithSpaceSep();
    void testTokenizeWithSpaceSepAndTrimIgnoreEmpty();    
protected:
};

FX_NS_END

#endif
