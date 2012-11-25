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
// Created : 2011-06-19 00:19:04

#ifndef __FX_PROGRESSOBSERVER_H
#define __FX_PROGRESSOBSERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(collection);

class ProgressObserver
{
public:
    virtual ~ProgressObserver() {}

public:
    /// Move ahead
    virtual void step() = 0;

    ///  Process complete
    virtual void complete() = 0;
};

DEFINE_TYPED_PTR(ProgressObserver);

FX_NS_END

#endif //__FX_PROGRESSOBSERVER_H
