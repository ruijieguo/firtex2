#include "EvHttpServerTestCase.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Condition.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/network/EvHttpAsyncClient.h"
#include <sstream>

using namespace std;
FX_NS_USE(thread);
FX_NS_USE(network);
FX_NS_USE(common);

FX_NS_DEF(network);

SETUP_STREAM_LOGGER(network, EvHttpServerTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(EvHttpServerTestCase);

const static int32_t LISTEN_PORT = 19811;

class FakeMessageReceiver
{
public:
    void handleResponse(EvHttpAsyncClient* pClient)
    {
        FastMutex::Guard lock(m_mutex);
        m_receiveCond.broadcast();
    }
    
    void wait()
    {
        FastMutex::Guard g(m_mutex);
        m_receiveCond.wait(m_mutex);
    }

private:
    FastMutex m_mutex;
    Condition m_receiveCond;
};

class EchoRunner : public Runnable
{
public:
    EchoRunner(size_t nTestFrom, size_t nTestTimes)
        : m_nTestFrom(nTestFrom)
        , m_nTestTimes(nTestTimes)
    {
    }

public:
    void run()
    {
        FakeMessageReceiver receiver;
        EvHttpAsyncClient client(EvHttpAsyncClient::CompleteHandlerBinder::bind(
                        &receiver, &FakeMessageReceiver::handleResponse));
#ifdef FX_WINDOWS    
	client.open("127.0.0.1", LISTEN_PORT);
#else
        client.open("localhost", LISTEN_PORT);
#endif
        Thread::sleep(500);
        for (size_t i = m_nTestFrom; i < m_nTestFrom + m_nTestTimes; ++i)
        {
            stringstream ss;
            ss << "/echo?hello_world_" << i;
            client.get(ss.str());
            receiver.wait();
            m_results.push_back(client.getResponse());
        }
        client.close();
    }

    bool assertSuccess()
    {
        if (m_results.size() != m_nTestTimes)
        {
            return false;
        }

        for (size_t i = m_nTestFrom; i < m_nTestTimes; ++i)
        {
            stringstream ss;
            ss << "hello_world_" << i;

            if (m_results[i] != ss.str())
            {
                return false;
            }
        }
        return true;
    }

private:
    size_t m_nTestFrom;
    size_t m_nTestTimes;
    std::vector<std::string> m_results;
};

EvHttpServerTestCase::EvHttpServerTestCase() 
{
}

EvHttpServerTestCase::~EvHttpServerTestCase() 
{
}

void EvHttpServerTestCase::setUp()
{
    m_pFakeServiceFactory = new EvHttpServiceFactory;
#ifdef FX_WINDOWS
    m_pService = new EvHttpServer("127.0.0.1", LISTEN_PORT, m_pFakeServiceFactory, 1);
#else
	m_pService = new EvHttpServer("localhost", LISTEN_PORT, m_pFakeServiceFactory, 1);
#endif
    m_pService->start(false);
    Thread::sleep(1500);
}

void EvHttpServerTestCase::tearDown()
{
    m_pService->stop();
    m_pService->join();

    m_pService.reset();
}

void EvHttpServerTestCase::testEchoService()
{
    FakeMessageReceiver receiver;
    EvHttpAsyncClient client(EvHttpAsyncClient::CompleteHandlerBinder::bind(
                    &receiver, &FakeMessageReceiver::handleResponse));
#ifdef FX_WINDOWS    
	client.open("127.0.0.1", LISTEN_PORT);
#else
	client.open("localhost", LISTEN_PORT);
#endif
    string testMsg = "hello_world";
    client.get("/echo?" + testMsg);
    receiver.wait();
    CPPUNIT_ASSERT_EQUAL(testMsg, client.getResponse());
}

void EvHttpServerTestCase::testNotFoundService()
{
    FakeMessageReceiver receiver;
    EvHttpAsyncClient client(EvHttpAsyncClient::CompleteHandlerBinder::bind(
                    &receiver, &FakeMessageReceiver::handleResponse));
#ifdef FX_WINDOWS    
	client.open("127.0.0.1", LISTEN_PORT);
#else
    client.open("localhost", LISTEN_PORT);
#endif
    string testMsg = "hello_world";

    client.get("/no_such_service?" + testMsg);
    receiver.wait();
    CPPUNIT_ASSERT(!client.good());
    CPPUNIT_ASSERT(client.error());

    CPPUNIT_ASSERT_EQUAL(string("Not Found"), client.getLastErrorMessage());
    CPPUNIT_ASSERT_EQUAL(string(""), client.getResponse());
}

void EvHttpServerTestCase::testEchoServiceWithMultiThread()
{
    const static size_t CONCURRENT_NUMBER = 16;
 
    typedef SharedPtr<EchoRunner> EchoRunnerPtr;
    typedef std::vector<ThreadPtr> ThreadPool;

    ThreadPool threadPool;
    threadPool.reserve(CONCURRENT_NUMBER);

    std::vector<EchoRunnerPtr> runners;
    runners.reserve(CONCURRENT_NUMBER);

    const static size_t ECHO_TEST_TIMES = 1;

    size_t nTestFrom = 0;
    size_t i = 0;
    for (i = 0; i < CONCURRENT_NUMBER; ++i)
    {
        EchoRunnerPtr runner(new EchoRunner(nTestFrom, ECHO_TEST_TIMES));
        runners.push_back(runner);

        ThreadPtr pNewThread(new Thread());
        pNewThread->start(*runner);
        threadPool.push_back(pNewThread);
        nTestFrom += ECHO_TEST_TIMES;
    }

    for (i = 0; i < CONCURRENT_NUMBER; ++i)
    {
        threadPool[i]->join();
    }

    for (i = 0; i < CONCURRENT_NUMBER; ++i)
    {
        CPPUNIT_ASSERT(runners[i]->assertSuccess());
    }
}

FX_NS_END

