#ifndef __DATETIMEINDEXTESTCASE_H
#define __DATETIMEINDEXTESTCASE_H

#include "firtex/common/StdHeader.h"

#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(index);

class DateTimeIndexTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( DateTimeIndexTestCase );
    CPPUNIT_TEST(testCTF);
    CPPUNIT_TEST(testPosting);
    CPPUNIT_TEST(testQuery);
    //CPPUNIT_TEST( testRangeQuery );
    CPPUNIT_TEST_SUITE_END();
public:
    DateTimeIndexTestCase(void);
    ~DateTimeIndexTestCase(void);

protected:
    void testCTF();	
    void testPosting();
    void testQuery();
    void testRangeQuery();

private:
    void buildDateTimeIndex(const std::string& sDocs);
    std::string getIndexPath();
};

FX_NS_END

#endif
