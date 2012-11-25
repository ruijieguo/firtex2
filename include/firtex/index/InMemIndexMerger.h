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
// Created : 2011-07-14 20:12:07

#ifndef __FX_INMEMINDEXMERGER_H
#define __FX_INMEMINDEXMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexMerger.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/InMemIndexBarrelReader.h"

FX_NS_DEF(index);

class InMemIndexMerger : public IndexMerger
{
public:
    InMemIndexMerger(IndexBarrelKeeper* pBarrelKeeper);
    ~InMemIndexMerger();

public:
    /**
     * Do merge in-memory indexes
     */
    virtual void merge();

    /** 
     * Add an in-memory index barrel to merge
     *
     * @param barrelInfo barrel info
     * @param pIndexBarrelWriter in-memory index barrel to merge
     */
    void addToMerge(const BarrelInfo& barrelInfo,
                    const IndexBarrelWriterPtr& pIndexBarrelWriter);

    /// Return number of in-memory barrel count
    size_t getInMemBarrelCount() const;

protected:
    /// Initialize barrels info
    virtual BarrelsInfoPtr initBarrelsInfo();

    /// Initialize deleted document filter
    virtual DeletedDocumentFilterPtr initDeletionFilter();

    /// Initialize merge field info for index
    virtual void initIndexMergeInfo(IndexMergeInfosPtr& pIndexMergeInfos,
                                    const MergeInfos& mergeInfos,
                                    const FX_NS(document)::FieldSchema* pFieldSchema);

    void initForwardIndexMergeInfos(ForwardIndexMergeInfosPtr& pFdMergeInfos,
                                    const MergeInfos&mergeInfos, 
                                    const FX_NS(document)::FieldSchema* pFieldSchema);

    /// Check and split proposal if necessary
    virtual MergeProposalPtr checkAndSplitProposal(MergeProposalPtr& pMergeProp);

    /// Create merge proposals by merge policy
    virtual MergeProposalPtr createMergeProposals(const BarrelsInfo& barrelsInfo);

    virtual BarrelsInfoPtr createMergedBarrelsInfo(
            const BarrelsInfoPtr& pOldBarrelsInfo,
            const BarrelsInfoPtr& pMergeBarrels);

private:
    typedef std::list<IndexBarrelWriterPtr> BarrelWriterList;

    typedef std::map<commitid_t, InMemIndexBarrelReaderPtr> InMemReaderMap;

    /// For holding IndexBarrelWriter
    BarrelWriterList m_writerHolder;

    /// Hold all barrels including in-memory barrels
    BarrelsInfoPtr m_pInMemBarrelsInfo;

    /// In-memory index barrel reader
    InMemReaderMap m_readers;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(InMemIndexMerger);

//////////////////////////////////////////////////////////////////////
///
inline size_t InMemIndexMerger::getInMemBarrelCount() const
{
    return m_pInMemBarrelsInfo->getBarrelCount();
}

FX_NS_END

#endif //__FX_INMEMINDEXMERGER_H
