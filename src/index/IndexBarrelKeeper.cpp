#include "firtex/index/IndexBarrelKeeper.h"
#include "firtex/index/MultiIndexBarrelReader.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/IndexCleanerFactory.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/index/InMemIndexMerger.h"

FX_NS_USE(store);
FX_NS_USE(document);
FX_NS_USE(analyzer);
FX_NS_USE(thread);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, IndexBarrelKeeper);

IndexBarrelKeeper::IndexBarrelKeeper(const FileSystemPtr& pFileSys,
                                     const DocumentSchema* pDocSchema,
                                     const ComponentBuilder* pCompBuilder,
                                     const AnalyzerMapper* pAnalyzeMapper)
    : m_pFileSys(pFileSys)
    , m_pDocSchema(pDocSchema)
    , m_pComponentBuilder(pCompBuilder)
    , m_pAnalyzerMapper(pAnalyzeMapper)
    , m_latestOnDiskCommit(INVALID_COMMIT)
    , m_mode(READ)
    , m_barrelCounter(0)
{
}

IndexBarrelKeeper::~IndexBarrelKeeper() 
{
}

void IndexBarrelKeeper::init(const BarrelsInfoPtr& pBarrelsInfo, Mode mode)
{
    FX_LOG(INFO, "Initialize index barrel keeper.");
    m_mode = mode;
    m_sEncoding = pBarrelsInfo->getEncoding();

    switch(mode)
    {
    case READ:
        loadOnDiskBarrel(pBarrelsInfo);
        break;
    case RDWR:
        setupIndexCleaner();
        loadOnDiskBarrel(pBarrelsInfo);
        break;
    case WRITE:
        loadOnDiskDataForUpdate(pBarrelsInfo);
        setupIndexCleaner();
        break;
    }
}

void IndexBarrelKeeper::loadOnDiskBarrel(const BarrelsInfoPtr& pBarrelsInfo)
{
    BarrelsInfoPtr pBarrelsInfo2 = pBarrelsInfo->clone();
    createOnDiskBarrelReader(pBarrelsInfo2);
}

commitid_t IndexBarrelKeeper::createOnDiskBarrelReader(const BarrelsInfoPtr& pBarrelsInfo)
{
    commitid_t latestCommit = pBarrelsInfo->getCommitId();
    FIRTEX_ASSERT2(latestCommit != INVALID_COMMIT);

    DeletedDocumentFilterPtr pDocFilter = new DeletedDocumentFilter(m_pFileSys);
    pDocFilter->open(pBarrelsInfo);

    PrimaryKeyIndexPtr pPrimKeyIndex = createPrimaryKeyIndex();

    IndexBarrelReaderPtr pReader;
    size_t bc = pBarrelsInfo->getBarrelCount();
    if (bc == 1)
    {
        const BarrelInfo& lastBarrel = pBarrelsInfo->getLastBarrel();
        const BitVector* pBitVector =
            pDocFilter->getDocFilter(lastBarrel.getBaseDocId());
        SingleIndexBarrelReader* pSingleReader = new SingleIndexBarrelReader(
                m_pFileSys, m_pDocSchema, m_pComponentBuilder);
        pReader.assign(pSingleReader);

        pSingleReader->open(&lastBarrel, pBitVector);

        if (pPrimKeyIndex.isNotNull())
        {
            TermReaderPtr pTermReader = pSingleReader->termReader(
                    pPrimKeyIndex->getPrimKeyField());
            PrimaryKeyTermReaderPtr pPrimKeyTermReader =
                pTermReader.cast<PrimaryKeyTermReader>();
            FIRTEX_ASSERT2(!pPrimKeyTermReader.isNull());
            
            pPrimKeyIndex->open(m_pFileSys, pBarrelsInfo,
                    pPrimKeyTermReader->getPostingTable());
        }
    }
    else if (bc > 1)
    {
        MultiIndexBarrelReader* pMultiReader = new MultiIndexBarrelReader(
                m_pFileSys, m_pDocSchema, m_pComponentBuilder);
        pReader.assign(pMultiReader);

        pMultiReader->open(pBarrelsInfo, pDocFilter);

        if (pPrimKeyIndex.isNotNull())
        {
            pPrimKeyIndex->open(m_pFileSys, pBarrelsInfo);
        }
    }

    {
        ScopedRWLock lock(m_lock, true);
        commitid_t lastCommit = INVALID_COMMIT;
        if (!m_commitMap.empty())
        {
            lastCommit = m_commitMap.rbegin()->second->getBarrelsInfo()->getCommitId();
        }

        if (lastCommit >= latestCommit)
        {
            return lastCommit;
        }

        IndexBarrelPtr pBarrel(new IndexBarrel(pBarrelsInfo, pReader, pDocFilter));
        pBarrel->setEncoding(m_sEncoding);
        m_commitMap.insert(make_pair(latestCommit, pBarrel));
        m_latestOnDiskCommit = latestCommit;
        m_pOnDiskDocFilter = pDocFilter;
        m_pOnDiskPrimKeyIndex = pPrimKeyIndex;
    }
    return latestCommit;
}

commitid_t IndexBarrelKeeper::loadOnDiskDataForUpdate(const BarrelsInfoPtr& pBarrelsInfo)
{
    commitid_t latestCommit = pBarrelsInfo->getCommitId();

    DeletedDocumentFilterPtr pDocFilter = new DeletedDocumentFilter(m_pFileSys);
    pDocFilter->open(pBarrelsInfo);

    PrimaryKeyIndexPtr pPrimKeyIndex = createPrimaryKeyIndex();
    if (pPrimKeyIndex.isNotNull())
    {
        pPrimKeyIndex->open(m_pFileSys, pBarrelsInfo);
    }

    ScopedRWLock lock(m_lock, true);
    {
        commitid_t lastCommit = INVALID_COMMIT;
        if (!m_commitMap.empty())
        {
            lastCommit = m_commitMap.rbegin()->second->getBarrelsInfo()->getCommitId();
        }

        if (lastCommit >= latestCommit)
        {
            if (latestCommit == INVALID_COMMIT)
            {
                m_pOnDiskPrimKeyIndex = pPrimKeyIndex;
                m_pOnDiskDocFilter = pDocFilter;
            }

            return lastCommit;
        }

        IndexBarrelReaderPtr pReader;
        IndexBarrelPtr pBarrel(new IndexBarrel(pBarrelsInfo, pReader, pDocFilter));
        pBarrel->setEncoding(m_sEncoding);
        m_commitMap.insert(make_pair(latestCommit, pBarrel));
        m_latestOnDiskCommit = latestCommit;
        m_pOnDiskDocFilter = pDocFilter;
        m_pOnDiskPrimKeyIndex = pPrimKeyIndex;
    }
    return latestCommit;
}

void IndexBarrelKeeper::refresh()
{
    switch(m_mode)
    {
    case READ:
    case RDWR:
        refreshDataForRead();
        break;
    case WRITE:
        refreshDataForUpdate();
        break;
    }
}

commitid_t IndexBarrelKeeper::refreshDataForRead()
{
    BarrelsInfoPtr pBarrelsInfo(new BarrelsInfo());
    pBarrelsInfo->read(m_pFileSys);
    commitid_t latestCommit = pBarrelsInfo->getCommitId();

    commitid_t prevCommit = INVALID_COMMIT;
    IndexBarrelPtr pLastBarrel;
    {
        ScopedRWLock lock(m_lock, false);
        if (!m_commitMap.empty())
        {
            pLastBarrel = m_commitMap.rbegin()->second;
            prevCommit = pLastBarrel->getBarrelsInfo()->getCommitId();
        }
    }
    if (latestCommit == prevCommit)
    {
        return prevCommit;
    }

    if (prevCommit == INVALID_COMMIT)
    {
        return createOnDiskBarrelReader(pBarrelsInfo);
    }

    FX_LOG(INFO, "Begin reopen index database, commitId: [%d]", latestCommit);

    IndexBarrelReaderPtr pReader;

    DeletedDocumentFilterPtr pDocFilter = pLastBarrel->getDeletedDocFilter()->clone();
    pDocFilter->reopen(pBarrelsInfo);

    PrimaryKeyIndexPtr pPrimKey;
    if (m_pOnDiskPrimKeyIndex.isNotNull())
    {
        pPrimKey = m_pOnDiskPrimKeyIndex->clone();
        pPrimKey->reopen(pBarrelsInfo);
    }

    BarrelsInfoPtr pLastBarrelsInfo = pLastBarrel->getBarrelsInfo();
    IndexBarrelReaderPtr pLastReader = pLastBarrel->getReader();

    size_t bc = pBarrelsInfo->getBarrelCount();
    if (bc == 1)
    {
        if (pLastBarrelsInfo->getBarrelCount() > 1 
            || pBarrelsInfo->getCommitId() - pLastBarrelsInfo->getCommitId() > 1)
        {
            const BarrelInfo& lastBarrelInfo = pBarrelsInfo->getLastBarrel();
            const BitVector* pBitVector =
                pDocFilter->getDocFilter(lastBarrelInfo.getBaseDocId());
            SingleIndexBarrelReader* pSingleReader = new SingleIndexBarrelReader(
                    m_pFileSys, m_pDocSchema, m_pComponentBuilder);
            pReader.assign(pSingleReader);
            
            pSingleReader->open(&lastBarrelInfo, pBitVector);
        }
        else
        {
            const BarrelInfo& lastBarrelInfo = pBarrelsInfo->getLastBarrel();
            const BitVector* pBitVector =
                pDocFilter->getDocFilter(lastBarrelInfo.getBaseDocId());
                
            pReader = pLastReader->clone();
            SingleIndexBarrelReaderPtr pSingleReader =
                pReader.cast<SingleIndexBarrelReader>();

            FIRTEX_ASSERT2(pSingleReader.isNotNull());
            pSingleReader->reopen(&lastBarrelInfo, pBitVector);
        }
    }
    else if (bc > 1)
    {
        if (pLastBarrelsInfo->getBarrelCount() > 1)
        {
            pReader = pLastReader->clone();
            MultiIndexBarrelReaderPtr pMultiReader =
                pReader.cast<MultiIndexBarrelReader>();
            FIRTEX_ASSERT2(pMultiReader.isNotNull());
            pMultiReader->reopen(pBarrelsInfo, pDocFilter);
        }
        else
        {
            //TODO: optimize?
            MultiIndexBarrelReader* pMultiReader = new MultiIndexBarrelReader(
                    m_pFileSys, m_pDocSchema, m_pComponentBuilder);
            pReader.assign(pMultiReader);

            pMultiReader->open(pBarrelsInfo, pDocFilter);
        }
    }
    FX_LOG(INFO, "End reopen index database.");

    IndexBarrelPtr pBarrel(new IndexBarrel(pBarrelsInfo, pReader, pDocFilter));
    pBarrel->setEncoding(m_sEncoding);
    {
        ScopedRWLock lock(m_lock, true);
        bool bInserted = doInsertCommit(latestCommit, pBarrel);
        if (bInserted)
        {
            m_pOnDiskDocFilter = pDocFilter;
            m_pOnDiskPrimKeyIndex = pPrimKey;
            return latestCommit;
        }
        return prevCommit;
    }
}

commitid_t IndexBarrelKeeper::refreshDataForUpdate()
{
    BarrelsInfoPtr pBarrelsInfo(new BarrelsInfo());
    pBarrelsInfo->read(m_pFileSys);
    commitid_t latestCommit = pBarrelsInfo->getCommitId();

    commitid_t prevCommit = INVALID_COMMIT;
    IndexBarrelPtr pLastBarrel;
    {
        ScopedRWLock lock(m_lock, false);
        if (!m_commitMap.empty())
        {
            pLastBarrel = m_commitMap.rbegin()->second;
            prevCommit = pLastBarrel->getBarrelsInfo()->getCommitId();
        }
    }
    if (latestCommit == prevCommit)
    {
        return prevCommit;
    }

    if (prevCommit == INVALID_COMMIT)
    {
        return loadOnDiskDataForUpdate(pBarrelsInfo);
    }

    IndexBarrelReaderPtr pReader;

    DeletedDocumentFilterPtr pDocFilter = pLastBarrel->getDeletedDocFilter()->clone();
    pDocFilter->reopen(pBarrelsInfo);

    PrimaryKeyIndexPtr pPrimKey;
    if (m_pOnDiskPrimKeyIndex.isNotNull())
    {
        pPrimKey = m_pOnDiskPrimKeyIndex->clone();
        pPrimKey->reopen(pBarrelsInfo);
    }

    IndexBarrelReaderPtr pLastReader;

    IndexBarrelPtr pBarrel(new IndexBarrel(pBarrelsInfo, pReader, pDocFilter));
    pBarrel->setEncoding(m_sEncoding);
    {
        ScopedRWLock lock(m_lock, true);
        bool bInserted = doInsertCommit(latestCommit, pBarrel);
        if (bInserted)
        {
            m_pOnDiskDocFilter = pDocFilter;
            m_pOnDiskPrimKeyIndex = pPrimKey;
            return latestCommit;
        }
        return prevCommit;
    }
}

IndexBarrelWriterPtr IndexBarrelKeeper::createBarrelWriter()
{
    FastMutex::Guard g(m_counterLock);
    FX_LOG(INFO, "Create new in-memory barrel: [%d]", m_barrelCounter);
    return IndexBarrelWriterPtr(new IndexBarrelWriter(this, m_barrelCounter++));
}

IndexBarrelReaderPtr IndexBarrelKeeper::getBarrelReader()
{
    ScopedRWLock lock(m_lock, false);
    if (m_commitMap.empty())
    {
        return IndexBarrelReaderPtr();
    }
    else
    {
        return m_commitMap.rbegin()->second->getReader();
    }
}

IndexBarrelKeeper::IndexBarrelPtr IndexBarrelKeeper::getLatestBarrel()
{
    ScopedRWLock lock(m_lock, false);
    if (m_commitMap.empty())
    {
        return IndexBarrelPtr();
    }
    else
    {
        return m_commitMap.rbegin()->second;
    }
}

PrimaryKeyIndexPtr IndexBarrelKeeper::createPrimaryKeyIndex()
{
    tstring sPrimKeyName;
    DocumentSchema::Iterator it = m_pDocSchema->iterator();
    while (it.hasNext())
    {
        const FieldSchema* pFieldSchema = it.next();
        if (pFieldSchema->getTypeName() == "PRIMARY_KEY")
        {
            sPrimKeyName = pFieldSchema->getName();
            break;
        }
    }

    if (!sPrimKeyName.empty())
    {
        PrimaryKeyIndexPtr pPrimKeyIndex(new PrimaryKeyIndex(sPrimKeyName));
        return pPrimKeyIndex;
    }
    else
    {
        return PrimaryKeyIndexPtr();
    }
}

bool IndexBarrelKeeper::deleteOnDiskDocument(const char* szPrimKey)
{
    ScopedRWLock lock(m_lock, false);
    if (m_pOnDiskPrimKeyIndex.isNull())
    {
        return false;
    }

    docid_t docId = m_pOnDiskPrimKeyIndex->lookup(szPrimKey);
    if (docId != INVALID_DOCID)
    {
        FX_TRACE("Delete on-disk document: [%s]->[%d]", szPrimKey, docId);
        return m_pOnDiskDocFilter->deleteDocument(docId);
    }
    return false;
}

void IndexBarrelKeeper::sealBarrel(const IndexBarrelWriterPtr& pInMemBarrel)
{
    FX_DEBUG("Seal barrel");

    IndexBarrelWriterPtr pBarrelWriter = pInMemBarrel;
    InMemIndexMergerPtr pInMemBarrelMerger;
    pBarrelWriter->sealBarrel();

    {
        ScopedRWLock lock(m_lock, true);

        IndexBarrelPtr pLastBarrel;
        if (!m_commitMap.empty())
        {
            pLastBarrel = m_commitMap.rbegin()->second;
        }
    
        BarrelsInfoPtr pNewBarrelsInfo;
        if (pLastBarrel.isNotNull())
        {
            pNewBarrelsInfo = pLastBarrel->getBarrelsInfo()->clone();
        }
        else 
        {
            pNewBarrelsInfo.assign(new BarrelsInfo);
        }

        /// Commit barrel to barrels info
        if (!pBarrelWriter->commitBarrelInfo(pNewBarrelsInfo))
        {
            FX_DEBUG("No barrel to commit");
            return ;
        }
        FX_DEBUG("Committed barrel info: commit: [%d], baseDocId: [%d]",
                 pNewBarrelsInfo->getLastBarrel().getCommitId(),
                 pNewBarrelsInfo->getLastBarrel().getBaseDocId());

        //TODO: implement real-time search
        IndexBarrelReaderPtr pReader;
        if (pLastBarrel.isNotNull())
        {
            pReader = pLastBarrel->getReader();
        }

        PrimaryKeyIndexPtr pPrimKey;
        if (m_pOnDiskPrimKeyIndex.isNotNull())
        {
            pPrimKey = m_pOnDiskPrimKeyIndex->clone();
            if (pPrimKey.isNotNull())
            {
                const PrimaryKeyIndexPtr& pPkIndex = pBarrelWriter->getPrimaryKey();
                if (pPkIndex.isNotNull())
                {
                    pPrimKey->reopen(pNewBarrelsInfo, pPkIndex->getHashMap());
                }
            }
        }
        else
        {
            pPrimKey = createPrimaryKeyIndex();
            if (pPrimKey.isNotNull())
            {
                const PrimaryKeyIndexPtr& pPkIndex = pBarrelWriter->getPrimaryKey();
                FIRTEX_ASSERT2(pPkIndex.isNotNull());
                pPrimKey->open(m_pFileSys, pNewBarrelsInfo, pPkIndex->getHashMap());
            }
        }

        IndexBarrelPtr pBarrel(new IndexBarrel(pNewBarrelsInfo, pReader, m_pOnDiskDocFilter));
        pBarrel->setEncoding(m_sEncoding);
        
        bool bInserted = doInsertCommit(pNewBarrelsInfo->getCommitId(), pBarrel);
        (void)bInserted;
        FIRTEX_ASSERT2(bInserted);

        m_pOnDiskDocFilter.assign(m_pOnDiskDocFilter->clone());
        m_pOnDiskPrimKeyIndex = pPrimKey;

        if (m_pInMemBarrelMerger.isNull())
        {
            m_pInMemBarrelMerger.assign(new InMemIndexMerger(this));
        }
        m_pInMemBarrelMerger->addToMerge(pNewBarrelsInfo->getLastBarrel(), 
                pBarrelWriter);
        if (m_pInMemBarrelMerger->getInMemBarrelCount() >= (size_t)GLOBAL_CONF().Build.buildThreadCount)
        {
            pInMemBarrelMerger = m_pInMemBarrelMerger;
            m_pInMemBarrelMerger.reset();
        }
    }
    
    if (pInMemBarrelMerger.isNotNull())
    {
        CommittablePtr pCommitObj = pInMemBarrelMerger.cast<Committable>();
        m_pCommitScheduler->commit(pCommitObj);
    }
}

void IndexBarrelKeeper::waitCommit()
{
    FX_LOG(INFO, "Waiting last commit...");
    forceCommit();

    //Wait until all documents committed to file system
    m_pCommitScheduler->waitCommit();

    m_pInMemBarrelMerger.reset();
}

void IndexBarrelKeeper::forceCommit()
{
    FX_LOG(INFO, "Waiting last commit...");
	InMemIndexMergerPtr pInMemBarrelMerger;

	{
		ScopedRWLock lock(m_lock, true);
		if (m_pInMemBarrelMerger.isNotNull())
		{
			pInMemBarrelMerger = m_pInMemBarrelMerger;
			m_pInMemBarrelMerger.reset(); 
		}
	}

	if (pInMemBarrelMerger.isNotNull())
	{
		CommittablePtr pCommitObj = pInMemBarrelMerger.cast<Committable>();
		m_pCommitScheduler->commit(pCommitObj);
	}
}

void IndexBarrelKeeper::setupIndexCleaner()
{
    string sCleaner = GLOBAL_CONF().Build.IndexCleaner.strategy;
    string sParam = GLOBAL_CONF().Build.IndexCleaner.param;
    if (sCleaner.empty())
    {
        sCleaner = "keep_by_commit";
    }
    if (sParam.empty())
    {
        sParam = "keep_count=";
        NumberFormatter::append(sParam, DEFAULT_COMMIT_KEEP_COUNT);
    }
    m_pIndexCleaner = IndexCleanerFactory::instance()->createIndexCleaner(sCleaner);
    if (m_pIndexCleaner.isNull())
    {
        FX_LOG(ERROR, "Create index cleaner: [%s] FAILED.", sCleaner.c_str());
        return;
    }

    m_pIndexCleaner->init(m_pFileSys, sParam);
}

void IndexBarrelKeeper::cleanStaledIndex()
{
    if (m_pIndexCleaner.isNotNull())
    {
        m_pIndexCleaner->cleanup();
    }
}

bool IndexBarrelKeeper::doInsertCommit(
        commitid_t commId, const IndexBarrelPtr& pIndexBarrel)
{
    commitid_t lastCommit = INVALID_COMMIT;
    if (!m_commitMap.empty())
    {
        IndexBarrelPtr pLastBarrel = m_commitMap.rbegin()->second;
        lastCommit = pLastBarrel->getBarrelsInfo()->getCommitId();
    }

    if (lastCommit >= commId)
    {
        return false;
    }

    FX_LOG(INFO, "Insert new commit: [%d]", commId);

    m_commitMap.insert(make_pair(commId, pIndexBarrel));
    m_latestOnDiskCommit = commId;

    return true;
}

void IndexBarrelKeeper::housekeep()
{
    ScopedRWLock lock(m_lock, true);

    CommitMap toReserve;
    CommitMap::reverse_iterator it = m_commitMap.rbegin();
    // Always reserve the last element 
    toReserve.insert(*it++);

    for (; it != m_commitMap.rend(); it++)
    {
        if (it->second.referenceCount() > 1)
        {
            FX_LOG(INFO, "Staled commit is in use: [%d]", it->first);
            toReserve.insert(*it);
        }
        else
        {
            FX_LOG(INFO, "Clean staled commit: [%d]", it->first);
            it->second.reset();
        }
    }

    toReserve.swap(m_commitMap);
}

FX_NS_END

