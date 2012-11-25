#include "RPCSearchServer.h"
#include "RPCSearchService.h"

using namespace std;
using namespace apache::thrift;

FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCSearchServer);

RPCSearchServer::RPCSearchServer() 
{
}

RPCSearchServer::~RPCSearchServer() 
{
}

void RPCSearchServer::run()
{
    boost::shared_ptr<ThriftSearchServiceIf> service(new RPCSearchService(m_resource));
    boost::shared_ptr<TProcessor> processor(new ThriftSearchServiceProcessor(service));

    m_pServer = new RPCServer(processor, m_pConf->serverConf.listen_port,
                              m_pConf->serverConf.thread_pool);
    m_pServer->start(false);
}

void RPCSearchServer::stop()
{
    if (m_pServer.isNotNull())
    {
        m_pServer->stop();
    }
    SearchServerBase::stop();
}

void RPCSearchServer::join()
{
    SearchServerBase::join();
    if (m_pServer.isNotNull())
    {
        m_pServer->join();
    }
}

FX_NS_END

