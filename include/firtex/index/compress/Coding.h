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
// Created	: 2010-08-06 22:07:56

#ifndef __FX_CODING_H
#define __FX_CODING_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class Coding
{
public:
    Coding() {}
    virtual ~Coding() {}

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
    virtual size_t encode(uint32_t* dest, size_t nDestLen,
                          const uint32_t* src, size_t nSrcLen, 
                          bool bSeal) const = 0;

    /**
     * Decode uint32 array
     * @param dest bytes array to decode to
     * @param nDestLen length of dest
     * @param src source uint32 array to decode
     * @param nSrcLen length of source array
     * @param bExhausted set true if encoded records are exhausted
     * @return number of decoded uint32 elements
     */
    virtual size_t decode(uint32_t* dest, size_t nDestLen,
                          const uint32_t* src, size_t nSrcLen,
                          bool& bExhausted) const = 0;
};

FX_NS_END

#endif //__FX_CODING_H
