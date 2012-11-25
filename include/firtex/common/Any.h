#ifndef __FX_ANY_H
#define __FX_ANY_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include <algorithm>
#include <typeinfo>

FX_NS_DEF(common);

class Any
{
public: 
    Any() : m_content(0) {}
    
    template<typename ValueType>
    Any(const ValueType& value) : m_content(new Holder<ValueType>(value)){}
    Any(const Any& other)
        : m_content(other.m_content ? other.m_content->clone() : 0){}

    ~Any()
    {
        delete m_content;
    }

public:
    Any & swap(Any& rhs)
    {
        std::swap(m_content, rhs.m_content);
        return *this;
    }
    
    template<typename ValueType>
    Any & operator=(const ValueType& rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    Any & operator=(const Any& rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

public:
    
    bool isEmpty() const { return !m_content; }
    
    const std::type_info & getType() const
    {
        return m_content ? m_content->getType() : typeid(void);
    }

private:
    
    class PlaceHolder
    {
    public:
        virtual ~PlaceHolder(){}

    public:
        virtual const std::type_info & getType() const = 0;
        virtual PlaceHolder * clone() const = 0;
    };

    template<typename ValueType>
    class Holder : public PlaceHolder
    {
    public:
        Holder(const ValueType& value) : m_held(value) {}

    public:
        virtual const std::type_info & getType() const
        {
            return typeid(ValueType);
        }

        virtual PlaceHolder * clone() const
        {
            return new Holder(m_held);
        }

    public:
        
        ValueType m_held;
    };

private:
    
    template<typename ValueType>
    friend ValueType* AnyCast(Any*);

    template<typename ValueType>
    friend const ValueType* AnyCast(const Any*);

    template<typename ValueType>
    friend ValueType& AnyCast(Any&);

    template<typename ValueType>
    friend const ValueType& AnyCast(const Any&);

    PlaceHolder * m_content;
};

/**
 * @class BadAnyCastException
 *
 * Used to describe a out of range exception
 */
class BadAnyCastException : public FX_NS(utility)::FirteXException
{
public:
	//! Create a new exception
	BadAnyCastException() : FX_NS(utility)::FirteXException( 
        _T("BadAnyCast: failed conversion using AnyCast") )
    {}

	//! Create a new exception
	BadAnyCastException(const tchar* msg) 
        : FX_NS(utility)::FirteXException(msg) { }

	//! Create a new exception
	BadAnyCastException(const tstring& msg) 
        : FX_NS(utility)::FirteXException(msg) { }
};

/**
 * @class BadAnyCastNumberException
 *
 * Used to describe a out of range exception
 */
class BadAnyCastNumberException : public FX_NS(utility)::FirteXException
{
public:
	//! Create a new exception
	BadAnyCastNumberException() : FX_NS(utility)::FirteXException( 
        _T("BadAnyCastNumber: failed conversion using AnyCast") )
    {}

	//! Create a new exception
	BadAnyCastNumberException(const tchar* msg) 
        : FX_NS(utility)::FirteXException(msg) { }

	//! Create a new exception
	BadAnyCastNumberException(const tstring& msg) 
        : FX_NS(utility)::FirteXException(msg) { }
};


template<typename ValueType>
ValueType* AnyCast(Any* operand)
{
    if (operand && operand->getType() == typeid(ValueType))
    {
        return &static_cast<Any::Holder<ValueType>* >(operand->m_content)->m_held;
    }
    else 
    {
        throw BadAnyCastException(tstring(_T("AnyCast: can't cast from "))
                                  + operand->getType().name() + " to "
                                  + typeid(ValueType).name());
    }
}

template<typename ValueType>
const ValueType* AnyCast(const Any* operand)
{
    if (operand && operand->getType()==typeid(ValueType))
        return &static_cast<const Any::Holder<ValueType>* >(operand->m_content)->m_held;
    else
        return NULL;
}

template<typename ValueType>
const ValueType& AnyCast(const Any& operand)
{
    if (operand.getType() == typeid(ValueType))
    {
        return static_cast<const Any::Holder<ValueType>* >(operand.m_content)->m_held;
    }
    else
    {
        throw BadAnyCastException(tstring(_T("AnyCast: can't cast from "))
                                  + operand.getType().name() + " to "
                                  + typeid(ValueType).name());
    }
}

template<typename ValueType>
ValueType& AnyCast(Any& operand)
{
    if (operand.getType() == typeid(ValueType))
    {
        return static_cast<Any::Holder<ValueType>* >(operand.m_content)->m_held;
    }
    else
    {
        throw BadAnyCastException(tstring(_T("AnyCast: can't cast from "))
                                  + operand.getType().name() + " to "
                                  + typeid(ValueType).name());
    }
}


#define FX_IF_CAST(tp)                             \
    if (type == typeid(tp))                             \
    {                                                   \
        return static_cast<T>(AnyCast<tp>(number));     \
    }


template<typename T>
T AnyCastNumber(const Any& number)
{
    const std::type_info& type = number.getType();
    FX_IF_CAST(uint8_t)
    else
    FX_IF_CAST(int8_t)
    else
    FX_IF_CAST(uint16_t)
    else
    FX_IF_CAST(int16_t)
    else
    FX_IF_CAST(uint32_t)
    else
    FX_IF_CAST(int32_t)
    else
    FX_IF_CAST(uint64_t)
    else
    FX_IF_CAST(int64_t)
    else
    FX_IF_CAST(float)
    else
    FX_IF_CAST(double)
    else
    {
        throw BadAnyCastNumberException();
    }
}

#undef FX_IF_CAST

FX_NS_END

#endif
