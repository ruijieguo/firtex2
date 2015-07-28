//
// Copyright(C) 2005--2006 Guo Ruijie. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2009/3/14
//
#ifndef __PRIMARYKEYINDEXER_H
#define __PRIMARYKEYINDEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/FieldIndexer.h"
#include "firtex/utility/HashMap.h"
#include <vector>

FX_NS_DEF(index);

class PrimaryKeyIndexer : public FieldIndexer
{
public:
    typedef FX_NS(utility)::HashMap<uint64_t, docid_t> PostingTable;
    DEFINE_TYPED_PTR(PostingTable);

public:
    PrimaryKeyIndexer(const FX_NS(document)::FieldSchema* pFieldSchema);
    virtual ~PrimaryKeyIndexer(void);

public:
    /**
     * Initialize
     *
     * @param pPostinPool memory pool for posting
     */
    virtual void init(const PostingPoolPtr& pPostingPool);

    /**
     * Seal the index . After sealing, modification is forbidden
     */
    void sealIndex() {};

    /**
     * Add a field to index
     * @param pField pField object
     */
    void addField(const FX_NS(document)::AnalyzedField* pField);

    /**
     * Finalize a document
     * @param docId docid of current document
     */
    void commitDocument(docid_t docId);

    /**
     * Finalize index and commit to file system
     *
     * @param pFileSys file system instance
     * @param sSuffix suffix of index file name
     */
    void commit(FX_NS(store)::FileSystemPtr& pFileSys,
                const tstring& sSuffix);

    /**
     * Finalize index and extract index meta
     *
     * @param fieldMeta field meta
     */
    void commitMeta(FieldMeta& fieldMeta);

    /**
     * Return number of dstinct terms
     */
    uint64_t distinctNumTerms() const;

    /**
     * Return total number of terms
     */
    uint64_t totalTerms() const {return distinctNumTerms();}

    /**
     * Create term reader for searching in-memory index
     * @return term reader object maintained by caller
     */
    TermReaderPtr termReader() const;
    
    /// Return the internal posting table
    const PostingTablePtr& getPostingTable() const;

    /**
     * Clear all resource
     */
    void clear();

protected:
    void commitValue(docid_t did);

protected:
    PostingTablePtr m_pPostingTable;
    const FX_NS(document)::AnalyzedField* m_pCurField;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PrimaryKeyIndexer);

/////////////////////////////////////////////////////////
//inline function
inline const PrimaryKeyIndexer::PostingTablePtr&
PrimaryKeyIndexer::getPostingTable() const
{
    return m_pPostingTable;
}

FX_NS_END

#endif
