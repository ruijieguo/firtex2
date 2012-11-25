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
// Created : 2011-07-05 22:39:38

#ifndef __FX_POSTINGPOOL_H
#define __FX_POSTINGPOOL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/LooseBoundedPool.h"

FX_NS_DEF(index);

struct PostingPool
{
    typedef FX_NS(utility)::LooseBoundedPool PoolType;

    PostingPool(PoolType* postPool, PoolType* buffPool)
        : postingPool(postPool)
        , bufferPool(buffPool)
    {
    }
    ~PostingPool()
    {
        delete postingPool;
        if (bufferPool)
        {
            delete bufferPool;
        }
    }

    void resetBufferPool()
    {
        delete bufferPool;
        bufferPool = NULL;
    }

    PoolType* postingPool;
    PoolType* bufferPool;
};

DEFINE_TYPED_PTR(PostingPool);

FX_NS_END

#endif //__FX_POSTINGPOOL_H
