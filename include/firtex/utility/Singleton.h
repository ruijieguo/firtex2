/*
 * Copyright (C) 2007 Guo Ruijie. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */

#ifndef __FXSINGLETON_H__
#define __FXSINGLETON_H__

#include "firtex/thread/FastMutex.h"

FX_NS_DEF(utility)

//
// This policy controls how an object is instantiated
// as well as how and when its destroyed. Phoenix-style
// singletons are not supported easily with type of policy, 
// this is intentional since I do not believe that is in
// the true spirit of a singleton.
//
// InstantiationPolicContract {
//
//  create(pointer_type&)
//
// }

/**
 *
 * The LocalStaticInstantiation policy allows the creation 
 * and lifetime of an instance of a particular type
 * to be managed using static local values. This will
 * abide by the standard C++ rules for static objects
 * lifetimes.* @class LocalStaticInstantiation
 *
 * @author Eric Crahen
 * @date <2003-07-16T17:57:45-0400>
 * @version 2.2.0 
 */
class LocalStaticInstantiation 
{
protected:
    /**
     * Create an instance of an object, using a local static. The
     * object will be destroyed by the system.
     *
     * @param ptr reference to location to receive the address 
     * of the allocated object
     */
    template <class T>
    static void create(T*& ptr) 
    {
        static T instance;
        ptr = &instance;
    }
};

#define DECLARE_LOCCALSTATIC_SINGLETON(type) friend class StaticInstantiation;

//! Helper class
template <class T>
class StaticInstantiationHelper 
{
    //! Friend class
    friend class StaticInstantiation;
    //! Holder
    static T instance;
};

template <class T>
T StaticInstantiationHelper<T>::instance;

/**
 * @class StaticInstantiation
 * @author Eric Crahen
 * @version 2.2.0 
 *
 * The StaticInstantiation policy allows the creation 
 * and lifetime of an instance of a particular type
 * to be managed using static instantiation. This will
 * abide by the standard C++ rules for static objects
 * lifetimes.
 */
class StaticInstantiation 
{
protected:

    /**
     * Create an instance of an object using by simply allocating it statically.
     *
     * @param ptr reference to location to receive the address 
     * of the allocated object
     */
    template <class T>
    static void create(T*& ptr) 
    {
        ptr = &StaticInstantiationHelper<T>::instance;
    }
};

#define DECLARE_STATIC_SINGLETON(type)                                  \
    friend class FX_NS(utility)::StaticInstantiation;                   \
    friend class FX_NS(utility)::StaticInstantiationHelper< type >;

//! SingletonDestroyer
template <class T>
class Destroyer 
{
public:

    Destroyer(T* q)
        : m_pDoomed(q) 
    {		
    }

    ~Destroyer();
protected:
    T* m_pDoomed;
};

template <class T>
Destroyer<T>::~Destroyer() 
{
    if(m_pDoomed)
        delete m_pDoomed;
    m_pDoomed = 0;
}   


/**
 * @class LazyInstantiation
 * @author Eric Crahen
 * @date <2003-07-16T17:57:45-0400>
 *
 * The LazyInstantiation policy allows the creation 
 * and lifetime of an instance of a particular type
 * to be managed using dynamic allocation and a singleton
 * destroyer. This will abide by the standard C++ rules 
 * for static objects lifetimes.
 */
class LazyInstantiation 
{
protected:

    /**
     * Create an instance of an object, using new, that will be
     * destroyed when an associated Destroyer object (allocated
     * statically) goes out of scope.
     *
     * @param ptr reference to location to receive the address 
     * of the allocated object
     */
    template <class T>
    static void create(T*& ptr) 
    {
        ptr = new T;
        static Destroyer<T> destroyer(ptr);
    }
};

#define DECLARE_LAZY_SINGLETON(type)                    \
    friend class FX_NS(utility)::LazyInstantiation;     \
    friend class FX_NS(utility)::Destroyer<type>;

/**
 * @class Singleton
 * @author Eric Crahen
 * @date <2003-07-16T17:57:45-0400>
 *
 * Based on the work of John Vlissidles in his book 'Pattern Hatching'
 * an article by Douglas Schmidtt on double-checked locking and policy
 * templates described by Andrei Alexandrescu.
 *
 * This is a thread safe wrapper for creating Singleton classes. The 
 * synchronization method and instantiation methods can be changed
 * easily by specifying different policy implementations as the 
 * templates parameters.
 *
 * @code
 *
 * // Most common Singleton
 * Singletion<LonesomeType>
 *
 * // Singleton that uses static storage 
 * Singletion<LonesomeType, StaticInstantiation>
 *
 * // Single-threaded singleton that uses static storage (Meyers-like)
 * Singletion<LonesomeType, LocalStaticInstantiation, NotLocked>
 *
 * @endcode
 */
template <class T, class InstantiationPolicy = LazyInstantiation, class LockType = FX_NS(thread)::FastMutex>
class Singleton : private InstantiationPolicy, private NonCopyable 
{
public:

    /**
     * Provide access to the single instance through double-checked locking 
     *
     * @return T* single instance 
     */
    static T* instance();
};

template <class T, class InstantiationPolicy, class LockType>
T* Singleton<T, InstantiationPolicy, LockType>::instance() 
{
    // Uses local static storage to avoid static construction
    // sequence issues. (regaring when the lock is created)
    static T* ptr = 0;    
    static LockType lock;

    if(!ptr) 
    {
        typename LockType::Guard g(lock);
        if(!ptr)        
            InstantiationPolicy::create(ptr);
    }

    return const_cast<T*>(ptr);
}

FX_NS_END

#endif


