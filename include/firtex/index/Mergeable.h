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
// Created : 2011-07-14 21:42:29

#ifndef __FX_MERGEABLE_H
#define __FX_MERGEABLE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class Mergeable
{
public:
    Mergeable() {}
    virtual ~Mergeable() {}

public:
    /// Perform merge operation.
    virtual void merge() = 0;
};

DEFINE_TYPED_PTR(Mergeable);

FX_NS_END

#endif //__FX_MERGEABLE_H
