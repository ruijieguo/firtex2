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
// Created : 2011-06-07 22:27:21

#ifndef __FX_INDEXBARRELKEEPER_H
#define __FX_INDEXBARRELKEEPER_H

#include <map>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/analyzer/AnalyzerMapper.h"
#include "firtex/index/DeletedDocumentFilter.h"
#include "firtex/index/IndexBarrelReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/PrimaryKeyIndex.h"
#include "firtex/index/CommitScheduler.h"
#include "firtex/index/IndexCleaner.h"
#include "firtex/thread/RWLock.h"
#include "firtex/utility/SyncSegregatedAllocator.h"

FX_NS_DEF(index);

class IndexBarrelWriter;
DEFINE_TYPED_PTR(IndexBarrelWriter);

class InMemIndexMerger;
DEFINE_TYPED_PTR(InMemIndexMerger);

class SingleIndexBarrelReader;
class MultiIndexBarrelReader;

class IndexBarrelKeeper
{
public:
    /// For clean staled index data in file system
    const static uint32_t DEFAULT_COMMIT_KEEP_COUNT = 2;

public:
    enum Mode
    {
        READ = 0x10,
        WRITE = 0x20,
        RDWR = READ | WRITE,
    };
public:
    class IndexBarrel
    {
    public:
        IndexBarrel(const BarrelsInfoPtr& b, const IndexBarrelReaderPtr& r,
                    const DeletedDocumentFilterPtr& f)
            : m_pBarrelsInfo(b), m_pReader(r), m_pDocFilter(f)
        {}

    public:
        inline const BarrelsInfoPtr& getBarrelsInfo() const
        {
            return m_pBarrelsInfo;
        }

        inline const IndexBarrelReaderPtr& getReader() const
        {
            return m_pReader;
        }
        
        inline const DeletedDocumentFilterPtr& getDeletedDocFilter() const
        {
            return m_pDocFilter;
        }

        inline void setEncoding(const std::string& sEncodeName)
        {
            m_pBarrelsInfo->setEncoding(sEncodeName);
        }
        
    private:
        BarrelsInfoPtr m_pBarrelsInfo;
        IndexBarrelReaderPtr m_pReader;
        DeletedDocumentFilterPtr m_pDocFilter;
    };

    DEFINE_TYPED_PTR(IndexBarrel);

public:
    IndexBarrelKeeper(const FX_NS(store)::FileSystemPtr& pFileSys,
                      const FX_NS(document)::DocumentSchema* pDocSchema,
                      const ComponentBuilder* pCompBuilder,
                      const FX_NS(analyzer)::AnalyzerMapper* pAnalyzeMapper);
    ~IndexBarrelKeeper();

public:
    /// Initialize
    void init(const BarrelsInfoPtr& pBarrelsInfo, Mode mode);

    /// Load on-disk barrels
    void loadOnDiskBarrel(const BarrelsInfoPtr& pBarrelsInfo);

    ///Refresh on-disk index barrels incrementally
    void refresh();

    /**
     * Create a new in-memory barrel writer
     */
    IndexBarrelWriterPtr createBarrelWriter();

    /**
     * Return the latest index barrel reader.
     */
    IndexBarrelReaderPtr getBarrelReader();

    /// Return the latest index barrel
    IndexBarrelPtr getLatestBarrel();

    /**
     * Seal the in-memory barrel and commit to file system. 
     *
     * @param pInMemBarrel the in-memory barrel to seal
     */
    void sealBarrel(const IndexBarrelWriterPtr& pInMemBarrel);

    /// Wait until all documents committed to file system
    void waitCommit();

    /// Force commit all in-memory data
    void forceCommit();

    /**
     * Delete document in on-disk barrels
     *
     * @return true if deletion is success, otherwise false
     */
    bool deleteOnDiskDocument(const char* szPrimKey);
    
    /// Return true if there is any deletion on on-disk barrels
    bool hasOnDiskDeletion() const;

    /// Return true if there is any deletion on on-disk barrels (no lock)
    bool hasOnDiskDeletionUnSafe() const;

    /// Return true if there is any document with the specified primary key.
    bool hasDocument(const std::string& sPrimkey) const;

    /// Clean up staled index data
    void cleanStaledIndex();

    /// Clean staled unreferenced index barrels
    void housekeep();

    /// Set encoding of input documents
    inline void setEncoding(const std::string& sEncodeName);

    /// Set encoding of input documents
    inline std::string getEncoding();

    /// Return held commit count
    inline size_t getHeldCommitCount() const;

public:
    inline const FX_NS(document)::DocumentSchema* getDocSchema() const;
    inline const FX_NS(store)::FileSystemPtr& getFileSystem() const;
    inline FX_NS(store)::FileSystemPtr& getFileSystem();
    inline const FX_NS(analyzer)::AnalyzerMapper* getAnalyzerMapper() const;
    inline const ComponentBuilder* getComponentBuilder() const;

    inline const DeletedDocumentFilterPtr& getOnDiskDocFilter() const;
    inline const DeletedDocumentFilterPtr& getOnDiskDocFilterUnSafe() const;
    inline void setCommitScheduler(const CommitSchedulerPtr& pCommitScheduler);

    /// Return the last commit time
    inline const FX_NS(utility)::Timestamp& getLastCommitTime() const;

private:
    commitid_t createOnDiskBarrelReader(const BarrelsInfoPtr& pBarrelsInfo);
    PrimaryKeyIndexPtr createPrimaryKeyIndex();
    commitid_t loadOnDiskDataForUpdate(const BarrelsInfoPtr& pBarrelsInfo);

    commitid_t refreshDataForRead();
    commitid_t refreshDataForUpdate();
    void setupIndexCleaner();

    bool doInsertCommit(commitid_t commId,
                        const IndexBarrelPtr& pIndexBarrel);

private:
    IndexBarrelKeeper(const IndexBarrelKeeper& );

private:
    typedef std::map<commitid_t, IndexBarrelPtr> CommitMap;

private:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    const FX_NS(document)::DocumentSchema* m_pDocSchema;
    const ComponentBuilder* m_pComponentBuilder;
    const FX_NS(analyzer)::AnalyzerMapper* m_pAnalyzerMapper;

    CommitSchedulerPtr m_pCommitScheduler;
    
    mutable FX_NS(thread)::RWLock m_lock;
    CommitMap m_commitMap;
    commitid_t m_latestOnDiskCommit;

    /// On-disk primary key index
    PrimaryKeyIndexPtr m_pOnDiskPrimKeyIndex;

    /// Filter of deleted documents in on-disk barrels
    DeletedDocumentFilterPtr m_pOnDiskDocFilter;

    /// For cleanup staled index data in file system
    IndexCleanerPtr m_pIndexCleaner;

    /// For merging in-memory barrels
    InMemIndexMergerPtr m_pInMemBarrelMerger;

    /// Encoding of input documents
    std::string m_sEncoding;

    /// Access mode
    Mode m_mode;

private:
    FX_NS(thread)::FastMutex m_counterLock;
    commitid_t m_barrelCounter;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(IndexBarrelKeeper);

//////////////////////////////////////
///
inline const FX_NS(document)::DocumentSchema* IndexBarrelKeeper::getDocSchema() const
{
    return m_pDocSchema;
}

inline const FX_NS(store)::FileSystemPtr& IndexBarrelKeeper::getFileSystem() const
{
    return m_pFileSys;
}

inline FX_NS(store)::FileSystemPtr& IndexBarrelKeeper::getFileSystem()
{
    return m_pFileSys;
}

inline const FX_NS(analyzer)::AnalyzerMapper* IndexBarrelKeeper::getAnalyzerMapper() const
{
    return m_pAnalyzerMapper;
}

inline const ComponentBuilder* IndexBarrelKeeper::getComponentBuilder() const
{
    return m_pComponentBuilder;
}

inline const DeletedDocumentFilterPtr& IndexBarrelKeeper::getOnDiskDocFilter() const
{
    FX_NS(thread)::ScopedRWLock lock(m_lock, false);
    return m_pOnDiskDocFilter;
}

inline const DeletedDocumentFilterPtr& IndexBarrelKeeper::getOnDiskDocFilterUnSafe() const
{
    return m_pOnDiskDocFilter;
}

inline bool IndexBarrelKeeper::hasOnDiskDeletion() const
{
    FX_NS(thread)::ScopedRWLock lock(m_lock, false);
    return (m_pOnDiskDocFilter.isNotNull() && m_pOnDiskDocFilter->isDirty());
}

inline bool IndexBarrelKeeper::hasOnDiskDeletionUnSafe() const
{
    return (m_pOnDiskDocFilter.isNotNull() && m_pOnDiskDocFilter->isDirty());
}

inline void IndexBarrelKeeper::setCommitScheduler(const CommitSchedulerPtr& pCommitScheduler)
{
    m_pCommitScheduler = pCommitScheduler;
}

inline bool IndexBarrelKeeper::hasDocument(const std::string& sPrimkey) const
{
    FX_NS(thread)::ScopedRWLock lock(m_lock, false);
    if (m_pOnDiskPrimKeyIndex.isNotNull())
    {
        return (m_pOnDiskPrimKeyIndex->lookup(sPrimkey) != INVALID_DOCID);
    }
    return false;
}

inline void IndexBarrelKeeper::setEncoding(const std::string& sEncodeName)
{
    m_sEncoding = sEncodeName;
}

inline std::string IndexBarrelKeeper::getEncoding()
{
    return m_sEncoding;
}

inline size_t IndexBarrelKeeper::getHeldCommitCount() const
{
    FX_NS(thread)::ScopedRWLock lock(m_lock, false);
    return m_commitMap.size();
}

inline const FX_NS(utility)::Timestamp& IndexBarrelKeeper::getLastCommitTime() const
{
    FIRTEX_ASSERT2(m_pCommitScheduler.isNotNull());
    return m_pCommitScheduler->getLastCommitTime();
}

FX_NS_END

#endif //__FX_INDEXBARRELKEEPER_H
