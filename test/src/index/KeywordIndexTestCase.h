#ifndef __KEYWORDINDEXTESTCASE_H
#define __KEYWORDINDEXTESTCASE_H

#include "firtex/common/StdHeader.h"

#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class KeywordIndexTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( KeywordIndexTestCase );
    CPPUNIT_TEST(testCTF);
//    CPPUNIT_TEST(testPosting);
    CPPUNIT_TEST_SUITE_END();
public:
    KeywordIndexTestCase(void);
    ~KeywordIndexTestCase(void);

    void setUp();
    void tearDown();

protected:
    void testCTF();	
    void testPosting();

private:
    void buildKeywordIndex(const std::string& sDocs);
    std::string getIndexPath();
};

FX_NS_END

#endif
