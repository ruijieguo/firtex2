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
// Created : 2012-01-07 14:54:55

#ifndef __FX_FRAMEDMEMORYTRANSPORT_H
#define __FX_FRAMEDMEMORYTRANSPORT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "thrift/transport/TBufferTransports.h"

#ifdef WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#else
#include <arpa/inet.h>
#endif

FX_NS_DEF(app);

class FramedMemoryTransport : public apache::thrift::transport::TMemoryBuffer
{
public:
    FramedMemoryTransport();
    ~FramedMemoryTransport();

public:
    virtual void flush();

    void resetBuffer(uint8_t* buf, uint32_t sz, MemoryPolicy policy = OBSERVE) 
    {
        apache::thrift::transport::TMemoryBuffer::resetBuffer(buf, sz, policy);
        int32_t len = ntohl(*(uint32_t*)rBase_);
        rBase_ += sizeof(len);
        rBound_ = rBase_ + len;
        FX_TRACE("Reset buffer, data size: [%d], frame size: [%d]",
                 len, (int32_t)sz);
    }

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_FRAMEDMEMORYTRANSPORT_H
