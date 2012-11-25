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
// Created : 2011-09-08 21:25:23

#ifndef __FX_COMMITCONDITION_H
#define __FX_COMMITCONDITION_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(index);

struct CommitCondition
{
    CommitCondition()
        : maxDocs(0)
        , maxTime(0)
    {
    }

    
    /// Maximum number of documents to add since the last
    /// commit before automatically triggering a new commit.
    int64_t maxDocs;

    /// Maximum amount of time (in milliscond) that is allowed to pass
    /// since a document was added before automatically
    /// triggering a new commit. 
    int64_t maxTime;
};

FX_NS_END

#endif //__FX_COMMITCONDITION_H
