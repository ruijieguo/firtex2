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
// Created : 2012-03-12 22:04:14

#ifndef __FX_PROCESS_H
#define __FX_PROCESS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <vector>

#ifdef FX_WINDOWS
#include "firtex/extension/util/Process_WIN32.h"
#elif defined FX_POSIX || defined FX_MACOS
#include "firtex/extension/util/Process_POSIX.h"
#else
#error "No Process implement."
#endif

#endif //__FX_PROCESS_H
