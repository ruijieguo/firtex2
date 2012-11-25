//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2005/12/10
//

#ifndef __FX_FIELDINDEXER_H
#define __FX_FIELDINDEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/AnalyzedField.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/PostingPool.h"
#include "firtex/utility/Pool.h"

FX_NS_DEF(index);

class FieldIndexer
{
public:
    FieldIndexer(const FX_NS(document)::FieldSchema* pFieldSchema)
        : m_pFieldSchema(pFieldSchema)
    {
    }

    FieldIndexer()
        : m_pFieldSchema(NULL)
    {
    }

    virtual ~FieldIndexer()
    {
        m_pFieldSchema = NULL;
    }

public:
    /**
     * Initialize
     *
     * @param pPostinPool memory pool for posting
     */
    virtual void init(const PostingPoolPtr& pPostinPool) = 0;

    /**
     * Seal the index . After sealing, modification is forbidden
     */
    virtual void sealIndex() = 0;

    /**
     * Add a field to index
     * @param pField pField object
     */
    virtual void addField(const FX_NS(document)::AnalyzedField* pField) = 0;

    /**
     * Finalize a document
     * @param docId docid of current document
     */
    virtual void commitDocument(docid_t docId) = 0;

    /**
     * Finalize index and commit to file system
     *
     * @param pFileSys file system instance
     * @param sPrefix prefix of index file name
     */
    virtual void commit(FX_NS(store)::FileSystemPtr& pFileSys,
                        const tstring& sPrefix) = 0;

    /**
     * Finalize index and extract index meta
     *
     * @param fieldMeta field meta
     */
    virtual void commitMeta(FieldMeta& fieldMeta) = 0;


    /**
     * Return number of distinct terms
     */
    virtual uint64_t distinctNumTerms() const = 0;

    /**
     * Return total number of terms
     */
    virtual uint64_t totalTerms() const = 0;

    /**
     * Create term reader for searching in-memory index
     * @return term reader object maintained by caller
     */
    virtual TermReaderPtr termReader() const = 0;

    /**
     * Clear all resource
     */
    virtual void clear() = 0;

public:
    /**
     * Return field info of this indexer
     */
    const FX_NS(document)::FieldSchema* getFieldSchema() const 
    {
        return m_pFieldSchema;
    }

private:
    const FX_NS(document)::FieldSchema* m_pFieldSchema;
};

DEFINE_TYPED_PTR(FieldIndexer);

FX_NS_END

#endif
