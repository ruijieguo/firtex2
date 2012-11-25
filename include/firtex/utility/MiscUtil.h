//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/20
//
#ifndef _MISC_UTIL_H
#define _MISC_UTIL_H

#include "firtex/common/StdHeader.h"
#include <string>
#include <algorithm>
#include <assert.h>

FX_NS_DEF(utility)

static uint64_t POW_TABLE[] = 
{
    1ULL, 2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL, 128ULL, 256ULL, 512ULL, 
    1024ULL, 2048ULL, 4096ULL, 8192ULL, 16384ULL, 32768ULL, 65536ULL, 131072ULL, 262144ULL, 524288ULL, 
    1048576ULL, 2097152ULL, 4194304ULL, 8388608ULL, 16777216ULL, 33554432ULL, 67108864ULL, 134217728ULL, 268435456ULL, 536870912ULL, 
    1073741824ULL, 2147483648ULL, 4294967296ULL, 8589934592ULL, 17179869184ULL, 34359738368ULL, 68719476736ULL, 137438953472ULL,
    274877906944ULL, 549755813888ULL, 1099511627776ULL, 2199023255552ULL, 4398046511104ULL, 8796093022208ULL, 17592186044416ULL,
    35184372088832ULL, 70368744177664ULL, 140737488355328ULL, 281474976710656ULL, 562949953421312ULL, 1125899906842624ULL, 
    2251799813685248ULL, 4503599627370496ULL, 9007199254740992ULL, 18014398509481984ULL, 36028797018963968ULL, 72057594037927936ULL,
    144115188075855872ULL, 288230376151711744ULL, 576460752303423488ULL, 1152921504606846976ULL, 2305843009213693952ULL,
    4611686018427387904ULL, 
};

class MiscUtil
{
public:
    /**
     * get current time
     */
    static int64_t currentTimeMillis();

    /** compute the upper bound of log2(value) */
    static uint32_t log2UpperBound(uint32_t val);
    static uint32_t log2UpperBound(uint64_t val);

    /** Compute the lower bound of log2(value) */
    static uint32_t log2LowerBound(uint32_t val);
    static uint32_t log2LowerBound(uint64_t val);

    static inline uint64_t power2(uint32_t val);

    static std::string getSelfPath();

    /**
     * Convert requset to dest encode
     *
     * @param sResult converted result
     * @param sRequet the source requset
     * @param sDstEncode dest encode name
     * @param sSrcEncodePrefix prefix of source encode name
     * @return -1 if failed, 0 if no convert, 1 if converted
     */
    static int32_t convertRequest(std::string& sResult, 
                                  const std::string& sRequet, 
                                  const std::string& sDstEncode,
                                  const std::string& sSrcEncodePrefix);
};

//////////////////////////////////////////////////////////////////////////
///
inline uint64_t MiscUtil::power2(uint32_t val)
{
    assert(val < sizeof(POW_TABLE));
    return POW_TABLE[val];
}

FX_NS_END

#endif
