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
// Created : 2011-08-01 19:47:40

#ifndef __FX_THREADLOCAL_H
#define __FX_THREADLOCAL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include <map>

FX_NS_DEF(thread);
class Thread;

class SlotBase
{
public:
    virtual ~SlotBase() {}
};

class ThreadLocalStorage
{
public:
    ThreadLocalStorage() {}
    ~ThreadLocalStorage();

    /// Return the slot for the given key.
    SlotBase*& get(const void* key);

    /// Return the TLS object for the current thread
    static ThreadLocalStorage& current();

    /// Clear the current thread's TLS object.
    static void clear();
	
private:
    typedef std::map<const void*, SlotBase*> SlotMap;
    SlotMap m_slots;
        
    friend class Thread;
};

/**
 * @class InitialValueFn
 */
template <typename T>
struct InitialValueFn 
{ 
    T init() 
    {
        return T();
    }

    void finish(T& v)
    {
    }
}; 

template <typename T, typename InitialValueT = InitialValueFn<T> >
class ThreadLocal
{
public:
    class Slot : public SlotBase
    {
    public:
	Slot() : m_value(InitialValueT().init()) {}
	~Slot() 
        {
            InitialValueT().finish(m_value);
        }

	T& value()
	{
            return m_value;
	}
	
    private:
	Slot(const Slot&);
	Slot& operator = (const Slot&);

	T m_value;
    };


public:
    ThreadLocal() {}
    ~ThreadLocal() {}

public:	
    T* operator -> ()
    {
        return &get();
    }
	
    T& operator* ()
    {
        return get();
    }

    T& get()
    {
        SlotBase*& p = ThreadLocalStorage::current().get(this);
        if (!p) 
        {
            p = new Slot;
        }
        return static_cast<Slot*>(p)->value();
    }
	
private:
    ThreadLocal(const ThreadLocal&);
    ThreadLocal& operator = (const ThreadLocal&);
};

FX_NS_END

#endif //__FX_THREADLOCAL_H
