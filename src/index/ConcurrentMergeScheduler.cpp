#include "firtex/index/ConcurrentMergeScheduler.h"

FX_NS_USE(thread);
FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, ConcurrentMergeScheduler);

ConcurrentMergeScheduler::ConcurrentMergeScheduler(size_t nThreadCount) 
    : m_threadPool(nThreadCount, nThreadCount)
    , m_taskQueue(nThreadCount)
{
}

ConcurrentMergeScheduler::~ConcurrentMergeScheduler() 
{
}

void ConcurrentMergeScheduler::merge(const MergeablePtr& pMergeObj)
{
    m_taskQueue.enqueue(pMergeObj);
    Task* pTask = new Task(m_taskQueue);
    m_threadPool.start(*pTask, true);
}

void ConcurrentMergeScheduler::waitFinish()
{
    m_threadPool.joinAll();
}

FX_NS_END

