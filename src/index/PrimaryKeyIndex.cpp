#include "firtex/index/PrimaryKeyIndex.h"
#include "firtex/index/IndexWriter.h"
#include "firtex/index/MultiFieldTermReader.h"
#include "firtex/index/primarykey/PrimaryKeyIndexer.h"
#include "firtex/index/primarykey/PrimaryKeyTermReader.h"
#include <assert.h>
#include <limits>

using namespace std;
FX_NS_USE(common);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PrimaryKeyIndex);

PrimaryKeyIndex::PrimaryKeyIndex(const tstring& sPrimKeyField)
    : m_sPrimKeyField(sPrimKeyField)
{
}

PrimaryKeyIndex::PrimaryKeyIndex(const PrimaryKeyIndex& src)
    : m_pFileSys(src.m_pFileSys)
    , m_pBarrelsInfo(src.m_pBarrelsInfo)
    , m_sPrimKeyField(src.m_sPrimKeyField)
    , m_pHashTable(src.m_pHashTable)
{
}

PrimaryKeyIndex::~PrimaryKeyIndex()
{
}

void PrimaryKeyIndex::open(const FileSystemPtr& pFileSys,
                           const BarrelsInfoPtr& pBarrelsInfo,
                           const HashTablePtr& pHashTable)
{
    m_pFileSys = pFileSys;
    m_pStreamPool = new InputStreamPool(pFileSys);
    m_pBarrelsInfo = pBarrelsInfo->clone();

    m_pHashTable = new HashTable(std::numeric_limits<uint64_t>::max(),
                                 INVALID_DOCID);
    
    if (pHashTable.isNotNull())
    {
        mergeWith(pBarrelsInfo->getLastBarrel().getBaseDocId(), pHashTable);
        return;
    }
    

    bool bSingle = (m_pBarrelsInfo->getBarrelCount() == 1);
    BarrelsInfo::Iterator iter = m_pBarrelsInfo->iterator();
    while (iter.hasNext())
    {
        const BarrelInfo& barrelInfo = iter.next();
        if (barrelInfo.getDocCount() > 0)
        {
            PrimaryKeyTermReaderPtr pTermReader = new PrimaryKeyTermReader();
            pTermReader->open(m_pFileSys, m_pStreamPool,
                    barrelInfo.getSuffix(), m_sPrimKeyField, NULL);
            HashTablePtr pHashMap = pTermReader->getPostingTable();
            if (bSingle)
            {
                m_pHashTable = pHashMap;
            }
            else
            {
                mergeWith(barrelInfo.getBaseDocId(), pHashMap);
            }
        }
    }
}

void PrimaryKeyIndex::reopen(const BarrelsInfoPtr& pBarrelsInfo, 
                             const HashTablePtr& pHashTable)
{
    BarrelsInfoPtr pLastBarrelsInfo = m_pBarrelsInfo;
    BarrelsInfoPtr pNewBarrelsInfo = pBarrelsInfo->clone();

    m_pBarrelsInfo = pNewBarrelsInfo;
    if (pHashTable.isNotNull())
    {
        mergeWith(pBarrelsInfo->getLastBarrel().getBaseDocId(), pHashTable);
        return;
    }

    size_t nBarrelCount = pNewBarrelsInfo->getBarrelCount();
    if (nBarrelCount == 1)
    {
        const BarrelInfo& barrelInfo = pNewBarrelsInfo->getLastBarrel();
        PrimaryKeyTermReaderPtr pTermReader = new PrimaryKeyTermReader();
        pTermReader->open(m_pFileSys, m_pStreamPool, barrelInfo.getSuffix(), m_sPrimKeyField, NULL);
        HashTablePtr pHashMap = pTermReader->getPostingTable();
        m_pHashTable = pHashMap;
    }
    else
    {
        if ((int32_t)(pNewBarrelsInfo->getCommitId() - pLastBarrelsInfo->getCommitId())
            == (int32_t)(pNewBarrelsInfo->getBarrelCount() - pLastBarrelsInfo->getBarrelCount()))
        {
            // Append new data

            BarrelsInfo::Iterator oldIt = pLastBarrelsInfo->iterator();
            BarrelsInfo::Iterator newIt = pNewBarrelsInfo->iterator();
        
            size_t cursor = 0;
            while (newIt.hasNext())
            {
                const BarrelInfo& newInfo = newIt.next();
                docid_t newBaseId = newInfo.getBaseDocId();

                if (oldIt.hasNext())
                {
                    const BarrelInfo& oldInfo = oldIt.next();
                    if (oldInfo.getCommitId() != newInfo.getCommitId())
                    {
                        FIRTEX_THROW(IndexCollapseException, "Inconsistent commit id");
                    }
                    ++cursor;
                    continue;
                }
                if (newInfo.getDocCount() > 0)
                {
                    PrimaryKeyTermReaderPtr pTermReader = new PrimaryKeyTermReader();
                    pTermReader->open(m_pFileSys, m_pStreamPool, newInfo.getSuffix(),
                            m_sPrimKeyField, NULL);
                    HashTablePtr pHashMap = pTermReader->getPostingTable();
                    mergeWith(newBaseId, pHashMap);
                }
            } // end while
        }
        else
        {
            // Re-load all data
            m_pHashTable.assign(new HashTable(std::numeric_limits<uint64_t>::max(),
                            INVALID_DOCID));

            BarrelsInfo::Iterator iter = pNewBarrelsInfo->iterator();
            while (iter.hasNext())
            {
                const BarrelInfo& barrelInfo = iter.next();
                
                PrimaryKeyTermReaderPtr pTermReader = new PrimaryKeyTermReader();
                pTermReader->open(m_pFileSys, m_pStreamPool, barrelInfo.getSuffix(),
                        m_sPrimKeyField, NULL);
                HashTablePtr pHashMap = pTermReader->getPostingTable();
                mergeWith(barrelInfo.getBaseDocId(), pHashMap);
            }
        }
    } // end else
    
    m_pBarrelsInfo = pNewBarrelsInfo;
}

void PrimaryKeyIndex::mergeWith(docid_t baseDocId, HashTablePtr pHashMap)
{
    HashTable::Iterator it = pHashMap->iterator();
    while (it.hasNext())
    {
        HashTable::KeyValueType& kv = it.next();
        m_pHashTable->insert(make_pair(kv.first, kv.second + baseDocId));
    }
}

PrimaryKeyIndex* PrimaryKeyIndex::clone() const
{
    return new PrimaryKeyIndex(*this);
}

FX_NS_END
