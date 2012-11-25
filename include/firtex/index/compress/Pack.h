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
// Created	: 2010-08-07 13:39:45

#ifndef __FX_PACK_H
#define __FX_PACK_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class Pack
{
public:
    inline static void pack(uint32_t nBits, uint32_t* dest, const uint32_t* src, size_t n);

protected:
    static void pack0(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack1(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack2(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack3(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack4(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack5(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack6(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack7(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack8(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack9(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack10(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack11(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack12(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack13(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack14(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack15(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack16(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack17(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack18(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack19(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack20(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack21(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack22(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack23(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack24(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack25(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack26(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack27(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack28(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack29(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack30(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack31(uint32_t* dest, const uint32_t* src, size_t n);
    static void pack32(uint32_t* dest, const uint32_t* src, size_t n);

    typedef void (*pack_func)(uint32_t* dest, const uint32_t* src, size_t n);

    inline static void packTail(uint32_t* dest, const uint32_t* src, size_t n, uint32_t b);

private:
    static pack_func PACK_FUNCTIONS[33];
};

////////////////////////////////////////////
//
inline void Pack::pack(uint32_t nBits, uint32_t* dest, const uint32_t* src, size_t n)
{
    PACK_FUNCTIONS[nBits](dest, src, n);
}

inline void Pack::packTail(uint32_t* dest, const uint32_t* src, size_t n, uint32_t b)
{
    size_t off = 0;
    size_t j = 0;
    for (size_t i = 0; i < n; ++i)
    {
        dest[j] |= src[i] << off;
        if (off + b - 1 > 31)
        {
            dest[j + 1] |= src[i] >> (32 - off);
        }
        
        j += (off + b) >> 5;
        off = (off + b) & 31;
    }
}

FX_NS_END

#endif //__FX_PACK_H
