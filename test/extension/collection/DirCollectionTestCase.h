#ifndef __FX_DIRCOLLECTIONTESTCASE_H
#define __FX_DIRCOLLECTIONTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(collection);

class DirCollectionTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DirCollectionTestCase);
    CPPUNIT_TEST(testScan);
    CPPUNIT_TEST_SUITE_END();
public:
    DirCollectionTestCase();
    ~DirCollectionTestCase();

    void setUp();
    void tearDown();

protected:
    void testScan();

private:
    std::string getTestPath();
    void checkIndex();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DIRCOLLECTIONTESTCASE_H
