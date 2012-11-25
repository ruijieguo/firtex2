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
// Created	: 2006/1/4
//
#ifndef __FX_MULTITERMPOSTINGITERATOR_H
#define __FX_MULTITERMPOSTINGITERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "firtex/index/BarrelTermPostingIterator.h"
#include "BarrelsInfo.h"
#include "firtex/utility/PriorityQueue.h"
#include <list>

FX_NS_DEF(index)

class MultiTermPostingIterator : public TermPostingIterator
{
    struct Entry
    {
    public:
        Entry(const BarrelInfo* pBarrelInfo, TermPostingIteratorPtr pPostIter)
            : barrelInfo(pBarrelInfo)
            , postingIterator(pPostIter)
        {
        }

    public:
        const BarrelInfo* barrelInfo;
        TermPostingIteratorPtr postingIterator;
    };

    DEFINE_TYPED_PTR(Entry);

    typedef std::list<EntryPtr> PostingIteratorList;

    struct QueueComparator
    {
        bool operator () (Entry* o1, Entry* o2) 
        {
            return (o1->barrelInfo->getBaseDocId() + o1->postingIterator->doc()) 
                < (o2->barrelInfo->getBaseDocId() + o2->postingIterator->doc());
        }
    };

    class Queue : public FX_NS(utility)::PriorityQueue<Entry*, QueueComparator>
    {
    public:
        Queue(size_t size) : FX_NS(utility)::PriorityQueue<Entry*, QueueComparator>(size, false)
        {
        }
    };

    DEFINE_TYPED_PTR(Queue);

public:
    const static size_t DEFAULT_BUFFER_SIZE = BarrelTermPostingIterator::DEFAULT_BUFFER_SIZE;

public:
    MultiTermPostingIterator(void);
    virtual ~MultiTermPostingIterator(void);

public:
    /**
     * Add a term posting iterator
     * @param pBarrelInfo information of index barrel
     * @param pPostingIter term docs&freqs of a index barrel
     */
    void add(const BarrelInfo* pBarrelInfo, const TermPostingIteratorPtr& pPostingIter);

    /**
     * Initialize
     */
    void init();

    /**
     * Return number of posting iterators
     */
    size_t size() const {return m_iterators.size();}

public:
    /**
     * Get term meta
     */
    const TermMeta& getTermMeta() const;

    /**
     * Move to the next documents block
     * @param docs documents block
     * @param freqs term frequency block
     * @return number of documents in the block
     */			
    df_t nextDocs(docid_t*& docs, tf_t*& freqs);

    /** 
     * skip to document whose id is equal to or
     *  greater than <i>docId</i>
     */ 
    docid_t skipTo(docid_t docId);
	
    /**
     * Get current doc id. Only valid after 
     * calling {@link skipTo()} 
     * @return current doc id
     */
    docid_t doc() const;

    /**
     * Get current in document tf. Only valid after 
     * calling {@link skipTo()} 
     * @return current in document tf
     */
    tf_t freq();

    /**
     * Move to next position block
     * @param positions position block
     * @return size of position block
     */
    tf_t nextPositions(loc_t*& positions);

    /**	 
     * Skip to position which is equal to or 
     * greater than <i>pos</i>
     */ 
    loc_t skipToPosition(loc_t pos);

    /**
     * Return posting decoder
     */
    PostingDecoderPtr getPostingDecoder();

protected:
    PostingIteratorList m_iterators;
    Entry* m_pCurrent;
    QueuePtr m_pPostingIterQueue;
    PostingDecoderPtr m_pPostingDecoder;

    TermMeta m_termMeta;
    
    docid_t m_docBuffer[DEFAULT_BUFFER_SIZE];

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
