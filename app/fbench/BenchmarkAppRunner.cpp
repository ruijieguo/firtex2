#include "BenchmarkAppRunner.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/OptionException.h"
#include "firtex/extension/network/NetworkException.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/TextFile.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/TimeProbe.h"


#ifdef HAVE_THRIFT
#include "../common/RPCSyncClient.h"
#include "../thrift/gen-cpp/ThriftSearchService.h"
#endif

using namespace std;

FX_NS_USE(util);
FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(network);
FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, BenchmarkAppRunner);

BenchmarkAppRunner::BenchmarkAppRunner()
    : m_nPort(0)
    , m_sServiceType("http")
    , m_nConcurrentThread(0)
    , m_nTotalQuery(-1)
    , m_nTimeout(30)
    , m_nAvgLatency(0)
    , m_queries(1024)
{
}

BenchmarkAppRunner::~BenchmarkAppRunner()
{
}

AppRunner::Status BenchmarkAppRunner::run()
{
    if (m_nConcurrentThread <= 0)
    {
        FX_LOG(ERROR, "Invalid thread-count parameter: [%d]", m_nConcurrentThread);
        return S_NONE;
    }

    if (m_sQueryFile.empty() && m_sQueryString.empty())
    {
        FX_LOG(ERROR, "No query string or query file specified (-q or -f)");
        return S_NONE;
    }

    doQuery();

    cout << "Qps: " << (int32_t)((double)m_result.m_nSuccessCount / 
                                 (double)(m_result.m_nElapsedTime) * 1000000) << endl;
    cout << "Average latency: " << m_nAvgLatency << " ms" << endl;
    cout << "Min latency: " << m_result.m_nMinLatency/1000 << " ms" << endl;
    cout << "Max latency: " << m_result.m_nMaxLatency/1000 << " ms" << endl;        
    cout << "Success: " << m_result.m_nSuccessCount << endl;
    cout << "Failed: " << m_result.m_nFailCount << endl;
    cout << "Timeout: " << m_result.m_nTimeoutCount << endl;
    cout << "Unmatched: " << m_result.m_nUnmatchedCount << endl;

    cout << "Average total hits: " << m_nAvgTotalHits << endl;

    return S_EXIT;
}

void BenchmarkAppRunner::Result::addHits(const std::string& sRequest,
        const std::string& sResult)
{
    size_t r = sResult.find("total_hits");
    if (r != string::npos)
    {
        char* tmp = NULL;
        int64_t nTotalHits = STRTOLL(sResult.c_str() + r + 12, &tmp, 10);
        HitsMap::iterator it= m_req2hits.find(sResult);
        if (it != m_req2hits.end() && it->second != nTotalHits)
        {
            ++m_nUnmatchedCount;
            FX_LOG(INFO, "Hits count unmatched of request: [%s]", sRequest.c_str()); 
        }
        else
        {
            m_nAggTotalHits += nTotalHits;
            m_req2hits.insert(make_pair(sRequest, nTotalHits));
        }
    }
    else 
    {
        FX_LOG(ERROR, "Invalid result of request: [%s]", sRequest.c_str());
    }
}

void BenchmarkAppRunner::HttpRunner::run()
{
    EvHttpSyncClientPtr pClient = new EvHttpSyncClient();
    pClient->open(m_pMain->m_sHost, m_pMain->m_nPort);
    pClient->setTimeout(m_pMain->m_nTimeout);

    while (true)
    {
        std::string pQueryStr = m_queries.waitDequeue();
        if (pQueryStr.empty())
        {
            break;
        }

        TimeProbe prob;
        prob.start();
        pClient->get(pQueryStr);
        prob.stop();

        if (pClient->good())
        {
            FX_TRACE("Receive the [%d] response success.",
                     m_result.m_nSuccessCount);
            ++m_result.m_nSuccessCount;
            int64_t latency = prob.elapsed();
            if (latency > m_result.m_nMaxLatency)
            {
                m_result.m_nMaxLatency = latency;
            }
            if (latency < m_result.m_nMinLatency)
            {
                m_result.m_nMinLatency = latency;
            }
            m_result.m_nElapsedTime += latency;
            m_result.addHits(pQueryStr, pClient->getResponse());
        }
        else if (pClient->error())
        {
            FX_LOG(ERROR, "Request: [%s] FAILED, error: [%s]", 
                   pQueryStr.c_str(), pClient->getLastErrorMessage().c_str());
            ++m_result.m_nFailCount;
        }
        else // timeout
        {
            FX_LOG(ERROR, "[%p] Request: [%s] TIMEOUT", pClient.get(), pQueryStr.c_str());
            ++m_result.m_nTimeoutCount;
        }
    }
}

#ifdef HAVE_THRIFT
void BenchmarkAppRunner::RPCRunner::run()
{
    typedef RPCSyncClient<ThriftSearchServiceClient> SearchServiceClient;

    SearchServiceClient client;
    client.open(m_pMain->m_sHost, m_pMain->m_nPort);
    //client.setTimeout(m_pMain->m_nTimeout);

    while (true)
    {
        std::string sQueryStr = m_queries.waitDequeue();
        if (sQueryStr.empty())
        {
            break;
        }

        try
        {
            TimeProbe prob;
            prob.start();
            string sResult;
            client.syntaxSearch(sResult, sQueryStr);
            prob.stop();

            FX_TRACE("Receive the [%d] response success.",
                     m_result.m_nSuccessCount);
            ++m_result.m_nSuccessCount;
            int64_t latency = prob.elapsed();
            if (latency > m_result.m_nMaxLatency)
            {
                m_result.m_nMaxLatency = latency;
            }
            if (latency < m_result.m_nMinLatency)
            {
                m_result.m_nMinLatency = latency;
            }
            m_result.m_nElapsedTime += latency;
            m_result.addHits(sQueryStr, sResult);
        }
        catch (const apache::thrift::TException& e)
        {
            FX_LOG(ERROR, "Request: [%s] FAILED, error: [%s]", 
                   sQueryStr.c_str(), e.what());
            ++m_result.m_nFailCount;
        }
    }
}
#endif

void BenchmarkAppRunner::doQuery()
{
    if (m_nTotalQuery == -1)
    {
        m_nTotalQuery = 1;
    }

    for (int32_t i = 0; i < m_nConcurrentThread; ++i)
    {
        RunnerBasePtr pRunner;
        if (m_sServiceType == "http")
        {
            pRunner = new HttpRunner(this, m_queries);
        }
        else 
        {
#ifdef HAVE_THRIFT
            pRunner = new RPCRunner(this, m_queries);
#endif
        }
        m_runners.push_back(pRunner);

        ThreadPtr pThread = new Thread();
        m_threads.push_back(pThread);
        pThread->start(*pRunner);
    }

    if (!m_sQueryFile.empty())
    {
        const static size_t MAX_QUERY_LENGTH = 128 * 1024;
        char* query = new char[MAX_QUERY_LENGTH];

        TextFile file;
        file.open(m_sQueryFile, "r");
        while (!file.isEof())
        {
            ssize_t ret = file.readLine(query, MAX_QUERY_LENGTH, true);
            if (ret > 0)
            {
                m_queries.enqueue(query);
            }
        }
        delete[] query;
    }
    else
    {
        for (int32_t i = 0; i < m_nTotalQuery; ++i)
        {
            m_queries.enqueue(m_sQueryString);
        }
    }

    for (size_t i = 0; i < m_threads.size(); ++i)
    {
        m_queries.enqueue("");
    }

    for (size_t i = 0; i < m_threads.size(); ++i)
    {
        m_threads[i]->join();
    }

    m_nAvgTotalHits = 0;
    m_nAvgLatency = 0;
    size_t avgCount = 0;
    for (size_t i = 0; i < m_runners.size(); ++i)
    {
        Result res = m_runners[i]->m_result;
        m_result.m_nSuccessCount += res.m_nSuccessCount;
        m_result.m_nFailCount += res.m_nFailCount;
        m_result.m_nTimeoutCount += res.m_nTimeoutCount;
        int64_t avgTotalHits = res.getAvgTotalHits();
        if (avgTotalHits > 0)
        {
            m_nAvgTotalHits += avgTotalHits;
            ++avgCount;
        }

        int64_t avgLatency = res.getAvgLatency();
        if (avgLatency > 0)
        {
            m_nAvgLatency += avgLatency;
        }
        
        if (res.m_nElapsedTime > m_result.m_nElapsedTime)
        {
            m_result.m_nElapsedTime = res.m_nElapsedTime;
        }
        
        if (m_result.m_nMinLatency > res.m_nMinLatency)
        {
            m_result.m_nMinLatency = res.m_nMinLatency;
        }

        if (m_result.m_nMaxLatency < res.m_nMaxLatency)
        {
            m_result.m_nMaxLatency = res.m_nMaxLatency;
        }
    }
    if (avgCount > 0)
    {
        m_nAvgLatency = m_nAvgLatency/avgCount;
        m_nAvgTotalHits = m_nAvgTotalHits/avgCount;
    }
    else 
    {
        m_nAvgLatency = 0;
        m_nAvgTotalHits = 0;
    }
}

void BenchmarkAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription("Make benchmark for search server, example: ");
    pApp->addDescription(string("  ") + pApp->getProgName() + 
                         " -s localhost -p 19801 -q \"query=BODY:test\""
                         " -n 100 -c 16 -m 10 -t rpc");

    pApp->addOption(Option("host", "s", "host name")
                    .required(true)
                    .repeatable(false)
                    .argument("[HOST_NAME]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("port", "p", "search host port")
                    .required(true)
                    .repeatable(false)
                    .argument("[PORT_NUM]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("thread-count", "c", "concurrent thread count")
                    .required(true)
                    .repeatable(false)
                    .argument("[THREAD_COUNT]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("query", "q", "query string")
                    .repeatable(false)
                    .argument("[QUERY_STRING]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("query-file", "f", "specify the query file")
                    .repeatable(false)
                    .argument("[QUERY_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("query-count", "n", "specify the query count to send")
                    .repeatable(false)
                    .argument("[QUERY_COUNT]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("service-type", "t", "specify service type (rpc/http)")
                    .repeatable(false)
                    .argument("[SERVICE_TYPE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("timeout", "m", "set timeout (in seconds)")
                    .repeatable(false)
                    .argument("[TIMEOUT]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

}

void BenchmarkAppRunner::optionCallback(const Option& option,
        const string& sValue)
{
    if (option.matchFull("host") || option.matchShort("s"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--host/-s");
        }
        m_sHost = sValue;
    }

    if (option.matchFull("port") || option.matchShort("p"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--port/-p");
        }
        m_nPort = (int)NumberParser::parseInt32(sValue);
    }

    if (option.matchFull("query") || option.matchShort("q"))
    {
        m_sQueryString = sValue;
    }

    if (option.matchFull("query-file") || option.matchShort("f"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--query-file/-f");
        }
        m_sQueryFile = sValue;
    }
    if (option.matchFull("thread-count") || option.matchShort("c"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--thread-count/-c");
        }
        m_nConcurrentThread = NumberParser::parseInt32(sValue);
    }

    if (option.matchFull("query-count") || option.matchShort("n"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--query-count/-n");
        }
        m_nTotalQuery = NumberParser::parseInt32(sValue);
    }

    if (option.matchFull("service-type") || option.matchShort("t"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--service-type/-t");
        }

        if (sValue != "rpc" && sValue != "http")
        {
            FIRTEX_THROW(BadParameterException, "the value of --service-type "
                         "must be rpc or http");
        }
        if (sValue == "rpc")
        {
#ifndef HAVE_THRIFT
            FIRTEX_THROW(UnsupportedException, "Un-support rpc mode, "
                         "please re-compile with thrift.");
#endif
        }

        m_sServiceType = sValue;
    }

    if (option.matchFull("timeout") || option.matchShort("m"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--timeout/-m");
        }
        m_nTimeout = NumberParser::parseInt32(sValue);
    }
}

void BenchmarkAppRunner::stop()
{
}
 
void BenchmarkAppRunner::waitStop()
{
}

FX_NS_END
