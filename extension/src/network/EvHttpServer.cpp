#include "firtex/extension/network/EvHttpServer.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/extension/network/EvThreadInitializer.h"
#include <memory>
#include <event2/thread.h>

FX_NS_USE(thread);
FX_NS_DEF(network);

SETUP_LOGGER(network, EvHttpServer);

EvHttpServer::EvHttpServer(const std::string& sAddr, int32_t nPort,
                           const EvHttpServiceFactoryPtr& pServiceFactory,
                           size_t nWorkThreadCount) 
    : m_pEventBase(NULL)
    , m_pEventHttp(NULL)
    , m_pRequestHandler(new EvHttpRequestHandler(pServiceFactory, nWorkThreadCount))
    , m_sHost(sAddr)
    , m_nPort(nPort)
{
}

EvHttpServer::~EvHttpServer() 
{
    if (m_pEventHttp != NULL) 
    {
        evhttp_free(m_pEventHttp);
        m_pEventHttp = NULL;
    }
    if (m_pEventBase != NULL) 
    {
        event_base_free(m_pEventBase);
        m_pEventBase = NULL;
    }
}

void EvHttpServer::start(bool bBlocking)
{
    EvThreadInitializer::instance();
    FX_EVENT_BASE_NEW(m_pEventBase);
    if (m_pEventBase == NULL)
    {
        FIRTEX_THROW(NetworkException, "event_base_new FAILED");
    }
    
    m_pEventHttp = evhttp_new(m_pEventBase);
    if (m_pEventHttp == NULL)
    {
        FX_EVENT_BASE_FREE(m_pEventBase);
        FIRTEX_THROW(NetworkException, "evhttp_new FAILED");
    }

    // Bind to port.
    int ret = evhttp_bind_socket(m_pEventHttp, m_sHost.empty() ? 
                                 NULL : m_sHost.c_str(), (unsigned short)m_nPort);
    if (ret < 0) 
    {
        evhttp_free(m_pEventHttp);
        event_base_free(m_pEventBase);
        m_pEventBase = NULL;
        m_pEventHttp = NULL;
        FIRTEX_THROW(NetworkException, "evhttp_bind_socket FAILED");
    }

    FX_LOG(INFO, "Server bind to [%s:%d]", m_sHost.c_str(), m_nPort);
    
    // Register request handler.
    evhttp_set_gencb(m_pEventHttp, handleRequest, (void*)this);

    ret = evthread_make_base_notifiable(m_pEventBase);
    if (ret < 0)
    {
        FX_LOG(WARN, "evthread_make_base_notifiable FAILED");
    }

    if (bBlocking)
    {
        event_base_dispatch(m_pEventBase);
    }
    else
    {
        m_pEventThread = new Thread();
        m_pEventThread->start(*this);
    }
}

void EvHttpServer::handleRequest(struct evhttp_request* pReq, void* self) 
{
    try 
    {
        EvHttpServer* pThis = static_cast<EvHttpServer*>(self);
        pThis->process(pReq);
    }
    catch(std::exception& e) 
    {
        evhttp_send_error(pReq, HTTP_INTERNAL, e.what());
    }
}

void EvHttpServer::process(struct evhttp_request* pReq) 
{
    EvHttpRequestContext* ctx = new EvHttpRequestContext(
            EvHttpRequestContext::CompleteHandlerBinder::bind(this,
                    &EvHttpServer::complete));
    if (!ctx->init(pReq))
    {
        ctx->complete();
        return;
    }
    FX_TRACE("Process resource: [%s]", ctx->getResource().c_str());
    m_pRequestHandler->process(ctx);
}

void EvHttpServer::complete(EvHttpRequestContext* ctx)
{
    std::auto_ptr<EvHttpRequestContext> ptr(ctx);

    int code = ctx->getResponseCode();
    const char* reason = ctx->good() ? "OK" : ctx->getResponseMessage().c_str();

    int rv = evhttp_add_header(ctx->getEvRequest()->output_headers,
                               "Content-Type", "application/XML");
    if (rv != 0)
    {
        FX_LOG(ERROR, "evhttp_add_header FAILED ");
    }

    FX_TRACE("Send reply");
    evbuffer* buf = ctx->getOutBuffer()->getEvBuffer();
    evhttp_send_reply(ctx->getEvRequest(), code, reason, buf);
}
 
void EvHttpServer::stop()
{
    if (m_pEventThread.isNotNull())
    {
        event_base_loopbreak(m_pEventBase);
    }

    if (m_pRequestHandler.isNotNull())
    {
        m_pRequestHandler->stop();
    }
}

void EvHttpServer::join()
{
    if (m_pEventThread.isNotNull())
    {
        m_pEventThread->join();
    }

    if (m_pRequestHandler.isNotNull())
    {
        m_pRequestHandler->join();
    }
}

void EvHttpServer::run()
{
    FX_DEBUG("Begin event loop");
    event_base_dispatch(m_pEventBase);
    FX_DEBUG("Exit event loop");
}

FX_NS_END

