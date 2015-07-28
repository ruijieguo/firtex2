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
// Created	: 2005/11/20
//

#include <memory>
#include <assert.h>
#include "firtex/index/IndexWriter.h"
#include "firtex/utility/Exception.h"
#include "firtex/index/IndexMerger.h"
#include "firtex/analyzer/AnalyzerFactory.h"
#include "firtex/index/Index.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/index/MergePolicyFactory.h"
#include "firtex/index/OptimizeMergePolicy.h"
#include "firtex/index/SyncCommitScheduler.h"
#include "firtex/index/AsyncCommitScheduler.h"
#include "firtex/index/AsyncDocumentConsumer.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(utility);
FX_NS_USE(document);
FX_NS_USE(config);
FX_NS_USE(analyzer);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, IndexWriter);

SETUP_LOGGER_EMBED(index, IndexWriter, AutoCommitTask);
//////////////////////////////////////////////////////////////////////////
//IndexWriter
IndexWriter::IndexWriter()
    : m_nOpCountSinceLastCommit(0)
{
    m_pCommitScheduler.reset(new AsyncCommitScheduler());
}

IndexWriter::~IndexWriter(void)
{
    close();
}

void IndexWriter::open(const IndexBarrelKeeperPtr& pBarrelKeeper)
{
    m_pKeeper = pBarrelKeeper;

    m_pKeeper->setCommitScheduler(m_pCommitScheduler);

    createMerger();
    m_pDocConsumer.reset(new AsyncDocumentConsumer(m_pKeeper));
    
    m_pDocConsumer->start();

}

void IndexWriter::updateDocument(const DocumentPtr& pDocument)
{
    m_pDocConsumer->consume(pDocument);

    ++m_nOpCountSinceLastCommit;
    if (autoCommit())
    {
        // Perform commit automatically
        commit();
    }
}

void IndexWriter::addDocument(const DocumentPtr& pDocument)
{
    FX_DEBUG("Add one document");
    pDocument->setAction(Document::AT_ADD);
    m_pDocConsumer->consume(pDocument);
    ++m_nOpCountSinceLastCommit;

    if (autoCommit())
    {
        // Perform commit automatically
        commit();
    }
}

void IndexWriter::deleteDocument(const std::string& sPrimkey)
{
    DocumentPtr pDoc(new Document(m_pKeeper->getDocSchema()));
    pDoc->setPrimaryKey(sPrimkey);
    pDoc->setAction(Document::AT_DELETE);
    m_pDocConsumer->consume(pDoc);
    ++m_nOpCountSinceLastCommit;

    if (autoCommit())
    {
        // Perform commit automatically
        commit();
    }
}

void IndexWriter::commit()
{
    FX_DEBUG("Commit in-memory data");
    m_pDocConsumer->commit();
    m_nOpCountSinceLastCommit = 0;
}

bool IndexWriter::addExistIndex(FileSystemPtr& pFileSys)
{
    ///TODO:
    assert(false);
    return false;
}

void IndexWriter::mergeIndex()
{
    if (!m_pIndexMerger)
    {
        createMerger();
    }

    if (m_pIndexMerger)
    {
        //Flush all documents in memory
        m_pDocConsumer->commit();
        CommittablePtr pCommitObj = std::dynamic_pointer_cast<Committable>(m_pIndexMerger);
        m_pCommitScheduler->commit(pCommitObj);
        m_pCommitScheduler->waitCommit();
    }
}

void IndexWriter::setMergePolicy(const MergePolicyPtr& pMergePolicy)
{
    m_pIndexMerger->setMergePolicy(pMergePolicy);
}

void IndexWriter::optimizeIndex()
{
    //Flush all documents in memory
    m_pDocConsumer->commit();
    
    MergePolicyPtr pPolicy(new OptimizeMergePolicy);
    IndexMergerPtr pIndexMerger(new IndexMerger(pPolicy, m_pKeeper.get()));

    CommittablePtr pCommitObj = std::dynamic_pointer_cast<Committable>(pIndexMerger);
    m_pCommitScheduler->commit(pCommitObj);
    m_pCommitScheduler->waitCommit();
}

void IndexWriter::close()
{
    if (m_pDocConsumer)
    {
        FX_TRACE("Close index writer");

        m_pDocConsumer->stop();
        m_pDocConsumer.reset();

        m_pKeeper->waitCommit();

        ///perform last merge
        if (m_pIndexMerger)
        {
            CommittablePtr pCommitObj = std::dynamic_pointer_cast<Committable>(m_pIndexMerger);
            m_pCommitScheduler->commit(pCommitObj);
            m_pCommitScheduler->waitCommit();
            m_pIndexMerger.reset();
        }

        m_pCommitScheduler->close();
        m_pCommitScheduler.reset();
    }

    if (m_pAutoCommitTask)
    {
        m_pAutoCommitTask->stop();
        m_autoCommitThread.join();
        m_pAutoCommitTask.reset();
    }
}

void IndexWriter::createMerger()
{
    if (!m_pIndexMerger)
    {
        m_pIndexMerger.reset(new IndexMerger(m_pKeeper.get()));
        std::string sIdent = GLOBAL_CONF().Merge.strategy;
        MergePolicyPtr pMergePolicy(MergePolicyFactory::instance()->createMergePolicy(sIdent));
        if (!pMergePolicy)
        {
            FX_LOG(WARN, "Invalid merge policy identifier: [%s]",
                   sIdent.c_str());
        }
        else
        {
            m_pIndexMerger->setMergePolicy(pMergePolicy);
        }
    }
}

IndexMergerPtr IndexWriter::acquireMerger()
{
    if (!m_pIndexMerger)
    {
        createMerger();
    }
    return m_pIndexMerger;
}

void IndexWriter::updateState(IndexWriterNotification::NotifyType nt)
{
    IndexWriterNotification notification(nt, this);
    update(notification);
}

void IndexWriter::setAutoCommit(const CommitCondition& cond)
{
    m_autoCommitCondition = cond;

    if (m_autoCommitCondition.maxTime > 0)
    {
        if (m_pAutoCommitTask)
        {
            m_pAutoCommitTask->stop();
            m_autoCommitThread.join();
        }

        m_pAutoCommitTask.reset(new AutoCommitTask(*this,
                        (int32_t)(m_autoCommitCondition.maxTime + 1)/2));
        m_autoCommitThread.start(*m_pAutoCommitTask);
    }
}

FX_NS_END

