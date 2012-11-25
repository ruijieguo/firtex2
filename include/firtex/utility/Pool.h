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
// Created : 2011-06-04 13:27:14

#ifndef __FX_POOL_H
#define __FX_POOL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(utility);

class Pool
{
public:
    virtual ~Pool() {}

public:
    /**
     * try allocate memory from pool. 
     *
     * @param nBytes number of byets to allocate
     * 
     * @return address of allocated memory
     */
    virtual void* tryAllocate(size_t nBytes) = 0;

    /**
     * Allocate memory from pool. 
     *
     * @param nBytes number of byets to allocate
     * 
     * @return address of allocated memory
     */
    virtual void* allocate(size_t nBytes) = 0;

    /// Clear allocated memory
    virtual void clear() = 0;
};

DEFINE_TYPED_PTR(Pool);

FX_NS_END

#endif //__FX_POOL_H
