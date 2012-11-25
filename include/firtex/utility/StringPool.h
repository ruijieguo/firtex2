//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-03-19 11:37:08

#ifndef __FX_STRINGPOOL_H
#define __FX_STRINGPOOL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(utility);

class StringPool
{
public:
    StringPool(size_t size) 
        : m_nPoolSize(size)
        , m_nCursor(0)
        , m_nStringCount(0)
        , m_bPoolOwner(true)
    {
        m_pool = new char[size];
    }

    StringPool(char* pool, size_t size) 
        : m_pool(pool)
        , m_nPoolSize(size)
        , m_nCursor(0)
        , m_nStringCount(0)
        , m_bPoolOwner(false)
    {
        if (!pool)
        {
            m_pool = new char[size];
            m_bPoolOwner = true;
        }
    }

    // StringPool(const StringPool& src) 
    //     : m_nPoolSize(src.m_nPoolSize)
    //     , m_nCursor(src.m_nCursor)
    //     , m_nStringCount(src.m_nStringCount)
    // {
    //     if (src.m_bPoolOwner)
    //     {
    //         m_pool = new char[m_nPoolSize];
    //         m_bPoolOwner = true;
    //     }
    //     else
    //     {
    //         m_pool = src.m_pool;
    //         m_bPoolOwner = false;
    //     }

    //     memcpy(m_pool, src.m_pool, m_nPoolSize);
    // }

    ~StringPool()
    {
        clear();

        if (m_bPoolOwner)
        {
            delete[] m_pool;
        }
        m_pool = NULL;
        m_nPoolSize = m_nCursor = 0;
    }

private:
    StringPool(const StringPool& src);
    StringPool& operator = (const StringPool& src);

public:
    size_t getStringCount() const 
    {
        return m_nStringCount;
    }

    const char* addString(const char* token, size_t length)
    {
        ensure(m_nCursor + length + 1);

        char* pStartPos = m_pool + m_nCursor;
        strncpy(pStartPos, token, length);
        m_nCursor += (length + 1);
        m_pool[m_nCursor - 1] = '\0';
        ++m_nStringCount;
        return pStartPos;
    }

    void ensure(size_t nEnsureSize)
    {
        if (m_nPoolSize >= nEnsureSize)
        {
            return;
        }

        if (m_bPoolOwner == false)
        {
            FIRTEX_THROW(RuntimeException, "Not the owner of pool");
        }

        size_t nNewSize = (m_nPoolSize << 1);
        if (nNewSize < nEnsureSize)
        {
            nNewSize = nEnsureSize;
        }
        
        char* newBuffer = new char[nNewSize];
        m_nPoolSize = (uint32_t)nNewSize;
        m_nCursor = 0;

        m_pools.push_back(m_pool);
        m_pool = newBuffer;
    }

    void clear()
    {
        m_nCursor = 0;
        m_nStringCount = 0;
        
        for (size_t i = 0; i < m_pools.size(); ++i)
        {
            delete[] m_pools[i];
        }
        m_pools.clear();
    }

    void attach(char* pool, size_t size, size_t tokenCount)
    {
        clear();

        if (m_bPoolOwner)
        {
            delete[] m_pool;
        }
        m_pool = pool;
        m_nCursor = m_nPoolSize = size;
        m_nStringCount = tokenCount;
        m_bPoolOwner = false;
    }

    // StringPool* clone() const
    // {
    //     return new StringPool(*this);
    // }

public:
    // class Iterator
    // {
    // public:
    //     Iterator(const StringPool* pPool)
    //         : m_pool(pPool)
    //         , m_nCursor(0)
    //     {
    //     }
        
    //     Iterator(const Iterator& src)
    //         : m_pool(src.m_pool)
    //         , m_nCursor(src.m_nCursor)
    //     {
    //     }

    //     ~Iterator(void)
    //     {
    //     }

    // public:
    //     void reset()
    //     {
    //         m_nCursor = 0;
    //     }

    //     bool hasNext() const
    //     {
    //         return (m_nCursor < m_pool->m_nCursor);
    //     }

    //     const char* next(size_t& len)
    //     {
    //         const char* pStringStart = m_pool->m_pool + m_nCursor;
    //         len = FX_NS(utility)::strLength(pStringStart);
    //         m_nCursor += (len + 1);
    //         return pStringStart;
    //     }

    //     size_t size() const
    //     {
    //         return m_pool->getStringCount();
    //     }

    // protected:
    //     const StringPool* m_pool;
    //     size_t m_nCursor;
    // };

    // Iterator iterator() const
    // {
    //     return Iterator(this);
    // }

private:
    typedef std::vector<char*> PoolVect;

    PoolVect m_pools;

    char* m_pool;
    size_t m_nPoolSize;
    size_t m_nCursor;
    size_t m_nStringCount;
    bool m_bPoolOwner;
};

FX_NS_END

#endif //__FX_STRINGPOOL_H
