//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/1/4
//

#ifndef __FX_LENGTHNORM_ENCODER_H
#define __FX_LENGTHNORM_ENCODER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/SmallFloat.h"

FX_NS_DEF(index);

class LengthNormEncoder
{					
public:
    LengthNormEncoder() {}
    ~LengthNormEncoder(void) {}

public:
    /**
     * Encode length and field boost to int32 value.
     * Format: boost(high 8bit) | length(low 24bit)
     * @return encoded int32 value
     */
    static int32_t encode(int32_t nLength, float fFieldBoost);

    /**
     * Decode int32 encoded value to length and field boost
     * @param  field boost value
     * @return length of field
     */
    static int32_t decode(int32_t nNorm, float& fFieldBoost);
};


//////////////////////////////////////////////////
//
inline int32_t LengthNormEncoder::encode(int32_t nLength, float fFieldBoost)
{
    byte b = 0;
    int32_t n = (nLength & 0x00ffffff);
    if(fFieldBoost != 1.0)
    {
        b = FX_NS(utility)::SmallFloat::floatToByte315(fFieldBoost);
        n |= ((uint32_t)b) << 24; 
    }
    return n;				
}

inline int32_t LengthNormEncoder::decode(int32_t nNorm, float& fFieldBoost)
{
    byte b = (byte)(nNorm >> 24);
    fFieldBoost = FX_NS(utility)::SmallFloat::byte315ToFloat(b);
    return (int32_t)(nNorm & 0x00ffffff);
}

FX_NS_END

#endif
