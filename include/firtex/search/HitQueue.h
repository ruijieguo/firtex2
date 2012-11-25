//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/7/15
//
#ifndef _HITQUEUE_H
#define _HITQUEUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/utility/PriorityQueue.h"
#include "firtex/index/IndexReader.h"
#include "firtex/search/Sorter.h"
#include "firtex/search/ScoredDoc.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/SimpleSegregatedAllocator.h"

FX_NS_DEF(search);

typedef FX_NS(utility)::SimpleSegregatedAllocator ScoredDocAllocator;
typedef FX_NS(utility)::SimpleSegregatedAllocatorPtr ScoredDocAllocatorPtr;

struct ScoredDocDestroyer
{
    ScoredDocDestroyer(ScoredDocAllocatorPtr& pAllocator)
        : m_pAllocator(pAllocator)
    {
    }

    void operator() (ScoredDoc*& pDoc)
    {
        pDoc->~ScoredDoc();
        m_pAllocator->free(pDoc);
        pDoc = NULL;
    }

private:
    ScoredDocAllocatorPtr m_pAllocator;
};

DEFINE_TYPED_PTR(ScoredDocDestroyer);

struct HitQueueComparator
{
    bool operator () (ScoredDoc* pDoc1, ScoredDoc* pDoc2)
    {
        if (pDoc1->getScore() == pDoc2->getScore())
        {
            return pDoc1->getDocId() > pDoc2->getDocId();
        }
        else
        {
            return pDoc1->getScore() < pDoc2->getScore();
        }
    }
};

class HitQueue
{
public:
    HitQueue(ScoredDocDestroyerPtr& pDestroyer)
        : m_pDestryer(pDestroyer)
    {}
    virtual ~HitQueue() {}

public:
    void freeScoredDoc(ScoredDoc*& pDoc)
    {
        (*m_pDestryer)(pDoc);
    }

public:
    /// Return size of the queue
    virtual size_t size() const = 0;

    /// pop top element
    virtual ScoredDoc* pop() = 0;

private:
    ScoredDocDestroyerPtr m_pDestryer;
};
DEFINE_TYPED_PTR(HitQueue);

class SimpleHitQueue : public HitQueue
{
public:
    typedef FX_NS(utility)::PriorityQueue<ScoredDoc*, HitQueueComparator, ScoredDocDestroyer> Queue;

public:
    SimpleHitQueue(size_t maxSize, ScoredDocDestroyerPtr& pDestroyer)
        : HitQueue(pDestroyer)
        , m_queue(maxSize, true, pDestroyer.get())
    {}

    virtual~SimpleHitQueue()
    {
        m_queue.clear();
    }

public:
    /// Return size of the queue
    inline virtual size_t size() const
    {
        return m_queue.size();
    }

    /// pop top element
    inline virtual ScoredDoc* pop()
    {
        return m_queue.pop();
    }

public:
    /// insert element
    inline bool insert(ScoredDoc* pDoc)
    {
        return m_queue.insert(pDoc);
    }

    /// insert element
    inline ScoredDoc* top() const
    {
        return m_queue.top();
    }

private:
    Queue m_queue;
};

DEFINE_TYPED_PTR(SimpleHitQueue);

struct SortedHitQueueCompartor
{
    SortedHitQueueCompartor(const SorterPtr& pSort)
        : m_pSort(pSort)
    {}

    bool operator () (ScoredDoc* pDoc1, ScoredDoc* pDoc2)
    {
        return m_pSort->lessThan(pDoc1, pDoc2);
    }

private:
    SorterPtr m_pSort;
};

class SortedHitQueue : public HitQueue
{
public:
    typedef FX_NS(utility)::PriorityQueue<ScoredDoc*, SortedHitQueueCompartor, ScoredDocDestroyer> Queue;

public:
    SortedHitQueue(const SorterPtr& pSort, size_t maxSize,
                   ScoredDocDestroyerPtr& pDestroyer)
        : HitQueue(pDestroyer)
        , m_queue(maxSize, true, pDestroyer.get(), SortedHitQueueCompartor(pSort))
        , m_pSort(pSort)
    {
    }

    ~SortedHitQueue() {}

public:
    /// Return size of the queue
    inline virtual size_t size() const
    {
        return m_queue.size();
    }

    /// pop top element
    inline virtual ScoredDoc* pop()
    {
        return m_queue.pop();
    }

public:
    /// insert element
    inline bool insert(ScoredDoc* pDoc)
    {
        return m_queue.insert(pDoc);
    }

    /// insert element
    inline ScoredDoc* top() const
    {
        return m_queue.top();
    }

private:
    Queue m_queue;
    SorterPtr m_pSort;
};

DEFINE_TYPED_PTR(SortedHitQueue);

FX_NS_END


#endif
