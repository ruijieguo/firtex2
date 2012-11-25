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
// Created : 2012-01-07 07:28:25

#ifndef __FX_ASYNCCLIENTCHANNEL_H
#define __FX_ASYNCCLIENTCHANNEL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Condition.h"
#include "firtex/thread/Runnable.h"
#include "firtex/extension/network/EvDataBuffer.h"

#include "FramedMemoryTransport.h"

#include <thrift/async/TAsyncChannel.h>
#include <event.h>

namespace apache { namespace thrift { namespace transport {
class TMemoryBuffer;
}}}

FX_NS_DEF(app);

class AsyncClientChannel : public apache::thrift::async::TAsyncChannel
{
public:
    enum State
    {
        ST_OK,
        ST_DISCONNECT,
        ST_CONNECTING,
        ST_CONNECTED,
        ST_ERROR,
        ST_TIMEOUT,
    };

public:
    using TAsyncChannel::VoidCallback;

public:
    AsyncClientChannel();
    ~AsyncClientChannel();

public:
    /**
     * Open the channal
     *
     * @param sHost the host name 
     * @param nPort the host port
     * @throw throw 
     */
    void open(const std::string& sHost, int nPort);

    /**
     * Close connection
     */
    void close();

public:
    virtual void sendAndRecvMessage(const VoidCallback& cob,
                                    apache::thrift::transport::TMemoryBuffer* sendBuf,
                                    apache::thrift::transport::TMemoryBuffer* recvBuf);
    virtual void sendMessage(const VoidCallback& cob,
                             apache::thrift::transport::TMemoryBuffer* message);
    virtual void recvMessage(const VoidCallback& cob,
                             apache::thrift::transport::TMemoryBuffer* message);

    virtual bool good() const { return m_lastState == ST_OK; }
    virtual bool error() const { return m_lastState == ST_ERROR; }
    virtual bool timedOut() const { return m_lastState == ST_TIMEOUT; }

    const std::string& getLastErrorMessage() const;

private:
    class EventLoopRunner : public FX_NS(thread)::Runnable
    {
    public:
        EventLoopRunner(struct event_base* evbase)
            : m_evbase(evbase)
        {
        }
        
        void run()
        {
            int ret = event_base_loop(m_evbase, 0);
            if (ret == -1)
            {
                FX_LOG(ERROR, "event_base_loop() FAILED");
            }
            else if (ret == 1)
            {
                FX_DEBUG("event_base_loop() exit:  no events were registered");
            }
        }

    private:
        struct event_base* m_evbase;

    private:
        DECLARE_LOGGER();
    };
    DEFINE_TYPED_PTR(EventLoopRunner);

private:
    void onComplete();
    void onConnect();
    void onConnectFail(const std::string& sError);
    void onErrorOccurred(const std::string& sError);
    void onRead();

private:
    static void handleEvent(struct bufferevent * bev, 
                            short events, void * ptr);
    inline static void handleRead(struct bufferevent * bev, void * ptr);

private:
    std::string m_sHost;
    int m_nPort;
    VoidCallback m_cob;
    FramedMemoryTransport* m_pRecvBuf;

    FX_NS(thread)::ThreadPtr m_pEventLoopThread;
    EventLoopRunnerPtr m_pLoopRunner;

    FX_NS(thread)::FastMutex m_mutex;
    FX_NS(thread)::Condition m_cond;
    
    std::string m_sLastError;
    State m_lastState;
    int32_t m_nConnFailCount;
    int32_t m_nErrorCount;

    struct event_base* m_evbase;
    struct evdns_base * m_dnsbase;
    struct bufferevent* m_bev;

    FX_NS(network)::EvDataBuffer m_buffer;
    int32_t m_nReceived;
    int32_t m_nDataSize;

    bool m_bOwnEventBase;

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////////////////
inline const std::string& AsyncClientChannel::getLastErrorMessage() const
{
    return m_sLastError;
}

inline void AsyncClientChannel::handleRead(struct bufferevent * bev, void * ptr) 
{
    AsyncClientChannel* pThis = (AsyncClientChannel*)ptr;
    pThis->onRead();
}

inline void AsyncClientChannel::onComplete()
{
    if (m_cob != NULL)
    {
        m_cob();
    }
}

FX_NS_END

#endif //__FX_ASYNCCLIENTCHANNEL_H
