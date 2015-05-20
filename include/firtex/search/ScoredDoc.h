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
#include "firtex/search/ScoreDoc.h"

FX_NS_DEF(search);

#pragma pack(push, 1)

/**
 * @class ScoredDoc
 * @brief Represent scored document with score value
 */
class ScoredDoc : public ScoreDoc
{
public:
    ScoredDoc(docid_t docId, score_t score)
        : ScoreDoc(docId, score)
        , m_uDataSize(0)
    {
    }

    ScoredDoc(docid_t docId, score_t score, uint32_t uDataSize)
        : ScoreDoc(docId, score)
        , m_uDataSize(uDataSize)
    {
    }

    ~ScoredDoc(void)
    {
    }

public:
    template<typename T>
    inline T& getCustomValue(uint32_t uValIdx);
    template<typename T>
    inline const T& getCustomValue(uint32_t uValIdx) const;

private:
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
