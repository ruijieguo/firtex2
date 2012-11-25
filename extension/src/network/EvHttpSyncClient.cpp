#include "firtex/extension/network/EvHttpSyncClient.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/extension/network/EvThreadInitializer.h"
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <event2/thread.h>

FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(network);

SETUP_LOGGER(network, EvHttpSyncClient);

EvHttpSyncClient::EvHttpSyncClient() 
    : m_nPort(-1)
    , m_lastState(ST_DISCONNECTED)
    , m_nErrorCount(0)
    , m_evbase(NULL)
    , m_pConn(NULL)
    , m_pLastRequest(NULL)
    , m_bOwnEventBase(false)
{
}

EvHttpSyncClient::~EvHttpSyncClient() 
{
    close();

    m_pLastRequest = NULL;
}

void EvHttpSyncClient::open(const std::string& sHost, int32_t nPort)
{
    m_sHost = sHost;
    m_nPort = nPort;
    m_bOwnEventBase = false;

    if (!m_evbase)
    {
        FX_EVENT_BASE_NEW(m_evbase);
        m_bOwnEventBase = true;
    }

    m_pConn = evhttp_connection_base_new(m_evbase, NULL, m_sHost.c_str(), (unsigned short)m_nPort);
    if (!m_pConn)
    {
        FIRTEX_THROW_AND_LOG(NetworkException, "evhttp_connection_base_new FAILED");
    }

    setTimeout(DEFAULT_TIMEOUT);
    setRetries(DEFAULT_MAX_RETRY);
    evhttp_connection_set_closecb(m_pConn, onCloseConnection, this);

    m_lastState = ST_OK;
}

void EvHttpSyncClient::close()
{
    FX_DEBUG("Close client");
    if (m_pConn)
    {
        evhttp_connection_free(m_pConn);
        m_pConn = NULL;
    }

    if (m_evbase && m_bOwnEventBase) 
    {
        event_base_loopbreak(m_evbase);
        event_base_free(m_evbase);
    }
    m_evbase = NULL;

    m_lastState = ST_DISCONNECTED;
    m_pLastRequest = NULL;
}

void EvHttpSyncClient::get(const std::string& sURI)
{
    struct evhttp_request* pReq = newReqest();
    if (!pReq)
    {
        setError(ST_ERROR, "new request FAILED");
        return ;
    }

    int rv = evhttp_make_request(m_pConn, pReq, EVHTTP_REQ_GET, sURI.c_str());
    if (rv != 0) 
    {
        FX_LOG(ERROR, "evhttp_make_request FAILED");
        setError(ST_ERROR, "evhttp_make_request FAILED");
        return;
    }

    rv = event_base_dispatch(m_evbase);
    if (rv != 0)
    {
        setError(ST_ERROR, "event loop failed");
        FX_LOG(ERROR, "event loop failed: [%d]", rv);
    }

    FX_TRACE("made request: [%s]", sURI.c_str());
    return ;
}

void EvHttpSyncClient::post(const std::string& sURI, EvDataBuffer& dataBuffer)
{
    struct evhttp_request* pReq = newReqest();
    if (!pReq)
    {
        setError(ST_ERROR, "new request FAILED");
        return;
    }
    
    int rv = evbuffer_add_buffer(evhttp_request_get_output_buffer(pReq),
                                 dataBuffer.getEvBuffer());
    if (rv != 0) 
    {
        setError(ST_ERROR, "evhttp_add_buffer FAILED");
        return;
    }

    rv = evhttp_make_request(m_pConn, pReq, EVHTTP_REQ_POST, sURI.c_str());
    if (rv != 0) 
    {
        setError(ST_ERROR, "evhttp_make_request FAILED");
        return;
    }

    rv = event_base_dispatch(m_evbase);
    if (rv != 0)
    {
        FX_LOG(WARN, "event loop failed: [%d]", rv);
    }
}

void EvHttpSyncClient::post(const std::string& sURI, const std::string& sData)
{
    struct evhttp_request* pReq = newReqest();
    if (!pReq)
    {
        return;
    }
    
    int rv = evbuffer_add(evhttp_request_get_output_buffer(pReq),
                                 sData.c_str(), sData.length());
    if (rv != 0) 
    {
        setError(ST_ERROR, "evhttp_add_buffer FAILED");
        return;
    }

    rv = evhttp_make_request(m_pConn, pReq, EVHTTP_REQ_POST, sURI.c_str());
    if (rv != 0) 
    {
        setError(ST_ERROR, "evhttp_make_request FAILED");
        return;
    }

    rv = event_base_dispatch(m_evbase);
    if (rv != 0)
    {
        FX_LOG(WARN, "event loop failed: [%d]", rv);
    }
}

void EvHttpSyncClient::setTimeout(int32_t nTimeoutInSeconds)
{
    evhttp_connection_set_timeout(m_pConn, (int)nTimeoutInSeconds);
}

void EvHttpSyncClient::setRetries(int32_t nRetryMax)
{
    evhttp_connection_set_retries(m_pConn, (int)nRetryMax);
}

/* static */ 
void EvHttpSyncClient::handleResponse(struct evhttp_request* req, void* arg) 
{
    FX_DEBUG("handleResponse");
    EvHttpSyncClient* pThis = (EvHttpSyncClient*)arg;
    try
    {
        pThis->done(req);
    }
    catch(std::exception& e) 
    {
        FX_LOG(ERROR, "Response exception thrown: [%s]: ", e.what());
    }
    catch(const FirteXException& e) 
    {
        FX_LOG(ERROR, "Response exception thrown: [%s]: ", e.what().c_str());
    }
}

struct evhttp_request* EvHttpSyncClient::newReqest()
{
    struct evhttp_request* pReq = evhttp_request_new(handleResponse, this);
    if (pReq == NULL) 
    {
        FX_LOG(ERROR, "evhttp_request_new FAILED");
        setError(ST_ERROR, "evhttp_request_new FAILED");
        return NULL;
    }

    int rv = evhttp_add_header(evhttp_request_get_output_headers(pReq), 
                               "Host", m_sHost.c_str());
    if (rv != 0) 
    {
        FX_LOG(ERROR, "evhttp_add_header FAILED");
        setError(ST_ERROR, "evhttp_add_header FAILED");
        return NULL;
    }

    rv = evhttp_add_header(evhttp_request_get_output_headers(pReq),
                           "Connection", "keep-alive");
    if (rv != 0) 
    {
        FX_LOG(ERROR, "evhttp_add_header FAILED");
        setError(ST_ERROR, "evhttp_add_header FAILED");
        return NULL;
    }
    return pReq;
}

void EvHttpSyncClient::setError(ServiceStatus s, const std::string& sError)
{
    m_lastState = s;
    m_sLastError = sError;
    ++m_nErrorCount;
    done(NULL);
}

void EvHttpSyncClient::onCloseConnection(struct evhttp_connection* evcon, void* args)
{
    EvHttpSyncClient* pThis = (EvHttpSyncClient*)args;
    pThis->m_lastState = ST_DISCONNECTED;
    pThis->m_sLastError = "Client closed";
    pThis->m_nErrorCount++;
}

FX_NS_END

