//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-11-06 21:49:06

#ifndef __FX_SYNCCOMMITSCHEDULER_H
#define __FX_SYNCCOMMITSCHEDULER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/CommitScheduler.h"
#include "firtex/thread/Condition.h"

FX_NS_DEF(index);

class SyncCommitScheduler : public CommitScheduler
{
public:
    SyncCommitScheduler();
    ~SyncCommitScheduler();

public:
    /**
     * Commit a committable object
     */
    void commit(const CommittablePtr& pCommitObj);

    /**
     * Wait until all committable object is committed.
     * Nothing to do for sync commit scheduler.
     */
    void waitCommit();

    /// Return the last commit time
    const FX_NS(utility)::Timestamp& getLastCommitTime() const;

    /**
     * Close the scheduler
     */
    void close() {}

private:
    volatile bool m_bCommitting;
    FX_NS(thread)::FastMutex m_mutex;
    FX_NS(thread)::Condition m_cond;          

    FX_NS(utility)::Timestamp m_lastCommitTime;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_SYNCCOMMITSCHEDULER_H
