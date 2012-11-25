#ifndef __FXAUTOPTR_H
#define __FXAUTOPTR_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <algorithm>

FX_NS_DEF(utility);

class AutoPtr
{
public:
	AutoPtr(): m_ptr(0)
	{
	}

	AutoPtr(T* ptr): m_ptr(ptr)
	{
	}

	AutoPtr(T* ptr, bool shared): m_ptr(ptr)
	{
		if (shared && m_ptr) m_ptr->duplicate();
	}

	AutoPtr(const AutoPtr& ptr): m_ptr(ptr.m_ptr)
	{
		if (m_ptr) m_ptr->duplicate();
	}

	template <class Other> 
	AutoPtr(const AutoPtr<Other>& ptr): m_ptr(const_cast<Other*>(ptr.get()))
	{
		if (m_ptr) m_ptr->duplicate();
	}

	~AutoPtr()
	{
		if (m_ptr) m_ptr->release();
	}
	
	AutoPtr& assign(T* ptr)
	{
		if (m_ptr != ptr)
		{
			if (m_ptr) m_ptr->release();
			m_ptr = ptr;
		}
		return *this;
	}

	AutoPtr& assign(T* ptr, bool shared)
	{
		if (m_ptr != ptr)
		{
			if (m_ptr) m_ptr->release();
			m_ptr = ptr;
			if (shared && m_ptr) m_ptr->duplicate();
		}
		return *this;
	}
	
	AutoPtr& assign(const AutoPtr& ptr)
	{
		if (&ptr != this)
		{
			if (m_ptr) m_ptr->release();
			m_ptr = ptr.m_ptr;
			if (m_ptr) m_ptr->duplicate();
		}
		return *this;
	}
	
	template <class Other> 
	AutoPtr& assign(const AutoPtr<Other>& ptr)
	{
		if (ptr.get() != m_ptr)
		{
			if (m_ptr) m_ptr->release();
			m_ptr = const_cast<Other*>(ptr.get());
			if (m_ptr) m_ptr->duplicate();
		}
		return *this;
	}

	AutoPtr& operator = (T* ptr)
	{
		return assign(ptr);
	}

	AutoPtr& operator = (const AutoPtr& ptr)
	{
		return assign(ptr);
	}
	
	template <class Other> 
	AutoPtr& operator = (const AutoPtr<Other>& ptr)
	{
		return assign<Other>(ptr);
	}

	void swap(AutoPtr& ptr)
	{
		std::swap(m_ptr, ptr.m_ptr);
	}
	
	template <class Other> 
	AutoPtr<Other> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(m_ptr);
		return AutoPtr<Other>(pOther, true);
	}

	template <class Other> 
	AutoPtr<Other> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(m_ptr);
		return AutoPtr<Other>(pOther, true);
	}

	T* operator -> ()
	{
		if (m_ptr)
			return m_ptr;
		else
			throw NullPointerException();
	}

	const T* operator -> () const
	{
		if (m_ptr)
			return m_ptr;
		else
			throw NullPointerException();
	}

	T& operator * ()
	{
		if (m_ptr)
			return *m_ptr;
		else
			throw NullPointerException();
	}

	const T& operator * () const
	{
		if (m_ptr)
			return *m_ptr;
		else
			throw NullPointerException();
	}

	T* get()
	{
		return m_ptr;
	}

	const T* get() const
	{
		return m_ptr;
	}

	operator T* ()
	{
		return m_ptr;
	}
	
	operator const T* () const
	{
		return m_ptr;
	}
	
	bool operator ! () const
	{
		return m_ptr == 0;
	}

	bool isNull() const
	{
		return m_ptr == 0;
	}
	
	T* duplicate()
	{
		if (m_ptr) m_ptr->duplicate();
		return m_ptr;
	}

	bool operator == (const AutoPtr& ptr) const
	{
		return m_ptr == ptr.m_ptr;
	}

	bool operator == (const T* ptr) const
	{
		return m_ptr == ptr;
	}

	bool operator == (T* ptr) const
	{
		return m_ptr == ptr;
	}

	bool operator != (const AutoPtr& ptr) const
	{
		return m_ptr != ptr.m_ptr;
	}

	bool operator != (const T* ptr) const
	{
		return m_ptr != ptr;
	}

	bool operator != (T* ptr) const
	{
		return m_ptr != ptr;
	}

	bool operator < (const AutoPtr& ptr) const
	{
		return m_ptr < ptr.m_ptr;
	}

	bool operator < (const T* ptr) const
	{
		return m_ptr < ptr;
	}

	bool operator < (T* ptr) const
	{
		return m_ptr < ptr;
	}

	bool operator <= (const AutoPtr& ptr) const
	{
		return m_ptr <= ptr.m_ptr;
	}

	bool operator <= (const T* ptr) const
	{
		return m_ptr <= ptr;
	}

	bool operator <= (T* ptr) const
	{
		return m_ptr <= ptr;
	}

	bool operator > (const AutoPtr& ptr) const
	{
		return m_ptr > ptr.m_ptr;
	}

	bool operator > (const T* ptr) const
	{
		return m_ptr > ptr;
	}

	bool operator > (T* ptr) const
	{
		return m_ptr > ptr;
	}

	bool operator >= (const AutoPtr& ptr) const
	{
		return m_ptr >= ptr.m_ptr;
	}

	bool operator >= (const T* ptr) const
	{
		return m_ptr >= ptr;
	}

	bool operator >= (T* ptr) const
	{
		return m_ptr >= ptr;
	}

private:
	T* m_ptr;
};

template <class T>
inline void swap(AutoPtr<T>& p1, AutoPtr<T>& p2)
{
	p1.swap(p2);
}

FX_NS_END

#endif 
