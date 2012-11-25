#ifndef __FXSHAREDOPTIONCALLBACK_H
#define __FXSHAREDOPTIONCALLBACK_H

#include "firtex/extension/util/OptionCallback.h"
#include "firtex/extension/util/AppRunner.h"
#include <vector>

FX_NS_DEF(util);

class SharedOptionCallback : public FX_NS(util)::OptionCallback<AppRunner>
{
public:
    typedef FX_NS(util)::OptionCallback<AppRunner>::Callback Callback;
public:
    SharedOptionCallback(AppRunner* pObject, Callback method)
        : FX_NS(util)::OptionCallback<AppRunner>(pObject, method)
    {
    }

    SharedOptionCallback(const SharedOptionCallback& src)
        : FX_NS(util)::OptionCallback<AppRunner>(src)
    {        
        for (CallbackVector::const_iterator it = src.m_callbacks.begin(); 
             it != src.m_callbacks.end(); it++)
        {
            m_callbacks.push_back((*it)->clone());
        }
    }

    ~SharedOptionCallback()
    {
        for (CallbackVector::const_iterator it = m_callbacks.begin(); 
             it != m_callbacks.end(); it++)
        {
            delete *it;
        }
        m_callbacks.clear();
    }

    AbstractOptionCallback* clone() const
	{
		SharedOptionCallback* pClone = new SharedOptionCallback(m_pObject, m_fnMethod);
        for (CallbackVector::const_iterator it = m_callbacks.begin(); 
             it != m_callbacks.end(); it++)
        {
            pClone->m_callbacks.push_back((*it)->clone());
        }
        return pClone;
	}

	void invoke(const FX_NS(util)::Option& option, const std::string& sValue)
	{
        FX_NS(util)::OptionCallback<AppRunner>::invoke(option, sValue);
        for (CallbackVector::const_iterator it = m_callbacks.begin(); 
             it != m_callbacks.end(); it++)
        {
            (*it)->invoke(option, sValue);
        }
	}

    void addCallback(const AbstractOptionCallback* cb)
    {        
        m_callbacks.push_back(cb->clone());
    }

private:
    typedef std::vector<AbstractOptionCallback*> CallbackVector;
    CallbackVector m_callbacks;
};

FX_NS_END

#endif 
