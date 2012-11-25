#include "firtex/index/AsyncCommitScheduler.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, AsyncCommitScheduler);

AsyncCommitScheduler::AsyncCommitScheduler() 
    : m_commitQueue(DEFAULT_COMMIT_QUEUE_SIZE)
    , m_commitThread("commit_thread")
    , m_bStopped(false)
    , m_bCommitting(false)
{
    m_commitThread.start(*this);
}

AsyncCommitScheduler::~AsyncCommitScheduler() 
{
    close();
}

void AsyncCommitScheduler::commit(const CommittablePtr& pCommitObj)
{
    FX_DEBUG("One barrel committed.");
    m_commitQueue.enqueue(pCommitObj);
}

void AsyncCommitScheduler::waitCommit()
{
    m_commitQueue.waitEmpty();

    {
        FastMutex::Guard g(m_mutex);
        if (m_bCommitting)
        {
            m_cond.wait(m_mutex);
        }
    }
}

void AsyncCommitScheduler::run()
{
    while (!m_bStopped)
    {
        CommittablePtr pCommitObj = m_commitQueue.waitDequeue();
        {
            if (pCommitObj.isNotNull())
            {
                FastMutex::Guard g(m_mutex);
                m_bCommitting = true;
                pCommitObj->commit();
                m_lastCommitTime.update();
                m_bCommitting = false;
                m_cond.signal();
            }
            else
            {
                break;
            }
        }
    }

    //Stopped, commit all objects left in commit queue
    while (!m_commitQueue.isEmpty())
    {
        CommittablePtr pCommitObj = m_commitQueue.waitDequeue();
        if (pCommitObj.isNotNull())
        {
            FastMutex::Guard g(m_mutex);
            m_bCommitting = true;
            pCommitObj->commit();
            m_lastCommitTime.update();
            m_bCommitting = false;
        }
    }
}

const FX_NS(utility)::Timestamp& AsyncCommitScheduler::getLastCommitTime() const
{
    return m_lastCommitTime;
}

void AsyncCommitScheduler::close()
{
    if (!m_bStopped)
    {
        m_bStopped = true;
        CommittablePtr pDummy;
        m_commitQueue.enqueue(pDummy);

        m_commitThread.join();
    }
}

FX_NS_END

