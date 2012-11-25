//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-02-20 15:17:54

#ifndef __FX_OPTIMIZEMERGEPOLICY_H
#define __FX_OPTIMIZEMERGEPOLICY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/MergePolicy.h"

FX_NS_DEF(index);

class OptimizeMergePolicy : public MergePolicy
{
public:
    OptimizeMergePolicy();
    ~OptimizeMergePolicy();

    DECLARE_MERGEPOLICY_CREATOR(OptimizeMergePolicy, "optimize");

public:
    /**
     * Create merge proposal
     * @param barrelsInfo the barrels need to merge
     * @return merge proposals
     */
    MergeProposalPtr createProposals(const BarrelsInfo& barrelsInfo);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_OPTIMIZEMERGEPOLICY_H
