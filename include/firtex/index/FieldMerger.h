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
// Created	: 2006/2/23
//

#ifndef __FX_FIELDMERGER_H
#define __FX_FIELDMERGER_H

#include "firtex/index/FieldIndexer.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/TermIterator.h"
#include "firtex/index/TermReader.h"
#include "firtex/store/FileSystem.h"
#include "firtex/utility/PriorityQueue.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/MergeInfo.h"
#include "firtex/index/DocIdRecycling.h"
#include "firtex/document/FieldSchema.h"
#include <string>
#include <vector>

FX_NS_DEF(index);

class MergingTerm
{
public:
    MergingTerm(const IndexMergeInfo& mergeFieldInfo)
        : m_mergeInfo(mergeFieldInfo)
    {
        TermReaderPtr pTermReader = m_mergeInfo.getTermReader();
        m_pIterator = pTermReader->termIterator(
                pTermReader->getFieldSchema()->getName());
    }

    ~MergingTerm()
    {
    }

public:
    inline bool next()
    {
        if (!m_pIterator.isNull() && m_pIterator->hasNext())
        {
            m_curEntry = m_pIterator->next();
            return true;
        }
        return false;
    }

    inline const IndexMergeInfo& getIndexMergeInfo() const
    {
        return m_mergeInfo;
    }

    inline const Term* getTerm() const 
    {
        return m_curEntry.term;
    }

    inline TermPostingIteratorPtr getPostingIterator() const 
    {
        return m_curEntry.postingIterator;
    }

    inline TermIteratorPtr getTermIterator() const 
    {
        return m_pIterator;
    }

    inline docid_t getNewBaseDocId() const 
    {
        return m_mergeInfo.getNewBaseDocId();
    }

    inline docid_t getBaseDocId() const 
    {
        return m_mergeInfo.getBarrelInfo().getBaseDocId();
    }

    inline const BarrelInfo& getBarrelInfo() const 
    {
        return m_mergeInfo.getBarrelInfo();
    }

    inline const DocIdRecycling* getDocIdRecycling() const 
    {
        return m_mergeInfo.getDocIdRecycling();
    }

private:
    IndexMergeInfo m_mergeInfo;
    TermIteratorPtr m_pIterator;
    TermIterator::TermEntry m_curEntry;
};

DEFINE_TYPED_PTR(MergingTerm);

typedef std::vector<const MergingTerm*> MergingTermVector;

struct FieldMergeComparator
{ 
    bool operator() (MergingTerm* a, MergingTerm* b)
    {
        int32_t ret = a->getTerm()->compare(b->getTerm());
        if (ret == 0)
        {
            return (a->getBaseDocId() < b->getBaseDocId());
        }
        return (ret < 0);
    }
};

class FieldMergeQueue : public FX_NS(utility)::PriorityQueue<MergingTerm*, FieldMergeComparator>
{
public:
    FieldMergeQueue(size_t maxSize)
        : FX_NS(utility)::PriorityQueue<MergingTerm*, FieldMergeComparator>(maxSize, true)
    {
    }
    ~FieldMergeQueue(){}
};

DEFINE_TYPED_PTR(FieldMergeQueue);

class FieldComponentCreator;

class FieldMerger
{
public:
    FieldMerger();
    virtual ~FieldMerger(void);

public:
    /**
     * Initialize 
     * @param pFileSys file system instance
     * @param pFieldSchema field info
     */
    virtual void init(const FX_NS(store)::FileSystemPtr& pFileSys,
                      const FX_NS(document)::FieldSchema* pFieldSchema);

    /**
     * Merge multi index barrel of one field to one barrel
     * @param mergeFieldInfos field infos of index barrel need merging
     * @param fieldMeta merged field meta
     */
    virtual void merge(const IndexMergeInfos& mergeFieldInfos,
                       FieldMeta& fieldMeta);

    /**
     * Return total number of merged terms
     * @return number of merged terms
     */
    int64_t numMergedTerms() const {return m_nMergedTerms;}

protected:
    /** Initialize merge queue */
    bool initQueue(const IndexMergeInfos& mergeFieldInfos);

protected:
    /**
     * Give dirived class a chance to know merging is begin
     * @param mergeFieldInfos field infos of multi index barrel
     */
    virtual void beginMerge(const IndexMergeInfos& mergeFieldInfos) = 0;

    /**
     * Merge some terms inn different barrel
     * @param terms terms vectory to merge
     */
    virtual void mergeTerms(const MergingTermVector& terms) = 0;

    /**
     * Give dirived class a chance to know merging is ended
     */
    virtual void endMerge() = 0;

public:
    FX_NS(store)::FileSystemPtr getFileSystem() const;
    const FX_NS(document)::FieldSchema* getFieldSchema() const;

protected:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    const FX_NS(document)::FieldSchema* m_pFieldSchema;
    FieldMergeQueuePtr m_pMergeQueue;

    int64_t m_nMergedTerms;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(FieldMerger);

////////////////////////////////////////////
//
inline FX_NS(store)::FileSystemPtr FieldMerger::getFileSystem() const
{
    return m_pFileSys;
}

inline const FX_NS(document)::FieldSchema* FieldMerger::getFieldSchema() const
{
    return m_pFieldSchema;
}

FX_NS_END

#endif

