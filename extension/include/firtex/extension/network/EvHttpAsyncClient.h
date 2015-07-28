//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2012-01-12 22:28:22

#ifndef __FX_EVHTTPASYNCCLIENT_H
#define __FX_EVHTTPASYNCCLIENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/FunctionBinder.h"
#include "firtex/extension/network/EvDataBuffer.h"
#include "firtex/extension/network/HTTPTypes.h"
#include "firtex/thread/Thread.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/extension/network/NetworkDefines.h"
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

#include <atomic>

FX_NS_DEF(network);

class EvHttpAsyncClient : public FX_NS(thread)::Runnable
{
public:
    /// Function called after the HTTP request has been processed
    DECLARE_CLASS_AND_BINDER1(CompleteHandler, EvHttpAsyncClient*);

public:
    const static int32_t DEFAULT_TIMEOUT = 60; // 60 seconds
    const static int32_t DEFAULT_MAX_RETRY = 3; // retry 3 times
    const static size_t MAX_REQUEST_QUEUE_SIZE = 1024;

public:
    EvHttpAsyncClient(CompleteHandler* pCompleteHandler);
    ~EvHttpAsyncClient();

public:
    /**
     * Open the client
     *
     * @param sHost the host to connect
     * @param nPort host port
     * @throw NetworkException if failed
     */
    void open(const std::string& sHost, int32_t nPort);

    /**
     * Send GET request to server asynchronously.
     *
     * @param sURI request uri
     */
    void get(const std::string& sURI);

    /**
     * Send POST request to server asynchronously.
     *
     * @param sURI request uri
     * @param dataBuffer data to post
     */
    void post(const std::string& sURI, FX_NS(network)::EvDataBuffer& dataBuffer);

    /**
     * Send POST request to server asynchronously.
     *
     * @param sURI request uri
     * @param sData data to post
     */
    void post(const std::string& sURI, const std::string& sData);

    /// Set the timeout for connection
    void setTimeout(int32_t nTimeoutInSeconds);

    /// Set the retry limit for connection
    void setRetries(int32_t nRetryMax);

    /// Close the connection
    void close();

    /// Set error message
    void setError(ServiceStatus s, const std::string& sError);

    /// Return true if no everything is ok
    bool good() const { return m_lastState == ST_OK; }

    /// Return true if any error occurred
    bool error() const { return m_lastState == ST_ERROR; }

    /// Return true if request is timeout
    bool timeout() const { return m_lastState == ST_TIMEOUT; }

    /// Return last error message if any error occurred
    const std::string& getLastErrorMessage() const;

    /// Return host name
    const std::string& getHost() const {return m_sHost;}

    /// Return port 
    int32_t getPort() const {return m_nPort;}

    /// Return response
    const std::string& getResponse() const;
    
    /// Return request
    const char* getRequest() const;

protected:
    void onComplete();

private:
    void run();
    void done(struct evhttp_request* req);
    struct evhttp_request* newReqest();

    static void onCloseConnection(struct evhttp_connection* evcon, void* args);
    static void handleResponse(struct evhttp_request* req, void* arg);
    static void onTimer(evutil_socket_t fd, short event, void *arg);

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<struct evhttp_request*> RequestQueue;
    DEFINE_TYPED_PTR(RequestQueue);

private:
    /// Server address
    std::string m_sHost;

    /// Server port
    int32_t m_nPort;
    
    /// The current response
    std::string m_sResponse;

    /// The last occurred error message
    std::string m_sLastError;

    /// The last status
    ServiceStatus m_lastState;
    
    int32_t m_nErrorCount;

    FX_NS(thread)::ThreadPtr m_pEventLoopThread;
    RequestQueue m_requests;

    struct event_base* m_evbase;
    struct evhttp_connection* m_pConn;
    struct evhttp_request* m_pLastRequest;

    /// Invoked when a request finished
    CompleteHandler* m_pCompleteHandler;

    bool m_bOwnEventBase;

    bool m_bStopRequested;

    std::atomic_long m_inCount;
    std::atomic_long m_procCount;
    std::atomic_long m_compCount;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(EvHttpAsyncClient);

/////////////////////////////////////////////
//
inline void EvHttpAsyncClient::done(struct evhttp_request* req)
{
    m_compCount++;
    FX_DEBUG("complete count : [%d]", std::atomic_load(&m_compCount));

    m_pLastRequest = req;
    if (!req)
    {
        // timeout
        m_lastState = ST_TIMEOUT;
        m_sLastError = "timeout";
        onComplete();
        return;
    }

    FX_TRACE("Request: [%s] is done, response code: [%d]", 
             evhttp_request_get_uri(req), req->response_code);
    if (req->response_code != FX_NS(network)::HTTPTypes::RESPONSE_CODE_OK)
    {
        m_lastState = ST_ERROR;
        ++m_nErrorCount;
        if (req->response_code_line)
        {
            m_sLastError = req->response_code_line;
        }
        else
        {
            if (req->evcon == NULL)
            {
                m_sLastError = "connection failed";
            }
        }
        onComplete();
        return;
    }

    m_lastState = ST_OK;
    evbuffer* inbuf = evhttp_request_get_input_buffer(req);
    m_sResponse.assign((const char*)evbuffer_pullup(inbuf, -1), 
                       evbuffer_get_length(inbuf));
    onComplete();
}

inline void EvHttpAsyncClient::onComplete()
{
    if (m_pCompleteHandler)
    {
        (*m_pCompleteHandler)(this);
    }
}

inline const std::string& EvHttpAsyncClient::getResponse() const
{
    return m_sResponse;
}

inline const char* EvHttpAsyncClient::getRequest() const
{
    if (m_pLastRequest)
    {
        return evhttp_request_get_uri(m_pLastRequest);
    }
    return NULL;
}

inline const std::string& EvHttpAsyncClient::getLastErrorMessage() const
{
    return m_sLastError;
}

FX_NS_END

#endif //__FX_EVHTTPASYNCCLIENT_H
