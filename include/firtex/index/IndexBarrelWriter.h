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
// Created	: 2005/12/07
//

#ifndef __FX_INDEXBARRELWRITER_H
#define __FX_INDEXBARRELWRITER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/Notifier.h"
#include "firtex/index/StoredFieldsWriter.h"
#include "firtex/index/StoredFieldsReader.h"
#include "firtex/index/Term.h"
#include "firtex/index/TermPostingIterator.h"
#include "firtex/index/FieldIndexer.h"
#include "firtex/index/LengthNormWriter.h"
#include "firtex/index/ForwardIndexWriter.h"
#include "firtex/index/DeletedDocumentFilter.h"
#include "firtex/index/Committable.h"
#include "firtex/index/PostingPool.h"
#include "firtex/index/PrimaryKeyIndex.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/Document.h"
#include "firtex/document/Field.h"
#include "firtex/document/AnalyzedDocument.h"
#include "firtex/utility/SyncSegregatedAllocator.h"
#include "firtex/utility/LooseBoundedPool.h"
#include "firtex/analyzer/AnalyzerMapper.h"

FX_NS_DEF(index);

class ComponentBuilder;
class IndexBarrelReader;
class IndexBarrelKeeper;

class IndexBarrelWriter : public FX_NS(common)::Notifier, public Committable
{
public:
    /// Define total chunks for in-memory barrel
    const static size_t DEFAULT_TOTAL_CHUNK_NUMBER = 10; 
    
    /// Define number of chunks used for buffer (less than DEFAULT_TOTAL_CHUNK_NUMBER)
    const static size_t DEFAULT_CHUNK_NUMBER_OF_BUFFER = 8; //8/10

    enum BarrelState
    {
        STATE_UNSEALED = 0,
        STATE_SEALED,
    };

public:
    IndexBarrelWriter(IndexBarrelKeeper* pBarrelKeeper,
                      commitid_t cachedBarrelId);
    virtual ~IndexBarrelWriter();

public:
    /**
     * Init the writer for building documents
     * 
     * @return false if failure
     */
    bool init(const FX_NS(utility)::SyncSegregatedAllocatorPtr& pAllocator);

    /**
     * Add an analyzed Document
     * @param pDoc analyzed document
     * @see Document
     */
    void addDocument(const FX_NS(document)::Document* pDoc);

    /**
     * Delete a document by document id
     * @param docId document id
     * @return true if deleted, otherwise false
     */
    bool deleteDocument(const FX_NS(document)::Document* pDoc);

    /** 
     * Undelete all deleted documents
     */
    void undeleteAll();

    /**
     * Determine whether the barrel has changed
     */
    bool isDirty() const;

    /**
     * Return doc count in the barrel
     */
    uint32_t getDocCount() const;

public:
    /**
     * Commit in-memory data to file system
     */
    void commit();

    /**
     * Generate a new barrel and commit to pBarrelsInfo
     *
     * @return true if there is a valid barrel
     */
    bool commitBarrelInfo(BarrelsInfoPtr& pBarrelsInfo);

    /**
     * Determine if the index barrel can seal or not
     *
     * @return true if no sufficient resource and need to seal, otherwise return false.
     */
    bool canSeal();

    /**
     * Seal the index barrel. After sealing, modification is forbidden
     */
    void sealBarrel();

public:
    /**
     * Return indexer of specified field
     * @param field field name
     * @see FieldIndexer
     */
    FieldIndexerPtr getFieldIndexer(const std::string& field) const;

    /**
     * Return indexer of specified field
     * @param fieldId field id
     * @return internal indexer
     * @see FieldIndexer
     */
    FieldIndexerPtr getFieldIndexer(fieldid_t fieldId) const;

    /**
     * Create index barrel reader of in-memory index
     * @return in-memory index reader
     */
    IndexBarrelReader* inMemoryReader() const;

    /**
     * Return stored fields writer
     * @see StoredFieldsWriter
     */
    StoredFieldsWriterPtr getStoredFieldsWriter() const;

    /// Return forward index writer by field name
    ForwardIndexWriterPtr getForwardIndexWriter(fieldid_t fieldId) const;

    /**
     * Return length norm writer
     * @see LengthNormWriter
     */
    LengthNormWriterPtr getLengthNormWriter() const;

    /// Return barrel info of the building barrel
    const BarrelInfo& getBarrelInfo() const;

public:
    /**
     * Set encoding of the index database
     * @param sEncodeName encode name, "GBK", "UTF-8", etc.
     */
    inline void setEncoding(const std::string& sEncodeName);

    /// Return current state of the barrel
    inline BarrelState getState() const;

    /// Return deleted document filter of this barrel
    const FX_NS(utility)::BitVectorPtr& getDocFilter() const;

    /// Return primary key
    const PrimaryKeyIndexPtr& getPrimaryKey() const;
    
protected:
    /**
     * Commit in-memory index to filesystem
     */
    void commitIndex(const std::string& sSuffix);

    /// Commit index meta
    void commitIndexMeta(IndexMeta& indexMeta);

    /**
     * Commit in-memory forward index to filesystem
     */
    void commitForwardIndex(const std::string& sSuffix);

    /**
     * Commit deleted document filter
     */
    void commitDeletion(const BarrelInfo& barrelInfo);

    /// Seal index 
    void sealIndex();
    
    /// Seal forward index
    void sealForwardIndex();

    /**
     * Add an indexer of field
     */
    void addIndex(fieldid_t fieldId);

    /**
     * Add an forward index of field
     */
    void addForwardIndex(fieldid_t fieldId);

    /// Update barrel info
    void updateBarrelInfo(BarrelsInfoPtr& pBarrelsInfo);

    /// Clean cache directory
    void cleanCacheDir();

private:
    typedef std::vector<FieldIndexerPtr> IndexerVector;
    typedef std::vector<ForwardIndexWriterPtr> ForwardIndexVect;

private:
    IndexBarrelKeeper* m_pKeeper;

    FX_NS(utility)::BoundedChunkAllocatorPtr m_pBoundedAllocator;

    PostingPoolPtr m_pPostingPool;

    StoredFieldsWriterPtr m_pStoredFieldsWriter;
    LengthNormWriterPtr m_pLengthNormWriter;

    IndexerVector m_indexers;
    ForwardIndexVect m_forwardIndices;
    FX_NS(utility)::BitVectorPtr m_pInMemDocFilter;
    PrimaryKeyIndexPtr m_pPrimaryKeyIndex;
    DeletedDocumentFilterPtr m_pDocFilterNeedCommit;

    FX_NS(analyzer)::TokenSourcePtr m_pTokenSource;
    FX_NS(document)::AnalyzedDocumentPtr m_pAnalyzedDoc;

    /// Barrel state
    BarrelState m_state;

    /// Encoding of raw data
    std::string m_sEncoding;

    /// For storing temporary data
    BarrelInfo m_barrelInfo;

    /// Uncommitted document count
    uint32_t m_nUncommittedDocs;

    friend class InMemoryIndexBarrelReader;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(IndexBarrelWriter);

////////////////////////////////////////////////////////
///
inline bool IndexBarrelWriter::canSeal()
{
    return ((m_pPostingPool->bufferPool && m_pPostingPool->bufferPool->hasException()) 
            || m_pPostingPool->postingPool->hasException());
}

inline StoredFieldsWriterPtr IndexBarrelWriter::getStoredFieldsWriter() const 
{
    return m_pStoredFieldsWriter;
}

inline LengthNormWriterPtr IndexBarrelWriter::getLengthNormWriter() const 
{
    return m_pLengthNormWriter;
}

inline bool IndexBarrelWriter::isDirty() const
{
    //return (m_nUncommittedDocs > 0 || m_pOnDiskDocFilter->isDirty());
    return (m_nUncommittedDocs > 0);
}

inline uint32_t IndexBarrelWriter::getDocCount() const
{
    return m_nUncommittedDocs;
}

inline void IndexBarrelWriter::setEncoding(const std::string& sEncodeName)
{
    m_sEncoding = sEncodeName;
}

inline IndexBarrelWriter::BarrelState IndexBarrelWriter::getState() const
{
    return m_state;
}


inline const FX_NS(utility)::BitVectorPtr& IndexBarrelWriter::getDocFilter() const
{
    return m_pInMemDocFilter;
}

inline const PrimaryKeyIndexPtr& IndexBarrelWriter::getPrimaryKey() const
{
    return m_pPrimaryKeyIndex;
}

inline const BarrelInfo& IndexBarrelWriter::getBarrelInfo() const
{
    return m_barrelInfo;
}

FX_NS_END

#endif
