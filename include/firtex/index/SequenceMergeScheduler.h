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
// Created : 2011-06-14 21:32:48

#ifndef __FX_SEQUENCEMERGESCHEDULER_H
#define __FX_SEQUENCEMERGESCHEDULER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/MergeScheduler.h"

FX_NS_DEF(index);

class SequenceMergeScheduler : public MergeScheduler
{
public:
    SequenceMergeScheduler();
    ~SequenceMergeScheduler();

public:
    /// Perform merge operation in sequence
    virtual void merge(const MergeablePtr& pMergeObj);

    /// Wait until all merge task finished
    virtual void waitFinish() {};

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_SEQUENCEMERGESCHEDULER_H
