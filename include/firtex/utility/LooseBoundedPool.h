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
// Created : 2011-06-04 10:51:59

#ifndef __FX_LOOSEBOUNDEDPOOL_H
#define __FX_LOOSEBOUNDEDPOOL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/SimpleAllocator.h"
#include "firtex/utility/BoundedChunkAllocator.h"
#include "firtex/utility/TypedSimpleChunkedPool.h"
#include "firtex/utility/Pool.h"
#include <vector>

FX_NS_DEF(utility);

class LooseBoundedPool : public Pool
{
public:
    const static uint32_t DEFAULT_EXCEPTION_CHUNK_SIZE = 1024 * 1024; // 1M

public:
    LooseBoundedPool(size_t nPoolSize);
    LooseBoundedPool(const BoundedChunkAllocatorPtr& pAllocator);
    ~LooseBoundedPool();

public:
    /**
     * try allocate memory from pool. 
     *
     * @param nBytes number of byets to allocate
     * 
     * @return address of allocated memory
     */
    void* tryAllocate(size_t nBytes);

    /**
     * Allocate memory from pool. 
     *
     * @param nBytes number of byets to allocate
     * 
     * @return address of allocated memory
     */
    void* allocate(size_t nBytes);

    /// Determine if there is any exception memory allocation
    bool hasException() const;

    /// Clear allocated memory
    void clear();

private:

    typedef TypedSimpleChunkedPool<BoundedChunkAllocator> NormalPool;
    typedef TypedSimpleChunkedPool<SimpleAllocator> ExceptionPool;
    
    enum AllocState
    {
        STATE_NORMAL = 1,
        STATE_EXCEPTION
    };
    
    /// Size of normal chunk
    uint32_t m_nNormChunkSize;

    /// Size of exception chunk
    uint32_t m_nExcepChunkSize;
    
    /// Allocation state, normal or exception
    AllocState m_eAllocState;

    /// Bounded chunk allocator for normal pool
    BoundedChunkAllocatorPtr m_pBoudedAllocator;

    /// Normal pool
    NormalPool* m_pNormPool;

    /// allocator for exception 
    SimpleAllocatorPtr m_pSimpleAllocator;

    /// Exception pool
    ExceptionPool m_excepPool;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(LooseBoundedPool);

/////////////////////////////////////////////
///
inline bool LooseBoundedPool::hasException() const
{
    return (m_eAllocState == STATE_EXCEPTION);
}

FX_NS_END

#endif //__FX_LOOSEBOUNDEDPOOL_H
