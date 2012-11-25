#ifndef __FXOPTIONCALLBACK_H
#define __FXOPTIONCALLBACK_H

#include "firtex/common/StdHeader.h"
#include <string>

FX_NS_DEF(util);

class Option;

/**
 * @class AbstractOptionCallback
 * Base class for OptionCallback.
 */
class AbstractOptionCallback
{
public:
    virtual ~AbstractOptionCallback(){}
public:
    /**
     * Invokes the callback member function.
     */
    virtual void invoke(const Option& option, const std::string& sValue) = 0;
    /** 
     * returns a copy of the object.
     */
    virtual AbstractOptionCallback* clone() const = 0;

protected:
    AbstractOptionCallback(){}
    AbstractOptionCallback(const AbstractOptionCallback&){}
};

/**
 * @class OptionCallback
 * This class is used as an argument to Option::callback().
 */
template <class T>
class OptionCallback : public AbstractOptionCallback
{
public:
    typedef void (T::*Callback)(const Option& option, const std::string& sValue);

public:
    OptionCallback(T* pObject, Callback method)
        : m_pObject(pObject)
        , m_fnMethod(method)
    {
    }

    OptionCallback(const OptionCallback& cb)
        : AbstractOptionCallback(cb)
        , m_pObject(cb.m_pObject)
        , m_fnMethod(cb.m_fnMethod)
    {
    }

    ~OptionCallback()
    {
    }

public:
    void invoke(const Option& option, const std::string& sValue)
    {
        (m_pObject->*m_fnMethod)(option, sValue);
    }

    AbstractOptionCallback* clone() const
    {
        return new OptionCallback(m_pObject, m_fnMethod);
    }

private:
    OptionCallback();

protected:
    T* m_pObject;
    Callback m_fnMethod;
};

FX_NS_END

#endif 
