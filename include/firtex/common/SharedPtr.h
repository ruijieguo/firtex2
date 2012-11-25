#ifndef __FXSHAREDPTR_H
#define __FXSHAREDPTR_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/thread/AtomicCount.h"
#include <algorithm>

FX_NS_DEF(common);

class ReferenceCount
{
public:
    ReferenceCount(): m_cnt(1)
    {
    }

    void duplicate()
    {
        ++m_cnt;
    }

    size_t release()
    {
        return --m_cnt;
    }
    
    size_t referenceCount() const
    {
        return m_cnt.value();
    }

private:
    FX_NS(thread)::AtomicCount m_cnt;
};

template <class C>
class ReleasePolicy
{
public:
    static void release(C* pObj)
    {
        delete pObj;
    }
};

template <class C>
class ReleaseArrayPolicy
{
public:
    static void release(C* pObj)
    {
        delete [] pObj;
    }
};

template<class T> 
struct SharedPtrTraits
{
    typedef T & reference;
};

template<> 
struct SharedPtrTraits<void>
{
    typedef void reference;
};

template<> 
struct SharedPtrTraits<void const>
{
    typedef void reference;
};

template<> 
struct SharedPtrTraits<void volatile>
{
    typedef void reference;
};

template<> 
struct SharedPtrTraits<void const volatile>
{
    typedef void reference;
};

////////////////////////////////////////////////////////
//SharedPtr

template <class C, class RC = ReferenceCount, class RP = ReleasePolicy<C> >
class SharedPtr
{
private:
    typedef SharedPtr<C> ThisType;

public:
    typedef C* Pointer;
    typedef C ValueType;
    typedef typename SharedPtrTraits<C>::reference Reference;

public:
    SharedPtr(): m_pCounter(new RC), m_ptr(NULL)
    {
    }

    SharedPtr(C* ptr): m_pCounter(new RC), m_ptr(ptr)
    {
    }

    template <class Other, class OtherRP> 
    SharedPtr(const SharedPtr<Other, RC, OtherRP>& ptr)
        : m_pCounter(ptr.m_pCounter), m_ptr(const_cast<Other*>(ptr.get()))
    {
        m_pCounter->duplicate();
    }

    SharedPtr(const SharedPtr& ptr): m_pCounter(ptr.m_pCounter), m_ptr(ptr.m_ptr)
    {
        m_pCounter->duplicate();
    }

    ~SharedPtr()
    {
        release();
    }

    void reset()
    {
        assign((C*)NULL);
    }

    SharedPtr& reset(C* ptr)
    {
        if (get() != ptr)
        {
            RC* pTmp = new RC;
            release();
            m_pCounter = pTmp;
            m_ptr = ptr;
        }
        return *this;
    }

    SharedPtr& assign(C* ptr)
    {
        return reset(ptr);
    }
    
    SharedPtr& assign(const SharedPtr& ptr)
    {
        if (&ptr != this)
        {
            SharedPtr tmp(ptr);
            swap(tmp);
        }
        return *this;
    }
    
    template <class Other, class OtherRP>
    SharedPtr& assign(const SharedPtr<Other, RC, OtherRP>& ptr)
    {
        if (ptr.get() != m_ptr)
        {
            SharedPtr tmp(ptr);
            swap(tmp);
        }
        return *this;
    }

    SharedPtr& operator = (C* ptr)
    {
        return assign(ptr);
    }

    SharedPtr& operator = (const SharedPtr& ptr)
    {
        return assign(ptr);
    }

    template <class Other, class OtherRP>
    SharedPtr& operator = (const SharedPtr<Other, RC, OtherRP>& ptr)
    {
        return assign<Other>(ptr);
    }

    void swap(SharedPtr& ptr)
    {
        std::swap(m_ptr, ptr.m_ptr);
        std::swap(m_pCounter, ptr.m_pCounter);
    }

    /**
     * Casts the SharedPtr via a dynamic cast to the given type.
     */

    template <class Other>
    SharedPtr<Other> cast() const
    {
        Other* pOther = dynamic_cast<Other*>(m_ptr);
        if (pOther)
            return SharedPtr<Other>(m_pCounter, pOther);
        return SharedPtr<Other>();
    }

    /**
     * Casts the SharedPtr via a static cast to the given type.
     */
    template <class Other> 
    SharedPtr<Other, RC, RP> unsafeCast() const
    {
        Other* pOther = static_cast<Other*>(m_ptr);
        return SharedPtr<Other, RC, RP>(m_pCounter, pOther);
    }

    C* operator -> ()
    {
        return deref();
    }

    const C* operator -> () const
    {
        return deref();
    }

// 	C& operator * ()
// 	{
// 		return *deref();
// 	}

    bool operator ! () const
    {
        return m_ptr == NULL;
    }

    bool isNull() const
    {
        return (m_ptr == NULL);
    }

    bool isNotNull() const
    {
        return (m_ptr != NULL);
    }

    bool operator == (const SharedPtr& ptr) const
    {
        return get() == ptr.get();
    }

    bool operator == (const C* ptr) const
    {
        return get() == ptr;
    }

    // bool operator == (C* ptr) const
    // {
    //     return get() == ptr;
    // }

    bool operator != (const SharedPtr& ptr) const
    {
        return get() != ptr.get();
    }

    bool operator != (const C* ptr) const
    {
        return get() != ptr;
    }

//	bool operator != (C* ptr) const
//	{
//		return get() != ptr;
//	}

    bool operator < (const SharedPtr& ptr) const
    {
        return get() < ptr.get();
    }

    bool operator < (const C* ptr) const
    {
        return get() < ptr;
    }

    bool operator < (C* ptr) const
    {
        return get() < ptr;
    }

    bool operator <= (const SharedPtr& ptr) const
    {
        return get() <= ptr.get();
    }

    bool operator <= (const C* ptr) const
    {
        return get() <= ptr;
    }

    bool operator <= (C* ptr) const
    {
        return get() <= ptr;
    }

    bool operator > (const SharedPtr& ptr) const
    {
        return get() > ptr.get();
    }

    bool operator > (const C* ptr) const
    {
        return get() > ptr;
    }

    bool operator > (C* ptr) const
    {
        return get() > ptr;
    }

    bool operator >= (const SharedPtr& ptr) const
    {
        return get() >= ptr.get();
    }

    bool operator >= (const C* ptr) const
    {
        return get() >= ptr;
    }

    bool operator >= (C* ptr) const
    {
        return get() >= ptr;
    }
	
    size_t referenceCount() const
    {
        return m_pCounter->referenceCount();
    }

    const C* get() const
    {
        return m_ptr;
    }

    C* get()
    {
        return m_ptr;
    }

// 	operator const C* () const
// 	{
// 		return m_ptr;
// 	}

// 	operator C* ()
// 	{
// 		return m_ptr;
// 	}

public:
    Reference operator * () const
    {
        return *deref();
    }

private:
    C* deref() const
    {
        assert(m_ptr);
        return m_ptr;
    }

    void release()
    {
        size_t i = m_pCounter->release();
        if (i == 0)
        {
            RP::release(m_ptr);
            m_ptr = 0;

            delete m_pCounter;
            m_pCounter = 0;
        }
    }

    SharedPtr(RC* pCounter, C* ptr) : m_pCounter(pCounter), m_ptr(ptr)
    {
        m_pCounter->duplicate();
    }

private:
    RC* m_pCounter;
    C*  m_ptr;

    template <class OtherC, class OtherRC, class OtherRP> friend class SharedPtr;
};

template <class C, class RC, class RP>
inline void swap(SharedPtr<C, RC, RP>& p1, SharedPtr<C, RC, RP>& p2)
{
    p1.swap(p2);
}

#define DEFINE_TYPED_PTR(classname)                             \
    typedef FX_NS(common)::SharedPtr<classname> classname##Ptr;

FX_NS_END


#endif
