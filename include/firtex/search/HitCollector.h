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
// Created	: 2010-07-11 22:17:29

#ifndef __FX_HITCOLLECTOR_H
#define __FX_HITCOLLECTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/MatchedDocSet.h"
#include "firtex/search/HitQueue.h"
#include "firtex/search/Filter.h"

FX_NS_DEF(search);

class HitCollector
{
public:
    HitCollector() {}
    virtual ~HitCollector() {}

public:
    /**
     * Collect top docs
     * @param matchedDocs matched documents
     */
    virtual void collect(MatchedDocSet& matchedDocs) = 0;
    
    /**
     * Return total hits
     */
    virtual size_t totalHits() const = 0;

    /**
     * Return hit queue
     */
    virtual HitQueuePtr getHitQueue() const = 0;

public:
    /**
     * Get/set filter
     */
    inline void setFilter(const FilterPtr& pFilter);
    inline FilterPtr getFilter() const;

    /**
     * Determin the document filtered or not
     */
    inline bool isFiltered(const ScoredDoc& scoredDoc);

private:
    FilterPtr m_pFilter;
};

DEFINE_TYPED_PTR(HitCollector);

//////////////////////////////////////
//
inline void HitCollector::setFilter(const FilterPtr& pFilter)
{
    m_pFilter = pFilter;
}

inline FilterPtr HitCollector::getFilter() const
{
    return m_pFilter;
}

inline bool HitCollector::isFiltered(const ScoredDoc& scoredDoc)
{
    return (m_pFilter.isNull() || m_pFilter->isFiltered(scoredDoc));
}

FX_NS_END

#endif //__FX_HITCOLLECTOR_H
