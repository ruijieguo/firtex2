#ifndef __FX_CLUSTERCLAUSETESTCASE_H
#define __FX_CLUSTERCLAUSETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(app);

class ClusterClauseTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ClusterClauseTestCase);
    CPPUNIT_TEST(testFromString);
    CPPUNIT_TEST_SUITE_END();
public:
    ClusterClauseTestCase();
    ~ClusterClauseTestCase();

    void setUp();
    void tearDown();

protected:
    void testFromString();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_CLUSTERCLAUSETESTCASE_H
