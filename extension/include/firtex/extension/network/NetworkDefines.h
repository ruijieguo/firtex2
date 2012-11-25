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
// Created : 2012-01-14 00:36:09

#ifndef __FX_NETWORKDEFINES_H
#define __FX_NETWORKDEFINES_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(network);

enum ServiceStatus
{
    ST_ERROR = 1,
    ST_TIMEOUT,
    ST_DISCONNECTED,
    ST_OK,
};

FX_NS_END

#endif //__FX_NETWORKDEFINES_H
