#include "firtex/utility/SyncSegregatedAllocator.h"

FX_NS_USE(thread);

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, SyncSegregatedAllocator);

SyncSegregatedAllocator::SyncSegregatedAllocator(size_t nPoolSize, size_t nMaxAllocChunks)
{
    size_t nAllocSize = (nPoolSize / nMaxAllocChunks);

    FX_DEBUG("Allocate size: [%llu], max chunks: [%llu]",
             (unsigned long long)nAllocSize, (unsigned long long)nMaxAllocChunks);
    
    m_pool = new SimpleSegregatedAllocator(nAllocSize, nMaxAllocChunks);

    m_nTotalSize = nPoolSize;
}

SyncSegregatedAllocator::SyncSegregatedAllocator(size_t nAllocSize)
    : m_pool(new SimpleSegregatedAllocator((uint32_t)nAllocSize, 1))
{
}

SyncSegregatedAllocator::~SyncSegregatedAllocator() 
{
    m_pool.reset();
}

FX_NS_END

