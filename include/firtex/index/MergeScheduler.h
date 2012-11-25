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
// Created : 2011-06-14 21:30:11

#ifndef __FX_MERGESCHEDULER_H
#define __FX_MERGESCHEDULER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/Mergeable.h"

FX_NS_DEF(index);

class MergeScheduler
{
public:
    MergeScheduler() {}
    virtual ~MergeScheduler() {}

public:
    /// Perform merge operation.
    virtual void merge(const MergeablePtr& pMergeObj) = 0;

    /// Wait until all merge task finished
    virtual void waitFinish() = 0;
};
DEFINE_TYPED_PTR(MergeScheduler);

FX_NS_END

#endif //__FX_MERGESCHEDULER_H
