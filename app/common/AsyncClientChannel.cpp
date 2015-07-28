#include "AsyncClientChannel.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/extension/network/EvThreadInitializer.h"
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TProtocolException.h>
#include <event2/dns.h>
#include <event2/thread.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using apache::thrift::transport::TTransportException;

FX_NS_USE(thread);
FX_NS_USE(network);

FX_NS_DEF(app);

SETUP_LOGGER(common, AsyncClientChannel);
SETUP_LOGGER_EMBED(common, AsyncClientChannel, EventLoopRunner);

AsyncClientChannel::AsyncClientChannel() 
    : m_nPort(-1)
    , m_cob(NULL)
    , m_pRecvBuf(NULL)
    , m_lastState(ST_DISCONNECT)
    , m_nConnFailCount(0)
    , m_nErrorCount(0)
    , m_evbase(NULL)
    , m_dnsbase(NULL)
    , m_bev(NULL)
    , m_nReceived(0)
    , m_nDataSize(-1)
    , m_bOwnEventBase(false)
{
}

AsyncClientChannel::~AsyncClientChannel() 
{
    close();
}

void AsyncClientChannel::open(const std::string& sHost, int nPort)
{
    m_sHost = sHost;
    m_nPort = nPort;
    m_pEventLoopThread.reset();
    m_bOwnEventBase = false;

    if (!m_evbase)
    {
        EvThreadInitializer::instance();

        m_evbase = event_base_new();
        if (evthread_make_base_notifiable(m_evbase) < 0)
        {
            FX_LOG(WARN, "evthread_make_base_notifiable FAILED");
        }

        m_pLoopRunner.reset(new EventLoopRunner(m_evbase));
        m_bOwnEventBase = true;
    }
    
    m_dnsbase = evdns_base_new(m_evbase, 1);
    m_bev = bufferevent_socket_new(m_evbase, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(m_bev, handleRead, NULL, handleEvent, this);
    bufferevent_enable(m_bev, EV_READ | EV_WRITE);
    int ret = bufferevent_socket_connect_hostname(m_bev, m_dnsbase, 
            AF_UNSPEC, m_sHost.c_str(), m_nPort);
    if (ret < 0)
    {
        FIRTEX_THROW_AND_LOG(DNSException, "Connect to: [%s:%d] FAILED, "
                             "error code of libevent: [%d]", 
                             m_sHost.c_str(), m_nPort, ret);
        m_sLastError = "call to bufferevent_socket_connect_hostname() failed";
        return;
    }

    m_lastState = ST_CONNECTING;
        
    if (m_bOwnEventBase)
    {
        m_pEventLoopThread.reset(new Thread());
        m_pEventLoopThread->start(*m_pLoopRunner);
    }

    // Wait connection complete
    while (m_lastState == ST_CONNECTING)
    {
        FastMutex::Guard lock(m_mutex);
        m_cond.wait(m_mutex, 100);
    }

    if (m_lastState != ST_CONNECTED)
    {
        FIRTEX_THROW_AND_LOG(ConnectionAbortedException, 
                             "Connect to: [%s:%d] FAILED: [%s]", 
                             m_sHost.c_str(), m_nPort, m_sLastError.c_str());
    }
}

void AsyncClientChannel::close()
{
    if (m_dnsbase)
    {
        evdns_base_free(m_dnsbase, 0);
        m_dnsbase = NULL;
    }
    
    if (m_bev)
    {
        bufferevent_free(m_bev);
        m_bev = NULL;
    }

    if (m_evbase && m_bOwnEventBase) 
    {
        event_base_loopbreak(m_evbase);
        m_pEventLoopThread->join();

        event_base_free(m_evbase);
    }

    m_lastState = ST_DISCONNECT;
    m_evbase = NULL;
}

void AsyncClientChannel::sendAndRecvMessage(const VoidCallback& cob,
        apache::thrift::transport::TMemoryBuffer* sendBuf,
        apache::thrift::transport::TMemoryBuffer* recvBuf)
{
    FX_DEBUG("sendAndRecvMessage");

    m_cob = cob;
    m_pRecvBuf = dynamic_cast<FramedMemoryTransport*>(recvBuf);
    FIRTEX_ASSERT2(m_pRecvBuf);

    size_t buflen = evbuffer_get_length(bufferevent_get_output(m_bev));
    if (buflen > 0)
    {
        evbuffer_drain(bufferevent_get_output(m_bev), buflen);
    }
    
    uint8_t* obuf;
    uint32_t sz;
    sendBuf->getBuffer(&obuf, &sz);
    int rv = evbuffer_add(bufferevent_get_output(m_bev), obuf, sz);
    if (rv != 0) 
    {
        throw TException("evbuffer_add failed");
    }
}
 
void AsyncClientChannel::sendMessage(const VoidCallback& cob,
                                     apache::thrift::transport::TMemoryBuffer* message)
{
    (void) cob;
    (void) message;
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
                             "Unexpected call to AsyncClientChannel::sendMessage");
}
 
void AsyncClientChannel::recvMessage(const VoidCallback& cob,
                                     apache::thrift::transport::TMemoryBuffer* message)
{
    (void) cob;
    (void) message;
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
                             "Unexpected call to AsyncClientChannel::recvMessage");
}

void AsyncClientChannel::handleEvent(struct bufferevent * bev,
                                     short events, void * ptr) 
{
    FX_DEBUG("handleEvent: [%d]", events);
    AsyncClientChannel* pThis = (AsyncClientChannel*)ptr;
    if (events & BEV_EVENT_CONNECTED)
    {
        pThis->onConnect();
    }
    else if (events & BEV_EVENT_EOF) 
    {
        pThis->onErrorOccurred("Server closed the connection");
    } 
    else if (events & BEV_EVENT_TIMEOUT) 
    {
        pThis->onErrorOccurred("I/O timeout");
    }
    else 
    {
        const char* msg = evutil_socket_error_to_string(
                evutil_socket_geterror());
        if (pThis->m_lastState == AsyncClientChannel::ST_CONNECTING) 
        {
            stringstream ss;
            int err = bufferevent_socket_get_dns_error(bev);
            if (err)
            {
                ss << "DNS error: [" << evutil_gai_strerror(err) << "]";
            }
            else
            {
                ss << "Error event: [" << events << "], "
                   << "message: [" << msg << "]";
            }

            pThis->onConnectFail(ss.str());
        } 
        else
        {
            stringstream ss;
            ss << "I/O error: [" << msg << "]";
            pThis->onErrorOccurred(ss.str());
        }
    }
}

void AsyncClientChannel::onConnect()
{
    FX_LOG(INFO, "Connected to: [%s:%d].", 
           m_sHost.c_str(), m_nPort);
    m_lastState = ST_CONNECTED;

    FastMutex::Guard lock(m_mutex);
    m_cond.broadcast();
}

void AsyncClientChannel::onConnectFail(const std::string& sError)
{
    m_sLastError = sError;
    m_lastState = ST_ERROR;
    ++m_nConnFailCount;

    FastMutex::Guard lock(m_mutex);
    m_cond.broadcast();
}

void AsyncClientChannel::onErrorOccurred(const std::string& sError)
{
    FX_LOG(WARN, sError.c_str());
    m_sLastError = sError;
    m_lastState = ST_ERROR;
    ++m_nErrorCount;
    onComplete();
}

void AsyncClientChannel::onRead()
{
    struct evbuffer* input = bufferevent_get_input(m_bev);
    if (m_nDataSize < 0)
    {
        evbuffer_copyout(input, &m_nDataSize, sizeof(m_nDataSize));
        m_nDataSize = ntohl(m_nDataSize);
        m_nDataSize += 4;
        
        m_buffer.clear();
    }

    m_nReceived += evbuffer_get_length(input);
    struct evbuffer* copy = evbuffer_new();
    bufferevent_read_buffer(m_bev, copy);
    m_buffer.addData(copy);
    evbuffer_free(copy);
    
    if (m_nReceived == m_nDataSize)
    {
        m_lastState = ST_OK;
        m_pRecvBuf->resetBuffer((uint8_t*)m_buffer.getData(),
                                (uint32_t)m_buffer.getLength());
        m_nDataSize = -1;
        m_nReceived = 0;        
        onComplete();
    }
    else if (m_nReceived > m_nDataSize)
    {
        FIRTEX_THROW_AND_LOG(NetworkException, "Data collapsed: received: [%d], "
                             "total size: [%d]", m_nReceived, m_nDataSize);
    }
}

FX_NS_END

