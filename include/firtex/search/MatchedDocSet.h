//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-07-11 17:09:29

#ifndef __FX_MATCHEDDOCSET_H
#define __FX_MATCHEDDOCSET_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/MatchedDoc.h"
#include "firtex/search/QueryTracer.h"
#include "firtex/utility/Pool.h"
#include <vector>

FX_NS_DEF(search);

class MatchedDocSet
{
public:
    MatchedDocSet(FX_NS(utility)::PoolPtr& pMemPool,
                  size_t nCapacity, size_t nNumOfTerms)
        : m_pMemPool(pMemPool)
        , m_nNumOfTermFeatures(nNumOfTerms)
    {
        resize(nCapacity);
    }

    ~MatchedDocSet() 
    {
    }

public:
    void reset();
    size_t size() const;
    void resize(size_t nNewSize);
    void reserve(size_t nCapacity);
    size_t capacity() const;

    QueryTracerPtr& getTracer();
    void setTracer(const QueryTracerPtr& pTracer);

    MatchedDoc& operator [] (size_t n);
    const MatchedDoc& operator [] (size_t n) const;

protected:
    typedef std::vector<MatchedDoc*> Vector;
    FX_NS(utility)::PoolPtr m_pMemPool;
    QueryTracerPtr m_pQueryTracer;
    Vector m_matchedDocs;
    size_t m_nNumOfTermFeatures;
    
private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////////////////////
//
inline void MatchedDocSet::reset()
{
    for (size_t i = 0; i < m_matchedDocs.size(); ++i)
    {
        m_matchedDocs[i]->reset();
    }
}

inline size_t MatchedDocSet::size() const
{
    return m_matchedDocs.size();
}

inline void MatchedDocSet::resize(size_t nNewSize)
{
    size_t nOldSize = m_matchedDocs.size();
    m_matchedDocs.resize(nNewSize);
    for (size_t i = nOldSize; i < nNewSize; ++i)
    {
        size_t nAllocSize = sizeof(MatchedDoc) + (m_nNumOfTermFeatures - 1)
                            * sizeof(TermScoreFeature);
        void* p = m_pMemPool->allocate(nAllocSize);
        m_matchedDocs[i] = new (p) MatchedDoc(m_nNumOfTermFeatures);
    }
}

inline void MatchedDocSet::reserve(size_t nCapacity)
{
    m_matchedDocs.reserve(nCapacity);
}

inline size_t MatchedDocSet::capacity() const
{
    return m_matchedDocs.capacity();
}

inline QueryTracerPtr& MatchedDocSet::getTracer()
{
    return m_pQueryTracer;
}
 
inline void MatchedDocSet::setTracer(const QueryTracerPtr& pTracer)
{
    m_pQueryTracer = pTracer;
}

inline MatchedDoc& MatchedDocSet::operator [] (size_t n)
{
    return *m_matchedDocs[n];
}

inline const MatchedDoc& MatchedDocSet::operator [] (size_t n) const
{
    return *m_matchedDocs[n];
}

FX_NS_END

#endif //__FX_MATCHEDDOCSET_H
