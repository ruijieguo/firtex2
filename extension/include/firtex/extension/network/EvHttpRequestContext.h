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
// Created : 2012-01-09 23:02:43

#ifndef __FX_EVHTTPREQUESTCONTEX_H
#define __FX_EVHTTPREQUESTCONTEX_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvDataBuffer.h"
#include "firtex/extension/network/FunctionBinder.h"
#include "firtex/extension/network/HTTPTypes.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Timestamp.h"
#include <event2/http.h>
#include <event2/http_struct.h>

FX_NS_DEF(network);

class EvHttpRequestContext
{
public:
    /// Function called after the HTTP request has been processed
    DECLARE_CLASS_AND_BINDER1(CompleteHandler, EvHttpRequestContext*);

    typedef HTTPTypes::ResponseCode ResponseCode;
    typedef std::vector<std::string> Resources;

public:
    EvHttpRequestContext(CompleteHandler* pCompleteHandler)
        : m_pCompleteHandler(pCompleteHandler)
        , m_pRequest(NULL)
        , m_inBuf(NULL, false)
        , m_outBuf(NULL, false)
        , m_resCode(HTTPTypes::RESPONSE_CODE_OK)
    {
    }

    ~EvHttpRequestContext()
    {
        delete m_pCompleteHandler;
        m_pCompleteHandler = NULL;
    }

    bool init(evhttp_request* pReq)
    {
        m_sResource.clear();
        m_sQuery.clear();

        m_pRequest = pReq;
        m_inBuf.assign(pReq->input_buffer, false);
        m_outBuf.assign(pReq->output_buffer, false);

        const char* uri = evhttp_request_get_uri(m_pRequest);
        if (!uri)
        {
            FX_LOG(WARN, "Parse evhttp_request_get_uri() return NULL");
            return false;
        }
        struct evhttp_uri* decoded = evhttp_uri_parse(uri);
        if (!decoded)
        {
            FX_LOG(WARN, "Parse http uri FAILED: [%s]", uri);
            return false;
        }

	const char* path = evhttp_uri_get_path(decoded);
        if (!path) 
            path = "/";
        char* decodedPath = evhttp_uridecode(path, 0, NULL);
        if (!decodedPath)
        {
            FX_LOG(WARN, "Decode uri path: [%s] FAILED", path);
            evhttp_uri_free(decoded);
            return false;
        }
        m_sResource = decodedPath;
        parseResources();
        free(decodedPath);
        
        const char* query = evhttp_uri_get_query(decoded);
        if (query)
        {
            char* decodedQ = evhttp_uridecode(query, 0, NULL);
            if (!decodedQ)
            {
                FX_LOG(WARN, "Decode uri query: [%s] FAILED", query);
                evhttp_uri_free(decoded);
                return false;
            }
            m_sQuery = decodedQ;
            free(decodedQ);
        }
        evhttp_uri_free(decoded);
        return true;
    }

    void parseResources()
    {
        m_resources.clear();

        std::size_t startPos = 1;
        while (true)
        {
            std::size_t n = m_sResource.find("/", startPos);
            if (n != std::string::npos)
            {
                if (n - startPos > 0)
                {
                    m_resources.push_back(m_sResource.substr(startPos, n - startPos));
                }
                startPos = n + 1;
            }
            else
            {
                if (m_sResource.size() - startPos > 0)
                {
                    m_resources.push_back(m_sResource.substr(startPos, m_sResource.size() - startPos));
                }
                break;
            }
        }
    }

    EvDataBuffer* getInBuffer()
    {
        return &m_inBuf;
    }

    EvDataBuffer* getOutBuffer()
    {
        return &m_outBuf;
    }
        
    evhttp_request* getEvRequest()
    {
        return m_pRequest;
    }

    evhttp_cmd_type getMethodType() const
    {
        return evhttp_request_get_command(m_pRequest);
    }

    const std::string getData()
    {
        const char* ret = (const char*)m_inBuf.getData();
        if (!ret)
            return "";
        return ret;
    }

    const char* getURI() const
    {
        return evhttp_request_get_uri(m_pRequest);
    }

    const std::string& getResource() const
    {
        return m_sResource;
    }

    const Resources& getResources() const
    {
        return m_resources;
    }

    const std::string& getQuery() const
    {
        return m_sQuery;
    }

    void complete()
    {
        (*m_pCompleteHandler)(this);
    }

    bool good() const
    {
        return (m_resCode == HTTPTypes::RESPONSE_CODE_OK);
    }

    void setResponseCode(ResponseCode code)
    {
        m_resCode = code;
    }

    int getResponseCode() const
    {
        return (int)m_resCode;
    }

    const std::string& getResponseMessage() const
    {
        return HTTPTypes::toMessage(m_resCode);
    }

private:
    CompleteHandler* m_pCompleteHandler;
    evhttp_request* m_pRequest;
    EvDataBuffer m_inBuf;
    EvDataBuffer m_outBuf;
    ResponseCode m_resCode;

    std::string m_sResource;
    std::string m_sQuery;

    Resources m_resources;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_EVHTTPREQUESTCONTEX_H
