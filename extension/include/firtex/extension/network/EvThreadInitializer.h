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
// Created : 2012-01-14 11:04:18

#ifndef __FX_EVTHREADINITIALIZER_H
#define __FX_EVTHREADINITIALIZER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(network);

class EvThreadInitializer : public FX_NS(utility)::Singleton<EvThreadInitializer>
{
public:
    EvThreadInitializer();
    ~EvThreadInitializer();

public:
    void initThreads();

private:
    DECLARE_LOGGER();
};

#ifdef FX_WINDOWS
#define FX_EVENT_BASE_NEW(base) \
struct event_config* cfg = event_config_new();\
	if (cfg) \
{\
	event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP);\
	base = event_base_new_with_config(cfg);\
	event_config_free(cfg);\
}\
	else\
{\
	base = event_base_new();\
}
#else
#define FX_EVENT_BASE_NEW(base) \
	base = event_base_new()
#endif

#define FX_EVENT_BASE_FREE(base)\
	event_base_free(base)

FX_NS_END

#endif //__FX_EVTHREADINITIALIZER_H
