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
// Created : 2011-06-03 19:30:01

#ifndef __FX_ASYNCDOCUMENTCONSUMER_H
#define __FX_ASYNCDOCUMENTCONSUMER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/Runnable.h"
#include "firtex/document/Document.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/DocumentConsumer.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/index/IndexBarrelKeeper.h"
#include "firtex/utility/SyncSegregatedAllocator.h"
#include "firtex/thread/ThreadPool.h"

FX_NS_DEF(index);

class AsyncDocumentConsumer : public DocumentConsumer
{
public:
    typedef int32_t taskid_t;
    const static taskid_t INVALID_TASKID = (taskid_t)-1;

    struct ActionDocument
    {
        ActionDocument() : taskId(INVALID_TASKID) {}
        ActionDocument(const FX_NS(document)::DocumentPtr& pDoc)
            : taskId(INVALID_TASKID), document(pDoc)
        {}

        FX_NS(document)::Document::Action getAction() const
        {
            return document ? document->getAction() 
                : FX_NS(document)::Document::AT_NONE;
        }

        taskid_t taskId;
        FX_NS(document)::DocumentPtr document;
    };
    DEFINE_TYPED_PTR(ActionDocument);
        
    typedef FX_NS(utility)::BoundedConcurrentQueue<ActionDocumentPtr> DocumentQueue;

    class Task : public FX_NS(thread)::Runnable
    {
    public:
        const static size_t DEFAULT_WAIT_MILLISECONDS = 5 * 1000; //5 seconds

    public:
        Task(taskid_t taskId, 
             taskid_t nextTaskId,
             IndexBarrelWriterPtr& pWriter,
             DocumentQueue& docQueue,
             AsyncDocumentConsumer& consumer);
        ~Task();

    public:
        /**
         * Fetch document from document queue
         */
        void run();

        void forceCommit();
    
    private:
        void doProcess(ActionDocumentPtr& pDoc);

        bool autoCommit() const;

    private:
        taskid_t m_taskId;
        taskid_t m_nextTaskId;
        IndexBarrelWriterPtr m_pIndexBarrelWriter;
        DocumentQueue& m_docQueue;
        AsyncDocumentConsumer& m_consumer;

		FX_NS(thread)::FastMutex m_commitLock;
		FX_NS(thread)::Condition m_commitCond;
		volatile bool m_bCommitting;

    private:
        DECLARE_STREAM_LOGGER();
    };
    DEFINE_TYPED_PTR(Task);

    class OnDiskUpdateTask : public FX_NS(thread)::Runnable
    {
    public:
        OnDiskUpdateTask(const IndexBarrelKeeperPtr& pBarrelKeeper,
                         DocumentQueue& docQueue,
                         DocumentQueue& docUpdateQueue);

    public:
        /// Consume document (delete or update)
        inline void consume(const FX_NS(document)::DocumentPtr& pDoc)
        {
            m_docUpdateQueue.enqueue(ActionDocumentPtr(new ActionDocument(pDoc)));
        }

        /**
         * Fetch document from document queue
         */
        void run();

		void forceCommit();

    private:
        IndexBarrelKeeperPtr m_pKeeper;
        DocumentQueue& m_docQueue;
        DocumentQueue& m_docUpdateQueue;

		FX_NS(thread)::FastMutex m_commitLock;
		FX_NS(thread)::Condition m_commitCond;
		volatile bool m_bCommitting;

    private:
        DECLARE_STREAM_LOGGER();
    };
    DEFINE_TYPED_PTR(OnDiskUpdateTask);

public:
    /**
     * Constructor
     *
     * @param pBarrelKeeper reference to the keeper of index barrel
     */
    AsyncDocumentConsumer(const IndexBarrelKeeperPtr& pBarrelKeeper);
    ~AsyncDocumentConsumer();

public:
    /**
     * Consume document 
     *
     * @param action action of this document (ADD/DELETE/UPDATE)
     * @param pDoc document to consume
     */
    virtual void consume(const FX_NS(document)::DocumentPtr& pDoc);

    /**
     * Commit all documents in queue to file system.
     */
    virtual void commit();

public:
    /// Initialize and start the consumer
    void start();

    /**
     * Commit in-memory barrel to disk
     */
    void commitBarrel(IndexBarrelWriterPtr& writer);

    /// Commit in-memory barrels and Stop all background threads
    void stop();

public:
    inline IndexBarrelKeeperPtr getKeeper() const;
    inline FX_NS(utility)::SyncSegregatedAllocatorPtr getAllocator() const;

private:
    typedef std::vector<TaskPtr> TaskVect;

private:
    IndexBarrelKeeperPtr m_pKeeper;

    FX_NS(thread)::ThreadPool m_threadPool;
    DocumentQueue m_docQueue;
    
    FX_NS(thread)::Thread m_updateThread;
    DocumentQueue m_docUpdateQueue;

    FX_NS(utility)::SyncSegregatedAllocatorPtr m_pAllocator;
    TaskVect m_tasks;

    OnDiskUpdateTaskPtr m_pUpdateTask;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(AsyncDocumentConsumer);

////////////////////////////////////////////////
//
inline IndexBarrelKeeperPtr AsyncDocumentConsumer::getKeeper() const
{
    return m_pKeeper;
}

inline FX_NS(utility)::SyncSegregatedAllocatorPtr
AsyncDocumentConsumer::getAllocator() const
{
    return m_pAllocator;
}

FX_NS_END

#endif //__FX_ASYNCDOCUMENTCONSUMER_H
