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
#include "firtex/search/DocScoreFeature.h"

FX_NS_DEF(search);

/**
 * @class MatchedDoc
 * @brief Represent a matched doc of a query
 */
class MatchedDoc
{
public:
    MatchedDoc(docid_t docId, score_t score, size_t nMaxNumTerms)
        : m_docId(docId)
        , m_fScore(score)
        , m_scoreFeature((uint32_t)nMaxNumTerms)
    {
    }

    MatchedDoc(size_t nMaxNumTerms)
        : m_docId(INVALID_DOCID)
        , m_fScore(0.0)
        , m_scoreFeature((uint32_t)nMaxNumTerms)
    {
    }

    ~MatchedDoc(void)
    {
    }		

public:
    inline void reset();
    inline docid_t getDocId() const {return m_docId;}
    inline score_t getScore() const {return m_fScore;}

    inline void setDocId(docid_t docId) {m_docId = docId;}
    inline void setScore(score_t s) {m_fScore = s;}
    inline void addScore(score_t s) {m_fScore += s;}
    
    inline DocScoreFeature& scoreFeature();
    inline const DocScoreFeature& scoreFeature() const;

protected:
    docid_t m_docId;	///global document id
    score_t m_fScore;	///score of document
    
    DocScoreFeature m_scoreFeature;    
};

////////////////////////////////////////////////
//
inline void MatchedDoc::reset()
{
    m_docId = INVALID_DOCID;
    m_fScore = 0.0f;
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
