//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2009-01-08 18:50:22

#ifndef __FX_SIMPLESEGREGATEDALLOCATOR_H
#define __FX_SIMPLESEGREGATEDALLOCATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(utility);

class SimpleSegregatedAllocator
{
public:
    SimpleSegregatedAllocator(size_t uAllocSize, size_t uMaxAllocChunks);
    SimpleSegregatedAllocator();
    virtual ~SimpleSegregatedAllocator();

public:
    /**
     * Allocate memory from the pool
     * @return allocated memory address
     */
    void* allocate();

    /**
     * Free memory 
     * @param pAddr chunk to free
     */
    void free(void* const pAddr);

    /**
     * Release all allocated memory
     */
    void release();

    /**
     * Reset chunks for reuse
     */
    void reset();

public:
    inline void setAllocSize(size_t uAllocSize, size_t uMaxAllocChunks);
    inline size_t getAllocSize() const;

protected:
    /**
     * Grow the free list
     */
    void  allocateChunk();

protected:
    /** 
     * Return next node of pAddr
     * @param pAddr pointer
     * @return next node 
     */
    inline static void*& nextOf(void* const pAddr);

    /**
     * Segregate chunk to partitions
     * @param pChunk memory chunk
     * @param nChunkSize size of chunk
     * @param uAllocSize allocation size
     * @param pEndNode the last node to link to
     * @return first node of partitions
     */
    static void* segregate(void* pChunk,
                           size_t nChunkSize,
                           size_t uAllocSize,
                           void* pEndNode = NULL);

private:
    SimpleSegregatedAllocator(const SimpleSegregatedAllocator&);
    void operator=(const SimpleSegregatedAllocator&);

private:
    void* m_pFirst;        ///first free node
    size_t m_uAllocSize; ///allocate size

    size_t m_uChunkSize;
    void * m_pChunk;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(SimpleSegregatedAllocator);

/////////////////////////////////////////
//inline functions
inline void SimpleSegregatedAllocator::setAllocSize(size_t uAllocSize,
        size_t uMaxAllocChunks) 
{
    m_uAllocSize = uAllocSize;
    m_uChunkSize = uMaxAllocChunks * m_uAllocSize;
    release();
    allocateChunk();
}

inline void* SimpleSegregatedAllocator::allocate() 
{
    void* const pRet = m_pFirst;
    if (pRet)
    {
        m_pFirst = nextOf(m_pFirst);
    }
    return pRet;
}

inline void SimpleSegregatedAllocator::free(void* const pAddr)
{
    nextOf(pAddr) = m_pFirst;
    m_pFirst = pAddr;
}

inline size_t SimpleSegregatedAllocator::getAllocSize() const  
{
    return m_uAllocSize;
}

//static
inline void*& SimpleSegregatedAllocator::nextOf(void* const pAddr)
{
    return *(static_cast<void**>(pAddr));
}

FX_NS_END

#endif //__FX_SIMPLESEGREGATEDALLOCATOR_H
