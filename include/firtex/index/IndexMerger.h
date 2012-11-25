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
// Created	: 2005/12/09
//

#ifndef __FX_INDEXMERGER_H
#define __FX_INDEXMERGER_H

#include "firtex/common/SharedPtr.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/index/FieldMerger.h"
#include "firtex/index/DocIdRecycling.h"
#include "firtex/index/ForwardIndexMerger.h"
#include "firtex/index/MergePolicy.h"
#include "firtex/index/Committable.h"
#include "firtex/index/IndexBarrelKeeper.h"
#include "firtex/index/MergeScheduler.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/FileSystem.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/utility/PriorityQueue.h"
#include "firtex/utility/StringUtils.h"
#include <vector>

FX_NS_DEF(index);

class ComponentBuilder;
class IndexMerger : public Committable
{
public:
    const static size_t MAX_OPENED_FILES_PER_BARREL = 4;
    const static size_t DEFAULT_CONCURRENT_MERGE_THREADS = 4;

public:
    IndexMerger(IndexBarrelKeeper* pBarrelKeeper);
    IndexMerger(const MergePolicyPtr& pMergePolicy,
                IndexBarrelKeeper* pBarrelKeeper);
    virtual ~IndexMerger();

public:
    /**
     * Do merge indexes in the file system
     */
    virtual void merge();

    /**
     * Set merge policy
     */
    void setMergePolicy(const MergePolicyPtr& pMergePolicy);

public:
    virtual void commit();

public:
    /** 
     * Add an index barrel to merge
     * @param pIndexBarrelWriter in-memory index barrel to merge
     */
    void addToMerge(IndexBarrelWriterPtr& pIndexBarrelWriter);

protected:
    /// Initialize barrels info
    virtual BarrelsInfoPtr initBarrelsInfo();

    /// Initialize deleted document filter
    virtual DeletedDocumentFilterPtr initDeletionFilter(const BarrelsInfoPtr& pMergeBarrels);

    /// Initialize merge field info for index
    virtual void initIndexMergeInfo(IndexMergeInfosPtr& pIndexMergeInfos,
                                    const MergeInfos& mergeInfos,
                                    const FX_NS(document)::FieldSchema* pFieldSchema);
    virtual void initForwardIndexMergeInfos(ForwardIndexMergeInfosPtr& pFdMergeInfos,
            const MergeInfos&mergeInfos, const FX_NS(document)::FieldSchema* pFieldSchema);

    /// Check and split proposal if necessary
    virtual MergeProposalPtr checkAndSplitProposal(MergeProposalPtr& pMergeProp);

    /// Create merge proposals by merge policy
    virtual MergeProposalPtr createMergeProposals(const BarrelsInfo& barrelsInfo);

    /// Create merged barrels info
    virtual BarrelsInfoPtr createMergedBarrelsInfo(
            const BarrelsInfoPtr& pOldBarrelsInfo,
            const BarrelsInfoPtr& pMergeBarrels);

protected:
    void initMergers();
    void initMergeInfo(MergeInfos& mergeInfos,
                       BarrelInfo& mergedBarrel,
                       const BarrelsInfo& barrelsInfo,
                       const DeletedDocumentFilterPtr& docFilter);
    void mergeIndexMeta(const MergeInfos& mergeInfos,
                        IndexMeta& indexMeta);

protected:
    typedef std::list<FieldMergerPtr> IndexMergerList;
    typedef std::list<ForwardIndexMergerPtr> FdIndexMergerList;

protected:
    IndexBarrelKeeper* m_pKeeper;

    FX_NS(store)::InputStreamPoolPtr m_pStreamPool;

    ///For create merge proposals
    MergePolicyPtr m_pMergePolicy;
    
    ///For schedule merge task 
    MergeSchedulerPtr m_pMergeScheduler;

    IndexMergerList m_indexMerges;
    FdIndexMergerList m_fdIndexMerges;

    DocIdRecyclingPtr m_pDocIdRecycling;

    FX_NS(utility)::TimeProbePtr m_pTimeProbe;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(IndexMerger);

FX_NS_END

#endif
