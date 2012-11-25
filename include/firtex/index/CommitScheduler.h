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
// Created	: 2010-11-06 21:39:37

#ifndef __FX_COMMITSCHEDULER_H
#define __FX_COMMITSCHEDULER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/Committable.h"

FX_NS_DEF(index);

class CommitScheduler
{
public:
    CommitScheduler() {}
    virtual ~CommitScheduler() {}

public:
    /**
     * Commit a committable object
     */
    virtual void commit(const CommittablePtr& pCommitObj) = 0;

    /// Wait until all committable object is committed
    virtual void waitCommit() = 0;

    /// Return the last commit time
    virtual const FX_NS(utility)::Timestamp& getLastCommitTime() const = 0;

    /**
     * Close the scheduler
     */
    virtual void close() = 0;
};

DEFINE_TYPED_PTR(CommitScheduler);

FX_NS_END

#endif //__FX_COMMITSCHEDULER_H
