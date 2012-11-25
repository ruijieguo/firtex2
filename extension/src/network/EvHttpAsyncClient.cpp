#include "firtex/extension/network/EvHttpAsyncClient.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/extension/network/EvThreadInitializer.h"
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <event2/thread.h>

FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(network);

SETUP_LOGGER(network, EvHttpAsyncClient);

EvHttpAsyncClient::EvHttpAsyncClient(CompleteHandler* pCompleteHandler) 
    : m_nPort(-1)
    , m_lastState(ST_DISCONNECTED)
    , m_nErrorCount(0)
    , m_requests(MAX_REQUEST_QUEUE_SIZE)
    , m_evbase(NULL)
    , m_pConn(NULL)
    , m_pLastRequest(NULL)
    , m_pCompleteHandler(pCompleteHandler)
    , m_bOwnEventBase(false)
    , m_bStopRequested(false)
{
}

EvHttpAsyncClient::~EvHttpAsyncClient() 
{
    close();
    if (m_pCompleteHandler)
    {
        delete m_pCompleteHandler;
        m_pCompleteHandler = NULL;
    }
    m_pLastRequest = NULL;
}

void EvHttpAsyncClient::open(const std::string& sHost, int32_t nPort)
{
    m_sHost = sHost;
    m_nPort = nPort;
    m_pEventLoopThread.reset();
    m_bOwnEventBase = false;

    if (!m_evbase)
    {
        EvThreadInitializer::instance();

        FX_EVENT_BASE_NEW(m_evbase);
        if (evthread_make_base_notifiable(m_evbase) < 0)
        {
            FX_LOG(WARN, "evthread_make_base_notifiable FAILED");
        }
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

    if (m_bOwnEventBase)
    {
        m_pEventLoopThread = new Thread();
        m_pEventLoopThread->start(*this);
    }
}

void EvHttpAsyncClient::close()
{
    if (m_bStopRequested)
    {
        return;
    }

    FX_DEBUG("Close client");
    m_bStopRequested = true;
    m_requests.enqueue(NULL);
    m_requests.waitEmpty();

    if (m_pConn)
    {
        evhttp_connection_free(m_pConn);
        m_pConn = NULL;
    }

    if (m_evbase && m_bOwnEventBase) 
    {
        event_base_loopbreak(m_evbase);
        m_pEventLoopThread->join();

        FX_EVENT_BASE_FREE(m_evbase);
    }
    m_evbase = NULL;

    m_lastState = ST_DISCONNECTED;
    m_pLastRequest = NULL;
}

void EvHttpAsyncClient::run()
{
    while (true)
    {
        m_requests.waitNotEmpty();

        if (m_bStopRequested && (m_requests.getFront() == NULL))
        {
            FX_DEBUG("Exit event loop");
            m_requests.dequeue();
            break;
        }
        m_procCount++;
        FX_DEBUG("Fetch one valid request, proccess count: [%d], queue size: [%d]",
                 m_procCount.value(), (int32_t)m_requests.getSize());
        int ret = event_base_loop(m_evbase, EVLOOP_ONCE);
        if (ret == -1)
        {
            FX_LOG(ERROR, "event_base_loop() FAILED");
        }
        else if (ret == 1)
        {
            FX_DEBUG("event_base_loop() exit: no events were registered");
        }
    }
}

void EvHttpAsyncClient::get(const std::string& sURI)
{
    struct evhttp_request* pReq = newReqest();
    if (!pReq)
    {
        setError(ST_ERROR, "new request FAILED");
        return;
    }

    int rv = evhttp_make_request(m_pConn, pReq, EVHTTP_REQ_GET, sURI.c_str());
    if (rv != 0) 
    {
        FX_LOG(ERROR, "evhttp_make_request FAILED");
        setError(ST_ERROR, "evhttp_make_request FAILED");
        return;
    }

    m_requests.enqueue(pReq);
    m_inCount++;
    FX_TRACE("made request: incoming count: [%d], [%s], queue size: [%d]",
             m_inCount.value(), sURI.c_str(), (int32_t)m_requests.getSize());
    return;
}

void EvHttpAsyncClient::post(const std::string& sURI, EvDataBuffer& dataBuffer)
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

    m_requests.enqueue(pReq);
}

void EvHttpAsyncClient::post(const std::string& sURI, const std::string& sData)
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
    }

    m_requests.enqueue(pReq);
}

void EvHttpAsyncClient::setTimeout(int32_t nTimeoutInSeconds)
{
    evhttp_connection_set_timeout(m_pConn, (int)nTimeoutInSeconds);
}

void EvHttpAsyncClient::setRetries(int32_t nRetryMax)
{
    evhttp_connection_set_retries(m_pConn, (int)nRetryMax);
}

/* static */ 
void EvHttpAsyncClient::handleResponse(struct evhttp_request* req, void* arg) 
{
    FX_DEBUG("handleResponse");
    EvHttpAsyncClient* pThis = (EvHttpAsyncClient*)arg;

    try
    {
        if (pThis->m_requests.getSize() > 0)
        {
            pThis->m_requests.dequeue();
        }
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

struct evhttp_request* EvHttpAsyncClient::newReqest()
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
                           "Content-Type", "application/x-firtex");
    if (rv != 0) 
    {
        FX_LOG(ERROR, "evhttp_add_header FAILED");
        setError(ST_ERROR, "evhttp_add_header FAILED");
        return NULL;
    }
    return pReq;
}

void EvHttpAsyncClient::setError(ServiceStatus s, const std::string& sError)
{
    m_lastState = s;
    m_sLastError = sError;
    ++m_nErrorCount;
    done(NULL);
}

void EvHttpAsyncClient::onCloseConnection(struct evhttp_connection* evcon, void* args)
{
    EvHttpAsyncClient* pThis = (EvHttpAsyncClient*)args;
    pThis->m_lastState = ST_DISCONNECTED;
    pThis->m_sLastError = "Client closed";
    pThis->m_nErrorCount++;
}

FX_NS_END

