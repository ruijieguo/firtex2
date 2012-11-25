#include "firtex/index/InMemIndexMerger.h"
#include "firtex/index/OptimizeMergePolicy.h"

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, InMemIndexMerger);

InMemIndexMerger::InMemIndexMerger(IndexBarrelKeeper* pBarrelKeeper) 
    : IndexMerger(pBarrelKeeper)
{
    m_pMergePolicy = new OptimizeMergePolicy();
}

InMemIndexMerger::~InMemIndexMerger() 
{
}

void InMemIndexMerger::merge()
{
    if (m_writerHolder.size() > 1)
    {
        IndexMerger::merge();
    }
    else if (m_writerHolder.size() == 1) 
    {
        m_readers.clear();
        m_writerHolder.front()->commit();
    }
    
    m_pInMemBarrelsInfo.reset();
    m_writerHolder.clear();
    m_readers.clear();
}

void InMemIndexMerger::addToMerge(const BarrelInfo& barrelInfo, 
                                  const IndexBarrelWriterPtr& pIndexBarrelWriter)
{
    if (m_pInMemBarrelsInfo.isNull())
    {
        m_pInMemBarrelsInfo = new BarrelsInfo();
    }

    FX_LOG(INFO, "Add in-memory barrel to merge: commit: [%d], "
            "baseDocId: [%d], docCount: [%d]", barrelInfo.getCommitId(),
            barrelInfo.getBaseDocId(), barrelInfo.getDocCount());

    if (m_pInMemBarrelsInfo->getBarrelCount() > 0)
    {
        const BarrelInfo& lastBarrel = m_pInMemBarrelsInfo->getLastBarrel();
        assert(lastBarrel.getBaseDocId() + lastBarrel.getDocCount() == barrelInfo.getBaseDocId());
        (void)lastBarrel;
    }

    IndexBarrelReader* pReader = pIndexBarrelWriter->inMemoryReader();
    InMemIndexBarrelReader* pInMemReader = dynamic_cast<InMemIndexBarrelReader*>(pReader);
    FIRTEX_ASSERT2(pInMemReader);
    InMemIndexBarrelReaderPtr pInMemReaderPtr(pInMemReader);

    m_pInMemBarrelsInfo->addBarrel(barrelInfo);
    m_readers.insert(make_pair(barrelInfo.getCommitId(), pInMemReaderPtr));

    m_writerHolder.push_back(pIndexBarrelWriter);
}

BarrelsInfoPtr InMemIndexMerger::initBarrelsInfo()
{
    BarrelsInfoPtr pInMemBarrelsInfo = m_pInMemBarrelsInfo;
    m_pInMemBarrelsInfo.reset();
    return pInMemBarrelsInfo;
}

DeletedDocumentFilterPtr InMemIndexMerger::initDeletionFilter()
{
    DeletedDocumentFilterPtr pDocFilter = new DeletedDocumentFilter(m_pKeeper->getFileSystem());
    for (InMemReaderMap::const_iterator it = m_readers.begin();
         it != m_readers.end(); ++it)
    {
        pDocFilter->appendBarrel(*(m_pInMemBarrelsInfo->getBarrel(it->first)),
                const_cast<BitVector*>(it->second->getDocFilter()));
    }
    return pDocFilter;
}

void InMemIndexMerger::initIndexMergeInfo(IndexMergeInfosPtr& pIndexMergeInfos,
        const MergeInfos& mergeInfos, const FieldSchema* pFieldSchema)
{
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();

    pIndexMergeInfos->reserve(mergeInfos.size());
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        const BarrelInfo& barrelInfo = mergeInfo.getBarrelInfo();

        TermReaderPtr pTermReader;
        if (pFieldSchema->isIndexed() && (barrelInfo.getDocCount() > 0))
        {
            commitid_t commId = barrelInfo.getCommitId();
            InMemReaderMap::iterator it = m_readers.find(commId);
            FIRTEX_ASSERT2(it != m_readers.end());

            InMemIndexBarrelReaderPtr& pInMemReader = it->second;
            TermReaderPtr pTermReader =pInMemReader->termReader(pFieldSchema->getName());
            IndexMergeInfo mergeFieldInfo(mergeInfo, pTermReader);
            pIndexMergeInfos->pushBack(mergeFieldInfo);
        }
    }
}

void InMemIndexMerger::initForwardIndexMergeInfos(ForwardIndexMergeInfosPtr& pFdMergeInfos,
        const MergeInfos&mergeInfos, const FieldSchema* pFieldSchema)
{
    pFdMergeInfos->reserve(mergeInfos.size());
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        const BarrelInfo& barrelInfo = mergeInfo.getBarrelInfo();

        TermReaderPtr pTermReader;
        if (pFieldSchema->hasForwardIndex() && (barrelInfo.getDocCount() > 0))
        {
            commitid_t commId = barrelInfo.getCommitId();
            InMemReaderMap::iterator it = m_readers.find(commId);
            FIRTEX_ASSERT2(it != m_readers.end());

            InMemIndexBarrelReaderPtr& pInMemReader = it->second;

            ForwardIndexMergeInfo mergeFieldInfo(mergeInfo, 
                    pInMemReader->forwardIndexReader(pFieldSchema->getId()));
                pFdMergeInfos->pushBack(mergeFieldInfo);
        }
    }
}

MergeProposalPtr InMemIndexMerger::checkAndSplitProposal(MergeProposalPtr& pMergeProp)
{
    return pMergeProp;
}

MergeProposalPtr InMemIndexMerger::createMergeProposals(const BarrelsInfo& barrelsInfo)
{
    return m_pMergePolicy->createProposals(barrelsInfo);
}

BarrelsInfoPtr InMemIndexMerger::createMergedBarrelsInfo(
        const BarrelsInfoPtr& pOldBarrelsInfo,
        const BarrelsInfoPtr& pMergeBarrels)
{
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    BarrelsInfoPtr pNewBarrelsInfo = new BarrelsInfo();
    pNewBarrelsInfo->read(pFileSys);
    return pNewBarrelsInfo;
}

FX_NS_END

