#ifndef __FX_RPCSERVERTESTCASE_H
#define __FX_RPCSERVERTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "RPCServer.h"
#include <string>

#include "cppunit/extensions/HelperMacros.h"
#include "gen-cpp/EchoService.h"

FX_NS_DEF(app);

class RPCServerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(RPCServerTestCase);
//    CPPUNIT_TEST(testSyncClient);
    CPPUNIT_TEST(testAsyncClient);
    CPPUNIT_TEST_SUITE_END();
public:
    RPCServerTestCase();
    ~RPCServerTestCase();

    void setUp();
    void tearDown();

protected:
    void testSyncClient();
    void testAsyncClient();

private:
    void asyncEchoReturn(EchoServiceCobClient* pClient);
    
private:
    RPCServerPtr m_pServer;
    int m_nPort;

    FX_NS(thread)::FastMutex m_mutex;
    FX_NS(thread)::Condition m_cond;

    std::string m_sResult;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_RPCSERVERTESTCASE_H
