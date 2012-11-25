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
// Created	: 2006/5/20
//
#ifndef __STDINT_H__
#define __STDINT_H__

#include "firtex/common/Common.h"
#include <limits>
#include <ostream>

#ifndef FX_WINDOWS
#include <inttypes.h>
typedef int64_t offset_t;
typedef uint8_t byte;

#else//for Win32
#include <sys/types.h>
#include <wchar.h>

typedef signed char	 int8_t;
typedef short		 int16_t;
typedef long             int32_t;
typedef __int64		 int64_t;
typedef unsigned char	 uint8_t;
typedef unsigned short	 uint16_t;
typedef unsigned long	 uint32_t;
typedef unsigned __int64 uint64_t;

typedef unsigned char	 byte;

typedef __int64		 intmax_t;
typedef unsigned __int64 uintmax_t;

typedef int64_t offset_t;

#ifdef _WIN64 
typedef __int64 ssize_t; 
#else 
typedef _W64 int ssize_t; 
#endif 

#endif

struct uint128_t
{
    uint128_t(uint64_t h, uint64_t l) : hi(h), lo(l) {}
    uint128_t(uint64_t l) : hi(0), lo(l) {}
    uint128_t() : hi(0), lo(0) {}
    uint128_t(const uint128_t& src) : hi(src.hi), lo(src.lo) {}

    uint64_t hi;
    uint64_t lo;

    inline uint128_t& operator = (const uint128_t& src)
    {
        hi = src.hi;
        lo = src.lo;
        return *this;
    }

    inline bool operator > (const uint128_t& src) const
    {
        if (hi > src.hi)
            return true;
        if (hi == src.hi)
            return lo > src.lo;
        return false;
    }

    inline bool operator >= (const uint128_t& src) const
    {
        if (hi > src.hi)
            return true;
        if (hi == src.hi)
            return lo >= src.lo;
        return false;
    }

    inline bool operator < (const uint128_t& src) const
    {
        if (hi < src.hi)
            return true;
        if (hi == src.hi)
            return lo < src.lo;
        return false;
    }

    inline bool operator <= (const uint128_t& src) const
    {
        if (hi < src.hi)
            return true;
        if (hi == src.hi)
            return lo <= src.lo;
        return false;
    }

    inline bool operator == (const uint128_t& src) const
    {
        return (hi == src.hi) && (lo == src.lo);
    }

    inline bool operator != (const uint128_t& src) const
    {
        return (hi != src.hi) || (lo != src.lo);
    }
};

namespace std
{
template<>
struct numeric_limits<uint128_t>
{
    static const bool is_specialized = true;

    static uint128_t min() throw()
    { return uint128_t(0, 0); }
    static uint128_t max() throw()
    {
        return uint128_t(std::numeric_limits<uint64_t>::max(), 
                         std::numeric_limits<uint64_t>::max()); 
    }
};
}

template<class Ch> 
inline std::basic_ostream<Ch>& operator << (std::basic_ostream<Ch>& out, const uint128_t& v)
{
    out << v.hi << v.lo;
    return out;
}


/* LIMIT MACROS */
#ifndef MAX_UINT64
#define MAX_UINT64  ( ~ ((uint64_t) 0) )
#endif

#ifndef MAX_INT64
#define MAX_INT64   ( (int64_t) ( MAX_UINT64 >> 1 ) )
#endif

#ifndef MIN_INT64
#define MIN_INT64   ( (int64_t) ( MAX_UINT64 ^ ( (uint64_t) MAX_INT64 ) ) )
#endif

#ifndef MAX_UINT32
#define MAX_UINT32  ( ~ ((uint32_t) 0) )
#endif

#ifndef MAX_INT32
#define MAX_INT32   ( (int32_t) (MAX_UINT32 >> 1) )
#endif

#ifndef MIN_INT32
#define MIN_INT32   ( (int32_t) ( MAX_UINT32 ^ ( (uint32_t) MAX_INT32 ) ) )
#endif

#undef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#undef 	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#ifndef NULL
#define NULL 0
#endif

#endif /* __STDINT_H__ */
