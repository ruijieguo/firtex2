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
// Created	: 2010-09-11 20:36:33

#ifndef __FX_BYTESLICE_H
#define __FX_BYTESLICE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

#pragma pack(push, 1)

struct ByteSlice
{	
    ByteSlice(size_t n) : size(n), next(NULL) {}
		
    size_t size;
    ByteSlice* next;
    uint8_t data[1];

    static void destroy(ByteSlice* pSlice)
    {
        delete[] (uint8_t*)pSlice;
    }
};

#pragma pack(pop)

FX_NS_END

#endif //__FX_BYTESLICE_H
