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
// Created : 2012-04-21 17:47:24

#ifndef __FX_NUMERICHASHER_H
#define __FX_NUMERICHASHER_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(index);

template <typename Key>
struct NumericHasher
{
    typedef Key KeyType;

    inline KeyType operator() (const char* str, size_t len) const
    {
        return (KeyType)STRTOLL(str, NULL, 10);
    }
};


FX_NS_END

#endif //__FX_NUMERICHASHER_H
