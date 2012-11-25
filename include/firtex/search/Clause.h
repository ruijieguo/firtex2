//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-12-20 22:59:36

#ifndef __FX_CLAUSE_H
#define __FX_CLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

#define DECLARE_CLAUSE_CREATOR(className)       \
    class Creator : public Clause::Creator      \
    {                                                           \
        Clause* create(const std::string& sName,                \
                       const std::string& sValue)               \
        {                                                       \
            return new className(sName, sValue);                \
        }                                                       \
    }

class Clause
{
public:
    class Creator
    {
    public:
        virtual ~Creator() {}

    public:
        virtual Clause* create(const std::string& sName,
                               const std::string& sValue) = 0;
    };

public:
    Clause(const std::string& sName,
           const std::string& sValue)
        : m_sClauseName(sName)
        , m_sClauseValue(sValue)
    {
    }
    
    virtual ~Clause() {}

protected:
    Clause() {}

public:
    inline const std::string& getName() const 
    {
        return m_sClauseName;
    }

    inline const std::string& getValue() const
    {
        return m_sClauseValue;
    }

    virtual void toString(std::stringstream& ss, const std::string& sEqSymbol) const
    {
        ss << m_sClauseName << sEqSymbol << m_sClauseValue;
    }

public:
    /**
     * Parse clause value string to key-value pair and skip block. 
     * For example, clause value string: 
     * k1:v1{!skip_block1},k2:v2{!skip_block2}
     *
     * @param sKey extracted key component
     * @param sValue extracted value component
     * @param sSkipBlock extracted skip block component
     * @param sClauseValue clause value string
     * @param offset parsed from here
     * @return where we parsed
     */
    static size_t parseClauseValue(std::string& sKey, std::string& sValue, 
                                   std::string& sSkipBlock,
                                   const std::string& sClauseValue,
                                   size_t offset)
    {
        size_t n = offset;
        size_t stat = 0;
        size_t nCompStart = n;
        while (n < sClauseValue.size())
        {
            if (sClauseValue[n] == ':')
            {
                if (stat == 0)
                {
                    sKey = sClauseValue.substr(nCompStart, n - nCompStart);
                    nCompStart = n + 1;
                    stat = 1;
                }
                ++n;
            }
            else if (sClauseValue[n] == '{')
            {
                if ((n + 1) < sClauseValue.size() && sClauseValue[n + 1] == '!')
                {
                    if (stat == 0)
                    {
                        sKey = sClauseValue.substr(nCompStart, n - nCompStart);
                    }
                    else if (stat == 1)
                    {
                        sValue = sClauseValue.substr(nCompStart, n - nCompStart);
                    }

                    n += 2;
                    nCompStart = n;
                    while (n < sClauseValue.size() && sClauseValue[n] != '}')
                    {
                        ++n;
                    }
                    sSkipBlock = sClauseValue.substr(nCompStart, n - nCompStart);
                    stat = 2;
                }
                ++n;
            }
            else if (sClauseValue[n] == ',')
            {
                if (stat == 0)
                {
                    sKey = sClauseValue.substr(nCompStart, n - nCompStart);
                    nCompStart = n + 1;
                }
                else if(stat == 1)
                {
                    sValue = sClauseValue.substr(nCompStart, n - nCompStart);
                    nCompStart = n + 1;
                }
                stat = 3;           
                return ++n;
            }
            else 
            {
                ++n;
            }
        }

        if (stat == 0)
        {
            sKey = sClauseValue.substr(nCompStart, n - nCompStart);
        }
        else if (stat == 1)
        {
            sValue = sClauseValue.substr(nCompStart, n - nCompStart);
        }
        return n;
    }

protected:
    std::string m_sClauseName;
    std::string m_sClauseValue;
};

DEFINE_TYPED_PTR(Clause);

FX_NS_END

#endif //__FX_CLAUSE_H
