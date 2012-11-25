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
// Created : 2011-06-03 22:39:04

#ifndef __FX_SYNCSEGREGATEDALLOCATOR_H
#define __FX_SYNCSEGREGATEDALLOCATOR_H

#include <map>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/SimpleSegregatedAllocator.h"
#include "firtex/thread/FastMutex.h"

FX_NS_DEF(utility);

class SyncSegregatedAllocator
{
public:
    /**
     * Constructor
     *
     * @param nPoolSize total size of pool
     * @param nMaxAllocChunks max allocation chunks
     */
    SyncSegregatedAllocator(size_t nPoolSize, size_t nMaxAllocChunks);

    /**
     * Constructor
     *
     * @param nAllocSize allocation size
     */
    SyncSegregatedAllocator(size_t nAllocSize);

    ~SyncSegregatedAllocator();

public:
    /**
     * Allocate one memory chunk from the pool
     *
     * @return the allocated chunk address, null if failure.
     */
    void* allocate();

    /**
     * De-allocate the chunk allocated from the pool
     *
     * @param pAddr the address of chunk allocated from the pool
     */
    void deallocate(void* const pAddr);

public:
    /// Return chunk size of pool
    inline size_t getChunkSize() const;

private:
    /// For thread safe
    mutable FX_NS(thread)::FastMutex m_mutex;
    
    /// The pair of pool
    SimpleSegregatedAllocatorPtr m_pool;

    /// Total pool size
    size_t m_nTotalSize;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(SyncSegregatedAllocator);

////////////////////////////////////////////////////////
inline void* SyncSegregatedAllocator::allocate()
{
    FX_NS(thread)::FastMutex::Guard g(m_mutex);
    FX_DEBUG("Allocate a chunk, size: [%u]", (uint32_t)m_pool->getAllocSize());

    return m_pool->allocate();
}

inline void SyncSegregatedAllocator::deallocate(void* const pAddr)
{
    FX_NS(thread)::FastMutex::Guard g(m_mutex);
    FX_DEBUG("Deallocate a chunk, size: [%u]", (uint32_t)m_pool->getAllocSize());

    m_pool->free(pAddr);
}

inline size_t SyncSegregatedAllocator::getChunkSize() const
{
    FX_NS(thread)::FastMutex::Guard g(m_mutex);

    return m_pool->getAllocSize();
}

FX_NS_END

#endif //__FX_SYNCSEGREGATEDALLOCATOR_H
