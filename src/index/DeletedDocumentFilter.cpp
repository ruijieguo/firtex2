#include "firtex/index/DeletedDocumentFilter.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/NumberFormatter.h"

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DeletedDocumentFilter);

DeletedDocumentFilter::DeletedDocumentFilter(const FileSystemPtr& pFileSys)
    : m_pFileSys(pFileSys)
    , m_maxDocId(INVALID_DOCID)
    , m_bSealed(false)
{
}

DeletedDocumentFilter::DeletedDocumentFilter(const DeletedDocumentFilter& src)
    : m_pFileSys(src.m_pFileSys)
    , m_pBarrelsInfo(src.m_pBarrelsInfo)
    , m_maxDocId(src.m_maxDocId)
    , m_bSealed(false)
{
    m_filters.reserve(src.m_filters.size());
    for (size_t i = 0; i < src.m_filters.size(); ++i)
    {
        m_filters.push_back(BarrelFilter(src.m_filters[i]));
    }
}

DeletedDocumentFilter::~DeletedDocumentFilter()
{
    close();
}

DeletedDocumentFilter* DeletedDocumentFilter::clone() const
{
    return new DeletedDocumentFilter(*this);
}

void DeletedDocumentFilter::open(const BarrelsInfoPtr& pBarrelsInfo)
{
    FIRTEX_ASSERT2(!m_pBarrelsInfo && m_filters.size() == 0);

    m_pBarrelsInfo.reset(pBarrelsInfo->clone());

    BarrelsInfo::Iterator iter = m_pBarrelsInfo->iterator();
    while (iter.hasNext())
    {
        const BarrelInfo& barrelInfo = iter.next();
        BitVectorPtr pFilter;
        loadLatestFilter(pFilter, barrelInfo,
                         barrelInfo.getCommitId(),
                         m_pBarrelsInfo->getCommitId());
        if (!pFilter)
        {
            pFilter.reset(new BitVector());
        }
        BarrelFilter barrelFilter(&barrelInfo, pFilter);
        m_filters.push_back(barrelFilter);
        m_maxDocId = barrelInfo.getBaseDocId() + barrelInfo.getDocCount() - 1;
    }
}

void DeletedDocumentFilter::reopen(const BarrelsInfoPtr& pBarrelsInfo)
{
    FIRTEX_ASSERT2(m_pBarrelsInfo);

    BarrelsInfoPtr pNewBarrelsInfo( pBarrelsInfo->clone());
    BarrelsInfo::Iterator oldIter = m_pBarrelsInfo->iterator();
    BarrelsInfo::Iterator newIter = pNewBarrelsInfo->iterator();
    
    FilterVector newFilters;
    size_t cursor = 0;
    while (newIter.hasNext())
    {
        const BarrelInfo& barrelInfo = newIter.next();
        BitVectorPtr pFilter;
        loadLatestFilter(pFilter, barrelInfo,
                         m_pBarrelsInfo->getCommitId() + 1,
                         pNewBarrelsInfo->getCommitId());

        if (pFilter)
        {
            BarrelFilter barrelFilter(&barrelInfo, pFilter);
            newFilters.push_back(barrelFilter);
            continue;
        }
        if (oldIter.hasNext())
        {
            const BarrelInfo& oldInfo = oldIter.next();
            if (oldInfo.getCommitId() == barrelInfo.getCommitId())
            {
                BarrelFilter barrelFilter(&barrelInfo,
                        m_filters[cursor].m_pFilter);
                newFilters.push_back(barrelFilter);
                ++cursor;
                continue;
            }
            ++cursor;
        }
        //insert an empty filter
        pFilter.reset(new BitVector());
        BarrelFilter barrelFilter(&barrelInfo, pFilter);
        newFilters.push_back(barrelFilter);
    }
    m_filters.swap(newFilters);

    m_pBarrelsInfo = pNewBarrelsInfo;
}

void DeletedDocumentFilter::loadLatestFilter(BitVectorPtr& pDocFilter,
        const BarrelInfo& barrelInfo, commitid_t beginCommitId,
        commitid_t endCommitId)
{
    for (commitid_t id = endCommitId; id >= beginCommitId; --id)
    {
        if (id < barrelInfo.getCommitId())
        {
            continue;
        }
        std::string sFilterFile = getDeletionFileName(barrelInfo, id);
        if (m_pFileSys->fileExists(sFilterFile))
        {
            InputStreamPtr pInStream = m_pFileSys->openFile(sFilterFile);
            pDocFilter.reset(new BitVector());
            pDocFilter->read(pInStream);
            break;
        }
    }
}

void DeletedDocumentFilter::appendBarrel(const BarrelInfo& barrelInfo,
        const BitVectorPtr& pDocFilter)
{
    docid_t expBaseDocId = (m_pBarrelsInfo->getBarrelCount() == 0) ? 0 
                           : (m_pBarrelsInfo->getLastBarrel().getBaseDocId() + 
                                   m_pBarrelsInfo->getLastBarrel().getDocCount());
    if (expBaseDocId != barrelInfo.getBaseDocId())
    {
        FIRTEX_THROW(InconsisitentException, "Barrel is inconsisitent");
    }

    BarrelInfo& newBarrel = m_pBarrelsInfo->newBarrel();
    newBarrel = barrelInfo;
    if (pDocFilter && pDocFilter->any())
    {
        m_filters.push_back(BarrelFilter(&newBarrel, pDocFilter, true));
    }
    else
    {
        m_filters.push_back(BarrelFilter(&newBarrel, pDocFilter, false));
    }
    m_maxDocId = newBarrel.getBaseDocId() + newBarrel.getDocCount() - 1;
}

bool DeletedDocumentFilter::deleteDocument(docid_t docId)
{
    FilterVector::iterator it = m_filters.begin();
    while (it != m_filters.end())
    {
        const BarrelInfo* pBarrelInfo = (*it).m_pBarrelInfo;
        if( (docId < pBarrelInfo->getBaseDocId()) ||
           (docId >= (pBarrelInfo->getBaseDocId() + pBarrelInfo->getDocCount())) )
        {
            ++it;
        }
        else 
        {
            BitVectorPtr& pFilter = (*it).m_pFilter;
            if (!pFilter)
            {
                pFilter.reset(new BitVector());
            }
            if (!pFilter->test(docId - pBarrelInfo->getBaseDocId()))
            {
                FX_TRACE("Delete document: global: [%d], local: [%d], this: [%p]",
                        docId, docId - pBarrelInfo->getBaseDocId(), this); 
                pFilter->set(docId - pBarrelInfo->getBaseDocId());
                (*it).m_bDirty = true;
                return true;
            }
        }
    }
    
    FX_TRACE("Delete document: global: [%d] FAILED.", docId);
    return false;
}

void DeletedDocumentFilter::close()
{
    if (isDirty())
    {
        FX_LOG(WARN, "The document deletion has changed, "
               "but not committed, this: [%p]", this);
    }
    m_filters.clear();
}

void DeletedDocumentFilter::commit(const BarrelInfo& barrelInfo)
{
    if (m_filters.empty() || barrelInfo.getCommitId() < m_filters.rbegin()->m_pBarrelInfo->getCommitId())
    {
        return;
    }

    for (FilterVector::iterator it = m_filters.begin();
         it != m_filters.end(); ++it)
    {
        if ((*it).m_bDirty)
        {
            const BarrelInfo* pBarrelInfo = (*it).m_pBarrelInfo;
            BitVectorPtr& pFilter = (*it).m_pFilter;
            std::string sBarrel = getDeletionFileName(*pBarrelInfo, barrelInfo.getCommitId());
            pFilter->write(m_pFileSys, sBarrel);
        }
        (*it).m_bDirty = false;
    }
}

void DeletedDocumentFilter::undeleteAll()
{
    for (FilterVector::iterator it = m_filters.begin();
         it != m_filters.end(); ++it)
    {
        const BarrelInfo* pBarrelInfo = (*it).m_pBarrelInfo;
        std::string sBarrel = BarrelDirectory::getFilePath(DELETION_FILENAME,
                pBarrelInfo->getSuffix());
        if (m_pFileSys->fileExists(sBarrel))
        {
            m_pFileSys->deleteFile(sBarrel);
            (*it).m_bDirty = true;
        }
        BitVectorPtr& pFilter = (*it).m_pFilter;
        if (pFilter)
        {
            pFilter->reset();
            (*it).m_bDirty = true;
        }
    }
}

bool DeletedDocumentFilter::isDeleted(docid_t docId) const
{
    FilterVector::const_iterator it = m_filters.begin();
    while (it != m_filters.end())
    {
        const BarrelInfo* pBarrelInfo = (*it).m_pBarrelInfo;
        if((docId < pBarrelInfo->getBaseDocId()) ||
           (docId >= (pBarrelInfo->getBaseDocId() + pBarrelInfo->getDocCount())))
        {
            ++it;
        }
        else 
        {
            const BitVectorPtr& pFilter = (*it).m_pFilter;
             if (pFilter)
             {
                 return false;
             }
             if (pFilter->test(docId - pBarrelInfo->getBaseDocId()))
             {
                 return true;
             }
             return false;
         }
     }
     return false;
 }

 bool DeletedDocumentFilter::hasDeletions() const
 {
     FilterVector::const_iterator it = m_filters.begin();
     while (it != m_filters.end())
     {
         const BitVectorPtr& pFilter = (*it).m_pFilter;
         if (pFilter && pFilter->any())
         {
             return true;
         }
    }
    return false;
}

df_t DeletedDocumentFilter::getDeletedDocCount() const
{
    df_t nNumDelDocs = 0;
    FilterVector::const_iterator it = m_filters.begin();
    for (; it != m_filters.end(); ++it)
    {
        const BitVectorPtr& pFilter = (*it).m_pFilter;
        if (pFilter)
        {
            nNumDelDocs += (df_t)pFilter->count();
        }
    }
    return nNumDelDocs;
}

const BitVector* DeletedDocumentFilter::getDocFilter(docid_t docId) const
{
    FilterVector::const_iterator it = m_filters.begin();
    while (it != m_filters.end())
    {
        const BarrelInfo* pBarrelInfo = (*it).m_pBarrelInfo;
        docid_t baseDocId = pBarrelInfo->getBaseDocId();

        if ((docId >= baseDocId) &&
            (docId < (baseDocId + pBarrelInfo->getDocCount())))
        {
            return (*it).m_pFilter.get();
        }
        ++it;
    }
    return NULL;
}

void DeletedDocumentFilter::merge(const MergeInfos& mergeInfos)
{
    FIRTEX_ASSERT2(mergeInfos.size() == m_filters.size());
    BitVector mergedFilter;
    size_t length = 0;
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        const DocIdRecycling* pDocIdRecy = mergeInfo.getDocIdRecycling();
        if (pDocIdRecy->hasDeletions())
        {
            const BitVector& filter = *(m_filters[i].m_pFilter);
            size_t count = filter.length();
            for (size_t j = 0; j < count; ++j)
            {
                if (filter.test(j) &&
                    pDocIdRecy->remap((docid_t)j) != INVALID_DOCID)
                {
                    mergedFilter.set((size_t)mergeInfo.getNewBaseDocId() + j);
                }
            }
        }
        else
        {
            if (length > 0)
            {
                mergedFilter.length(length);
            }
            mergedFilter += *(m_filters[i].m_pFilter);
        }
        length += (size_t)pDocIdRecy->getDocCountAfterRecle();
    }

    if (mergedFilter.any())
    {
        OutputStreamPtr pOutStream = m_pFileSys->createFile(
                BarrelDirectory::getFilePath(DELETION_FILENAME, 
                        mergeInfos.getSuffix()));
        mergedFilter.write(pOutStream);
        pOutStream->close();
    }
}

FX_NS_END
