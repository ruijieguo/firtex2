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
// Created : 2012-01-07 09:32:31

#ifndef __FX_RPCASYNCCLIENT_H
#define __FX_RPCASYNCCLIENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

#include "AsyncClientChannel.h"

#include <async/TAsyncChannel.h>
#include <protocol/TProtocol.h>

FX_NS_DEF(app);

template <typename Client>
class RPCAsyncClient : public Client
{
public:
    typedef boost::shared_ptr<apache::thrift::async::TAsyncChannel> TAsyncChannelPtr;
    typedef boost::shared_ptr<apache::thrift::protocol::TProtocolFactory> TProtocolFactoryPtr;

    typedef Client ClientType;
    DEFINE_TYPED_PTR(ClientType);

public:
    RPCAsyncClient(apache::thrift::protocol::TProtocolFactory* pProtocolFactory);
    ~RPCAsyncClient();

public:
    /**
     * Open the client
     *
     * @param sHost the host to connect
     * @param nPort host port
     * @throw NetworkException if failed
     */
    void open(const std::string& sHost, int32_t nPort);

    /// Close the connection
    void close();

    /// Return true if no everything is ok
    bool good() const { return m_pChannel->good(); }

    /// Return true if any error occurred
    bool error() const { return m_pChannel->error(); }

    /// Return true if request is timeout
    bool timeout() const { return m_pChannel->timedOut(); }

    /// Return last error message if any error occurred
    const std::string& getLastErrorMessage() const;

    /// Return host name
    const std::string& getHost() const {return m_sHost;}

    /// Return port 
    int32_t getPort() const {return m_nPort;}

private:
    // thrift async client channel
    AsyncClientChannel* m_pChannel;

    std::string m_sHost;
    int32_t m_nPort;

private:
    DECLARE_LOGGER();
};

///////////////////////////////////////////

SETUP_LOGGER_TEMP(app, RPCAsyncClient);

template <typename Client>
RPCAsyncClient<Client>::RPCAsyncClient(apache::thrift::protocol::TProtocolFactory* pProtocolFactory)
    : Client(boost::shared_ptr< ::apache::thrift::async::TAsyncChannel>(), pProtocolFactory)
    , m_pChannel(NULL)
    , m_nPort(-1)
{
    this->itrans_.reset(new FramedMemoryTransport);
    this->otrans_.reset(new FramedMemoryTransport);
    this->piprot_ = pProtocolFactory->getProtocol(this->itrans_);
    this->poprot_ = pProtocolFactory->getProtocol(this->otrans_);
    this->iprot_ = this->piprot_.get();
    this->oprot_ = this->poprot_.get();
}

template <typename Client>
RPCAsyncClient<Client>::~RPCAsyncClient()
{
}

template <typename Client>
void RPCAsyncClient<Client>::open(const std::string& sHost, int32_t nPort)
{
    this->m_sHost = sHost;
    this->m_nPort = nPort;

    m_pChannel.reset(new AsyncClientChannel());
    this->channel_.reset(m_pChannel);
    m_pChannel->open(m_sHost, nPort);
}

template <typename Client>
void RPCAsyncClient<Client>::close()
{
    if (m_pChannel)
    {
        m_pChannel->close();
        m_pChannel = NULL;
    }
}

template <typename Client>
inline const std::string& RPCAsyncClient<Client>::getLastErrorMessage() const
{
    return m_pChannel->getLastErrorMessage();
}

FX_NS_END

#endif //__FX_RPCASYNCCLIENT_H
