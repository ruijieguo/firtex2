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
// Created : 2012-01-09 21:19:24

#ifndef __FX_EVDATABUFFER_H
#define __FX_EVDATABUFFER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <event2/buffer.h>

FX_NS_DEF(network);

class EvDataBuffer
{
public:
    EvDataBuffer(evbuffer* buf, bool bOwner = false)
        : m_evbuf(buf)
        , m_bBufOwner(bOwner)
    {
    }

    EvDataBuffer()
    {
        m_evbuf = evbuffer_new();
        m_bBufOwner = true;
    }

    ~EvDataBuffer()
    {
        if (m_evbuf && m_bBufOwner)
        {
            evbuffer_free(m_evbuf);
        }
        m_evbuf = NULL;
    }

public:
    void assign(evbuffer* buf, bool bOwner = false)
    {
        if (m_evbuf && m_bBufOwner)
        {
            evbuffer_free(m_evbuf);
        }
        m_evbuf = buf;
        m_bBufOwner = bOwner;
    }

    void addData(const void* data, size_t len)
    {
        int ret = evbuffer_add(m_evbuf, data, len);
        if (ret == -1)
        {
            FIRTEX_THROW(RuntimeException, "evbuffer_add FAILED");
        }
    }

    void addData(struct evbuffer* data)
    {
        int ret = evbuffer_add_buffer(m_evbuf, data);
        if (ret == -1)
        {
            FIRTEX_THROW(RuntimeException, "evbuffer_add FAILED");
        }
    }

    const void* getData(ssize_t size = -1)
    {
        return evbuffer_pullup(m_evbuf, size);
    }

    size_t getLength()
    {
        return evbuffer_get_length(m_evbuf);
    }

    void clear()
    {
        //TODO: optimize
        evbuffer_free(m_evbuf);
        m_evbuf = evbuffer_new();
    }

    evbuffer* getEvBuffer()
    {
        return m_evbuf;
    }

private:
    struct evbuffer* m_evbuf;
    bool m_bBufOwner;
};

FX_NS_END

#endif //__FX_EVDATABUFFER_H
