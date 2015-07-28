#include "firtex/index/AsyncDocumentConsumer.h"
#include "firtex/thread/Thread.h"
#include "firtex/config/GlobalConfig.h"

FX_NS_USE(thread);
FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, AsyncDocumentConsumer);

AsyncDocumentConsumer::AsyncDocumentConsumer(
        const IndexBarrelKeeperPtr& pBarrelKeeper)
    : m_pKeeper(pBarrelKeeper)
    , m_threadPool(GLOBAL_CONF().Build.buildThreadCount, GLOBAL_CONF().Build.buildThreadCount)
    , m_docQueue(GLOBAL_CONF().Build.documentQueueSize)
    , m_docUpdateQueue(GLOBAL_CONF().Build.documentQueueSize)
{
}

AsyncDocumentConsumer::~AsyncDocumentConsumer()
{
    m_tasks.clear();
    m_pAllocator.reset();
}

void AsyncDocumentConsumer::start()
{
    size_t nSizeInMB = (size_t)GLOBAL_CONF().Build.memory;
    FX_LOG(INFO, "Allocate memory for building, total size: [%u] MB, segment count: [%u]",
           (uint32_t)nSizeInMB, (uint32_t)m_threadPool.capacity());
    m_pAllocator.reset(new SyncSegregatedAllocator((size_t)(nSizeInMB * 1024 * 1024),
                    m_threadPool.capacity() * MAX_CHUNK_COUNT_PER_SEGMENT));

    m_pUpdateTask.reset(new OnDiskUpdateTask(m_pKeeper, m_docQueue, m_docUpdateQueue));
    m_updateThread.start(*m_pUpdateTask);
    
    taskid_t taskId = 0, nextTaskId = INVALID_TASKID;
    for (size_t i = 0; i < m_threadPool.capacity(); i++)
    {
        IndexBarrelWriterPtr pIndexBarrelWriter = m_pKeeper->createBarrelWriter(); 
        if (!pIndexBarrelWriter->init(m_pAllocator))
        {
            FIRTEX_THROW_AND_LOG(RuntimeException, "Init index writer FAILED.");
            break;
        }
        
        if (i + 1 != m_threadPool.capacity())
        {
            nextTaskId = taskId + 1;
        }

        TaskPtr pTask(new Task(taskId, nextTaskId, pIndexBarrelWriter,
                               m_docQueue, *this));
        
        m_threadPool.start(*pTask);
        m_tasks.push_back(pTask);
    }
}

void AsyncDocumentConsumer::consume(const FX_NS(document)::DocumentPtr& pDoc)
{
    if (pDoc->getAction() == Document::AT_ADD)
    {
        m_docQueue.enqueue(ActionDocumentPtr(new ActionDocument(pDoc)));
    }
    else 
    {
        m_docUpdateQueue.enqueue(ActionDocumentPtr(new ActionDocument(pDoc)));
    }
}

void AsyncDocumentConsumer::stop()
{
    if (m_tasks.size() > 0)
    {
        FX_TRACE("Document queue size: %u", m_docQueue.getSize());

        /// Wait until the document queue is empty
        m_docUpdateQueue.enqueue(ActionDocumentPtr());
        m_updateThread.join();        

        for (size_t i = 0; i < m_threadPool.capacity(); ++i)
        {
            m_docQueue.enqueue(ActionDocumentPtr());
        }
        FX_TRACE("Wait document queue empty");
        FX_TRACE("Document queue size: %u", m_docQueue.getSize());
        m_docQueue.waitEmpty();
        FX_TRACE("End wait document queue empty, queue size: [%lu]", 
                 (uint32_t)m_docQueue.getSize());

        m_threadPool.joinAll();
        m_tasks.clear();
    }
}

void AsyncDocumentConsumer::commit()
{
    FX_DEBUG("Begin force commit.");
 
    m_pUpdateTask->forceCommit();

    for (TaskVect::iterator it = m_tasks.begin(); it != m_tasks.end(); ++it)
    {
        (*it)->forceCommit();
    }

    m_pKeeper->forceCommit();
    m_pKeeper->waitCommit();

    FX_DEBUG("Force commit returned.");
}


//////////////////////////////////////////////////////////////////////
///
SETUP_STREAM_LOGGER_EMBED(index, AsyncDocumentConsumer, Task);

AsyncDocumentConsumer::Task::Task(taskid_t taskId, 
                                  taskid_t nextTaskId,
                                  IndexBarrelWriterPtr& pWriter,
                                  DocumentQueue& docQueue,
                                  AsyncDocumentConsumer& consumer)
    : m_taskId(taskId)
    , m_nextTaskId(nextTaskId)
    , m_pIndexBarrelWriter(pWriter)
    , m_docQueue(docQueue)
    , m_consumer(consumer)
    , m_bCommitting(false)
{
}

AsyncDocumentConsumer::Task::~Task() 
{
}

void AsyncDocumentConsumer::Task::run()
{
    while (true)
    {
        if (m_pIndexBarrelWriter->canSeal())
        {
            FX_DEBUG("Seal the barrel");
            
            m_consumer.getKeeper()->sealBarrel(m_pIndexBarrelWriter);
            
            FX_DEBUG("Create a new barrel");
            m_pIndexBarrelWriter = m_consumer.getKeeper()->createBarrelWriter();

            while (!m_pIndexBarrelWriter->init(m_consumer.getAllocator()))
            {
                FX_LOG(WARN, "Waiting for sufficient memory");

                /// Waiting for sufficient memory
                Thread::sleep(DEFAULT_WAIT_MILLISECONDS);
            }
        }

		
        ActionDocumentPtr pDoc = m_docQueue.waitDequeue();
        if (!pDoc)
        {
            FX_TRACE("Fetched a null document, exit building thread.");
            if (m_pIndexBarrelWriter)
            {
                m_consumer.getKeeper()->sealBarrel(m_pIndexBarrelWriter);
                m_pIndexBarrelWriter.reset();
            }
            break;
        }
        
        if (pDoc->getAction() != Document::AT_NONE)
        {
            doProcess(pDoc);
        }
    }
}

void AsyncDocumentConsumer::Task::forceCommit()
{
    FX_DEBUG("Force commit.");
    
    m_docQueue.enqueue(ActionDocumentPtr(new ActionDocument(DocumentPtr())));
    m_docQueue.waitEmpty();
    m_consumer.getKeeper()->sealBarrel(m_pIndexBarrelWriter);

    FX_DEBUG("Create a new in-memory barrel");
    m_pIndexBarrelWriter = m_consumer.getKeeper()->createBarrelWriter();

    while (!m_pIndexBarrelWriter->init(m_consumer.getAllocator()))
    {
        FX_LOG(WARN, "Waiting for sufficient memory");
                
        /// Waiting for sufficient memory
        Thread::sleep(DEFAULT_WAIT_MILLISECONDS);
    }
}

void AsyncDocumentConsumer::Task::doProcess(ActionDocumentPtr& pDoc)
{
    FX_TRACE("Fetched one document, taskId: [%d], action: [%d]",
             pDoc->taskId, (int32_t)pDoc->getAction());

    if (pDoc->taskId == INVALID_TASKID || pDoc->taskId == m_taskId)
    {
        switch (pDoc->getAction())
        {
        case Document::AT_ADD:
            m_pIndexBarrelWriter->addDocument(pDoc->document.get());
            return;
        case Document::AT_DELETE:
            if (m_pIndexBarrelWriter->deleteDocument(pDoc->document.get()))
            {
                FX_TRACE("Delete document: [%s] in task: [%d] success",
                        pDoc->document->getPrimaryKey(), m_taskId);
                return;
            }
            break;
        case Document::AT_UPDATE:
            //TODO:
            FIRTEX_ASSERT2(false);
            break;
        default:
            FIRTEX_ASSERT2(false);
            break;
        }
    }
    
    if (m_nextTaskId != INVALID_TASKID)
    {
        FX_TRACE("Passing the deletion of document: [%s] to next task: [%d].",
                 pDoc->document->getPrimaryKey(), m_nextTaskId);
        pDoc->taskId = m_nextTaskId;
        
        //Pass to next task
        m_docQueue.enqueue(pDoc);
    }
}

/////////////////////////////////////////////////
//
SETUP_STREAM_LOGGER_EMBED(index, AsyncDocumentConsumer, OnDiskUpdateTask);

AsyncDocumentConsumer::OnDiskUpdateTask::OnDiskUpdateTask(
        const IndexBarrelKeeperPtr& pBarrelKeeper,
        DocumentQueue& docQueue,
        DocumentQueue& docUpdateQueue)
    : m_pKeeper(pBarrelKeeper)
    , m_docQueue(docQueue)
    , m_docUpdateQueue(docUpdateQueue)
    , m_bCommitting(false)
{
}

void AsyncDocumentConsumer::OnDiskUpdateTask::forceCommit()
{
    m_docUpdateQueue.enqueue(ActionDocumentPtr(new ActionDocument(DocumentPtr())));
    m_docUpdateQueue.waitEmpty();
}

void AsyncDocumentConsumer::OnDiskUpdateTask::run()
{
    while (true)
    {
        ActionDocumentPtr pDoc = m_docUpdateQueue.waitDequeue();
        if (!pDoc)
        {
            FX_DEBUG("Fetched a null document, exit updating thread.");
            break;
        }

        if (pDoc->getAction() != Document::AT_NONE)
        {
            const char* szPk = pDoc->document->getPrimaryKey();
            if (szPk)
            {
                FX_TRACE("Try delete document: [%s] in on-disk barrels", szPk);
                if (!m_pKeeper->deleteOnDiskDocument(szPk))
                {
                    FX_TRACE("Passing the deletion of document: [%s] to in-memory barrel ", szPk);
                    
                    /// Pass to the first building task
                    pDoc->taskId = 0;
                    m_docQueue.enqueue(pDoc);
                }
            }
        }		
    }
}
    
FX_NS_END

