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
// Created	: 2010-08-07 16:10:06

#ifndef __FX_S16CODING_H
#define __FX_S16CODING_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/compress/Coding.h"
#include <assert.h>

FX_NS_DEF(index);

/**
 * @class S16Coding
 * @brief Implementation of Simple16 integer compression
 */
class S16Coding : public Coding
{
public:
    S16Coding();
    ~S16Coding();

public:
    /**
     * Encode uint32 array
     * @param dest bytes array to encode to
     * @param nDestLen length of dest
     * @param src source uint32 array to encode
     * @param nSrcLen length of source array
     * @param bSeal seal the last record if true
     * @return encoded length in uint32 size
     */
    size_t encode(uint32_t* dest, size_t nDestLen,
                  const uint32_t* src, size_t nSrcLen,
                  bool bSeal) const;

    /**
     * Decode uint32 array
     * @param dest bytes array to decode to
     * @param nDestLen length of dest
     * @param src source uint32 array to decode
     * @param nSrcLen length of source array
     * @param bExhausted set true if encoded records are exhausted
     * @return number of decoded uint32 elements
     */
    size_t decode(uint32_t* dest, size_t nDestLen,
                  const uint32_t* src, size_t nSrcLen,
                  bool& bExhausted) const;

public:
    inline size_t doEncode(uint32_t* dest, size_t nDestLen, 
                           const uint32_t* src, size_t nSrcLen) const;
    inline size_t doDecode(uint32_t* dest, size_t nDestLen,
                           const uint32_t* src, size_t nSrcLen) const;

protected:
    uint32_t PACK_COUNT[16];
    static uint32_t MASK[16][28];
};

//////////////////////////////////////////////////////////////
//
inline size_t S16Coding::doEncode(uint32_t* dest, size_t nDestLen, 
                                  const uint32_t* src, size_t nSrcLen) const
{
    uint32_t* p = dest;
    for (size_t off = 0; off < nSrcLen; )
    {
        uint32_t i, j, m, k;
        for (i = 0; i < 16; i++)
        {
            *p = (i << 28);
            m = PACK_COUNT[i];
            if (nSrcLen - off < m)
            {
                continue;
            }

            for (j = 0, k = 0; (j < m) && (*(src + j) < (uint32_t)(1 << MASK[i][j])); )
            {
                *p += ((*(src + j)) << k);
                k += MASK[i][j];
                j++;
            }
            if (j == m)
            {
                src += m;
                ++p;
                break;
            }
        }
        FIRTEX_ASSERT2(i <= 15);
        off += m;
    }
    FIRTEX_ASSERT2((size_t)(p - dest) <= nDestLen);
    return (p - dest);
}

inline size_t S16Coding::doDecode(uint32_t* dest, size_t nDestLen,
                                  const uint32_t* src, size_t nSrcLen) const
{
    size_t nDecoded = 0;
    for (size_t i = 0; i < nSrcLen; ++i, ++src)
    {
        uint32_t v = *src;
        uint32_t k = (v) >> 28;
        switch(k)
        {
        case 0:
            dest[0] = (v) & 1;
            dest[1] = (v >> 1) & 1;
            dest[2] = (v >> 2) & 1;
            dest[3] = (v >> 3) & 1;
            dest[4] = (v >> 4) & 1;
            dest[5] = (v >> 5) & 1;
            dest[6] = (v >> 6) & 1;
            dest[7] = (v >> 7) & 1;
            dest[8] = (v >> 8) & 1;
            dest[9] = (v >> 9) & 1;
            dest[10] = (v >> 10) & 1;
            dest[11] = (v >> 11) & 1;
            dest[12] = (v >> 12) & 1;
            dest[13] = (v >> 13) & 1;
            dest[14] = (v >> 14) & 1;
            dest[15] = (v >> 15) & 1;
            dest[16] = (v >> 16) & 1;
            dest[17] = (v >> 17) & 1;
            dest[18] = (v >> 18) & 1;
            dest[19] = (v >> 19) & 1;
            dest[20] = (v >> 20) & 1;
            dest[21] = (v >> 21) & 1;
            dest[22] = (v >> 22) & 1;
            dest[23] = (v >> 23) & 1;
            dest[24] = (v >> 24) & 1;
            dest[25] = (v >> 25) & 1;
            dest[26] = (v >> 26) & 1;
            dest[27] = (v >> 27) & 1;
            break;
        case 1:
            dest[0] = (v) & 3;
            dest[1] = (v >> 2) & 3;
            dest[2] = (v >> 4) & 3;
            dest[3] = (v >> 6) & 3;
            dest[4] = (v >> 8) & 3;
            dest[5] = (v >> 10) & 3;
            dest[6] = (v >> 12) & 3;
            dest[7] = (v >> 14) & 1;
            dest[8] = (v >> 15) & 1;
            dest[9] = (v >> 16) & 1;
            dest[10] = (v >> 17) & 1;
            dest[11] = (v >> 18) & 1;
            dest[12] = (v >> 19) & 1;
            dest[13] = (v >> 20) & 1;
            dest[14] = (v >> 21) & 1;
            dest[15] = (v >> 22) & 1;
            dest[16] = (v >> 23) & 1;
            dest[17] = (v >> 24) & 1;
            dest[18] = (v >> 25) & 1;
            dest[19] = (v >> 26) & 1;
            dest[20] = (v >> 27) & 1;
            break;
        case 2:
            dest[0] = (v) & 1;
            dest[1] = (v >> 1) & 1;
            dest[2] = (v >> 2) & 1;
            dest[3] = (v >> 3) & 1;
            dest[4] = (v >> 4) & 1;
            dest[5] = (v >> 5) & 1;
            dest[6] = (v >> 6) & 1;
            dest[7] = (v >> 7) & 3;
            dest[8] = (v >> 9) & 3;
            dest[9] = (v >> 11) & 3;
            dest[10] = (v >> 13) & 3;
            dest[11] = (v >> 15) & 3;
            dest[12] = (v >> 17) & 3;
            dest[13] = (v >> 19) & 3;
            dest[14] = (v >> 21) & 1;
            dest[15] = (v >> 22) & 1;
            dest[16] = (v >> 23) & 1;
            dest[17] = (v >> 24) & 1;
            dest[18] = (v >> 25) & 1;
            dest[19] = (v >> 26) & 1;
            dest[20] = (v >> 27) & 1;
            break;
        case 3:
            dest[0] = (v) & 1;
            dest[1] = (v >> 1) & 1;
            dest[2] = (v >> 2) & 1;
            dest[3] = (v >> 3) & 1;
            dest[4] = (v >> 4) & 1;
            dest[5] = (v >> 5) & 1;
            dest[6] = (v >> 6) & 1;
            dest[7] = (v >> 7) & 1;
            dest[8] = (v >> 8) & 1;
            dest[9] = (v >> 9) & 1;
            dest[10] = (v >> 10) & 1;
            dest[11] = (v >> 11) & 1;
            dest[12] = (v >> 12) & 1;
            dest[13] = (v >> 13) & 1;
            dest[14] = (v >> 14) & 3;
            dest[15] = (v >> 16) & 3;
            dest[16] = (v >> 18) & 3;
            dest[17] = (v >> 20) & 3;
            dest[18] = (v >> 22) & 3;
            dest[19] = (v >> 24) & 3;
            dest[20] = (v >> 26) & 3;
            break;
        case 4:
            dest[0] = (v) & 3;
            dest[1] = (v >> 2) & 3;
            dest[2] = (v >> 4) & 3;
            dest[3] = (v >> 6) & 3;
            dest[4] = (v >> 8) & 3;
            dest[5] = (v >> 10) & 3;
            dest[6] = (v >> 12) & 3;
            dest[7] = (v >> 14) & 3;
            dest[8] = (v >> 16) & 3;
            dest[9] = (v >> 18) & 3;
            dest[10] = (v >> 20) & 3;
            dest[11] = (v >> 22) & 3;
            dest[12] = (v >> 24) & 3;
            dest[13] = (v >> 26) & 3;
            break;
        case 5:
            dest[0] = (v) & 15;
            dest[1] = (v >> 4) & 7;
            dest[2] = (v >> 7) & 7;
            dest[3] = (v >> 10) & 7;
            dest[4] = (v >> 13) & 7;
            dest[5] = (v >> 16) & 7;
            dest[6] = (v >> 19) & 7;
            dest[7] = (v >> 22) & 7;
            dest[8] = (v >> 25) & 7;
            break;
        case 6:
            dest[0] = (v) & 7;
            dest[1] = (v >> 3) & 15;
            dest[2] = (v >> 7) & 15;
            dest[3] = (v >> 11) & 15;
            dest[4] = (v >> 15) & 15;
            dest[5] = (v >> 19) & 7;
            dest[6] = (v >> 22) & 7;
            dest[7] = (v >> 25) & 7;
            break;
        case 7:
            dest[0] = (v) & 15;
            dest[1] = (v >> 4) & 15;
            dest[2] = (v >> 8) & 15;
            dest[3] = (v >> 12) & 15;
            dest[4] = (v >> 16) & 15;
            dest[5] = (v >> 20) & 15;
            dest[6] = (v >> 24) & 15;
            break;
        case 8:
            dest[0] = (v) & 31;
            dest[1] = (v >> 5) & 31;
            dest[2] = (v >> 10) & 31;
            dest[3] = (v >> 15) & 31;
            dest[4] = (v >> 20) & 15;
            dest[5] = (v >> 24) & 15;
            break;
        case 9:
            dest[0] = (v) & 15;
            dest[1] = (v >> 4) & 15;
            dest[2] = (v >> 8) & 31;
            dest[3] = (v >> 13) & 31;
            dest[4] = (v >> 18) & 31;
            dest[5] = (v >> 23) & 31;
            break;
        case 10:
            dest[0] = (v) & 63;
            dest[1] = (v >> 6) & 63;
            dest[2] = (v >> 12) & 63;
            dest[3] = (v >> 18) & 31;
            dest[4] = (v >> 23) & 31;
            break;
        case 11:
            dest[0] = (v) & 31;
            dest[1] = (v >> 5) & 31;
            dest[2] = (v >> 10) & 63;
            dest[3] = (v >> 16) & 63;
            dest[4] = (v >> 22) & 63;
            break;
        case 12:
            dest[0] = (v) & 127;
            dest[1] = (v >> 7) & 127;
            dest[2] = (v >> 14) & 127;
            dest[3] = (v >> 21) & 127;
            break;
        case 13:
            dest[0] = (v) & 1023;
            dest[1] = (v >> 10) & 511;
            dest[2] = (v >> 19) & 511;
            break;
        case 14:
            dest[0] = (v) & 16383;
            dest[1] = (v >> 14) & 16383;
            break;
        case 15:
            dest[0] = (v) & ((1 << 28)-1);
            break;
        }
        dest += PACK_COUNT[k];
        nDecoded += PACK_COUNT[k];
    }
    FIRTEX_ASSERT2(nDecoded <= nDestLen);
    return nDecoded;
}

FX_NS_END

#endif //__FX_S16CODING_H
