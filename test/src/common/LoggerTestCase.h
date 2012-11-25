#ifndef __LOGGERTESTCASE_H
#define __LOGGERTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"

FX_NS_DEF(common);

class LoggerTestCase : public CPPUNIT_NS::TestFixture
                     , public LoggerBuilder
{
    CPPUNIT_TEST_SUITE( LoggerTestCase );
    CPPUNIT_TEST(testLog);
    CPPUNIT_TEST(testGetLogger);
    CPPUNIT_TEST(testConfigureLog);
    CPPUNIT_TEST_SUITE_END();
public:
    LoggerTestCase(void);
    ~LoggerTestCase(void);

public:
    void setUp();
    void tearDown();

protected:
    void testLog();
    void testGetLogger();
    void testConfigureLog();
};

FX_NS_END

#endif
