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
// Created	: 2010-08-05 22:08:29

#ifndef __FX_UNPACK_H
#define __FX_UNPACK_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class Unpack
{
public:
    inline static void unpack(uint32_t nBits, uint32_t* dest, const uint32_t* src, size_t n);

protected:
    static void unpack0(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack1(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack2(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack3(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack4(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack5(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack6(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack7(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack8(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack9(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack10(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack11(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack12(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack13(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack14(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack15(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack16(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack17(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack18(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack19(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack20(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack21(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack22(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack23(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack24(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack25(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack26(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack27(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack28(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack29(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack30(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack31(uint32_t* dest, const uint32_t* src, size_t n);
    static void unpack32(uint32_t* dest, const uint32_t* src, size_t n);


    typedef void (*unpack_func)(uint32_t* dest, const uint32_t* src, size_t n);

    static unpack_func UNPACK_FUNCTIONS[33];

protected:
    inline static void unpackTail1(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail2(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail3(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail4(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail5(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail6(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail7(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail8(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail9(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail10(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail11(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail12(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail13(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail14(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail15(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail16(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail17(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail18(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail19(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail20(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail21(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail22(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail23(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail24(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail25(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail26(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail27(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail28(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail29(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail30(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail31(uint32_t* dest, const uint32_t* src, size_t n);
    inline static void unpackTail32(uint32_t* dest, const uint32_t* src, size_t n);
};

//////////////////////////////////////////////
//
inline void Unpack::unpack(uint32_t nBits, uint32_t* dest, const uint32_t* src, size_t n)
{
    UNPACK_FUNCTIONS[nBits](dest, src, n);
}

inline void Unpack::unpackTail1(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t rest = n;
    uint32_t v = src[0];

    dest[0] = (v << 31) >> 31;
    if(--rest == 0) return;
    dest[1] = (v << 30) >> 31;
    if(--rest == 0) return;
    dest[2] = (v << 29) >> 31;
    if(--rest == 0) return;
    dest[3] = (v << 28) >> 31;
    if(--rest == 0) return;
    dest[4] = (v << 27) >> 31;
    if(--rest == 0) return;
    dest[5] = (v << 26) >> 31;
    if(--rest == 0) return;
    dest[6] = (v << 25) >> 31;
    if(--rest == 0) return;
    dest[7] = (v << 24) >> 31;
    if(--rest == 0) return;
    dest[8] = (v << 23) >> 31;
    if(--rest == 0) return;
    dest[9] = (v << 22) >> 31; //10
    if(--rest == 0) return;
    dest[10] = (v << 21) >> 31;
    if(--rest == 0) return;
    dest[11] = (v << 20) >> 31;
    if(--rest == 0) return;
    dest[12] = (v << 19) >> 31;
    if(--rest == 0) return;
    dest[13] = (v << 18) >> 31;
    if(--rest == 0) return;
    dest[14] = (v << 17) >> 31;
    if(--rest == 0) return;
    dest[15] = (v << 16) >> 31;
    if(--rest == 0) return;
    dest[16] = (v << 15) >> 31;
    if(--rest == 0) return;
    dest[17] = (v << 14) >> 31;
    if(--rest == 0) return;
    dest[18] = (v << 13) >> 31;
    if(--rest == 0) return;
    dest[19] = (v << 12) >> 31;//20
    if(--rest == 0) return;
    dest[20] = (v << 11) >> 31;
    if(--rest == 0) return;
    dest[21] = (v << 10) >> 31;
    if(--rest == 0) return;
    dest[22] = (v << 9) >> 31;
    if(--rest == 0) return;
    dest[23] = (v << 8) >> 31;
    if(--rest == 0) return;
    dest[24] = (v << 7) >> 31;
    if(--rest == 0) return;
    dest[25] = (v << 6) >> 31;
    if(--rest == 0) return;
    dest[26] = (v << 5) >> 31;
    if(--rest == 0) return;
    dest[27] = (v << 4) >> 31;
    if(--rest == 0) return;
    dest[28] = (v << 3) >> 31;
    if(--rest == 0) return;
    dest[29] = (v << 2) >> 31;
    if(--rest == 0) return;
    dest[30] = (v << 1) >> 31;
    return;
}

inline void Unpack::unpackTail2(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = n >> 4;
    size_t rest = n % 16;
    size_t y = 0;

    uint32_t v = 0;

    for (size_t i = 0; i < b; i++, dest += 16)
    {
        v = src[x++];

        dest[0]  = ( v << 30 ) >> 30;
        dest[1]  = ( v << 28 ) >> 30;
        dest[2]  = ( v << 26 ) >> 30;
        dest[3]  = ( v << 24 ) >> 30;
        dest[4]  = ( v << 22 ) >> 30;
        dest[5]  = ( v << 20 ) >> 30;
        dest[6]  = ( v << 18 ) >> 30;
        dest[7]  = ( v << 16 ) >> 30;
        dest[8]  = ( v << 14 ) >> 30;
        dest[9]  = ( v << 12 ) >> 30;
        dest[10]  = ( v << 10 ) >> 30;
        dest[11]  = ( v <<  8 ) >> 30;
        dest[12]  = ( v <<  6 ) >> 30;
        dest[13]  = ( v <<  4 ) >> 30;
        dest[14]  = ( v <<  2 ) >> 30;
        dest[15]  = v >> 30;
    }

    if (rest == 0) return;

    v = src[x++];
    dest[y] = (v << 30) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 28) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 26) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 24) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 22) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 20) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 18) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 16) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 14) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 12) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v << 10) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v <<  8) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v <<  6) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v <<  4) >> 30;
    if(--rest == 0) return;
    dest[++y] = (v <<  2) >> 30;
    return;
}

inline void Unpack::unpackTail3(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1 = src[x++];
    dest[y  ] = (v1 << 29) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 << 26) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 << 23) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 << 20) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 << 17) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 << 14) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 << 11) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  8) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  5) >>  29;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  2) >>  29;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y] = ((v2 << 31) >> 29) | (v1 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v2 << 28) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 << 25) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 << 22) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 << 19) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 << 16) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 << 13) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 << 10) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 <<  7) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 <<  4) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v2 <<  1) >> 29;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 30) >> 29) | (v2 >> 31);
    if(--rest == 0) return;
    dest[++y] = (v3 << 27) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 << 24) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 << 21) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 << 18) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 << 15) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 << 12) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 <<  9) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 <<  6) >> 29;
    if(--rest == 0) return;
    dest[++y] = (v3 <<  3) >> 29;
    return;
}


inline void Unpack::unpackTail4(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = n >> 3;
    size_t rest = n % 8;
    size_t y = 0;

    for (size_t i = 0; i < b ; i++, dest += 8)
    {

        uint32_t v1 = src[ x++ ];

        dest[0]  = ( v1 << 28 ) >> 28 ;
        dest[1]  = ( v1 << 24 ) >> 28 ;
        dest[2]  = ( v1 << 20 ) >> 28 ;
        dest[3]  = ( v1 << 16 ) >> 28 ;
        dest[4]  = ( v1 << 12 ) >> 28 ;
        dest[5]  = ( v1 << 8  ) >> 28 ;
        dest[6]  = ( v1 << 4  ) >> 28 ;
        dest[7]  = v1 >> 28 ;
    }

    if (rest == 0 ) return;

    uint32_t v1 = src[x++];

    dest[y]  = (v1 << 28) >> 28;
    if(--rest == 0) return;
    dest[++y]  = (v1 << 24) >> 28;
    if(--rest == 0) return;
    dest[++y]  = (v1 << 20) >> 28;
    if(--rest == 0) return;
    dest[++y]  = (v1 << 16) >> 28;
    if(--rest == 0) return;
    dest[++y]  = (v1 << 12) >> 28;
    if(--rest == 0) return;
    dest[++y]  = (v1 << 8 ) >> 28;
    if(--rest == 0) return;
    dest[++y]  = (v1 << 4 ) >> 28;
    return;
}

inline void Unpack::unpackTail5(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1 = src[x++ ];
    dest[y] = (v1 << 27) >>  27;
    if(--rest == 0) return;
    dest[++y] = (v1 << 22) >>  27;
    if(--rest == 0) return;
    dest[++y] = (v1 << 17) >>  27;
    if(--rest == 0) return;
    dest[++y] = (v1 << 12) >>  27;
    if(--rest == 0) return;
    dest[++y] = (v1 << 7 ) >>  27;
    if(--rest == 0) return;
    dest[++y] = (v1 << 2 ) >>  27;
    if(--rest == 0) return;

    uint32_t v2 = src[x++ ];
    dest[++y]  = ((v2 << 29) >> 27) | (v1 >> 30);
    if(--rest == 0) return;
    dest[++y]  = (v2 << 24) >> 27;
    if(--rest == 0) return;
    dest[++y]  = (v2 << 19) >> 27;
    if(--rest == 0) return;
    dest[++y]  = (v2 << 14) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v2 << 9 ) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v2 << 4 ) >> 27;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 31) >> 27) | (v2 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v3 << 26) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v3 << 21) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v3 << 16) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v3 << 11) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v3 << 6 ) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v3 << 1 ) >> 27;
    if(--rest == 0) return;

    uint32_t v4 = src[x++];
    dest[++y] = ((v4 << 28) >> 27) | (v3 >> 31);
    if(--rest == 0) return;
    dest[++y] = (v4 << 23) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v4 << 18) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v4 << 13) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v4 << 8 ) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v4 << 3 ) >> 27;
    if(--rest == 0) return;

    uint32_t v5 = src[x++];
    dest[++y] = ((v5 << 30) >> 27) | (v4 >> 29);
    if(--rest == 0) return;
    dest[++y] = (v5 << 25) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v5 << 20) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v5 << 15) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v5 << 10) >> 27;
    if(--rest == 0) return;
    dest[++y] = (v5 << 5 ) >> 27;
    return;
}

inline void Unpack::unpackTail6(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = n >> 4;
    size_t rest = n % 16;
    size_t y = 0;

    for (uint32_t  i = 0 ; i < b ; i++, dest += 16)
    {
        uint32_t v1 = src[ x++ ];
        uint32_t v2 = src[ x++ ];
        uint32_t v3 = src[ x++ ];

        dest[0] = ( v1 << 26 ) >> 26 ;
        dest[1] = ( v1 << 20 ) >> 26 ;
        dest[2] = ( v1 << 14 ) >> 26 ;
        dest[3] = ( v1 <<  8 ) >> 26 ;
        dest[4] = ( v1 <<  2 ) >> 26 ;

        dest[5]  = ( ( v2 << 28 ) >> 26 ) | ( v1 >> 30 );
        dest[6] = ( v2 << 22 ) >> 26 ;
        dest[7] = ( v2 << 16 ) >> 26 ;
        dest[8] = ( v2 << 10 ) >> 26 ;
        dest[9] = ( v2 <<  4 ) >> 26 ;

        dest[10] = ( ( v3 << 30 ) >> 26 ) | ( v2 >> 28 );
        dest[11] = ( v3 << 24 )  >> 26 ;
        dest[12] = ( v3 << 18 )  >> 26 ;
        dest[13] = ( v3 << 12 ) >> 26 ;
        dest[14] = ( v3 <<  6 ) >> 26 ;
        dest[15] = v3 >> 26 ;
    }

    if( rest == 0 ) return;

    uint32_t v1 = src[x++];
    dest[y ] = (v1 << 26) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v1 << 20) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v1 << 14) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  8) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  2) >> 26;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y]  = ((v2 << 28) >> 26) | (v1 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v2 << 22) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v2 << 16) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v2 << 10) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v2 <<  4) >> 26;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 30) >> 26) | (v2 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v3 << 24)  >> 26;
    if(--rest == 0) return;
    dest[++y] = (v3 << 18)  >> 26;
    if(--rest == 0) return;
    dest[++y] = (v3 << 12) >> 26;
    if(--rest == 0) return;
    dest[++y] = (v3 <<  6) >> 26;
    if(--rest == 0) return;
    dest[++y] = v3 >> 26;
    return;
}

inline void Unpack::unpackTail7(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1 = src[x++];
    dest[y  ] = (v1 << 25) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v1 << 18) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v1 << 11) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  4) >> 25;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y] = ((v2 << 29) >> 25) | (v1 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v2 << 22) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v2 << 15) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v2 << 8 ) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v2 << 1 ) >> 25;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 26) >> 25) | (v2 >> 31);
    if(--rest == 0) return;
    dest[++y] = (v3 << 19) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v3 << 12) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v3 <<  5) >> 25;
    if(--rest == 0) return;

    uint32_t v4 = src[x++];
    dest[++y] = ((v4 << 30) >> 25) | (v3 >> 27);
    if(--rest == 0) return;
    dest[++y] = (v4 << 23) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v4 << 16) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v4 << 9 ) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v4 << 2 ) >> 25;
    if(--rest == 0) return;

    uint32_t v5 = src[x++];
    dest[++y] = ((v5 << 27) >> 25) | (v4 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v5 << 20) >> 25 ;
    if(--rest == 0) return;
    dest[++y] = (v5 << 13) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v5 <<  6) >> 25;
    if(--rest == 0) return;

    uint32_t v6 = src[x++];
    dest[++y] = ((v6 << 31) >> 25) |  (v5 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v6 << 24) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v6 << 17) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v6 << 10) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v6 <<  3) >> 25;
    if(--rest == 0) return;

    uint32_t v7 = src[x++];
    dest[++y] = ((v7 << 28) >> 25) |  (v6 >> 29);
    if(--rest == 0) return;
    dest[++y] = (v7 << 21) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v7 << 14) >> 25;
    if(--rest == 0) return;
    dest[++y] = (v7 << 7 ) >> 25;
    return;
}

inline void Unpack::unpackTail8(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = n >> 2;
    size_t rest = n % 4;
    size_t y = 0;

    for ( uint32_t  i = 0 ; i < b ; i++, dest += 4)
    {
        uint32_t v1 = src[ x++ ];

        dest[0] = ( v1 << 24 ) >> 24 ;
        dest[1] = ( v1 << 16 ) >> 24 ;
        dest[2] = ( v1 <<  8 ) >> 24 ;
        dest[3] = v1 >> 24 ;
    }

    if (rest == 0 ) return;

    uint32_t v1 = src[x++];

    dest[y  ] = (v1 << 24) >> 24;
    if(--rest == 0) return;
    dest[++y] = (v1 << 16) >> 24;
    if(--rest == 0) return;
    dest[++y] = (v1 << 8 ) >> 24;
    return;
}

inline void Unpack::unpackTail9(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1 = src[x++];
    dest[y] = (v1 << 23) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v1 << 14) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v1 << 5 ) >> 23;
    if(--rest == 0) return;

    uint32_t v2 = src[x++ ];
    dest[++y] = ((v2 << 28) >> 23) | (v1 >> 27);
    if(--rest == 0) return;
    dest[++y] = (v2 << 19) >> 23; //5
    if(--rest == 0) return;
    dest[++y] = (v2 << 10) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v2 << 1 ) >> 23;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 24) >> 23) | (v2 >> 31);
    if(--rest == 0) return;
    dest[++y] = (v3 << 15) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v3 << 6 ) >> 23; //10
    if(--rest == 0) return;

    uint32_t v4 = src[x++];
    dest[++y] = ((v4 << 29) >> 23) | (v3 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v4 << 20) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v4 << 11) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v4 << 2 ) >> 23;
    if(--rest == 0) return;

    uint32_t v5 = src[x++];
    dest[++y] = ((v5 << 25) >> 23) | (v4 >> 30);//15
    if(--rest == 0) return;
    dest[++y] = (v5 << 16) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v5 << 7 ) >> 23;
    if(--rest == 0) return;

    uint32_t v6 = src[x++];
    dest[++y] = ((v6 << 30) >> 23) | (v5 >> 25);
    if(--rest == 0) return;
    dest[++y] = (v6 << 21) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v6 << 12) >> 23;//20
    if(--rest == 0) return;
    dest[++y] = (v6 << 3 ) >> 23;
    if(--rest == 0) return;

    uint32_t v7 = src[x++];
    dest[++y] = ((v7 << 26) >> 23) | (v6 >> 29);
    if(--rest == 0) return;
    dest[++y] = (v7 << 17) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v7 << 8 ) >> 23;
    if(--rest == 0) return;

    uint32_t v8 = src[x++];
    dest[++y] = ((v8 << 31) >> 23) | (v7 >> 24);//25
    if(--rest == 0) return;
    dest[++y] = (v8 << 22) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v8 << 13) >> 23;
    if(--rest == 0) return;
    dest[++y] = (v8 << 4 ) >> 23;
    if(--rest == 0) return;

    uint32_t v9 = src[x++];
    dest[++y] = ((v9 << 27) >> 23) | (v8 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v9 << 18) >> 23;//30
    if(--rest == 0) return;
    dest[++y] = (v9 << 9 ) >> 23;
    return;
}

inline void Unpack::unpackTail10(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = n >> 4;
    size_t rest = n % 16;
    size_t y = 0;

    for ( uint32_t  i = 0 ; i < b ; i++, dest += 16)
    {
        uint32_t v1 = src[ x++ ];
        uint32_t v2 = src[ x++ ];
        uint32_t v3 = src[ x++ ];
        uint32_t v4 = src[ x++ ];
        uint32_t v5 = src[ x++ ];

        dest[0] = ( v1 << 22 ) >> 22 ;
        dest[1] = ( v1 << 12 ) >> 22 ;
        dest[2] = ( v1 <<  2 ) >> 22 ;
        dest[3] = ( ( v2 << 24 ) >> 22 ) | ( v1 >> 30 ) ;

        dest[4] = ( v2 << 14 ) >> 22 ;
        dest[5] = ( v2 <<  4 ) >> 22 ;
        dest[6] = ( ( v3 << 26 ) >> 22 ) | ( v2 >> 28 ) ;

        dest[7] = ( v3 << 16 ) >> 22 ;
        dest[8] = ( v3 <<  6 ) >> 22 ;
        dest[9] = ( ( v4 << 28 ) >> 22 ) | ( v3 >> 26 ) ;

        dest[10] = ( v4 << 18 ) >> 22 ;
        dest[11] = ( v4 <<  8 ) >> 22 ;
        dest[12] = ( ( v5 << 30 ) >> 22 ) | ( v4 >> 24 ) ;

        dest[13] = ( v5 << 20 ) >> 22 ;
        dest[14] = ( v5 << 10 ) >> 22 ;
        dest[15] = v5 >> 22 ;
    }

    if( rest == 0 ) return;

    uint32_t v1 = src[x++];
    dest[  y] = (v1 << 22) >> 22;
    if(--rest == 0) return;
    dest[++y] = (v1 << 12) >> 22;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  2) >> 22;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y] = ((v2 << 24) >> 22) | (v1 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v2 << 14) >> 22;
    if(--rest == 0) return;
    dest[++y] = (v2 <<  4) >> 22;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 26) >> 22) | (v2 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v3 << 16) >> 22;
    if(--rest == 0) return;
    dest[++y] = (v3 <<  6) >> 22;
    if(--rest == 0) return;

    uint32_t v4 = src[x++];
    dest[++y] = ((v4 << 28) >> 22) | (v3 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v4 << 18) >> 22;
    if(--rest == 0) return;
    dest[++y] = (v4 <<  8) >> 22;
    if(--rest == 0) return;

    uint32_t v5 = src[x++];
    dest[++y] = ((v5 << 30) >> 22) | (v4 >> 24);
    if(--rest == 0) return;
    dest[++y] = (v5 << 20) >> 22;
    if(--rest == 0) return;
    dest[++y] = (v5 << 10) >> 22;
    return;
}

inline void Unpack::unpackTail11(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1 = src[x++];
    dest[y] = (v1 << 21) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v1 << 10) >> 21;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y] = ((v2 << 31) >> 21) | (v1 >> 22);
    if(--rest == 0) return;
    dest[++y] = (v2 << 20) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v2 << 9 ) >> 21;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 30) >> 21) | (v2 >> 23);
    if(--rest == 0) return;
    dest[++y] = (v3 << 19) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v3 <<  8) >> 21;
    if(--rest == 0) return;

    uint32_t v4 = src[x++];
    dest[++y] = ((v4 << 29) >> 21) | (v3 >> 24);
    if(--rest == 0) return;
    dest[++y]  = (v4 << 18 ) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v4 <<  7 ) >> 21;
    if(--rest == 0) return;

    uint32_t v5 = src[x++];
    dest[++y] = ((v5 << 28) >> 21) | (v4 >> 25);
    if(--rest == 0) return;
    dest[++y] = (v5 << 17) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v5 <<  6) >> 21;
    if(--rest == 0) return;

    uint32_t v6 = src[x++];
    dest[++y] = ((v6 << 27) >> 21) | (v5 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v6 << 16) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v6 <<  5) >> 21;
    if(--rest == 0) return;

    uint32_t v7 = src[x++];
    dest[++y] = ((v7 << 26) >> 21) | (v6 >> 27);
    if(--rest == 0) return;
    dest[++y] = (v7 << 15) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v7 <<  4) >> 21;
    if(--rest == 0) return;

    uint32_t v8 = src[x++];
    dest[++y] = ((v8 << 25) >> 21) | (v7 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v8 << 14) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v8 <<  3) >> 21;
    if(--rest == 0) return;

    uint32_t v9 = src[x++];
    dest[++y] = ((v9 << 24) >> 21) | (v8 >> 29);
    if(--rest == 0) return;
    dest[++y] = (v9 << 13) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v9 <<  2) >> 21;
    if(--rest == 0) return;

    uint32_t v10 = src[x++];
    dest[++y] = ((v10 << 23) >> 21) | (v9 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v10 << 12) >> 21;
    if(--rest == 0) return;
    dest[++y] = (v10 <<  1) >> 21;
    if(--rest == 0) return;

    uint32_t v11 = src[x++];
    dest[++y] = ((v11 << 22) >> 21 ) | (v10 >> 31);
    if(--rest == 0) return;
    dest[++y] = (v11 << 11) >> 21;
    return;
}

inline void Unpack::unpackTail12(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = n >> 3;
    size_t rest = n % 8;
    size_t y = 0;

    for (uint32_t  i = 0 ; i < b ; i++, dest += 8)
    {
        uint32_t v1 = src[ x++ ];
        uint32_t v2 = src[ x++ ];
        uint32_t v3 = src[ x++ ];

        dest[0] = ( v1 << 20 ) >> 20;
        dest[1] = ( v1 <<  8  ) >> 20;
        dest[2] = ( ( v2 << 28 ) >> 20 ) | ( v1 >> 24 );

        dest[3] = ( v2 << 16 ) >> 20;
        dest[4] = ( v2 <<  4 ) >> 20;
        dest[5] = ( ( v3 << 24 ) >> 20 ) | ( v2 >> 28 );

        dest[6] = ( v3 << 12 ) >> 20;
        dest[7] = v3 >> 20;
    }

    if( rest == 0 ) return;


    uint32_t v1 = src[x++];
    dest[y  ] = (v1 << 20) >> 20;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  8 ) >> 20;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y] = ((v2 << 28) >> 20) | (v1 >> 24);
    if(--rest == 0) return;
    dest[++y] = (v2 << 16) >> 20;
    if(--rest == 0) return;
    dest[++y] = (v2 <<  4) >> 20;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 24) >> 20) | (v2 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v3 << 12) >> 20;
    if(--rest == 0) return;
    dest[++y] = v3 >> 20;
    return;
}

inline void Unpack::unpackTail13(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1  = src[x++];
    dest[y  ] = (v1 << 19) >> 19;
    if(--rest == 0) return;
    dest[++y] = (v1 << 6 ) >> 19;
    if(--rest == 0) return;

    uint32_t v2  = src[x++];
    dest[++y] = ((v2 << 25) >> 19) | (v1 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v2 << 12) >> 19;
    if(--rest == 0) return;

    uint32_t v3  = src[x++];
    dest[++y] = ((v3 << 31) >> 19) | (v2 >> 20);
    if(--rest == 0) return;
    dest[++y] = (v3 << 18) >> 19;
    if(--rest == 0) return;
    dest[++y] = (v3 << 5 ) >> 19;
    if(--rest == 0) return;

    uint32_t v4  = src[x++];
    dest[++y] = ((v4 << 24) >> 19) | (v3 >> 27);
    if(--rest == 0) return;
    dest[++y] = (v4 << 11) >> 19;
    if(--rest == 0) return;

    uint32_t v5  = src[x++];
    dest[++y] = ((v5 << 30) >> 19) | (v4 >> 21);
    if(--rest == 0) return;
    dest[++y] = (v5 << 17) >> 19;
    if(--rest == 0) return;
    dest[++y] = (v5 << 4 ) >> 19;
    if(--rest == 0) return;

    uint32_t v6  = src[x++];
    dest[++y] = ((v6 << 23) >> 19) | (v5 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v6 << 10) >> 19;
    if(--rest == 0) return;

    uint32_t v7  = src[x++];
    dest[++y] = ((v7 << 29) >> 19) | (v6 >> 22);
    if(--rest == 0) return;
    dest[++y] = (v7 << 16) >> 19;
    if(--rest == 0) return;
    dest[++y] = (v7 <<  3) >> 19;
    if(--rest == 0) return;

    uint32_t v8  = src[x++];
    dest[++y] = ((v8 << 22) >> 19) | (v7 >> 29);
    if(--rest == 0) return;
    dest[++y] = (v8 << 9) >> 19;
    if(--rest == 0) return;

    uint32_t v9  = src[x++];
    dest[++y] = ((v9 << 28 ) >> 19) | (v8 >> 23);
    if(--rest == 0) return;
    dest[++y] = (v9 << 15) >> 19;
    if(--rest == 0) return;
    dest[++y] = (v9 << 2 ) >> 19;
    if(--rest == 0) return;

    uint32_t v10 = src[x++];
    dest[++y] = ((v10 << 21 ) >> 19) | (v9 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v10 << 8) >> 19;
    if(--rest == 0) return;

    uint32_t v11 = src[x++];
    dest[++y] = ((v11 << 27 ) >> 19) | (v10 >> 24);
    if(--rest == 0) return;
    dest[++y] = (v11 << 14) >> 19;
    if(--rest == 0) return;
    dest[++y] = (v11 <<  1) >> 19;
    if(--rest == 0) return;

    uint32_t v12 = src[x++];
    dest[++y] = ((v12 << 20 ) >> 19) | (v11 >> 31);
    if(--rest == 0) return;
    dest[++y] = (v12 << 7) >> 19;
    if(--rest == 0) return;

    uint32_t v13 = src[x++];
    dest[++y] = ((v13 << 26 ) >> 19) | (v12 >> 25);
    if(--rest == 0) return;
    dest[++y] = (v13 << 13 ) >> 19;
    return;
}

inline void Unpack::unpackTail14(uint32_t* dest, const uint32_t* src, size_t n)
{

    size_t x = 0;
    size_t b	=  n >> 4;
    size_t rest		=  n % 16;
    size_t y = 0;

    for( uint32_t  i = 0 ; i < b ; i++, dest += 16)
    {
        uint32_t v1 = src[ x++ ];
        uint32_t v2 = src[ x++ ];
        uint32_t v3 = src[ x++ ];
        uint32_t v4 = src[ x++ ];
        uint32_t v5 = src[ x++ ];
        uint32_t v6 = src[ x++ ];
        uint32_t v7 = src[ x++ ];

        dest[0] = ( v1 << 18 ) >> 18;
        dest[1] = ( v1 <<  4 ) >> 18;
        dest[2] = ( ( v2 << 22 ) >> 18 ) | ( v1 >> 28 );

        dest[3] = ( v2 << 8  ) >> 18;
        dest[4] = ( ( v3 << 26 ) >> 18 ) | ( v2 >> 24 );

        dest[5] = ( v3 << 12 ) >> 18;
        dest[6] = ( ( v4 << 30 ) >> 18 ) | ( v3 >> 20 );

        dest[7] = ( v4 << 16 ) >> 18;
        dest[8] = ( v4 << 2  ) >> 18;
        dest[9] = ( ( v5 << 20 ) >> 18 ) | ( v4 >> 30 );

        dest[10] = ( v5 << 6 )  >> 18;
        dest[11] = ( ( v6 << 24 ) >> 18 ) | ( v5 >> 26 );

        dest[12] = ( v6 << 10 ) >> 18;
        dest[13] = ( ( v7 << 28 ) >> 18 ) | ( v6 >> 22 );

        dest[14] = ( v7 << 14 ) >> 18;
        dest[15] = v7 >> 18;
    }

    if( rest == 0 ) return;

    uint32_t v1 = src[x++];
    dest[y] = (v1 << 18) >> 18;
    if(--rest == 0) return;
    dest[++y] = (v1 <<  4) >> 18;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y] = ((v2 << 22) >> 18) | (v1 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v2 << 8 ) >> 18;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 26) >> 18) | (v2 >> 24);
    if(--rest == 0) return;
    dest[++y] = (v3 << 12) >> 18;
    if(--rest == 0) return;

    uint32_t v4 = src[x++];
    dest[++y] = ((v4 << 30) >> 18) | (v3 >> 20);
    if(--rest == 0) return;
    dest[++y] = (v4 << 16) >> 18;
    if(--rest == 0) return;
    dest[++y] = (v4 << 2 ) >> 18;
    if(--rest == 0) return;

    uint32_t v5 = src[x++];
    dest[++y] = ((v5 << 20) >> 18) | (v4 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v5 << 6)  >> 18;
    if(--rest == 0) return;

    uint32_t v6 = src[x++];
    dest[++y] = ((v6 << 24) >> 18) | (v5 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v6 << 10) >> 18;
    if(--rest == 0) return;

    uint32_t v7 = src[x++];
    dest[++y] = ((v7 << 28) >> 18) | (v6 >> 22);
    if(--rest == 0) return;
    dest[++y] = (v7 << 14) >> 18;
    if(--rest == 0) return;
    dest[++y] = v7 >> 18;
    return;
}

inline void Unpack::unpackTail15(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1  = src[x++];
    dest[y  ] = (v1 << 17) >> 17;
    if(--rest == 0) return;
    dest[++y] = (v1 << 2 ) >> 17;
    if(--rest == 0) return;

    uint32_t v2  = src[x++];
    dest[++y] = ((v2 << 19) >>  17) | (v1 >> 30);
    if(--rest == 0) return;
    dest[++y] = (v2 << 4) >> 17;
    if(--rest == 0) return;

    uint32_t v3  = src[x++];
    dest[++y] = ((v3 << 21) >> 17) | (v2 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v3 << 6 ) >> 17;
    if(--rest == 0) return;

    uint32_t v4  = src[x++];
    dest[++y] = ((v4 << 23 ) >> 17) | (v3 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v4 << 8 ) >> 17;
    if(--rest == 0) return;

    uint32_t v5  = src[x++];
    dest[++y] = ((v5 << 25) >> 17) | (v4 >> 24);
    if(--rest == 0) return;
    dest[++y] = (v5 << 10) >> 17;
    if(--rest == 0) return;

    uint32_t v6  = src[x++];
    dest[++y] = ((v6 << 27) >> 17) | (v5 >> 22);
    if(--rest == 0) return;
    dest[++y] = (v6 << 12) >> 17;
    if(--rest == 0) return;

    uint32_t v7  = src[x++];
    dest[++y] = ((v7 << 29) >> 17) | (v6 >> 20);
    if(--rest == 0) return;
    dest[++y] = (v7  << 14) >> 17;
    if(--rest == 0) return;

    uint32_t v8  = src[x++];
    dest[++y] = ((v8 << 31) >> 17) | (v7 >> 18);
    if(--rest == 0) return;
    dest[++y] = (v8 << 16) >> 17;
    if(--rest == 0) return;
    dest[++y] = (v8 <<  1) >> 17;
    if(--rest == 0) return;

    uint32_t v9  = src[x++];
    dest[++y] = ((v9 << 18 ) >> 17) | (v8 >> 31);
    if(--rest == 0) return;
    dest[++y] = (v9 << 3) >> 17;
    if(--rest == 0) return;

    uint32_t v10 = src[x++];
    dest[++y] = ((v10 << 20) >> 17) | (v9 >> 29);
    if(--rest == 0) return;
    dest[++y] = (v10 << 5) >> 17;
    if(--rest == 0) return;

    uint32_t v11 = src[x++];
    dest[++y] = ((v11 << 22) >> 17) | (v10 >> 27);
    if(--rest == 0) return;
    dest[++y] = (v11 << 7) >> 17;
    if(--rest == 0) return;

    uint32_t v12 = src[x++];
    dest[++y] = ((v12 << 24) >> 17) | (v11 >> 25);
    if(--rest == 0) return;
    dest[++y] = (v12 << 9) >> 17;
    if(--rest == 0) return;

    uint32_t v13 = src[x++];
    dest[++y] = ((v13 << 26) >> 17) | (v12 >> 23);
    if(--rest == 0) return;
    dest[++y] = (v13 << 11) >> 17;
    if(--rest == 0) return;

    uint32_t v14 = src[x++];
    dest[++y] = ((v14 << 28) >> 17) | (v13 >> 21);
    if(--rest == 0) return;
    dest[++y] = (v14 << 13 ) >> 17;
    if(--rest == 0) return;

    uint32_t v15 = src[x++];
    dest[++y] = ((v15 << 30) >> 17) | (v14 >> 19);
    if(--rest == 0) return;
    dest[++y] = (v15 << 15 ) >> 17;
    if(--rest == 0) return;
    dest[++y] = v15 >> 17;
    return;
}

inline void Unpack::unpackTail16(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t y = 0;
    size_t b = n >> 1;
    size_t rest = n  %  2;

    for( uint32_t  i = 0 ; i < b ; i++, dest += 2 )
    {
        uint32_t v1 = src[ x++ ];

        dest[0] = ( v1 << 16 ) >> 16;
        dest[1] = ( v1 >> 16 ) ;
    }

    if( rest == 0 ) return;

    uint32_t v1 = src[x++];
    dest[y] = (v1 << 16) >>  16;
    return;

}

inline void Unpack::unpackTail17(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1  = src[x++];
    dest[y] = (v1 << 15) >> 15;
    if(--rest == 0) return;

    uint32_t v2  = src[x++];
    dest[++y] = ((v2 << 30) >> 15) | (v1 >> 17);
    if(--rest == 0) return;
    dest[++y] = (v2 << 13) >> 15;
    if(--rest == 0) return;

    uint32_t v3  = src[x++];
    dest[++y] = ((v3 << 28) >> 15) | (v2 >> 19);
    if(--rest == 0) return;
    dest[++y] = (v3 << 11) >> 15;
    if(--rest == 0) return;

    uint32_t v4  = src[x++];
    dest[++y] = ((v4 << 26) >> 15) | (v3 >> 21);
    if(--rest == 0) return;
    dest[++y] = (v4 << 9 ) >> 15;
    if(--rest == 0) return;

    uint32_t v5  = src[x++];
    dest[++y] = ((v5 << 24) >> 15) | (v4 >> 23);
    if(--rest == 0) return;
    dest[++y] = (v5 << 7 ) >> 15;
    if(--rest == 0) return;

    uint32_t v6  = src[x++];
    dest[++y] = ((v6 << 22) >> 15) | (v5 >> 25);
    if(--rest == 0) return;
    dest[++y] = (v6 << 5 ) >>  15;
    if(--rest == 0) return;

    uint32_t v7  = src[x++];
    dest[++y] = ((v7 << 20) >> 15) | (v6 >> 27);
    if(--rest == 0) return;
    dest[++y] = (v7 << 3 ) >> 15;
    if(--rest == 0) return;

    uint32_t v8  = src[x++];
    dest[++y] = ((v8 << 18) >> 15) | (v7 >> 29);
    if(--rest == 0) return;
    dest[++y] = (v8 << 1 ) >>  15;
    if(--rest == 0) return;

    uint32_t v9  = src[x++];
    dest[++y] = ((v9 << 16) >> 15) | (v8 >> 31);
    if(--rest == 0) return;

    uint32_t v10 = src[x++];
    dest[++y] = ((v10 << 31) >> 15) | (v9 >> 16);
    if(--rest == 0) return;
    dest[++y] = (v10 << 14) >> 15;
    if(--rest == 0) return;

    uint32_t v11 = src[x++];
    dest[++y] = ((v11 << 29) >> 15) | (v10 >> 18);
    if(--rest == 0) return;
    dest[++y] = (v11 << 12) >> 15;
    if(--rest == 0) return;

    uint32_t v12 = src[x++];
    dest[++y] = ((v12 << 27) >> 15) | (v11 >> 20);
    if(--rest == 0) return;
    dest[++y] = (v12 << 10) >> 15;
    if(--rest == 0) return;

    uint32_t v13 = src[x++];
    dest[++y] = ((v13 << 25) >> 15) | (v12 >> 22);
    if(--rest == 0) return;
    dest[++y] = (v13 << 8) >> 15;
    if(--rest == 0) return;

    uint32_t v14 = src[x++];
    dest[++y] = ((v14 << 23) >> 15) | (v13 >> 24);
    if(--rest == 0) return;
    dest[++y] = (v14 << 6) >> 15;
    if(--rest == 0) return;

    uint32_t v15 = src[x++];
    dest[++y] = ((v15 << 21) >> 15) | (v14 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v15 << 4) >> 15;
    if(--rest == 0) return;

    uint32_t v16 = src[x++];
    dest[++y] = ((v16 << 19) >> 15) | (v15 >> 28);
    if(--rest == 0) return;
    dest[++y] = (v16 << 2) >> 15;
    if(--rest == 0) return;

    uint32_t v17 = src[x++];
    dest[++y] = ((v17 << 17) >> 15) | (v16 >> 30);
    return;
}

inline void Unpack::unpackTail18(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = n  >>  4;
    size_t rest = n  %  16;
    size_t y = 0;

    for (size_t  i = 0 ; i < b; i++, dest += 16)
    {
        uint32_t v1 = src[x++];
        uint32_t v2 = src[x++];
        uint32_t v3 = src[x++];
        uint32_t v4 = src[x++];
        uint32_t v5 = src[x++];
        uint32_t v6 = src[x++];
        uint32_t v7 = src[x++];
        uint32_t v8 = src[x++];
        uint32_t v9 = src[x++];

        dest[0] = (v1 << 14 ) >> 14;
        dest[1] = ( ( v2 << 28 ) >> 14 ) | ( v1 >> 18 ) ;

        dest[2] = ( v2 << 10 ) >> 14 ;
        dest[3] = ( ( v3 << 24 ) >> 14 ) | ( v2 >> 22 ) ;

        dest[4] = ( v3 << 6  ) >> 14 ;
        dest[5] = ( ( v4 << 20 ) >> 14 ) | ( v3 >> 26 ) ;

        dest[6] = ( v4 << 2  ) >> 14 ;
        dest[7] = ( ( v5 << 16 ) >> 14 ) | ( v4 >> 30 ) ;

        dest[8] = ( ( v6 << 30 ) >> 14 ) | ( v5 >> 16 ) ;

        dest[9] = ( v6 << 12 ) >> 14 ;
        dest[10] = ( ( v7 << 26 ) >> 14 ) | ( v6 >> 20 ) ;

        dest[11] = ( v7 << 8  ) >> 14 ;
        dest[12] = ( ( v8 << 22 ) >> 14 ) | ( v7 >> 24 ) ;

        dest[13] = ( v8 << 4  ) >> 14 ;
        dest[14] = ( ( v9 << 18 ) >> 14 ) | ( v8 >> 28 ) ;

        dest[15] = v9 >> 14;
    }

    if( rest == 0 ) return;


    uint32_t v1 = src[x++];
    dest[y] = (v1 << 14) >> 14;
    if(--rest == 0) return;

    uint32_t v2 = src[x++];
    dest[++y] = ((v2 << 28) >> 14) | (v1 >> 18);
    if(--rest == 0) return;
    dest[++y] = (v2 << 10) >> 14;
    if(--rest == 0) return;

    uint32_t v3 = src[x++];
    dest[++y] = ((v3 << 24) >> 14) | (v2 >> 22);
    if(--rest == 0) return;
    dest[++y] = (v3 << 6 ) >> 14;
    if(--rest == 0) return;

    uint32_t v4 = src[x++];
    dest[++y] = ((v4 << 20) >> 14) | (v3 >> 26);
    if(--rest == 0) return;
    dest[++y] = (v4 << 2 ) >> 14;
    if(--rest == 0) return;

    uint32_t v5 = src[x++];
    dest[++y] = ((v5 << 16) >> 14) | (v4 >> 30);
    if(--rest == 0) return;

    uint32_t v6 = src[x++];
    dest[++y] = ((v6 << 30) >> 14) | (v5 >> 16);
    if(--rest == 0) return;
    dest[++y] = (v6 << 12) >> 14;
    if(--rest == 0) return;

    uint32_t v7 = src[x++];
    dest[++y] = ((v7 << 26) >> 14) | (v6 >> 20);
    if(--rest == 0) return;
    dest[++y] = (v7 << 8 ) >> 14;
    if(--rest == 0) return;

    uint32_t v8 = src[x++];
    dest[++y] = ((v8 << 22) >> 14) | (v7 >> 24);
    if(--rest == 0) return;
    dest[++y] = (v8 << 4 ) >> 14;
    if(--rest == 0) return;

    uint32_t v9 = src[x++];
    dest[++y] = ((v9 << 18) >> 14) | (v8 >> 28);
    return;

}

inline void Unpack::unpackTail19(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t rest = n;

    dest[0] = src[0] & 0x7FFFF;
    if (--rest == 0) return;
    dest[1] = ((src[0] >> 19) | (src[1] << 13)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[2] = (src[1] >> 6) & 0x7FFFF;
    if (--rest == 0) return;
    dest[3] = ((src[1] >> 25) | (src[2] << 7)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[4] = (src[2] >> 12) & 0x7FFFF;
    if (--rest == 0) return;
    dest[5] = ((src[2] >> 31) | (src[3] << 1)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[6] = ((src[3] >> 18) | (src[4] << 14)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[7] = (src[4] >> 5) & 0x7FFFF;
    if (--rest == 0) return;
    dest[8] = ((src[4] >> 24) | (src[5] << 8)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[9] = (src[5] >> 11) & 0x7FFFF;
    if (--rest == 0) return;
    dest[10] = ((src[5] >> 30) | (src[6] << 2)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[11] = ((src[6] >> 17) | (src[7] << 15)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[12] = (src[7] >> 4) & 0x7FFFF;
    if (--rest == 0) return;
    dest[13] = ((src[7] >> 23) | (src[8] << 9)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[14] = (src[8] >> 10) & 0x7FFFF;
    if (--rest == 0) return;
    dest[15] = ((src[8] >> 29) | (src[9] << 3)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[16] = ((src[9] >> 16) | (src[10] << 16)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[17] = (src[10] >> 3) & 0x7FFFF;
    if (--rest == 0) return;
    dest[18] = ((src[10] >> 22) | (src[11] << 10)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[19] = (src[11] >> 9) & 0x7FFFF;
    if (--rest == 0) return;
    dest[20] = ((src[11] >> 28) | (src[12] << 4)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[21] = ((src[12] >> 15) | (src[13] << 17)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[22] = (src[13] >> 2) & 0x7FFFF;
    if (--rest == 0) return;
    dest[23] = ((src[13] >> 21) | (src[14] << 11)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[24] = (src[14] >> 8) & 0x7FFFF;
    if (--rest == 0) return;
    dest[25] = ((src[14] >> 27) | (src[15] << 5)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[26] = ((src[15] >> 14) | (src[16] << 18)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[27] = (src[16] >> 1) & 0x7FFFF;
    if (--rest == 0) return;
    dest[28] = ((src[16] >> 20) | (src[17] << 12)) & 0x7FFFF;
    if (--rest == 0) return;
    dest[29] = (src[17] >> 7) & 0x7FFFF;
    if (--rest == 0) return;
    dest[30] = ((src[17] >> 26) | (src[18] << 6)) & 0x7FFFF;
}

inline void Unpack::unpackTail20(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b = (n >> 3);
    size_t rest	= (n % 8);

    size_t y = 0;

    for (size_t i = 0; i < b; i++, dest += 8)
    {
        uint32_t v1  = src[ x++ ];
        uint32_t v2  = src[ x++ ];
        uint32_t v3  = src[ x++ ];
        uint32_t v4  = src[ x++ ];
        uint32_t v5  = src[ x++ ];

        dest[0] = ( v1 << 12 ) >> 12 ;
        dest[1] = ( ( v2 << 24 ) >> 12 ) | ( v1 >> 20 );

        dest[2] = ( v2 << 4 ) >> 12 ;
        dest[3] = ( ( v3 << 16 ) >> 12 ) | ( v2 >> 28 );

        dest[4] = ( ( v4 << 28 ) >> 12 ) | ( v3 >> 16 );

        dest[5] = ( v4 << 8 ) >> 12 ;
        dest[6] = ( ( v5 << 20 ) >> 12 ) | ( v4 >> 24 );

        dest[7] = v5 >> 12;
    }

    if( rest == 0 ) return;

    uint32_t v1  = src[x++];
    dest[y  ] = (v1 << 12) >> 12;
    if(--rest == 0) return;

    uint32_t v2  = src[x++];
    dest[++y] = ((v2 << 24) >> 12) | (v1 >> 20);
    if(--rest == 0) return;
    dest[++y] = (v2 << 4) >> 12;
    if(--rest == 0) return;

    uint32_t v3  = src[x++];
    dest[++y] = ((v3 << 16) >> 12) | (v2 >> 28);
    if(--rest == 0) return;

    uint32_t v4  = src[x++];
    dest[++y] = ((v4 << 28) >> 12) | (v3 >> 16);
    if(--rest == 0) return;
    dest[++y] = (v4 << 8) >> 12;
    if(--rest == 0) return;

    uint32_t v5  = src[x++];
    dest[++y] = ((v5 << 20) >> 12) | (v4 >> 24);
    return;
}

inline void Unpack::unpackTail21(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1  = src[x++];
    dest[y  ] = (v1 << 11) >> 11;
    if(--rest == 0) return;

    uint32_t v2  = src[x++];
    dest[++y] = ((v2 << 22) >> 11) | (v1 >> 21);
    if(--rest == 0) return;

    uint32_t v3  = src[x++];
    dest[++y] = (v2 << 1) >> 11;
    if(--rest == 0) return;
    dest[++y] = ((v3 << 12) >> 11) | (v2 >> 31);
    if(--rest == 0) return;

    uint32_t v4  = src[x++];
    dest[++y] = ((v4 << 23) >> 11) | (v3 >> 20);
    if(--rest == 0) return;
    dest[++y] = (v4 << 2) >> 11;
    if(--rest == 0) return;

    uint32_t v5  = src[x++];
    dest[++y] = ((v5 << 13) >> 11) | (v4 >> 30);
    if(--rest == 0) return;

    uint32_t v6  = src[x++];
    dest[++y] = ((v6 << 24) >> 11) | (v5 >> 19);
    if(--rest == 0) return;
    dest[++y] = (v6 << 3) >> 11;
    if(--rest == 0) return;

    uint32_t v7  = src[x++];
    dest[++y] = ((v7 << 14) >> 11) | (v6 >> 29);
    if(--rest == 0) return;

    uint32_t v8  = src[x++];
    dest[++y] = ((v8 << 25) >> 11) | (v7 >> 18);
    if(--rest == 0) return;
    dest[++y] = (v8 << 4) >> 11;
    if(--rest == 0) return;

    uint32_t v9  = src[x++];
    dest[++y] = ((v9 << 15) >> 11) | (v8 >> 28);
    if(--rest == 0) return;

    uint32_t v10  = src[x++];
    dest[++y] = ((v10 << 26) >> 11) | (v9 >> 17);
    if(--rest == 0) return;
    dest[++y] = (v10 << 5) >> 11;
    if(--rest == 0) return;

    uint32_t v11  = src[x++];
    dest[++y] = ((v11 << 16) >> 11) | (v10 >> 27);
    if(--rest == 0) return;

    uint32_t v12  = src[x++];
    dest[++y] = ((v12 << 27) >> 11) | (v11 >> 16);
    if(--rest == 0) return;
    dest[++y] = (v12 << 6) >> 11;
    if(--rest == 0) return;

    uint32_t v13  = src[x++];
    dest[++y] = ((v13 << 17) >> 11) | (v12 >> 26);
    if(--rest == 0) return;

    uint32_t v14  = src[x++];
    dest[++y] = ((v14 << 28) >> 11) | (v13 >> 15);
    if(--rest == 0) return;
    dest[++y] = (v14 << 7) >> 11;
    if(--rest == 0) return;

    uint32_t v15  = src[x++];
    dest[++y] = ((v15 << 18) >> 11) | (v14 >> 25);
    if(--rest == 0) return;

    uint32_t v16  = src[x++];
    dest[++y] = ((v16 << 29) >> 11) | (v15 >> 14);
    if(--rest == 0) return;
    dest[++y] = (v16 << 8) >> 11;
    if(--rest == 0) return;

    uint32_t v17  = src[x++];
    dest[++y] = ((v17 << 19) >> 11) | (v16 >> 24);
    if(--rest == 0) return;

    uint32_t v18  = src[x++];
    dest[++y] = ((v18 << 30) >> 11) | (v17 >> 13);
    if(--rest == 0) return;
    dest[++y] = (v18 << 9) >> 11;
    if(--rest == 0) return;

    uint32_t v19  = src[x++];
    dest[++y] = ((v19 << 20) >> 11) | (v18 >> 23);
    if(--rest == 0) return;

    uint32_t v20  = src[x++];
    dest[++y] = ((v20 << 31) >> 11) | (v19 >> 12);
    if(--rest == 0) return;
    dest[++y] = (v20 << 10) >> 11;
    if(--rest == 0) return;

    uint32_t v21  = src[x++];
    dest[++y] = ((v21 << 21) >> 11) | (v20 >> 22);
    return;
}

inline void Unpack::unpackTail22(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b	=  n  >>  4;
    size_t rest		=  n  %  16;
    size_t y = 0;


    // i process
    for( uint32_t  i = 0 ; i < b ; i++, dest += 16)
    {
        uint32_t v1   = src[ x++ ];
        uint32_t v2   = src[ x++ ];
        uint32_t v3   = src[ x++ ];
        uint32_t v4   = src[ x++ ];
        uint32_t v5   = src[ x++ ];
        uint32_t v6   = src[ x++ ];
        uint32_t v7   = src[ x++ ];
        uint32_t v8   = src[ x++ ];
        uint32_t v9   = src[ x++ ];
        uint32_t v10  = src[ x++ ];
        uint32_t v11  = src[ x++ ];

        dest[0] = ( v1 << 10 ) >> 10;
        dest[1] = ( ( v2 << 20 ) >> 10 ) | ( v1 >> 22 );

        dest[2] = ( ( v3 << 30 ) >> 10 ) | ( v2 >> 12 );

        dest[3] = ( v3 << 8 ) >> 10;
        dest[4] = ( ( v4 << 18 ) >> 10 ) | ( v3 >> 24 );

        dest[5] = ( ( v5 << 28 ) >> 10 ) | ( v4 >> 14 );

        dest[6] = ( v5 << 6 ) >> 10;
        dest[7] = ( ( v6 << 16 ) >> 10 ) | ( v5 >> 26 );

        dest[8] = ( ( v7 << 26 ) >> 10 ) | ( v6 >> 16 );

        dest[9] = ( v7 << 4 ) >> 10;
        dest[10] = ( ( v8 << 14 ) >> 10 ) | ( v7 >> 28 );

        dest[11] = ( ( v9 << 24 ) >> 10 ) | ( v8 >> 18 );

        dest[12] = ( v9 << 2 ) >> 10;
        dest[13] = ( ( v10 << 12 ) >> 10 ) | ( v9 >> 30 );

        dest[14] = ( ( v11 << 22 ) >> 10 ) | ( v10 >> 20 );

        dest[15] = v11 >> 10;
    }

    if( rest == 0 ) return;

    uint32_t v1  = src[x++];
    dest[y++] = (v1 << 10) >> 10;
    if(--rest == 0) return;

    uint32_t v2  = src[x++];
    dest[y++] = ((v2 << 20) >> 10) | (v1 >> 22);
    if(--rest == 0) return;

    uint32_t v3  = src[x++];
    dest[y++] = ((v3 << 30) >> 10) | (v2 >> 12);
    if(--rest == 0) return;
    dest[y++] = (v3 << 8) >> 10;
    if(--rest == 0) return;

    uint32_t v4  = src[x++];
    dest[y++] = ((v4 << 18) >> 10) | (v3 >> 24);
    if(--rest == 0) return;

    uint32_t v5  = src[x++];
    dest[y++] = ((v5 << 28) >> 10) | (v4 >> 14);
    if(--rest == 0) return;
    dest[y++] = (v5 << 6) >> 10;
    if(--rest == 0) return;

    uint32_t v6  = src[x++];
    dest[y++] = ((v6 << 16) >> 10) | (v5 >> 26);
    if(--rest == 0) return;

    uint32_t v7  = src[x++];
    dest[y++] = ((v7 << 26) >> 10) | (v6 >> 16);
    if(--rest == 0) return;
    dest[y++] = (v7 << 4) >> 10;
    if(--rest == 0) return;

    uint32_t v8  = src[x++];
    dest[y++] = ((v8 << 14) >> 10) | (v7 >> 28);
    if(--rest == 0) return;

    uint32_t v9  = src[x++];
    dest[y++] = ((v9 << 24) >> 10) | (v8 >> 18);
    if(--rest == 0) return;
    dest[y++] = (v9 << 2) >> 10;
    if(--rest == 0) return;

    uint32_t v10  = src[x++];
    dest[y++] = ((v10 << 12) >> 10) | (v9 >> 30);
    if(--rest == 0) return;

    uint32_t v11  = src[x++];
    dest[y++] = ((v11 << 22) >> 10) | (v10 >> 20);
    return;
}

inline void Unpack::unpackTail23(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t rest = n;
    dest[0] = src[0] & 0x7FFFFF;
    if (--rest == 0) return;
    dest[1] = ((src[0] >> 23) | (src[1] << 9)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[2] = ((src[1] >> 14) | (src[2] << 18)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[3] = (src[2] >> 5) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[4] = ((src[2] >> 28) | (src[3] << 4)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[5] = ((src[3] >> 19) | (src[4] << 13)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[6] = ((src[4] >> 10) | (src[5] << 22)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[7] = (src[5] >> 1) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[8] = ((src[5] >> 24) | (src[6] << 8)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[9] = ((src[6] >> 15) | (src[7] << 17)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[10] = (src[7] >> 6) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[11] = ((src[7] >> 29) | (src[8] << 3)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[12] = ((src[8] >> 20) | (src[9] << 12)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[13] = ((src[9] >> 11) | (src[10] << 21)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[14] = (src[10] >> 2) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[15] = ((src[10] >> 25) | (src[11] << 7)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[16] = ((src[11] >> 16) | (src[12] << 16)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[17] = (src[12] >> 7) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[18] = ((src[12] >> 30) | (src[13] << 2)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[19] = ((src[13] >> 21) | (src[14] << 11)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[20] = ((src[14] >> 12) | (src[15] << 20)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[21] = (src[15] >> 3) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[22] = ((src[15] >> 26) | (src[16] << 6)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[23] = ((src[16] >> 17) | (src[17] << 15)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[24] = (src[17] >> 8) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[25] = ((src[17] >> 31) | (src[18] << 1)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[26] = ((src[18] >> 22) | (src[19] << 10)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[27] = ((src[19] >> 13) | (src[20] << 19)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[28] = (src[20] >> 4) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[29] = ((src[20] >> 27) | (src[21] << 5)) & 0x7FFFFF;
    if (--rest == 0) return;
    dest[30] = ((src[21] >> 18) | (src[22] << 14)) & 0x7FFFFF;
}

inline void Unpack::unpackTail24(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t b = n >> 2;
    size_t rest =  n % 4;

    // i process
    for (uint32_t  i = 0 ; i < b ; i++, dest += 4, src += 3)
    {
        uint32_t v1   = src[0];
        uint32_t v2   = src[1];
        uint32_t v3   = src[2];

        dest[0] = v1 & 0xFFFFFF;
        dest[1] = ((v1 >> 24) | (v2 << 8)) & 0xFFFFFF;
        dest[2] = ((v2 >> 16) | (v3 << 16)) & 0xFFFFFF;
        dest[3] = (v3 >> 8) & 0xFFFFFF;
    }

    if (rest == 0 ) return;

    dest[0] = src[0] & 0xFFFFFF;
    if (--rest == 0 ) return;
    dest[1] = ((src[0] >> 24) | (src[1] << 8)) & 0xFFFFFF;
    if (--rest == 0 ) return;
    dest[2] = ((src[1] >> 16) | (src[2] << 16)) & 0xFFFFFF;
}

inline void Unpack::unpackTail25(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t rest = n;

    dest[0] = src[0] & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[1] = ((src[0] >> 25) | (src[1] << 7)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[2] = ((src[1] >> 18) | (src[2] << 14)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[3] = ((src[2] >> 11) | (src[3] << 21)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[4] = (src[3] >> 4) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[5] = ((src[3] >> 29) | (src[4] << 3)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[6] = ((src[4] >> 22) | (src[5] << 10)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[7] = ((src[5] >> 15) | (src[6] << 17)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[8] = ((src[6] >> 8) | (src[7] << 24)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[9] = (src[7] >> 1) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[10] = ((src[7] >> 26) | (src[8] << 6)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[11] = ((src[8] >> 19) | (src[9] << 13)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[12] = ((src[9] >> 12) | (src[10] << 20)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[13] = (src[10] >> 5) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[14] = ((src[10] >> 30) | (src[11] << 2)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[15] = ((src[11] >> 23) | (src[12] << 9)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[16] = ((src[12] >> 16) | (src[13] << 16)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[17] = ((src[13] >> 9) | (src[14] << 23)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[18] = (src[14] >> 2) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[19] = ((src[14] >> 27) | (src[15] << 5)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[20] = ((src[15] >> 20) | (src[16] << 12)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[21] = ((src[16] >> 13) | (src[17] << 19)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[22] = (src[17] >> 6) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[23] = ((src[17] >> 31) | (src[18] << 1)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[24] = ((src[18] >> 24) | (src[19] << 8)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[25] = ((src[19] >> 17) | (src[20] << 15)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[26] = ((src[20] >> 10) | (src[21] << 22)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[27] = (src[21] >> 3) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[28] = ((src[21] >> 28) | (src[22] << 4)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[29] = ((src[22] >> 21) | (src[23] << 11)) & 0x1FFFFFF;
    if (--rest == 0) return;
    dest[30] = ((src[23] >> 14) | (src[24] << 18)) & 0x1FFFFFF;
}

inline void Unpack::unpackTail26(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b	=  n  >>  4;
    size_t rest		=  n  %  16;
    size_t y = 0;

    // i process
    for( uint32_t  i = 0 ; i < b ; i++, dest += 16)
    {
        uint32_t v1   = src[ x++ ];
        uint32_t v2   = src[ x++ ];
        uint32_t v3   = src[ x++ ];
        uint32_t v4   = src[ x++ ];
        uint32_t v5   = src[ x++ ];
        uint32_t v6   = src[ x++ ];
        uint32_t v7   = src[ x++ ];
        uint32_t v8   = src[ x++ ];
        uint32_t v9   = src[ x++ ];
        uint32_t v10  = src[ x++ ];
        uint32_t v11  = src[ x++ ];
        uint32_t v12  = src[ x++ ];
        uint32_t v13  = src[ x++ ];

        dest[0] = ( v1 << 6 ) >> 6;
        dest[1] = ( ( v2 << 12 ) >> 6 ) | ( v1 >> 26 );

        dest[2] = ( ( v3 << 18 ) >> 6 ) | ( v2 >> 20 );

        dest[3] = ( ( v4 << 24 ) >> 6 ) | ( v3 >> 14 );

        dest[4] = ( ( v5 << 30 ) >> 6 ) | ( v4 >> 8 );

        dest[5] = ( v5 << 4 ) >> 6;
        dest[6] = ( ( v6 << 10 ) >> 6 ) | ( v5 >> 28 );

        dest[7] = ( ( v7 << 16 ) >> 6 ) | ( v6 >> 22 );

        dest[8] = ( ( v8 << 22 ) >> 6 ) | ( v7 >> 16 );

        dest[9] = ( ( v9 << 28 ) >> 6 ) | ( v8 >> 10 );

        dest[10] = ( v9 << 2 ) >> 6;
        dest[11] = ( ( v10 << 8 ) >> 6 ) | ( v9 >> 30 );

        dest[12] = ( ( v11 << 14 ) >> 6 ) | ( v10 >> 24 );

        dest[13] = ( ( v12 << 20 ) >> 6 ) | ( v11 >> 18 );

        dest[14] = ( ( v13 << 26 ) >> 6 ) | ( v12 >> 12 );
        dest[15] = v13 >> 6;

    }

    if( rest == 0 ) return;

    uint32_t v1   = src[x++];
    dest[ y ] = (v1 << 6) >> 6;
    if(--rest == 0) return;

    uint32_t v2   = src[x++];
    dest[++y] = ((v2 << 12) >> 6) | (v1 >> 26);
    if(--rest == 0) return;

    uint32_t v3   = src[x++];
    dest[++y] = ((v3 << 18) >> 6) | (v2 >> 20);
    if(--rest == 0) return;

    uint32_t v4   = src[x++];
    dest[++y] = ((v4 << 24) >> 6) | (v3 >> 14);
    if(--rest == 0) return;

    uint32_t v5   = src[x++];
    dest[++y] = ((v5 << 30) >> 6) | (v4 >> 8);
    if(--rest == 0) return;
    dest[++y] = (v5 << 4) >> 6;
    if(--rest == 0) return;

    uint32_t v6   = src[x++];
    dest[++y] = ((v6 << 10) >> 6) | (v5 >> 28);
    if(--rest == 0) return;

    uint32_t v7   = src[x++];
    dest[++y] = ((v7 << 16) >> 6) | (v6 >> 22);
    if(--rest == 0) return;

    uint32_t v8   = src[x++];
    dest[++y] = ((v8 << 22) >> 6) | (v7 >> 16);
    if(--rest == 0) return;

    uint32_t v9   = src[x++];
    dest[++y] = ((v9 << 28) >> 6) | (v8 >> 10);
    if(--rest == 0) return;
    dest[++y] = (v9 << 2) >> 6;
    if(--rest == 0) return;

    uint32_t v10   = src[x++];
    dest[++y] = ((v10 << 8) >> 6) | (v9 >> 30);
    if(--rest == 0) return;

    uint32_t v11   = src[x++];
    dest[++y] = ((v11 << 14) >> 6) | (v10 >> 24);
    if(--rest == 0) return;

    uint32_t v12   = src[x++];
    dest[++y] = ((v12 << 20) >> 6) | (v11 >> 18);
    if(--rest == 0) return;

    uint32_t v13   = src[x++];
    dest[++y] = ((v13 << 26) >> 6) | (v12 >> 12);
    return;
}

inline void Unpack::unpackTail27(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t rest = n;
    dest[0] = src[0] & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[1] = ((src[0] >> 27) | (src[1] << 5)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[2] = ((src[1] >> 22) | (src[2] << 10)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[3] = ((src[2] >> 17) | (src[3] << 15)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[4] = ((src[3] >> 12) | (src[4] << 20)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[5] = ((src[4] >> 7) | (src[5] << 25)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[6] = (src[5] >> 2) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[7] = ((src[5] >> 29) | (src[6] << 3)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[8] = ((src[6] >> 24) | (src[7] << 8)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[9] = ((src[7] >> 19) | (src[8] << 13)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[10] = ((src[8] >> 14) | (src[9] << 18)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[11] = ((src[9] >> 9) | (src[10] << 23)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[12] = (src[10] >> 4) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[13] = ((src[10] >> 31) | (src[11] << 1)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[14] = ((src[11] >> 26) | (src[12] << 6)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[15] = ((src[12] >> 21) | (src[13] << 11)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[16] = ((src[13] >> 16) | (src[14] << 16)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[17] = ((src[14] >> 11) | (src[15] << 21)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[18] = ((src[15] >> 6) | (src[16] << 26)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[19] = (src[16] >> 1) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[20] = ((src[16] >> 28) | (src[17] << 4)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[21] = ((src[17] >> 23) | (src[18] << 9)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[22] = ((src[18] >> 18) | (src[19] << 14)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[23] = ((src[19] >> 13) | (src[20] << 19)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[24] = ((src[20] >> 8) | (src[21] << 24)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[25] = (src[21] >> 3) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[26] = ((src[21] >> 30) | (src[22] << 2)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[27] = ((src[22] >> 25) | (src[23] << 7)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[28] = ((src[23] >> 20) | (src[24] << 12)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[29] = ((src[24] >> 15) | (src[25] << 17)) & 0x7FFFFFF;
    if (--rest == 0) return;
    dest[30] = ((src[25] >> 10) | (src[26] << 22)) & 0x7FFFFFF;
}

inline void Unpack::unpackTail28(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t b	=  n  >>	3;
    size_t rest		=  n  %	8;
    size_t y = 0;

    // i process
    for( uint32_t  i = 0 ; i < b ; i++, dest += 8)
    {
        uint32_t v1   = src[ x++ ];
        uint32_t v2   = src[ x++ ];
        uint32_t v3   = src[ x++ ];
        uint32_t v4   = src[ x++ ];
        uint32_t v5   = src[ x++ ];
        uint32_t v6   = src[ x++ ];
        uint32_t v7   = src[ x++ ];

        dest[0] = ( v1 << 4 ) >> 4;
        dest[1] = ( ( v2 << 8 ) >> 4 ) | ( v1 >> 28 );

        dest[2] = ( ( v3 << 12 ) >> 4 ) | ( v2 >> 24 );

        dest[3] = ( ( v4 << 16 ) >> 4 ) | ( v3 >> 20 );

        dest[4] = ( ( v5 << 20 ) >> 4 ) | ( v4 >> 16 );

        dest[5] = ( ( v6 << 24 ) >> 4 ) | ( v5 >> 12 );

        dest[6] = ( ( v7 << 28 ) >> 4 ) | ( v6 >> 8 );

        dest[7] = v7 >> 4 ;
    }

    if( rest == 0 ) return;

    uint32_t v1   = src[x++];
    dest[ y ] = (v1 << 4) >> 4;
    if(--rest == 0) return;

    uint32_t v2   = src[x++];
    dest[++y] = ((v2 << 8) >> 4) | (v1 >> 28);
    if(--rest == 0) return;

    uint32_t v3   = src[x++];
    dest[++y] = ((v3 << 12) >> 4) | (v2 >> 24);
    if(--rest == 0) return;

    uint32_t v4   = src[x++];
    dest[++y] = ((v4 << 16) >> 4) | (v3 >> 20);
    if(--rest == 0) return;

    uint32_t v5   = src[x++];
    dest[++y] = ((v5 << 20) >> 4) | (v4 >> 16);
    if(--rest == 0) return;

    uint32_t v6   = src[x++];
    dest[++y] = ((v6 << 24) >> 4) | (v5 >> 12);
    if(--rest == 0) return;

    uint32_t v7   = src[x++];
    dest[++y] = ((v7 << 28) >> 4) | (v6 >> 8);
    return;
}

inline void Unpack::unpackTail29(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t x = 0;
    size_t rest = n;
    size_t y = 0;
    uint32_t v1   = src[x++];
    dest[ y ] = (v1 << 3) >> 3;
    if(--rest == 0) return;

    uint32_t v2   = src[x++];
    dest[++y] = ((v2 << 6) >> 3) | (v1 >> 29);
    if(--rest == 0) return;

    uint32_t v3   = src[x++];
    dest[++y] = ((v3 << 9) >> 3) | (v2 >> 26);
    if(--rest == 0) return;

    uint32_t v4   = src[x++];
    dest[++y] = ((v4 << 12) >> 3) | (v3 >> 23);
    if(--rest == 0) return;

    uint32_t v5   = src[x++];
    dest[++y] = ((v5 << 15) >> 3) | (v4 >> 20);
    if(--rest == 0) return;

    uint32_t v6   = src[x++];
    dest[++y] = ((v6 << 18) >> 3) | (v5 >> 17);
    if(--rest == 0) return;

    uint32_t v7   = src[x++];
    dest[++y] = ((v7 << 21) >> 3) | (v6 >> 14);
    if(--rest == 0) return;

    uint32_t v8   = src[x++];
    dest[++y] = ((v8 << 24) >> 3) | (v7 >> 11);
    if(--rest == 0) return;

    uint32_t v9   = src[x++];
    dest[++y] = ((v9 << 27) >> 3) | (v8 >> 8);
    if(--rest == 0) return;

    uint32_t v10   = src[x++];
    dest[++y] = ((v10 << 30) >> 3) | (v9 >> 5);
    if(--rest == 0) return;
    dest[++y] = (v10 << 1) >> 3;
    if(--rest == 0) return;

    uint32_t v11   = src[x++];
    dest[++y] = ((v11 << 4) >> 3) | (v10 >> 31);
    if(--rest == 0) return;

    uint32_t v12   = src[x++];
    dest[++y] = ((v12 << 7 ) >> 3) | (v11 >> 28);
    if(--rest == 0) return;

    uint32_t v13   = src[x++];
    dest[++y] = ((v13 << 10) >> 3) | (v12 >> 25);
    if(--rest == 0) return;

    uint32_t v14   = src[x++];
    dest[++y] = ((v14 << 13) >> 3) | (v13 >> 22);
    if(--rest == 0) return;

    uint32_t v15   = src[x++];
    dest[++y] = ((v15 << 16) >> 3) | (v14 >> 19);
    if(--rest == 0) return;

    uint32_t v16   = src[x++];
    dest[++y] = ((v16 << 19) >> 3) | (v15 >> 16);
    if(--rest == 0) return;

    uint32_t v17   = src[x++];
    dest[++y] = ((v17 << 22) >> 3) | (v16 >> 13);
    if(--rest == 0) return;

    uint32_t v18   = src[x++];
    dest[++y] = ((v18 << 25) >> 3) | (v17 >> 10);
    if(--rest == 0) return;

    uint32_t v19   = src[x++];
    dest[++y] = ((v19 << 28) >> 3) | (v18 >> 7);
    if(--rest == 0) return;

    uint32_t v20   = src[x++];
    dest[++y] = ((v20 << 31) >> 3) | (v19 >> 4);
    if(--rest == 0) return;
    dest[++y] = (v20 << 2) >> 3;
    if(--rest == 0) return;

    uint32_t v21   = src[x++];
    dest[++y] = ((v21 << 5) >> 3) | (v20 >> 30);
    if(--rest == 0) return;

    uint32_t v22   = src[x++];
    dest[++y] = ((v22 << 8) >> 3) | (v21 >> 27);
    if(--rest == 0) return;

    uint32_t v23   = src[x++];
    dest[++y] = ((v23 << 11) >> 3) | (v22 >> 24);
    if(--rest == 0) return;

    uint32_t v24   = src[x++];
    dest[++y] = ((v24 << 14) >> 3) | (v23 >> 21);
    if(--rest == 0) return;

    uint32_t v25   = src[x++];
    dest[++y] = ((v25 << 17) >> 3) | (v24 >> 18);
    if(--rest == 0) return;

    uint32_t v26   = src[x++];
    dest[++y] = ((v26 << 20) >> 3) | (v25 >> 15);
    if(--rest == 0) return;

    uint32_t v27   = src[x++];
    dest[++y] = ((v27 << 23) >> 3) | (v26 >> 12);
    if(--rest == 0) return;

    uint32_t v28   = src[x++];
    dest[++y] = ((v28 << 26) >> 3) | (v27 >> 9);
    if(--rest == 0) return;

    uint32_t v29   = src[x++];
    dest[++y] = ((v29 << 29) >> 3) | (v28 >> 6);
    return;
}

inline void Unpack::unpackTail30(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t b = n >> 4;
    size_t rest = n % 16;

    for (size_t i = 0; i < b; i++, dest += 16, src += 15)
    {
        dest[0] = src[0] & 0x3FFFFFFF;
        dest[1] = ((src[0] >> 30) | (src[1] << 2)) & 0x3FFFFFFF;
        dest[2] = ((src[1] >> 28) | (src[2] << 4)) & 0x3FFFFFFF;
        dest[3] = ((src[2] >> 26) | (src[3] << 6)) & 0x3FFFFFFF;
        dest[4] = ((src[3] >> 24) | (src[4] << 8)) & 0x3FFFFFFF;
        dest[5] = ((src[4] >> 22) | (src[5] << 10)) & 0x3FFFFFFF;
        dest[6] = ((src[5] >> 20) | (src[6] << 12)) & 0x3FFFFFFF;
        dest[7] = ((src[6] >> 18) | (src[7] << 14)) & 0x3FFFFFFF;
        dest[8] = ((src[7] >> 16) | (src[8] << 16)) & 0x3FFFFFFF;
        dest[9] = ((src[8] >> 14) | (src[9] << 18)) & 0x3FFFFFFF;
        dest[10] = ((src[9] >> 12) | (src[10] << 20)) & 0x3FFFFFFF;
        dest[11] = ((src[10] >> 10) | (src[11] << 22)) & 0x3FFFFFFF;
        dest[12] = ((src[11] >> 8) | (src[12] << 24)) & 0x3FFFFFFF;
        dest[13] = ((src[12] >> 6) | (src[13] << 26)) & 0x3FFFFFFF;
        dest[14] = ((src[13] >> 4) | (src[14] << 28)) & 0x3FFFFFFF;
        dest[15] = (src[14] >> 2) & 0x3FFFFFFF;
    }
    if (rest == 0) return;

    dest[0] = src[0] & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[1] = ((src[0] >> 30) | (src[1] << 2)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[2] = ((src[1] >> 28) | (src[2] << 4)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[3] = ((src[2] >> 26) | (src[3] << 6)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[4] = ((src[3] >> 24) | (src[4] << 8)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[5] = ((src[4] >> 22) | (src[5] << 10)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[6] = ((src[5] >> 20) | (src[6] << 12)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[7] = ((src[6] >> 18) | (src[7] << 14)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[8] = ((src[7] >> 16) | (src[8] << 16)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[9] = ((src[8] >> 14) | (src[9] << 18)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[10] = ((src[9] >> 12) | (src[10] << 20)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[11] = ((src[10] >> 10) | (src[11] << 22)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[12] = ((src[11] >> 8) | (src[12] << 24)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[13] = ((src[12] >> 6) | (src[13] << 26)) & 0x3FFFFFFF;
    if (--rest == 0) return;
    dest[14] = ((src[13] >> 4) | (src[14] << 28)) & 0x3FFFFFFF;
}

inline void Unpack::unpackTail31(uint32_t* dest, const uint32_t* src, size_t n)
{
    size_t rest = n;

    dest[0] = src[0] & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[1] = ((src[0] >> 31) | (src[1] << 1)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[2] = ((src[1] >> 30) | (src[2] << 2)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[3] = ((src[2] >> 29) | (src[3] << 3)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[4] = ((src[3] >> 28) | (src[4] << 4)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[5] = ((src[4] >> 27) | (src[5] << 5)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[6] = ((src[5] >> 26) | (src[6] << 6)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[7] = ((src[6] >> 25) | (src[7] << 7)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[8] = ((src[7] >> 24) | (src[8] << 8)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[9] = ((src[8] >> 23) | (src[9] << 9)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[10] = ((src[9] >> 22) | (src[10] << 10)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[11] = ((src[10] >> 21) | (src[11] << 11)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[12] = ((src[11] >> 20) | (src[12] << 12)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[13] = ((src[12] >> 19) | (src[13] << 13)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[14] = ((src[13] >> 18) | (src[14] << 14)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[15] = ((src[14] >> 17) | (src[15] << 15)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[16] = ((src[15] >> 16) | (src[16] << 16)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[17] = ((src[16] >> 15) | (src[17] << 17)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[18] = ((src[17] >> 14) | (src[18] << 18)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[19] = ((src[18] >> 13) | (src[19] << 19)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[20] = ((src[19] >> 12) | (src[20] << 20)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[21] = ((src[20] >> 11) | (src[21] << 21)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[22] = ((src[21] >> 10) | (src[22] << 22)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[23] = ((src[22] >> 9) | (src[23] << 23)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[24] = ((src[23] >> 8) | (src[24] << 24)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[25] = ((src[24] >> 7) | (src[25] << 25)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[26] = ((src[25] >> 6) | (src[26] << 26)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[27] = ((src[26] >> 5) | (src[27] << 27)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[28] = ((src[27] >> 4) | (src[28] << 28)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[29] = ((src[28] >> 3) | (src[29] << 29)) & 0x7FFFFFFF;
    if (--rest == 0) return;
    dest[30] = ((src[29] >> 2) | (src[30] << 30)) & 0x7FFFFFFF;
}

inline void Unpack::unpackTail32(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        dest[i] = src[i] & 0xFFFFFFFF;
    }
}

FX_NS_END

#endif //__FX_UNPACK_H
