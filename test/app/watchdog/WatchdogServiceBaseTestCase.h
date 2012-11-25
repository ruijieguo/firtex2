#ifndef __FX_WATCHDOGSERVICEBASETESTCASE_H
#define __FX_WATCHDOGSERVICEBASETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(app);

class WatchdogServiceBaseTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(WatchdogServiceBaseTestCase);
//    CPPUNIT_TEST(testStartProcess);
    CPPUNIT_TEST(testRecover);
    CPPUNIT_TEST_SUITE_END();
public:
    WatchdogServiceBaseTestCase();
    ~WatchdogServiceBaseTestCase();

    void setUp();
    void tearDown();

protected:
    void testStartProcess();
    void testRecover();

private:
    std::string getTestPath();

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_WATCHDOGSERVICEBASETESTCASE_H
