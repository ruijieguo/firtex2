#include "firtex/index/SyncCommitScheduler.h"

FX_NS_USE(thread);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, SyncCommitScheduler);

SyncCommitScheduler::SyncCommitScheduler() 
    : m_bCommitting(false)
{
}

SyncCommitScheduler::~SyncCommitScheduler() 
{
}

void SyncCommitScheduler::commit(const CommittablePtr& pCommitObj)
{
    FastMutex::Guard g(m_mutex);
    
    CommittablePtr pCommitObj2 = pCommitObj;
    m_bCommitting = true;
    FX_TRACE("Sync scheduler begin commit...");
    pCommitObj2->commit();
    m_lastCommitTime.update();
    FX_TRACE("Sync scheduler end commit.");
    m_bCommitting = false;

    m_cond.signal();
}

void SyncCommitScheduler::waitCommit() 
{
    FastMutex::Guard g(m_mutex);
    if (m_bCommitting)
    {
        FX_TRACE("Sync scheduler wait commit.");
        m_cond.wait(m_mutex);
        FX_TRACE("Sync scheduler wait returned.");
    }
}

const FX_NS(utility)::Timestamp& SyncCommitScheduler::getLastCommitTime() const
{
    return m_lastCommitTime;
}

FX_NS_END

