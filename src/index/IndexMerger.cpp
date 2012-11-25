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
// Created	: 2005/12/9
//
#include "firtex/index/IndexMerger.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/index/IndexWriter.h"
#include "firtex/index/StoredFieldsMerger.h"
#include "firtex/index/ForwardIndexMerger.h"
#include "firtex/index/LengthNormMerger.h"
#include "firtex/index/DeletedDocumentFilter.h"
#include "firtex/index/Index.h"
#include "firtex/index/OptimizeMergePolicy.h"
#include "firtex/index/SequenceMergeScheduler.h"
#include "firtex/index/ConcurrentMergeScheduler.h"
#include "firtex/index/NoMergePolicy.h"
#include "firtex/utility/BitVector.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/KeyValueParser.h"
#include <sstream>
#include <memory>

#define  MAX_BARREL_LEVEL 32

FX_NS_USE(document);
FX_NS_USE(store);
FX_NS_USE(utility);

FX_NS_DEF(index);

//////////////////////////////////////////////////////////////////////////
///IndexMerger
SETUP_STREAM_LOGGER(index, IndexMerger);

//////////////////////////////////////////////////////////////////////////
/// Define merge tasks
class IndexMergeTask : public Mergeable
{
public:
    IndexMergeTask(const std::string& sField,
                   const FieldMergerPtr& pMerger, 
                   const IndexMergeInfosPtr& pIndexMergeInfos,
                   FieldMeta& fieldMeta)
        : m_sField(sField)
        , m_pFieldMerger(pMerger)
        , m_pIndexMergeInfos(pIndexMergeInfos)
        , m_fieldMeta(fieldMeta)
    {}

    virtual void merge()
    {
        FX_LOG(INFO, "Begin merge index: [%s]", m_sField.c_str());
        m_pFieldMerger->merge(*m_pIndexMergeInfos, m_fieldMeta);
        FX_LOG(INFO, "End merge index: [%s]", m_sField.c_str());
    }

private:
    std::string m_sField;
    FieldMergerPtr m_pFieldMerger;
    IndexMergeInfosPtr m_pIndexMergeInfos;
    FieldMeta& m_fieldMeta;

private:
    DECLARE_LOGGER();
};
DEFINE_TYPED_PTR(IndexMergeTask);

SETUP_LOGGER(index, IndexMergeTask);

class ForwardIndexMergeTask : public Mergeable
{
public:
    ForwardIndexMergeTask(const std::string& sField,
                          ForwardIndexMergerPtr& pMerger,
                          const ForwardIndexMergeInfosPtr& pMergeInfos)
        : m_sField(sField)
        , m_pMerger(pMerger)
        , m_pMergeInfos(pMergeInfos)
    {}

    virtual void merge()
    {
        FX_LOG(INFO, "Begin merge forward index: [%s]", m_sField.c_str());
        m_pMerger->merge(*m_pMergeInfos);
        FX_LOG(INFO, "End merge forward index: [%s]", m_sField.c_str());
    }

private:
    std::string m_sField;
    ForwardIndexMergerPtr m_pMerger;
    ForwardIndexMergeInfosPtr m_pMergeInfos;

private:
    DECLARE_LOGGER();
};
DEFINE_TYPED_PTR(ForwardIndexMergeTask);
SETUP_LOGGER(index, ForwardIndexMergeTask);

class StoredFieldMergeTask : public Mergeable
{
public:
    StoredFieldMergeTask(StoredFieldsMergerPtr& pMerger,
                         const FX_NS(store)::FileSystemPtr& pFileSys,
                         const MergeInfos& mergeInfos)
        : m_pMerger(pMerger)
        , m_pFileSys(pFileSys)
        , m_mergeInfos(mergeInfos)
    {}

    virtual void merge()
    {
        FX_LOG(INFO, "Begin merge stored fields");
        m_pMerger->merge(m_pFileSys, m_mergeInfos);
        FX_LOG(INFO, "End merge stored fields");
    }

private:
    StoredFieldsMergerPtr m_pMerger;
    FX_NS(store)::FileSystemPtr m_pFileSys;
    const MergeInfos& m_mergeInfos;

private:
    DECLARE_LOGGER();
};
DEFINE_TYPED_PTR(StoredFieldMergeTask);
SETUP_LOGGER(index, StoredFieldMergeTask);

class LengthNormMergeTask : public Mergeable
{
public:
    LengthNormMergeTask(LengthNormMergerPtr& pMerger,
                        const FX_NS(store)::FileSystemPtr& pFileSys,
                        const MergeInfos& mergeInfos)
        : m_pMerger(pMerger)
        , m_pFileSys(pFileSys)
        , m_mergeInfos(mergeInfos)
    {}

    virtual void merge()
    {
        FX_LOG(INFO, "Begin merge length norm");
        m_pMerger->merge(m_pFileSys, m_mergeInfos);
        FX_LOG(INFO, "End merge length norm");
    }

private:
    LengthNormMergerPtr m_pMerger;
    FX_NS(store)::FileSystemPtr m_pFileSys;
    const MergeInfos& m_mergeInfos;

private:
    DECLARE_LOGGER();
};
DEFINE_TYPED_PTR(LengthNormMergeTask);
SETUP_LOGGER(index, LengthNormMergeTask);

class DocFilterMergeTask : public Mergeable
{
public:
    DocFilterMergeTask(DeletedDocumentFilterPtr& pMerger,
                       const MergeInfos& mergeInfos)
        : m_pMerger(pMerger)
        , m_mergeInfos(mergeInfos)
    {}

    virtual void merge()
    {
        FX_LOG(INFO, "Begin merge deleted documents filter");
        m_pMerger->merge(m_mergeInfos);
        FX_LOG(INFO, "End merge deleted documents filter");
    }
    
private:
    DeletedDocumentFilterPtr m_pMerger;
    const MergeInfos& m_mergeInfos;

private:
    DECLARE_LOGGER();
};
DEFINE_TYPED_PTR(DocFilterMergeTask);
SETUP_LOGGER(index, DocFilterMergeTask);

////////////////////////////////////////////////////////
///Define IndexMerge

IndexMerger::IndexMerger(IndexBarrelKeeper* pBarrelKeeper)
    : m_pKeeper(pBarrelKeeper)
{
    m_pMergePolicy = new NoMergePolicy();
    m_pMergeScheduler = new ConcurrentMergeScheduler(DEFAULT_CONCURRENT_MERGE_THREADS);
    m_pTimeProbe = new TimeProbe;
}

IndexMerger::IndexMerger(const MergePolicyPtr& pMergePolicy, 
                         IndexBarrelKeeper* pBarrelKeeper)
    : m_pKeeper(pBarrelKeeper)
{
    m_pMergePolicy = pMergePolicy;
    m_pMergeScheduler = new ConcurrentMergeScheduler(4);
}

IndexMerger::~IndexMerger()
{
}

void IndexMerger::initMergers()
{
    const DocumentSchema* pDocSchema = m_pKeeper->getDocSchema();
    const ComponentBuilder* pCompBuilder = m_pKeeper->getComponentBuilder();
    const FileSystemPtr pFileSys = m_pKeeper->getFileSystem();

    m_indexMerges.clear();
    m_fdIndexMerges.clear();

    DocumentSchema::Iterator it = pDocSchema->iterator();
    while (it.hasNext())
    {
        const FieldSchema* pFieldSchema = it.next();
        fieldid_t fieldId = pFieldSchema->getId();
        if (pFieldSchema->isIndexed())
        {
            FieldMergerPtr pFieldMerger =
                pCompBuilder->buildMerger(fieldId);
            FIRTEX_ASSERT2(pFieldMerger != NULL);
            
            pFieldMerger->init(pFileSys, pFieldSchema);
            m_indexMerges.push_back(pFieldMerger);
        }
        if (pFieldSchema->hasForwardIndex())
        {
            ForwardIndexMergerPtr pFDIndexMerger =
                pCompBuilder->buildForwardIndexMerger(pFieldSchema->getId());
            FIRTEX_ASSERT2(pFDIndexMerger != NULL);

            pFDIndexMerger->init(pFileSys, pFieldSchema);
            m_fdIndexMerges.push_back(pFDIndexMerger);
        }
    }
}

void IndexMerger::merge()
{
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();

    for (;;)
    {
        BarrelsInfoPtr pBarrelsInfo = initBarrelsInfo();
        if (pBarrelsInfo.isNull() || pBarrelsInfo->getBarrelCount() <= 1)
        {
            /// Clean staled index data if possible
            m_pKeeper->cleanStaledIndex();
            return ;
        }

        MergeProposalPtr pMergeProp = createMergeProposals(*pBarrelsInfo);
        if (pMergeProp.isNull())
        {
            /// Clean staled index data if possible
            m_pKeeper->cleanStaledIndex();
            return ;
        }
        pMergeProp = checkAndSplitProposal(pMergeProp);

        BarrelsInfoPtr pOrgBarrelsInfo = pBarrelsInfo;
        for (size_t i = 0; i < pMergeProp->size(); ++i)
        {
            initMergers();

            const BarrelsInfoPtr& pMergeBarrels = (*pMergeProp)[i];

            BarrelsInfoPtr pMergedBarrelsInfo = 
                createMergedBarrelsInfo(pOrgBarrelsInfo, pMergeBarrels);

            BarrelInfo& newBarrel = pMergedBarrelsInfo->newBarrel();
            std::string sSuffix = newBarrel.getSuffix();
            BarrelDirectory::createBarrel(pFileSys, sSuffix);

            FX_LOG(INFO, "Begin merge, result barrel: [%s]", sSuffix.c_str());

            //init deleted document fitler merger
            DeletedDocumentFilterPtr pDocFilter = initDeletionFilter(pMergeBarrels);

            MergeInfos mergeInfos(sSuffix);
            initMergeInfo(mergeInfos, newBarrel, *pMergeBarrels, pDocFilter);

            //merge index
            {
                IndexMeta& indexMeta = newBarrel.getIndexMeta();
                for (IndexMergerList::iterator it = m_indexMerges.begin();
                     it != m_indexMerges.end(); ++it)
                {
                    const FieldSchema* pFieldSchema = (*it)->getFieldSchema();
                    IndexMergeInfosPtr pIndexMergeInfos(new IndexMergeInfos(sSuffix));
                    initIndexMergeInfo(pIndexMergeInfos, mergeInfos, pFieldSchema);

                    FieldMeta& fieldMeta = indexMeta.fieldMeta(pFieldSchema->getName());
                
                    IndexMergeTaskPtr pIndexMergeTask = new IndexMergeTask(
                            pFieldSchema->getName(), *it, pIndexMergeInfos, fieldMeta);
                    m_pMergeScheduler->merge(pIndexMergeTask.cast<Mergeable>());
                }
            }

            //merge forward index
            {
                for (FdIndexMergerList::iterator it = m_fdIndexMerges.begin();
                     it != m_fdIndexMerges.end(); ++it)
                {
                    const FieldSchema* pFieldSchema = (*it)->getFieldSchema();
                    ForwardIndexMergeInfosPtr pFdMergeInfos(new ForwardIndexMergeInfos(sSuffix));
                    initForwardIndexMergeInfos(pFdMergeInfos, mergeInfos, pFieldSchema);
                    ForwardIndexMergeTaskPtr pFdMergeTask = new ForwardIndexMergeTask(
                            (*it)->getFieldSchema()->getName(), *it, pFdMergeInfos);
                    m_pMergeScheduler->merge(pFdMergeTask.cast<Mergeable>());
                }
            }

            //merge stored fields
            {
                StoredFieldsMergerPtr pStoredMerger = new StoredFieldsMerger();
                StoredFieldMergeTaskPtr pStoredMergeTask = new StoredFieldMergeTask(
                        pStoredMerger, pFileSys, mergeInfos);
                m_pMergeScheduler->merge(pStoredMergeTask.cast<Mergeable>());
            }

            //merge length norm
            {
                const DocumentSchema* pDocSchema = m_pKeeper->getDocSchema();
                LengthNormMergerPtr pMerger = new LengthNormMerger();
                pMerger->init(pDocSchema);
                LengthNormMergeTaskPtr pLenTask = new LengthNormMergeTask(pMerger, pFileSys, mergeInfos);
                m_pMergeScheduler->merge(pLenTask.cast<Mergeable>());
            }

            m_pMergeScheduler->waitFinish();

            //merge deletion filter
            DocFilterMergeTaskPtr pDocFilterTask = new DocFilterMergeTask(pDocFilter, mergeInfos);
            m_pMergeScheduler->merge(pDocFilterTask.cast<Mergeable>());
            m_pMergeScheduler->waitFinish();

            IndexMeta& indexMeta = newBarrel.getIndexMeta();
            mergeIndexMeta(mergeInfos, indexMeta);
            
            pMergedBarrelsInfo->setEncoding(m_pKeeper->getEncoding());
            pMergedBarrelsInfo->write(pFileSys);

            FX_LOG(INFO, "End merge, result barrel: [%s]", sSuffix.c_str());

            pOrgBarrelsInfo = pMergedBarrelsInfo;
        }

        pBarrelsInfo.reset();
    }// end for
}

MergeProposalPtr IndexMerger::checkAndSplitProposal(MergeProposalPtr& pMergeProp)
{
    size_t nMaxAllowedOpenFiles = (size_t)(GLOBAL_CONF().Merge.maxAllowedOpenFiles);
    ///Exclude files opened for write
    nMaxAllowedOpenFiles -= MAX_OPENED_FILES_PER_BARREL;
    size_t nMaxAllowedBarrels = nMaxAllowedOpenFiles/MAX_OPENED_FILES_PER_BARREL;
    
    MergeProposalPtr pNewProprosal = new MergeProposal();
    for (size_t i = 0; i < pMergeProp->size(); ++i)
    {
        const BarrelsInfoPtr& pMergeBarrels = (*pMergeProp)[i];
        size_t nBarrelCount = pMergeBarrels->getBarrelCount();
        size_t nSplitCount = (nBarrelCount + nMaxAllowedBarrels - 1)/nMaxAllowedBarrels;
        if (nSplitCount == 1)
        {
            pNewProprosal->push_back(pMergeBarrels);
            continue;
        }
        
        FX_LOG(INFO, "Split merge proposal [%u], Barrel count: [%u], max allowed open files: [%u]",
               (uint32_t)i, (uint32_t)nBarrelCount, (uint32_t)GLOBAL_CONF().Merge.maxAllowedOpenFiles);

        for (size_t j = 0; j < nBarrelCount; )
        {
            BarrelsInfoPtr pSplitBarrel = new BarrelsInfo;
            for (size_t k = 0; j < nBarrelCount && k < nBarrelCount/nSplitCount; ++j, ++k)
            {
                pSplitBarrel->addBarrel((*pMergeBarrels)[j]);
            }
            FX_LOG(INFO, "Split barrel count: [%u]", (uint32_t)pSplitBarrel->getBarrelCount());
            pNewProprosal->push_back(pSplitBarrel);
        }
    }
    return pNewProprosal;
}

void IndexMerger::initMergeInfo(MergeInfos& mergeInfos,
                                BarrelInfo& mergedBarrel,
                                const BarrelsInfo& barrelsInfo,
                                const DeletedDocumentFilterPtr& pDocFilter)
{
    df_t mergedDocCount = 0;
    df_t mergedDelDocCount = 0;
    docid_t newBaseDocId = 0;
    mergeInfos.reserve(barrelsInfo.getBarrelCount());
    for (size_t j = 0; j < barrelsInfo.getBarrelCount(); ++j)
    {
        const BarrelInfo& barrelInfo = barrelsInfo[j];

        const BitVector* pFilter = (*pDocFilter)[j];
        df_t docCount = barrelInfo.getDocCount();
        
        DocIdRecyclingPtr pDocIdRecy = new DocIdRecycling();
        pDocIdRecy->init(docCount, pFilter);
        
        mergedDocCount += pDocIdRecy->getDocCountAfterRecle();

        MergeInfo mergeInfo(barrelInfo, pDocIdRecy, newBaseDocId);
        mergeInfos.pushBack(mergeInfo);

        newBaseDocId += docCount;
        if (pFilter && pFilter->any())
        {
            newBaseDocId -= (docid_t)pFilter->count();
        }
    }
    mergedBarrel.setDocCount(mergedDocCount);
    mergedBarrel.setDeletedDocs(mergedDelDocCount);
}

void IndexMerger::initIndexMergeInfo(IndexMergeInfosPtr& pIndexMergeInfos,
                                     const MergeInfos& mergeInfos,
                                     const FieldSchema* pFieldSchema)
{
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    m_pStreamPool = new InputStreamPool(pFileSys);
    const ComponentBuilder* pCompBuilder = m_pKeeper->getComponentBuilder();

    pIndexMergeInfos->reserve(mergeInfos.size());
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        const BarrelInfo& barrelInfo = mergeInfo.getBarrelInfo();

        TermReaderPtr pTermReader;
        if (pFieldSchema->isIndexed() && (barrelInfo.getDocCount() > 0))
        {
            const FieldComponentCreator* pFieldCompCreator = 
                pCompBuilder->getFieldComponentCreator(pFieldSchema->getId());
            pTermReader = pFieldCompCreator->createTermReader();
            pTermReader->open(pFileSys, m_pStreamPool, barrelInfo.getSuffix(),
                    pFieldSchema, NULL);
            IndexMergeInfo mergeFieldInfo(mergeInfo, pTermReader);
            pIndexMergeInfos->pushBack(mergeFieldInfo);
        }
    }
}

void IndexMerger::initForwardIndexMergeInfos(ForwardIndexMergeInfosPtr& pFdMergeInfos,
        const MergeInfos&mergeInfos, const FieldSchema* pFieldSchema)
{
    pFdMergeInfos->reserve(mergeInfos.size());
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        const BarrelInfo& barrelInfo = mergeInfo.getBarrelInfo();

        TermReaderPtr pTermReader;
        if (pFieldSchema->hasForwardIndex())
        {
            if (barrelInfo.getDocCount() > 0)
            {
                ForwardIndexMergeInfo mergeFieldInfo(mergeInfo);
                pFdMergeInfos->pushBack(mergeFieldInfo);
            }
        }
    }
}

BarrelsInfoPtr IndexMerger::createMergedBarrelsInfo(
        const BarrelsInfoPtr& pOldBarrelsInfo,
        const BarrelsInfoPtr& pMergeBarrels)
{
    BarrelsInfoPtr pNewBarrelsInfo = new BarrelsInfo();
    pNewBarrelsInfo->setCommitId(pOldBarrelsInfo->getCommitId());
    BarrelsInfo::Iterator it = pOldBarrelsInfo->iterator();
    while (it.hasNext())
    {
        const BarrelInfo& barrelInfo = it.next();
        if (!pMergeBarrels->hasBarrel(barrelInfo.getCommitId()))
        {
            pNewBarrelsInfo->addBarrel(barrelInfo);
        }
    }
    return pNewBarrelsInfo;
}

void IndexMerger::addToMerge(IndexBarrelWriterPtr& pIndexBarrelWriter)
{
    //TODO: optimize: merge the in-memory index barrel directly.
    pIndexBarrelWriter->commit();
    merge();
}

void IndexMerger::mergeIndexMeta(const MergeInfos& mergeInfos,
                                 IndexMeta& indexMeta)
{
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const IndexMergeInfo& mergeInfo = mergeInfos[i];
        const IndexMeta& meta = mergeInfo.getBarrelInfo().getIndexMeta();
        for (size_t j = 0; j < meta.size(); ++j)
        {
            const FieldMeta& fieldMeta1 = meta[j];
            FieldMeta& fieldMeta2 = indexMeta.fieldMeta(fieldMeta1.fieldName);
            fieldMeta2.totalTermCount += fieldMeta1.totalTermCount;
        }
    }
}

void IndexMerger::setMergePolicy(const MergePolicyPtr& pMergePolicy)
{
    m_pMergePolicy.assign(pMergePolicy);
}

void IndexMerger::commit() 
{
    merge();
}

BarrelsInfoPtr IndexMerger::initBarrelsInfo()
{
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    BarrelsInfoPtr pBarrelsInfo = new BarrelsInfo();
    pBarrelsInfo->read(pFileSys);
    return pBarrelsInfo;
}

DeletedDocumentFilterPtr IndexMerger::initDeletionFilter(const BarrelsInfoPtr& pMergeBarrels)
{
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    DeletedDocumentFilterPtr pDocFilter(new DeletedDocumentFilter(pFileSys));
    pDocFilter->open(pMergeBarrels);
    return pDocFilter;
}

MergeProposalPtr IndexMerger::createMergeProposals(const BarrelsInfo& barrelsInfo)
{
    return m_pMergePolicy->createProposals(barrelsInfo);
}

FX_NS_END

