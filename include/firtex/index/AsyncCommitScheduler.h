//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-02-15 21:10:32

#ifndef __FX_ASYNCCOMMITSCHEDULER_H
#define __FX_ASYNCCOMMITSCHEDULER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/index/CommitScheduler.h"
#include "firtex/thread/Thread.h"

FX_NS_DEF(index);

class AsyncCommitScheduler : public CommitScheduler, public FX_NS(thread)::Runnable
{
public:
    const static size_t DEFAULT_COMMIT_QUEUE_SIZE = 32;

public:
    AsyncCommitScheduler();
    ~AsyncCommitScheduler();

public:
    /**
     * Commit a committable object
     */
    void commit(const CommittablePtr& pCommitObj);

    /// Wait until all committable object in queue is committed
    void waitCommit();

    /// Return the last commit time
    const FX_NS(utility)::Timestamp& getLastCommitTime() const;

    /**
     * Close the scheduler
     */
    void close();

protected:
    void run();

protected:
    typedef FX_NS(utility)::BoundedConcurrentQueue<CommittablePtr> CommitQueue;

    CommitQueue m_commitQueue;
    FX_NS(thread)::Thread m_commitThread;
    volatile bool m_bStopped;

    volatile bool m_bCommitting;
    FX_NS(thread)::FastMutex m_mutex;
    FX_NS(thread)::Condition m_cond;

    FX_NS(utility)::Timestamp m_lastCommitTime;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_ASYNCCOMMITSCHEDULER_H
