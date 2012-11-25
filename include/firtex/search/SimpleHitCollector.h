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
// Created	: 2010-07-11 22:20:31

#ifndef __FX_SIMPLEHITCOLLECTOR_H
#define __FX_SIMPLEHITCOLLECTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/HitCollector.h"
#include "firtex/search/HitQueue.h"

FX_NS_DEF(search);

class SimpleHitCollector : public HitCollector
{
public:
    SimpleHitCollector(size_t nTopDocs,
                       score_t minScore = -1.0f);
    ~SimpleHitCollector();

public:
    /**
     * Collect top docs
     * @param matchedDocs matched documents
     */
    void collect(MatchedDocSet& matchedDocs);

    /**
     * Return total hits
     */
    size_t totalHits() const;

    /**
     * Return hit queue
     */
    HitQueuePtr getHitQueue() const;

protected:
    ScoredDocAllocatorPtr m_pAllocator;
    SimpleHitQueuePtr m_pHitQueue;
    size_t m_nTotalHits;
    size_t m_nTopDocs;
    score_t m_minScore;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(SimpleHitCollector);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_SIMPLEHITCOLLECTOR_H
