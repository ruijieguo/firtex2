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

#ifndef __FX_EVHTTPSYNCCLIENT_H
#define __FX_EVHTTPSYNCCLIENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvDataBuffer.h"
#include "firtex/extension/network/HTTPTypes.h"
#include "firtex/extension/network/NetworkDefines.h"
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

FX_NS_DEF(network);

class EvHttpSyncClient
{
public:
    const static int32_t DEFAULT_TIMEOUT = 60; // 60 seconds
    const static int32_t DEFAULT_MAX_RETRY = 3; // retry 3 times

public:
    EvHttpSyncClient();
    ~EvHttpSyncClient();

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

private:
    void done(struct evhttp_request* req);
    struct evhttp_request* newReqest();

    static void onCloseConnection(struct evhttp_connection* evcon, void* args);
    static void handleResponse(struct evhttp_request* req, void* arg);

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

    struct event_base* m_evbase;
    struct evhttp_connection* m_pConn;
    struct evhttp_request* m_pLastRequest;

    bool m_bOwnEventBase;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(EvHttpSyncClient);

/////////////////////////////////////////////
//
inline void EvHttpSyncClient::done(struct evhttp_request* req)
{
    m_pLastRequest = req;
    if (!req)
    {
        // timeout
        m_lastState = ST_TIMEOUT;
        m_sLastError = "timeout";
        event_base_loopexit(m_evbase, 0);
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
        event_base_loopexit(m_evbase, 0);
        return;
    }

    m_lastState = ST_OK;
    evbuffer* inbuf = evhttp_request_get_input_buffer(req);
    m_sResponse.assign((const char*)evbuffer_pullup(inbuf, -1), 
                       evbuffer_get_length(inbuf));
    event_base_loopexit(m_evbase, 0);
}

inline const std::string& EvHttpSyncClient::getResponse() const
{
    return m_sResponse;
}

inline const char* EvHttpSyncClient::getRequest() const
{
    if (m_pLastRequest)
    {
        return evhttp_request_get_uri(m_pLastRequest);
    }
    return NULL;
}

inline const std::string& EvHttpSyncClient::getLastErrorMessage() const
{
    return m_sLastError;
}

FX_NS_END

#endif //__FX_EVHTTPSYNCCLIENT_H
