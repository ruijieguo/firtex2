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
// Created	: 2007/5/25
//

#ifndef __FX_SYNCHRONIZEDOBJECT_H
#define __FX_SYNCHRONIZEDOBJECT_H

#include "firtex/thread/Mutex.h"
#include "firtex/utility/NonCopyable.h"
#include "firtex/thread/FastMutex.h"

#define SCOPED_LOCK() Guard guardZone(m_lock)

FX_NS_DEF(thread);

class SynchronizedObject : public FX_NS(utility)::NonCopyable
{
public:
    typedef FastMutex::Guard Guard;

public:
    SynchronizedObject ()
    {
    };

    virtual ~SynchronizedObject ()
    {
    };

public:
    FastMutex& getLocker() {return m_lock;}

    void lock()
    {
        m_lock.lock();
    }

    void unlock()
    {
        m_lock.unlock();
    }

protected:
    FastMutex m_lock;
};

FX_NS_END

#endif

