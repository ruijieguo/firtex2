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
// Created	: 2011-02-15 21:01:41

#ifndef __FX_COMMITTABLE_H
#define __FX_COMMITTABLE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class Committable
{
public:
    virtual ~Committable() {}

public:
    virtual void commit() = 0;
};

DEFINE_TYPED_PTR(Committable);

FX_NS_END

#endif //__FX_COMMITTABLE_H
