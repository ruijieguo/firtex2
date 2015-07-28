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
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/7/20
//
#ifndef _MULTITERMITERATOR_H
#define _MULTITERMITERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TermIterator.h"
#include "firtex/utility/PriorityQueue.h"
#include "firtex/index/TermInfo.h"
#include "firtex/index/BarrelsInfo.h"
#include <vector>

FX_NS_DEF(index);

class MultiTermIterator : public TermIterator
{
    struct Entry
    {
    public:
        Entry(const BarrelInfo* pBarrelInfo, const TermIteratorPtr& iter)
            : barrelInfo(pBarrelInfo)
            , termIterator(iter)
        {
        }

        ~Entry()
        {
            barrelInfo = NULL;
        }

        bool next()
        {
            if (!termIterator)
                return false;
            bool ret = termIterator->hasNext();
            if (ret == false)
                return false;
            termEntry = termIterator->next();
            return true;
        }

    public:
        const BarrelInfo* barrelInfo;
        TermIteratorPtr termIterator;
        TermIterator::TermEntry termEntry;
    };
    DEFINE_TYPED_PTR(Entry);

    static bool SORT_CMP(const Entry* e1, const Entry* e2)
    {
        return e1->barrelInfo->getBaseDocId() < e2->barrelInfo->getBaseDocId();
    }

    struct IteratorQueueComparator
    {
        bool operator () (Entry* o1, Entry* o2) 
        {
            return (o1->termEntry.term->compare(o2->termEntry.term) < 0);
        }
    };

    class IteratorQueue : public FX_NS(utility)::PriorityQueue<Entry*, IteratorQueueComparator>
    {
    public:
        IteratorQueue(size_t size) 
            : FX_NS(utility)::PriorityQueue<Entry*, IteratorQueueComparator>(size, false)
        {
        }
    };
    DEFINE_TYPED_PTR(IteratorQueue);

    typedef std::vector<EntryPtr> IteratorVector;

public:
    MultiTermIterator(void);
    virtual ~MultiTermIterator(void);

public:
    /**
     * add term iterator
     * @param iter term iterator,internal object
     */
    void addIterator(const BarrelInfo* pBarrelInfo, const TermIteratorPtr& iter);

    /**
     * Initialize
     */
    void init();

public:
    /**
     * Check if the iterator has next element or not
     */
    bool hasNext();
    
    /**
     * Return the next element
     * Example code:
     * 
     *   while (hasNext())
     *   {
     *      TermEntry t = next();
     *   }
     */
    TermEntry next();

    /**
     * Return number of terms
     */
    int64_t size() const;

private:
    typedef std::vector<Entry*> EntryVect;

    IteratorQueuePtr m_pItersQueue;
    IteratorVector m_iterators;
    EntryVect m_entryHolder;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
