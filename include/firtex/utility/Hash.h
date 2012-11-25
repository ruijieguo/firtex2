/*
 * Copyright (C) 2007 Guo Ruijie. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */

#ifndef __HASH_H
#define __HASH_H

#include "firtex/common/StdHeader.h"
#include <cstring>
#include <cctype>

FX_NS_DEF(utility)

/**
 * @class Hash
 * @author Guo Ruijie
 * @date <2008-05-03>  
 *
 * This class provides some hash functions 
 */

class Hash
{
public:
    static uint32_t ELFhash(const char* pcUrl);
    static uint32_t JSHash(const char* pcUrl);
    static uint32_t APHash(const char *pcUrl);
    static uint32_t DJBHash(const char* pcUrl);
    static uint32_t RSHash(const char *pcUrl);


    /**
     * Paul Hseih's hash algorithm
     * from http://www.azillionmonkeys.com/qed/hash.html
     */
    inline static uint32_t hseihHash(const char* key, size_t len);
	
    static uint32_t hashString(const char* pszStr, uint32_t nHashType);
    static uint64_t hashString64(const char* pszStr);

    static uint32_t caseInsensitiveHashString(const char* pszStr, uint32_t nHashType);
    static uint64_t caseInsensitiveHashString64(const char* pszStr);

    static uint32_t hashString(const char* pszStr, size_t len, uint32_t nHashType);
    static uint64_t hashString64(const char* pszStr, size_t len);

public:
    static void initCryptTable();

public:
    static uint32_t CRYPT_TABLE[0x500];
};

//////////////////////////////////////////////////////////
///
#define get16bits(d) (*((const uint16_t*)(d)))

inline uint32_t Hash::hseihHash(const char* key, size_t len) 
{  
    uint32_t hash = (uint32_t)len;
    uint32_t tmp;
    int rem;
    if (len <= 0 || key == NULL) return 0;
    rem = (int)(len & 3);
    len >>= 2;

    // Main loop 
    for (; len > 0; --len)
    {
        hash  += get16bits (key);
        tmp    = (get16bits (key+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        key  += 4;
        hash  += hash >> 11;
    }

    // Handle end cases 
    switch (rem) 
    {
    case 3: 
        hash += get16bits (key);
        hash ^= hash << 16;
        hash ^= key[2] << 18;
        hash += hash >> 11;
        break;

    case 2:
        hash += get16bits(key);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1:
        hash += *key;
        hash ^= hash << 10;
        hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

#undef get16bits

//static 
inline uint32_t Hash::caseInsensitiveHashString(const char* pszStr, uint32_t nHashType)
{
    unsigned char* key = (unsigned char*)pszStr;
    uint32_t seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
    int32_t ch;

    while(*key != 0)
    { 
        ch = std::tolower(*key++);

        seed1 = CRYPT_TABLE[(nHashType << 8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
    }
    return seed1; 
}
 
//static 
inline uint64_t Hash::caseInsensitiveHashString64(const char* pszStr)
{
    uint64_t nHashValue = caseInsensitiveHashString(pszStr, 0);
    nHashValue <<= 32;
    nHashValue |= caseInsensitiveHashString(pszStr, 1);
    return nHashValue;
}

FX_NS_END

#endif
