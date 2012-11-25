#ifndef __FX_EVEVHTTPSERVERTESTCASE_H
#define __FX_EVEVHTTPSERVERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/extension/network/EvHttpServiceFactory.h"
#include "firtex/extension/network/EvHttpServer.h"

FX_NS_DEF(network);

class EvHttpServerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(EvHttpServerTestCase);
    CPPUNIT_TEST(testEchoService);
    CPPUNIT_TEST(testNotFoundService);
    CPPUNIT_TEST(testEchoServiceWithMultiThread);
    CPPUNIT_TEST_SUITE_END();
public:
    EvHttpServerTestCase();
    ~EvHttpServerTestCase();

    void setUp();
    void tearDown();

protected:
    void testEchoService();
    void testNotFoundService();
    void testEchoServiceWithMultiThread();

private:
    FX_NS(network)::EvHttpServiceFactoryPtr m_pFakeServiceFactory;
    FX_NS(network)::EvHttpServerPtr m_pService;
    
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_EVEVHTTPSERVERTESTCASE_H
