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
// Created	: 2010-07-11 10:58:00

#ifndef __FX_SCOREDDOC_H
#define __FX_SCOREDDOC_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

#pragma pack(push, 1)

/**
 * @class ScoredDoc
 * @brief Represent scored document with score value
 */
class ScoredDoc
{
public:
    ScoredDoc(docid_t docId, float score)
        : m_docId(docId)
        , m_fScore(score)
        , m_uDataSize(0)
    {
    }

    ScoredDoc(docid_t docId, float score, uint32_t uDataSize)
        : m_docId(docId)
        , m_fScore(score)
        , m_uDataSize(uDataSize)
    {
    }

    ~ScoredDoc(void)
    {
    }

public:
    inline docid_t getDocId() const {return m_docId;}
    inline float getScore() const {return m_fScore;}

    inline void setDocId(docid_t docId) {m_docId = docId;}
    inline void setScore(float s) {m_fScore = s;}

    template<typename T>
    inline T& getCustomValue(uint32_t uValIdx);
    template<typename T>
    inline const T& getCustomValue(uint32_t uValIdx) const;

public:
    docid_t m_docId;
    float m_fScore;
    uint32_t m_uDataSize;
    uint8_t m_customData[1];
};

#pragma pack(pop)

////////////////////////////////////////////////
//
template<typename T>
inline T& ScoredDoc::getCustomValue(uint32_t uValIdx)
{
    FIRTEX_ASSERT2(uValIdx < m_uDataSize);
    return *(T*)(m_customData + uValIdx);
}

template<typename T>
inline const T& ScoredDoc::getCustomValue(uint32_t uValIdx) const
{
    FIRTEX_ASSERT2(uValIdx < m_uDataSize);
    return *(T*)(m_customData + uValIdx);
}

FX_NS_END

#endif //__FX_SCOREDDOC_H
