//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2013-01-26 16:07:10

#ifndef __FX_SCOREDOC_H
#define __FX_SCOREDOC_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

class ScoreDoc
{
public:
    ScoreDoc(docid_t docId, score_t score)
        : m_docId(docId)
        , m_fScore(score)
    {}

    ScoreDoc()
        : m_docId(INVALID_DOCID)
        , m_fScore(0.0f)
    {}

    ~ScoreDoc() {}

public:
    inline docid_t getDocId() const {return m_docId;}
    inline score_t getScore() const {return m_fScore;}

    inline void setDocId(docid_t docId) {m_docId = docId;}
    inline void setScore(score_t s) {m_fScore = s;}
    inline void addScore(score_t s) {m_fScore += s;}

    inline void reset();

private:
    docid_t m_docId;
    score_t m_fScore;
};

////////////////////////////////////////////
inline void ScoreDoc::reset()
{
    m_docId = INVALID_DOCID;
    m_fScore = 0.0f;
}

FX_NS_END

#endif //__FX_SCOREDOC_H
