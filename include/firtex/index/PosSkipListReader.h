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
// Created	: 2010-08-15 14:07:54

#ifndef __FX_POSSKIPLISTREADER_H
#define __FX_POSSKIPLISTREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TypedSkipListReader.h"

FX_NS_DEF(index);

class PosSkipListReader : public TypedSkipListReader<offset_t>
{
public:
    PosSkipListReader() 
        : TypedSkipListReader<offset_t>()
    {
        m_curPoint = 0;
    }
    ~PosSkipListReader();

public:
    /**
     * Skip specified number of positions
     * @param nNumPos number of positions to skip
     * @return offset of current record
     */
    offset_t skipTo(ctf_t nNumPos);

    /**
     * Read skip data from input stream
     * @param nLevel level to read
     * @param pInStream input stream
     * @param pPointers skip pointers array
     * @param pChildOffsets child offset array, only valid when nLevel > 0
     * @param nSize size of array
     * @param bExhausted set true if encoded records are exhausted
     * @return number of elements has been read
     */
    size_t readRecord(uint32_t nLevel,
                      FX_NS(store)::InputStream* pInStream,
                      SkipPointType* pPointers,
                      offset_t* pChildOffsets,
                      uint32_t nSize, bool& bExhausted);

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PosSkipListReader);

//////////////////////////////////////////////////
//
inline offset_t PosSkipListReader::skipTo(ctf_t nNumPos)
{
//    std::cout << nNumPos << ",";
    int32_t level = 0;
    while (level < (int32_t)m_nNumSkipLevels - 1
           && nNumPos >= m_pNumSkipped[level + 1])
    {
        level++;
    }

    while (level >= 0) 
    {
        if (nNumPos >= m_pNumSkipped[level])
        {
            if (!moveToNextSkip(level))
            {
                m_pNumSkipped[level] = (uint32_t)-1;
                continue;
            }
        }
        else 
        {
            /// No more skips on this level, go down one level
            if (level > 0 && (m_nCurChildOffset + m_pStartOffsets[level - 1]) >
                m_ppSkipStreams[level - 1]->tell())
            {
                seekChild(level - 1);
            } 
            level--;
        }
    }
    return this->getCurPoint();
}

FX_NS_END

#endif //__FX_POSSKIPLISTREADER_H
