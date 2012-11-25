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
// Created	: 2005/11/20
// Modified	: 2006/10/11 
//

#ifndef __FX_INDEXWRITER_H
#define __FX_INDEXWRITER_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "firtex/common/SharedPtr.h"
#include "firtex/common/Notifier.h"
#include "firtex/index/IndexWriterNotification.h"
#include "firtex/analyzer/Analyzer.h"
#include "firtex/utility/Reader.h"
#include "firtex/document/Document.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/IndexMerger.h"
#include "firtex/analyzer/AnalyzerMapper.h"
#include "firtex/index/DeletedDocumentFilter.h"
#include "firtex/index/PrimaryKeyIndex.h"
#include "firtex/index/DocumentConsumer.h"
#include "firtex/index/CommitScheduler.h"
#include "firtex/index/IndexBarrelKeeper.h"
#include "firtex/index/CommitCondition.h"
#include "firtex/thread/SynchronizedObject.h"
#include "firtex/thread/ThreadPool.h"

FX_NS_DEF(index);

class Index;
class ComponentBuilder;
class IndexBarrelReader;

class IndexWriter : public FX_NS(common)::Notifier
{
public:
    IndexWriter();

public:
    virtual~IndexWriter();

public:
    /**
     * Prepare for Add/Delete documents
     */
    void open(const IndexBarrelKeeperPtr& pBarrelKeeper);

    /// Update document, delete/add/update
    void updateDocument(const FX_NS(document)::DocumentPtr& pDocument);

    /**
     * Add a document to index database
     */
    void addDocument(const FX_NS(document)::DocumentPtr& pDocument);

    /**
     * Delete a document by primary key
     * @param sPrimkey primary key of document
     */
    void deleteDocument(const std::string& sPrimkey);

    /**
     * Is there any documents with the specific primary key.
     *
     * @param sPrimkey primary key of documents
     * @return true if the document with the specific primary key is existed,
     * otherwise return false.
     */
    bool hasDocument(const std::string& sPrimkey) const;

    /**
     * Add and merge index
     * @param[in] pFileSys the place where index stored,must compatible
     * @see mergeIndex(), mergeIndex(IndexMerger*)
     */
    bool addExistIndex(FX_NS(store)::FileSystemPtr& pFileSys);

    /**
     * Commit in-memory index to file system
     */
    void commit();

    /**        
     * Merge index in an explicit form, the writer will create 
     * index merge according to the configure parameter by itself
     * @see mergeIndex(IndexMerger*)
     */
    void mergeIndex();

    /**
     * Optimize index
     */
    void optimizeIndex();

    /**        
     * Set specific merge policy
     * @param[in] pMergePolicy merge policy
     */
    void setMergePolicy(const MergePolicyPtr& pMergePolicy);

    /**
     * Set commit scheduler
     */
    void setCommitScheduler(const CommitSchedulerPtr& pScheduler);

    /**
     * Close index writer
     */
    void close();

    /**
     * Acquire index merger
     */
    IndexMergerPtr acquireMerger();
    
    /**
     * Set encoding of the index database
     * @param sEncodeName encode name, "GBK", "UTF-8", etc.
     */
    void setEncoding(const std::string& sEncodeName);

    /// Return document schema
    const FX_NS(document)::DocumentSchema* getDocSchema() const;

    /**
     * Set commit condition for performing commit automatically
     * 
     * @param cond commit condition
     */
    void setAutoCommit(const CommitCondition& cond);
    
    /// Return auto commit condition
    inline const CommitCondition& getAutoCommit() const;

    /// Determine whether perform commit automatically or not
    inline bool autoCommit() const;

protected:
    /**
     * Create index merger 
     */
    void createMerger();

    /**
     * Send notification in sync mode
     */
    void updateState(IndexWriterNotification::NotifyType nt);

    /**
     * Create a new in-memory index barrel
     */
    void createNewBarrel(const BarrelInfo* pLastBarrel);

    /**
     * Update primary key index
     */
    void updatePrimaryKeyIndex();

private:
    IndexBarrelKeeperPtr m_pKeeper;

    IndexMergerPtr m_pIndexMerger;

    CommitSchedulerPtr m_pCommitScheduler;
    DocumentConsumerPtr m_pDocConsumer;

    CommitCondition m_autoCommitCondition;
    volatile int64_t m_nOpCountSinceLastCommit;

private:
    class AutoCommitTask : public FX_NS(thread)::Runnable
    {
    public:
        const static int32_t CHECK_INTERVAL = 500; //in ms

    public:
        AutoCommitTask(IndexWriter& writer, int32_t nCheckInterval)
            : m_writer(writer)
            , m_nCheckInterval(nCheckInterval > CHECK_INTERVAL ? CHECK_INTERVAL : nCheckInterval)
            , m_bStopRequested(false)
        {
        }

        void run()
        {
            while (!m_bStopRequested)
            {
                if (m_writer.autoCommit())
                {
                    FX_LOG(INFO, "Perform commit automatically");
                    m_writer.commit();
                }
                FX_NS(thread)::Thread::sleep(m_nCheckInterval);
            }
        }

        void stop()
        {
            m_bStopRequested = true;
        }

    private:
        IndexWriter& m_writer;
        int32_t m_nCheckInterval;
        bool m_bStopRequested;

    private:
        DECLARE_LOGGER();
    };
    DEFINE_TYPED_PTR(AutoCommitTask);

    AutoCommitTaskPtr m_pAutoCommitTask;
    FX_NS(thread)::Thread m_autoCommitThread;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(IndexWriter);

///////////////////////////////////////////////
inline void IndexWriter::setCommitScheduler(const CommitSchedulerPtr& pScheduler)
{
    m_pCommitScheduler = pScheduler;
    if (m_pKeeper.isNotNull())
    {
        m_pKeeper->setCommitScheduler(m_pCommitScheduler);
    }
}

inline void IndexWriter::setEncoding(const std::string& sEncodeName)
{
    m_pKeeper->setEncoding(sEncodeName);
}

inline const FX_NS(document)::DocumentSchema* IndexWriter::getDocSchema() const
{
    return m_pKeeper->getDocSchema();
}

inline bool IndexWriter::hasDocument(const std::string& sPrimkey) const
{
    return m_pKeeper->hasDocument(sPrimkey);
}

inline const CommitCondition& IndexWriter::getAutoCommit() const
{
    return m_autoCommitCondition;
}

inline bool IndexWriter::autoCommit() const
{
    return ((m_autoCommitCondition.maxDocs > 0 && m_nOpCountSinceLastCommit >= m_autoCommitCondition.maxDocs)
            || (m_autoCommitCondition.maxTime > 0 
                && ((m_pKeeper->getLastCommitTime().elapsed() / 1000) >= m_autoCommitCondition.maxTime) 
                && m_nOpCountSinceLastCommit > 0));
}

FX_NS_END

#endif
