#ifndef __FX_BENCHMARKAPPRUNNER_H
#define __FX_BENCHMARKAPPRUNNER_H

#define USE_DEBUG

#include "firtex/common/Logger.h"
#include "firtex/extension/util/AppRunner.h"
#include "firtex/extension/network/EvHttpAsyncClient.h"
#include "firtex/extension/network/EvHttpSyncClient.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/thread/Condition.h"
#include <limits>

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class BenchmarkAppRunner : public FX_NS(util)::AppRunner
{
public:
    typedef FX_NS(utility)::BoundedConcurrentQueue<std::string> QueryQueue;

public:

    BenchmarkAppRunner();
    ~BenchmarkAppRunner();

public:
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);
    void optionCallback(const FX_NS(util)::Option& option, 
                        const std::string& sValue);

    void stop();
    void waitStop();

private:
    void doQuery();

private:
    struct Result
    {
        Result()
            : m_nSuccessCount(0)
            , m_nFailCount(0)
            , m_nTimeoutCount(0)
            , m_nElapsedTime(0)
            , m_nMaxLatency(0)
            , m_nMinLatency((std::numeric_limits<int64_t>::max)())
            , m_nUnmatchedCount(0)
            , m_nAggTotalHits(0)
        {
        }

        int64_t getAvgLatency() const
        {
            if (m_nSuccessCount > 0)
            {
                return (int64_t)(((double)m_nElapsedTime / 1000) / (double)m_nSuccessCount);
            }
            return 0;
        }

        int64_t getAvgTotalHits() const
        {
            if (m_nSuccessCount > 0)
            {
                return (m_nAggTotalHits/m_nSuccessCount);
            }
            return 0;
        }

        void addHits(const std::string& sRequest,
                     const std::string& sResult);

        int64_t m_nSuccessCount;
        int64_t m_nFailCount;
        int64_t m_nTimeoutCount;
        int64_t m_nElapsedTime;
        int64_t m_nMaxLatency;
        int64_t m_nMinLatency;
        int64_t m_nUnmatchedCount;

        int64_t m_nAggTotalHits;

        typedef std::map<std::string, int64_t> HitsMap;
        HitsMap m_req2hits;
    };

    class RunnerBase : public FX_NS(thread)::Runnable
    {
    public:
        RunnerBase(BenchmarkAppRunner* pMain,
                   QueryQueue& queries) 
            : m_pMain(pMain) 
            , m_queries(queries)
        {}
        virtual ~RunnerBase() {}

    private:
        BenchmarkAppRunner* m_pMain;
        QueryQueue& m_queries;

        Result m_result;

        FX_NS(thread)::FastMutex m_mutex;
        FX_NS(thread)::Condition m_cond;

        friend class BenchmarkAppRunner;
    };
    DEFINE_TYPED_PTR(RunnerBase);

    class HttpRunner : public RunnerBase
    {
    public:
        HttpRunner(BenchmarkAppRunner* pMain,
               QueryQueue& queries) 
            : RunnerBase(pMain, queries)
        {
        }
        
        void run();
    };
    DEFINE_TYPED_PTR(HttpRunner);

#ifdef HAVE_THRIFT
    class RPCRunner : public RunnerBase
    {
    public:
        RPCRunner(BenchmarkAppRunner* pMain,
                  QueryQueue& queries) 
            : RunnerBase(pMain, queries)
        {
        }
        
        void run();
    };
    DEFINE_TYPED_PTR(RPCRunner);
#endif

private:
    std::string m_sHost;
    int32_t m_nPort;
    std::string m_sQueryString;
    std::string m_sQueryFile;
    std::string m_sServiceType;
    int32_t m_nConcurrentThread;
    int32_t m_nTotalQuery;
    int32_t m_nTimeout;

    Result m_result;
    int64_t m_nAvgLatency;
    int64_t m_nAvgTotalHits;
    
    FX_NS(thread)::FastMutex m_mutex;
    FX_NS(thread)::Condition m_cond;

    QueryQueue m_queries;
    std::vector<FX_NS(thread)::ThreadPtr> m_threads;
    std::vector<RunnerBasePtr> m_runners;

private:
    DECLARE_LOGGER();
};


FX_NS_END

#endif
