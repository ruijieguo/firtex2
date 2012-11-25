#include "RPCServerTestCase.h"
#include "RPCSyncClient.h"
#include "RPCAsyncClient.h"
#include <protocol/TBinaryProtocol.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;

FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(NONE, RPCServerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(RPCServerTestCase);

class RPCEchoHandler : public EchoServiceIf
{
public:
    void echo(std::string& sResult, const std::string& sSrc)
    {
        sResult = sSrc;
    }
};

class RPCEchoAsyncHandler : public EchoServiceCobSvIf 
{
public:
    RPCEchoAsyncHandler() 
    {
        syncHandler_ = std::auto_ptr<RPCEchoHandler>(new RPCEchoHandler);
        // Your initialization goes here
    }
    virtual ~RPCEchoAsyncHandler();

    void echo(std::tr1::function<void(std::string const& _return)> cob, const std::string& str) 
    {
        std::string _return = "";
        syncHandler_->echo(_return, str);
        return cob(_return);
    }

protected:
    std::auto_ptr<RPCEchoHandler> syncHandler_;
};

RPCServerTestCase::RPCServerTestCase() 
    : m_nPort(9986)
{
}

RPCServerTestCase::~RPCServerTestCase() 
{
}

void RPCServerTestCase::setUp()
{
    boost::shared_ptr<EchoServiceIf> service(new RPCEchoHandler());
    boost::shared_ptr<TProcessor> processor(new EchoServiceProcessor(service));

    m_pServer = new RPCServer(processor, m_nPort, 1);
    m_pServer->start(false);
    FX_NS(thread)::Thread::sleep(500);

    m_sResult = "";
}

void RPCServerTestCase::tearDown()
{
    m_pServer->stop();
    FX_NS(thread)::Thread::sleep(100);
    m_pServer->join();
    m_pServer.reset();
}

void RPCServerTestCase::testSyncClient()
{
    RPCSyncClient<EchoServiceClient> client;
    client.open("localhost", m_nPort);

    string sSrc = "test";
    string sResult;
    client.echo(sResult, sSrc);
    CPPUNIT_ASSERT_EQUAL(sSrc, sResult);

    client.close();
}

void RPCServerTestCase::asyncEchoReturn(EchoServiceCobClient* pClient)
{
    pClient->recv_echo(m_sResult);
    
    FastMutex::Guard lock(m_mutex);
    m_cond.broadcast();
}

void RPCServerTestCase::testAsyncClient()
{
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    RPCAsyncClient<EchoServiceCobClient> client(protocolFactory.get());
    client.open("localhost", m_nPort);

    string sSrc = "test";
    client.echo(std::tr1::bind(&RPCServerTestCase::asyncEchoReturn, this,
                               std::tr1::placeholders::_1), sSrc);
    FastMutex::Guard lock(m_mutex);
    m_cond.wait(m_mutex);
    CPPUNIT_ASSERT_EQUAL(sSrc, m_sResult);

    client.close();
}

FX_NS_END

