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
// Created	: 2010-07-11 11:26:01

#ifndef __FX_MATCHEDDOC_H
#define __FX_MATCHEDDOC_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ScoreDoc.h"
#include "firtex/search/DocScoreFeature.h"

FX_NS_DEF(search);

/**
 * @class MatchedDoc
 * @brief Represent a matched doc of a query
 */
class MatchedDoc : public ScoreDoc
{
public:
    MatchedDoc(docid_t docId, score_t score, size_t nMaxNumTerms)
        : ScoreDoc(docId, score)
        , m_scoreFeature((uint32_t)nMaxNumTerms)
    {
    }

    MatchedDoc(size_t nMaxNumTerms)
        : m_scoreFeature((uint32_t)nMaxNumTerms)
    {
    }

    ~MatchedDoc(void)
    {
    }

public:
    inline void reset();
    
    inline DocScoreFeature& scoreFeature();
    inline const DocScoreFeature& scoreFeature() const;

protected:
    DocScoreFeature m_scoreFeature;    
};

////////////////////////////////////////////////
//
inline void MatchedDoc::reset()
{
    ScoreDoc::reset();
    m_scoreFeature.reset();
}

inline DocScoreFeature& MatchedDoc::scoreFeature()
{
    return m_scoreFeature;
}

inline const DocScoreFeature& MatchedDoc::scoreFeature() const
{
    return m_scoreFeature;
}

FX_NS_END

#endif //__FX_MATCHEDDOC_H
