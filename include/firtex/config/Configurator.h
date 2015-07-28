#ifndef __CONFIGURATOR_H
#define __CONFIGURATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Iterator.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/Number.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/config/Configurable.h"
#include "firtex/common/Any.h"
#include <map>
#include <memory>
#include <vector>
#include <set>

FX_NS_DEF(config);

/**
 * @class NotConfigurableException
 *
 * Used to describe a cast exception
 */
class NotConfigurableException : public FX_NS(utility)::FirteXException
{
public:
    //! Create a new exception
    NotConfigurableException() : FirteXException( _T("NotConfigurable exception") ){}

    //! Create a new exception
    NotConfigurableException(const tchar* msg) : FirteXException(msg) { }

    //! Create a new exception
    NotConfigurableException(const tstring& msg) : FirteXException(msg) { }
};

/**
 * @class BadConfigureValueException
 *
 * Used to describe a cast exception
 */
class BadConfigureValueException : public FX_NS(utility)::FirteXException
{
public:
    //! Create a new exception
    BadConfigureValueException() : FirteXException( _T("BadConfigureValue exception") ){}

    //! Create a new exception
    BadConfigureValueException(const tchar* msg) : FirteXException(msg) { }

    //! Create a new exception
    BadConfigureValueException(const tstring& msg) : FirteXException(msg) { }
};

class Configurator
{
public:
    typedef std::pair<tstring, FX_NS(common)::Any> KeyValuePair;
    typedef std::vector<KeyValuePair> ConfMap;
    typedef ConfMap::const_iterator ConfMapIterator;


    enum Mode {TO_CONF, FROM_CONF};

public:
    class Iterator 
    {
    public:
        Iterator(const Configurator::ConfMap& confMap) 
            : m_pConfMap(&confMap) 
        {
            m_iterator = m_pConfMap->begin();          
        }

        Iterator(const Iterator& other)
            : m_pConfMap(other.m_pConfMap)
        {
            m_iterator = m_pConfMap->begin();
        }

        Iterator(const Configurator::ConfMap& confMap, const ConfMapIterator& iter)
            : m_pConfMap(&confMap)
            , m_iterator(iter)
        {
        }

    public:
        /**
         * reset the iterator
         */
        void reset() {m_iterator = m_pConfMap->begin();};
      
        /**
         * whether reach to end or not
         * @return true for not, false for yes
         */
        bool hasNext() const 
        {
            return (m_iterator != m_pConfMap->end());
        }

        /**
         * retrieval current element
         * @return current element
         */
        KeyValuePair next()
        {
            return (*m_iterator++);
        }
        
        /**
         * number of elements
         * @return number of elements
         */
        size_t size() const { return m_pConfMap->size(); }

        Iterator& operator = (const Iterator& other)
        {
            m_pConfMap = other.m_pConfMap;
            m_iterator = other.m_iterator;
            return *this;
        }

        bool operator == (const Iterator& other) const
        {
            return ((m_pConfMap == other.m_pConfMap) 
                    && (m_iterator == other.m_iterator));
        }

        bool operator != (const Iterator& other) const 
        {
            return !(*this == other);
        }

    private:
        const Configurator::ConfMap* m_pConfMap;
        ConfMapIterator m_iterator;
    };

public:
    Configurator();
    Configurator(Mode eMode);
    Configurator(const FX_NS(common)::Any& conf, Mode eMode = FROM_CONF);
    virtual ~Configurator();

public:
    Mode getMode() const {return m_eMode;}
    void setMode(Mode mode) { m_eMode = mode; }
    const ConfMap& getMap() const {return m_confMap;}
    ConfMap& getMap() {return m_confMap;}

    Iterator findConf(const tstring& sKey) const;

    Iterator iterator() const
    {
        Configurator::Iterator it(m_confMap);
        return it;
    }      

public:
    template<typename T>
    void configure(const tstring& key, T& value)
    {
        if (m_eMode == TO_CONF)
        {
            m_confMap.push_back(KeyValuePair(key, toConf(value)));
        }
        else
        {
            Configurator::Iterator it = findConf(key);
            if (!it.hasNext())
            {
                throw NotConfigurableException(
                        key + " not found when try to parse from configure.");
            }
            fromConf(value, it.next().second);
        }
    }

    template<typename T>
    void configure(const tstring& key, T& value, const T& defaultValue)
    {
        if (m_eMode == TO_CONF)
        {
            m_confMap.push_back(KeyValuePair(key, toConf(value)));
        }
        else
        {
            Configurator::Iterator it = findConf(key);
            if (!it.hasNext())
            {
                value = defaultValue;
            }
            else
            {
                fromConf(value, it.next().second);
            }
        }
    }

    void configure(const tstring& key, tstring& value, const tstring& defaultValue)
    { return configure<tstring>(key, value, defaultValue); }

    void configure(const tstring& key, int32_t& value, const int32_t& defaultValue)
    { return configure<int32_t>(key, value, defaultValue); }

public:
    static inline FX_NS(common)::Any toConf(const FX_NS(common)::Any& a)
    {
        return a;
    }

    static inline void fromConf(FX_NS(common)::Any& t, const FX_NS(common)::Any& a)
    {
        t = a;
    }

    static inline FX_NS(common)::Any toConf(const Configurable& t)
    {
        Configurator c;
        const_cast<Configurable&>(t).configure(c);
        return c.getMap();
    }

    static inline void fromConf(Configurable& t, const FX_NS(common)::Any& a)
    {
        Configurator c(a);
        t.configure(c);
    }

    static inline FX_NS(common)::Any toConf(int32_t i)
    {
        FX_NS(utility)::Number number(i);
        return toConf(number.toString());
    }

    static inline void fromConf(int32_t& i, const FX_NS(common)::Any& a)
    {
        try
        {
            i = FX_NS(utility)::NumberParser::parseInt32(FX_NS(common)::AnyCast<tstring>(a));
        }
        catch(const FX_NS(utility)::NumberFormatException& )
        {
            throw BadConfigureValueException();
        }
    }

    static inline FX_NS(common)::Any toConf(uint32_t i)
    {
        FX_NS(utility)::Number number(i);
        return toConf(number.toString());
    }

    static inline void fromConf(uint32_t& i, const FX_NS(common)::Any& a)
    {
        try
        {
            i = (uint32_t)FX_NS(utility)::NumberParser::parseInt32(
                    FX_NS(common)::AnyCast<tstring>(a));
        }
        catch(const FX_NS(utility)::NumberFormatException&)
        {
            throw BadConfigureValueException();
        }
    }

    static inline FX_NS(common)::Any toConf(int64_t i)
    {
        FX_NS(utility)::Number number(i);
        return toConf(number.toString());
    }

    static inline void fromConf(int64_t& i, const FX_NS(common)::Any& a)
    {
        try
        {
            i = FX_NS(utility)::NumberParser::parseInt64(FX_NS(common)::AnyCast<tstring>(a));
        }
        catch(const FX_NS(utility)::NumberFormatException&)
        {
            throw BadConfigureValueException();
        }
    }

    static inline FX_NS(common)::Any toConf(uint64_t i)
    {
        FX_NS(utility)::Number number(i);
        return toConf(number.toString());
    }

    static inline void fromConf(uint64_t& i, const FX_NS(common)::Any& a)
    {
        try
        {
            i = (uint64_t)FX_NS(utility)::NumberParser::parseInt64(
                    FX_NS(common)::AnyCast<tstring>(a));
        }
        catch(const FX_NS(utility)::NumberFormatException& )
        {
            throw BadConfigureValueException();
        }
    }

    static inline FX_NS(common)::Any toConf(bool b)
    {
        return toConf(b ? _T("true") : _T("false"));
    }

    static inline void fromConf(bool& b, const FX_NS(common)::Any& a)
    {
        tstring str = FX_NS(common)::AnyCast<tstring>(a);
        b = ((str == _T("true")) ? true : false);
    }

    static inline FX_NS(common)::Any toConf(const std::string& s)
    {
        return s;
    }

    static inline void fromConf(std::string& s, const FX_NS(common)::Any& a)
    {
        s = FX_NS(common)::AnyCast<std::string>(a);
    }

    static inline FX_NS(common)::Any toConf(const char* s)
    {
        return std::string(s);
    }

    static inline FX_NS(common)::Any toConf(double d)
    {
        return d;
    }

    static inline void fromConf(double& d, const FX_NS(common)::Any& a)
    {
        d = FX_NS(common)::AnyCastNumber<double>(a);
    }

    template<typename T>
    FX_NS(common)::Any toConf(T* p);

    template<typename T>
    void fromConf(T*& p, const FX_NS(common)::Any& a);

    template<typename T>
    FX_NS(common)::Any toConf(const std::vector<T>& m);

    template<typename T>
    void fromConf(std::vector<T>& tVec, const FX_NS(common)::Any& a);

    template<typename T>
    FX_NS(common)::Any toConf(const std::set<T>& s);

    template<typename T>
    void fromConf(std::set<T>& tSet, const FX_NS(common)::Any& a);

    template<typename T, typename U>
    FX_NS(common)::Any toConf(const std::pair<T, U>& s);

    template<typename T, typename U>
    FX_NS(common)::Any toConf(const std::map<T, U>& m);

    template<typename T>
    FX_NS(common)::Any toConf(const std::map<std::string, T>& m);

    template<typename T>
    void fromConf(std::map<std::string, T>& tMap, const FX_NS(common)::Any& a);

    template<typename T>
    static FX_NS(common)::Any toConf(const std::multimap<std::string, T>& m);

    template<typename T>
    static void fromConf(std::multimap<std::string, T>& tMap, const FX_NS(common)::Any& a);

    template<typename T>
    static FX_NS(common)::Any toConf(const std::vector<std::pair<tstring, T> >& m);

    template<typename T>
    static void fromConf(std::vector<std::pair<tstring, T> >& m, const FX_NS(common)::Any& a);

    template<typename T>
    static FX_NS(common)::Any toConf(const std::shared_ptr<T>& m);

    template<typename T>
    static void fromConf(const std::shared_ptr<T>& m, const FX_NS(common)::Any& a);

protected:
    Mode m_eMode;
    ConfMap m_confMap;
};

////////////////////////////////////////////////////////////
//
template<typename T>
FX_NS(common)::Any Configurator::toConf(const std::vector<T>& m)
{
    std::vector<FX_NS(common)::Any> anyVec;
    for (typename std::vector<T>::const_iterator it = m.begin();
         it != m.end(); it++)
    {
        anyVec.push_back(toConf(*it));
    }
    return anyVec;
}

template<typename T>
void Configurator::fromConf(std::vector<T>& tVec, const FX_NS(common)::Any& a)
{
    tVec.clear();
    std::vector<FX_NS(common)::Any> anyVec = FX_NS(common)::AnyCast<std::vector<FX_NS(common)::Any> >(a);
    for (typename std::vector<FX_NS(common)::Any>::const_iterator it = anyVec.begin();
         it != anyVec.end(); it++)
    {
        T t;
        fromConf(t, *it);
        tVec.push_back(t);
    }
}

template<typename T, typename U>
FX_NS(common)::Any Configurator::toConf(const std::pair<T, U>& s)
{
    std::vector<FX_NS(common)::Any> anyVec;
    anyVec.push_back(toConf(s.first));
    anyVec.push_back(toConf(s.second));
    return anyVec;
}

template<typename T, typename U>
FX_NS(common)::Any Configurator::toConf(const std::map<T, U>& m)
{
    std::vector<FX_NS(common)::Any> anyVec;
    std::vector<std::pair<T, U> > vec(m.begin(), m.end());

    for (typename std::vector<std::pair<T, U> >::const_iterator it = vec.begin();
         it != vec.end(); it++)
    {
        anyVec.push_back(toConf(*it));
    }

    return anyVec;
}

template<typename T>
FX_NS(common)::Any Configurator::toConf(const std::map<std::string, T>& m)
{
    std::map<std::string, FX_NS(common)::Any> anyMap;
    for (typename std::map<std::string, T>::const_iterator it = m.begin();
         it != m.end(); it++)
    {
        anyMap[it->first] = toConf(it->second);
    }
    return anyMap;
}

template<typename T>
void Configurator::fromConf(std::map<std::string, T>& tMap, const FX_NS(common)::Any& a)
{
    tMap.clear();
    std::map<std::string, FX_NS(common)::Any> anyMap = FX_NS(common)::AnyCast<std::map<std::string, FX_NS(common)::Any> >(a);
    for (typename std::map<std::string, FX_NS(common)::Any>::const_iterator it = anyMap.begin();
         it != anyMap.end(); it++)
    {
        fromConf(tMap[it->first], it->second);
    }
}

template<typename T>
FX_NS(common)::Any Configurator::toConf(const std::multimap<std::string, T>& m)
{
    std::multimap<std::string, FX_NS(common)::Any> anyMap;
    for (typename std::multimap<std::string, T>::const_iterator it = m.begin();
         it != m.end(); it++)
    {
        anyMap.insert(make_pair(it->first, toConf(it->second)));
    }
    return anyMap;
}

template<typename T>
void Configurator::fromConf(std::multimap<std::string, T>& m, const FX_NS(common)::Any& a)
{
    m.clear();
    std::multimap<std::string, FX_NS(common)::Any> anyMap 
        = FX_NS(common)::AnyCast<std::multimap<std::string, FX_NS(common)::Any> >(a);
    for (typename std::multimap<std::string, FX_NS(common)::Any>::const_iterator it 
             = anyMap.begin(); it != anyMap.end(); it++)
    {
        T value;
        fromConf(value, it->second);
        m.insert(make_pair(it->first, value));
    }
}

template<typename T>
FX_NS(common)::Any Configurator::toConf(const std::vector<std::pair<tstring, T> >& m)
{
    std::vector<std::pair<tstring, FX_NS(common)::Any> >anyMap;
    for (typename std::vector<std::pair<tstring, T> >::const_iterator it = m.begin();
         it != m.end(); it++)
    {
        anyMap.push_back(make_pair(it->first, toConf(it->second)));
    }
    return anyMap;
}

template<typename T>
void Configurator::fromConf(std::vector<std::pair<tstring, T> >& m, const FX_NS(common)::Any& a)
{
    m.clear();
    std::vector<std::pair<tstring, FX_NS(common)::Any> >anyMap 
        = FX_NS(common)::AnyCast<std::vector<std::pair<tstring, FX_NS(common)::Any> > >(a);
    for (typename std::vector<std::pair<tstring, FX_NS(common)::Any> >::const_iterator it 
             = anyMap.begin(); it != anyMap.end(); it++)
    {
        T value;
        fromConf(value, it->second);
        m.push_back(make_pair(it->first, value));
    }
}

template<typename T>
FX_NS(common)::Any Configurator::toConf(const std::shared_ptr<T>& m)
{
    return toConf(m.get());
}

template<typename T>
void Configurator::fromConf(const std::shared_ptr<T>& m,
                            const FX_NS(common)::Any& a)
{
    fromConf(m.get(), a);
}

template<typename T>
FX_NS(common)::Any Configurator::toConf(const std::set<T>& m)
{
    std::vector<FX_NS(common)::Any> anyVec(m.begin(), m.end());
    return anyVec;
}

template<typename T>
void Configurator::fromConf(std::set<T>& tSet, const FX_NS(common)::Any& a)
{
    tSet.clear();
    std::vector<FX_NS(common)::Any> anyVec = FX_NS(common)::AnyCast<std::vector<FX_NS(common)::Any> >(a);
    for (typename std::vector<FX_NS(common)::Any>::const_iterator it = anyVec.begin();
         it != anyVec.end(); it++)
    {
        T t;
        fromConf(t, *it);
        tSet.insert(t);
    }
}

template<typename T>
FX_NS(common)::Any Configurator::toConf(T* p)
{
    return *p;
}

template<typename T>
void Configurator::fromConf(T*& p, const FX_NS(common)::Any& a)
{
    std::auto_ptr<T> t(new T);
    fromConf(*t, a);
    p = t.release();
}

FX_NS_END

#endif
