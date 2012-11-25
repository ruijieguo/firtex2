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
// Created : 2011-08-16 23:28:43

#ifndef __FX_HASHCOMPARATOR_H
#define __FX_HASHCOMPARATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Hash.h"

FX_NS_DEF(utility);

///////////////////////////////////////////////////////////////////////////
//Hasher
template<typename T>
struct Hasher 
{
    uint32_t operator()(const T& x) const 
    { 
        return Hash::hseihHash(reinterpret_cast<const char*>(&x), sizeof(T));
    }  
};

template<>
struct Hasher<const char*> 
{
    uint32_t operator()(const char* x) const 
    { 
        return Hash::hseihHash(x, strLength(x));
    }  
};

template<>
struct Hasher<std::string> 
{
    uint32_t operator()(const std::string& x) const 
    { 
        return Hash::hseihHash(x.c_str(), x.length());
    }  
};


template<typename T>
struct HashComparator 
{
    bool operator()(const T& x, const T& y) const 
    {
        return x == y;
    }
};

template<>
struct HashComparator<const char*> 
{
    bool operator()(const char* x, const char* y) const 
    {
        if (x == y) return true;
        if (!x || !y) return false;
        while (*x && *y) 
        {
            if (*x++ != *y++) return false;
        }
        return *x == *y;
    }
};

template<>
struct HashComparator<std::string> 
{
    bool operator()(const std::string& x, const std::string& y) const 
    {
        return (x == y);
    }
};

FX_NS_END

#endif //__FX_HASHCOMPARATOR_H
