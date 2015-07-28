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
// Created : 2011-06-04 11:41:43

#ifndef __FX_TYPEDSIMPLECHUNKEDPOOL_H
#define __FX_TYPEDSIMPLECHUNKEDPOOL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(utility);

template <typename Allocator>
class TypedSimpleChunkedPool
{
public:
    typedef Allocator AllocType;
    DEFINE_TYPED_PTR(AllocType);

public:
    TypedSimpleChunkedPool(const AllocTypePtr& pAlloc, size_t nChunkSize);
    ~TypedSimpleChunkedPool();

public:
    /**
     * Allocate memory from pool. 
     *
     * @param nBytes number of byets to allocate
     * 
     * @return address of allocated memory
     */
    void* allocate(size_t nBytes);

    /// Reset the pool to empty state
    void reset();

    /// Clear all allocated chunks
    void clear();

    /// Return total number of allocated bytes
    uint64_t getAllocatedBytes() const;

private:
    bool allocateChunk();

private:
    typedef std::vector<void*> ChunkVect;

    /// Chunk allocator
    AllocTypePtr m_pAllocator;
    
    /// The size of chunk
    size_t m_uChunkSize;
    
    ChunkVect m_chunks;

    /// The last chunk in chunk vector
    char* m_pLastChunk;

    /// Allocate offset in current chunk
    size_t m_uUpToInChunk;

private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////////////
///

SETUP_STREAM_LOGGER_TEMP(utility, TypedSimpleChunkedPool);

template <typename Allocator>
TypedSimpleChunkedPool<Allocator>::TypedSimpleChunkedPool(
        const AllocTypePtr& pAlloc, size_t uChunkSize)
    : m_pAllocator(pAlloc)
    , m_uChunkSize(uChunkSize)
    , m_pLastChunk(NULL)
    , m_uUpToInChunk(0)
{
    bool ret = allocateChunk();
    (void)ret;
    FIRTEX_ASSERT2(ret);
}

template <typename Allocator>
TypedSimpleChunkedPool<Allocator>::~TypedSimpleChunkedPool()
{
    clear();
}

template <typename Allocator>
inline void* TypedSimpleChunkedPool<Allocator>::allocate(size_t nBytes)
{
    FX_TRACE("Allocate [%u] bytes.", (uint32_t)nBytes);

    uint32_t n = (uint32_t)nBytes;
    if (n + m_uUpToInChunk <= m_uChunkSize)
    {
        void* p = m_pLastChunk + m_uUpToInChunk;
        m_uUpToInChunk += n;
        assert(p);
        return p;
    }

    if (!allocateChunk())
    {
        FX_DEBUG("Allocate chunk FAILED.");
        return NULL;
    }

    if (n <= m_uChunkSize)
    {
        void* p = m_pLastChunk;
        m_uUpToInChunk = n;
        assert(p);
        return p;
    }
    FX_DEBUG("Allocate FAILED: request size: [%u], chunk size: [%u]", n, m_uChunkSize);
    return NULL;
}

template <typename Allocator>
bool TypedSimpleChunkedPool<Allocator>::allocateChunk()
{
    FX_DEBUG("Allocate chunk, size: [%u]", m_uChunkSize);
    
    void* pChunk = m_pAllocator->allocate(m_uChunkSize);
    if (pChunk)
    {
        m_chunks.push_back(pChunk);
        m_pLastChunk = (char*)pChunk;
        m_uUpToInChunk = 0;
        return true;
    }
    return false;
}

template <typename Allocator>
void TypedSimpleChunkedPool<Allocator>::reset()
{
    clear();
}

template <typename Allocator>
void TypedSimpleChunkedPool<Allocator>::clear()
{
    for (size_t i = 0; i < m_chunks.size(); ++i)
    {
        m_pAllocator->deallocate(m_chunks[i]);
    }
    m_chunks.clear();
    m_pLastChunk = NULL;
    m_uUpToInChunk = m_uChunkSize;
}

template <typename Allocator>
inline uint64_t TypedSimpleChunkedPool<Allocator>::getAllocatedBytes() const
{
    return ((m_chunks.size() - 1) * m_uChunkSize + m_uUpToInChunk);
}

FX_NS_END

#endif //__FX_TYPEDSIMPLECHUNKEDPOOL_H
