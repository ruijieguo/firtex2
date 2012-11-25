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
// Created : 2011-07-13 00:22:57

#ifndef __FX_BOUNDEDCHUNKALLOCATOR_H
#define __FX_BOUNDEDCHUNKALLOCATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/SyncSegregatedAllocator.h"
#include <vector>

FX_NS_DEF(utility);

class BoundedChunkAllocator
{
public:
    BoundedChunkAllocator(const SyncSegregatedAllocatorPtr& pAllocator) 
        : m_pAllocator(pAllocator)
        , m_nAllocated(0) 
    {
    }
    ~BoundedChunkAllocator()
    {
        clear();
    }

public:
    void* allocate(size_t nBytes)
    {
        if (m_nAllocated < m_chunks.size())
        {
            return m_chunks[m_nAllocated++];
        }
        return NULL;
    }
        
    void deallocate(void* pChunk)
    {
        for (size_t i = 0; i < m_chunks.size(); ++i)
        {
            if (m_chunks[i] == pChunk)
            {
                m_pAllocator->deallocate(pChunk);
                m_chunks[i] = NULL;
                break;
            }
        }
    }

    bool init(size_t nChunks)
    {
        for (size_t i = 0; i < nChunks; ++i)
        {
            void* p = m_pAllocator->allocate();
            if (!p)
            {
                clear();
                return false;
            }
            m_chunks.push_back(p);
        }
        return true;
    }

    void clear()
    {
        for (size_t i = 0; i < m_chunks.size(); ++i)
        {
            if (m_chunks[i])
            {
                m_pAllocator->deallocate(m_chunks[i]);
            }
        }
        m_chunks.clear();
        m_nAllocated = 0;
    }

    size_t getChunkSize() const
    {
        return m_pAllocator->getChunkSize();
    }

private:
    typedef std::vector<void*> ChunkVect;

    /// The parent allocator;
    SyncSegregatedAllocatorPtr m_pAllocator;

    ChunkVect m_chunks;
    size_t m_nAllocated;
};

DEFINE_TYPED_PTR(BoundedChunkAllocator);

FX_NS_END

#endif //__FX_BOUNDEDCHUNKALLOCATOR_H
