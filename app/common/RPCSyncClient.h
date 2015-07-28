#ifndef __FX_RPC_CLIENT_H
#define __FX_RPC_CLIENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/extension/network/NetworkException.h"

#ifdef FX_WINDOWS
#include <ctype.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <io.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/TTransport.h>

FX_NS_DEF(app);

template <typename Client>
class RPCSyncClient : public Client
{
public:
    typedef boost::shared_ptr<apache::thrift::transport::TTransport> TTransportPtr;
    typedef boost::shared_ptr<apache::thrift::protocol::TProtocol> TProtocolPtr;

    typedef Client ClientType;
    DEFINE_TYPED_PTR(ClientType);

public:
    RPCSyncClient();
    ~RPCSyncClient();

public:
    void open(const std::string& sHost, int32_t nPort);
    void close();


    const std::string& getHost() const
    {
        return m_sHost;
    }

    int32_t getPort() const
    {
        return m_nPort;
    }

private:
    void setProtocol(const TProtocolPtr& pProtocol);

private:
    // thrift transport
    TTransportPtr m_pTransport;

    std::string m_sHost;
    int32_t m_nPort;

private:
    DECLARE_LOGGER();
};

///////////////////////////////////////////

SETUP_LOGGER_TEMP(app, RPCSyncClient);

template <typename Client>
RPCSyncClient<Client>::RPCSyncClient()
    : Client(boost::shared_ptr< ::apache::thrift::protocol::TProtocol>())
    , m_nPort(-1)
{
}

template <typename Client>
RPCSyncClient<Client>::~RPCSyncClient()
{
}

template <typename Client>
void RPCSyncClient<Client>::setProtocol(const TProtocolPtr& pProtocol)
{
    this->piprot_ = pProtocol;
    this->poprot_ = pProtocol;
    this->iprot_ = pProtocol.get();
    this->oprot_ = pProtocol.get();
}

template <typename Client>
void RPCSyncClient<Client>::open(const std::string& sHost, int32_t nPort)
{
    m_sHost = sHost;
    m_nPort = nPort;

    try
    {
        boost::shared_ptr<apache::thrift::transport::TTransport> socket(
                new apache::thrift::transport::TSocket(m_sHost, m_nPort));
        m_pTransport.reset(new apache::thrift::transport::TFramedTransport(socket));
        TProtocolPtr protocol(new apache::thrift::protocol::TBinaryProtocol(m_pTransport));
        this->setProtocol(protocol);
        m_pTransport->open();
    }
    catch(const apache::thrift::TException& e)
    {
        FIRTEX_THROW(NetworkException, "%s", e.what());
    }
}

template <typename Client>
void RPCSyncClient<Client>::close()
{
    if (m_pTransport)
    {
        m_pTransport->close();
    }
}

FX_NS_END

#endif
