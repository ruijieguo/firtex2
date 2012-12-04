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
// Created	: 2006/4/27
//

#ifndef __FX_INDEXREADER_H
#define __FX_INDEXREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Notifiable.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/Document.h"
#include "firtex/index/IndexBarrelReader.h"
#include "firtex/index/TermInfo.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/thread/SynchronizedObject.h"
#include "firtex/index/DeletedDocumentFilter.h"
#include "firtex/index/ForwardIndexIterator.h"
#include "firtex/index/LengthNormReader.h"
#include "firtex/index/IndexBarrelKeeper.h"

FX_NS_DEF(analyzer);
class AnalyzerMapper;
FX_NS_END

FX_NS_DEF(index);

class IndexReader;
DEFINE_TYPED_PTR(IndexReader);

class SingleIndexBarrelReader;
class MultiIndexBarrelReader;

class IndexReader : public FX_NS(common)::Notifiable
{
    friend class Index;
protected:
    IndexReader();
    IndexReader(const IndexReader& src);

public:
    virtual ~IndexReader(void);

protected:
    /**
     * Open the index database
     */
    void open(const IndexBarrelKeeperPtr& pBarrelKeeper);

    /**
     * Reopen the index database. 
     */
    void reopen();

public:
    /**
     * Clone a new instance
     */
    IndexReader* clone() const;

    /**
     * Close it and release resource
     */
    void close();

public:	
    /**
     * Return the version of index database
     * @return version string
     */
    tstring getIndexVersion() const;

    /** 
     * is document deleted or not
     * @param nDocID document ID
     */
    bool isDeleted(docid_t nDocId) const;

    /** 
     * is any deletions
     * @return true if there is
     */
    bool hasDeletions() const;

    /**
     * Return number of deleted documents
     */
    df_t getDeletedDocCount() const;

    /**
     * Return total number of documents in index database.
     */
    df_t getDocCount() const;

    /**
     * Return max document number, if no deleted documents,
     * getMaxDocs() is equal to getDocCount()
     */
    df_t getMaxDocs() const;

    /**
     * Create stored fields reader 
     * @return stored fields reader object which is thread unsafe
     */
    StoredFieldsReaderPtr createStoredFieldsReader() const;

    /**
     * Return length norm by field name
     * @param sField field name
     */
    LengthNormIteratorPtr lengthNorm(const tstring& sField) const;

    /**
     * Return barrels info
     */
    const BarrelsInfo* getBarrelsInfo() const;

    /**
     * Return index meta
     */
    IndexMeta getIndexMeta() const;

    /**
     * Return term reader
     * @return term reader, MUST be deleted by caller.
     */
    TermReaderPtr termReader() const;

    /**
     * Return term reader of the index barrel
     * @param szField field name
     * @return term reader, MUST be detroied by caller
     */
    TermReaderPtr termReader(const tstring& sField) const;

    /**
     * Create the iterator of forward index by field name
     * @param sField field name
     * @return forward index iterator
     */
    ForwardIndexIteratorPtr forwardIndexReader(const tstring& sField) const;

    /**
     * Create the iterator of forward index by fieldId
     * @param sField field name
     * @return forward index iterator
     */
    ForwardIndexIteratorPtr forwardIndexReader(fieldid_t fieldId) const;


    /**
     * Return last modified time 
     */
    FX_NS(utility)::Timestamp lastModified();

    /** 
     * Return analyzer mapper
     * @return reference to analyzer maper
     */
    const FX_NS(analyzer)::AnalyzerMapper* getAnalyzerMapper() const;

    /**
     * Return document schema of index 
     */
    const FX_NS(document)::DocumentSchema* getDocSchema() const;

    /**
     * Return encoding of documents in the index database
     * @return encode name of documents in the index database
     */
    tstring getEncoding() const;

protected:
    void update(const FX_NS(common)::Notification& notif);

private:
    IndexBarrelKeeperPtr m_pKeeper;
    IndexBarrelKeeper::IndexBarrelPtr m_pBarrel;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(IndexReader);

//////////////////////////////////////////////////////////////////////////
//inline functions
inline const BarrelsInfo* IndexReader::getBarrelsInfo() const
{
    return m_pBarrel->getBarrelsInfo().get();
}

inline const FX_NS(analyzer)::AnalyzerMapper* 
IndexReader::getAnalyzerMapper() const
{
    return m_pKeeper->getAnalyzerMapper();
}

inline TermReaderPtr IndexReader::termReader() const
{
    const IndexBarrelReaderPtr& pReader = m_pBarrel->getReader();
    if (pReader.isNotNull())
    {
        return pReader->termReader();
    }
    return TermReaderPtr();
}

inline TermReaderPtr IndexReader::termReader(const tstring& sField) const
{
    const IndexBarrelReaderPtr& pReader = m_pBarrel->getReader();
    if (pReader.isNotNull())
    {
        return pReader->termReader(sField);
    }
    return TermReaderPtr();
}

inline LengthNormIteratorPtr IndexReader::lengthNorm(const tstring& sField) const
{
    const IndexBarrelReaderPtr& pReader = m_pBarrel->getReader();
    if (pReader.isNotNull())
    {
        return pReader->lengthNorm(sField);
    }
    return LengthNormIteratorPtr();
}

inline const FX_NS(document)::DocumentSchema* IndexReader::getDocSchema() const
{
    return m_pKeeper->getDocSchema();
}

inline tstring IndexReader::getEncoding() const
{
    BarrelsInfoPtr pBarrelsInfo = m_pBarrel->getBarrelsInfo();
    return pBarrelsInfo->getEncoding();
}

FX_NS_END

#endif
