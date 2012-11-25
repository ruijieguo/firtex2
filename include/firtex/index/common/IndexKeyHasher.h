//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-12-25 09:46:15

#ifndef __FX_INDEXER_HASHER_H
#define __FX_INDEXER_HASHER_H

#include "firtex/utility/Hash.h"

FX_NS_DEF(index);

template <typename Key>
struct IndexKeyHasher
{
    typedef Key KeyType;

    inline KeyType operator() (const char* str, size_t len) const
    {
        assert(false);
    }
};

template <>
struct IndexKeyHasher<uint64_t>
{
    typedef uint64_t KeyType;

    inline KeyType operator() (const char* str, size_t len) const
    {
        return FX_NS(utility)::Hash::hashString64(str);
    }
};

template <typename Key>
struct DateTimeHasher
{
    typedef Key KeyType;

    inline KeyType operator() (const char* str, size_t len) const
    {
        return (KeyType)STRTOLL(str, NULL, 10);
    }
};

FX_NS_END

#endif
