//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-14 22:31:49

#ifndef __FX_CONCURRENTMERGESCHEDULER_H
#define __FX_CONCURRENTMERGESCHEDULER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/MergeScheduler.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/thread/ThreadPool.h"

FX_NS_DEF(index);

class ConcurrentMergeScheduler : public MergeScheduler
{
public:
    ConcurrentMergeScheduler(size_t nThreadCount);
    ~ConcurrentMergeScheduler();

public:
    /// Perform merge operation.
    virtual void merge(const MergeablePtr& pMergeObj);

    /// Wait until all merge task finished
    virtual void waitFinish();

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<MergeablePtr> TaskQueue;

    class Task : public FX_NS(thread)::Runnable
    {
    public:
        Task(TaskQueue& queue) : m_queue(queue) {}

    private:
        void run()
        {
            MergeablePtr pMerger = m_queue.waitDequeue();
            if (pMerger.isNotNull())
            {
                pMerger->merge();
            }
            
            delete this;
        }
        
    private:
        TaskQueue& m_queue;
    };

private:
    FX_NS(thread)::ThreadPool m_threadPool;
    TaskQueue m_taskQueue;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_CONCURRENTMERGESCHEDULER_H
