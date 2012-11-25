#include "firtex/utility/SimpleSegregatedAllocator.h"

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, SimpleSegregatedAllocator);

SimpleSegregatedAllocator::SimpleSegregatedAllocator(size_t uAllocSize,
        size_t uMaxAllocChunks)
{
    m_pFirst = NULL;
    m_uAllocSize = ((uAllocSize > sizeof(void*) ) ? uAllocSize : sizeof(void*));
    m_uChunkSize = uMaxAllocChunks * m_uAllocSize;
    m_pChunk = NULL;
    allocateChunk();
}

SimpleSegregatedAllocator::SimpleSegregatedAllocator() 
{
    m_pFirst = NULL;
    m_uAllocSize = 0;
    m_uChunkSize = 0;
    m_pChunk = NULL;
}

SimpleSegregatedAllocator::~SimpleSegregatedAllocator() 
{
    release();
}

void* SimpleSegregatedAllocator::segregate(void* pChunk, size_t nChunkSize, 
        size_t uAllocSize, void* pEndNode )
{
    char* pOld = static_cast<char*>(pChunk) + 
                 ((nChunkSize - uAllocSize)/uAllocSize ) * uAllocSize;
    nextOf(pOld) = pEndNode;///pOld point to the end node
    //only one partition
    if(pOld == pChunk) 
    {
        return pChunk;
    }
    for(char* pIter = pOld - uAllocSize; pIter != pChunk; 
        pOld = pIter,pIter -= uAllocSize)
    {
        nextOf(pIter) = pOld;
    }
    nextOf(pChunk) = pOld;
    return pChunk;
}

void SimpleSegregatedAllocator::allocateChunk() 
{
    FX_DEBUG("Segregated allocator malloc [%u] bytes.", m_uChunkSize);

    void* pChunk = ::malloc(m_uChunkSize);
    FIRTEX_ASSERT2(pChunk);

    m_pChunk = pChunk;
    m_pFirst = segregate(pChunk, m_uChunkSize, m_uAllocSize, m_pFirst);
}

void SimpleSegregatedAllocator::reset()
{
    m_pFirst = NULL;
    m_pFirst = segregate(m_pChunk, m_uChunkSize, m_uAllocSize, m_pFirst);
}

void SimpleSegregatedAllocator::release() 
{
    m_pFirst = NULL;
    if (m_pChunk) 
    {
        FX_DEBUG("Segregated allocator free memory chunk.");
        ::free(m_pChunk);
        m_pChunk = NULL;
    }
}

FX_NS_END

