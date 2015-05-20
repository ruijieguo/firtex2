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
// Created	: 2011-01-03 15:18:14

#ifndef __FX_EXPRNODE_H
#define __FX_EXPRNODE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <vector>
#include <string>
#include <sstream>

FX_NS_DEF(queryparser);

class ConstList;

enum ValueType
{
    VALUE_NONE = 0,
    VALUE_INT32,
    VALUE_INT64,
    VALUE_FLOAT,
    VALUE_DOUBLE,

    VALUE_UINT32,
    VALUE_UINT64,
    VALUE_STRING,
    VALUE_TYPE_MAX,
};

struct ExprNode
{
public:
    enum TokenType
    {
        TOK_NONE = 0,
        TOK_INT,
        TOK_DOUBLE,
        TOK_IDENT,
        TOK_QUOTED_STRING,
        TOK_FUNC,
        TOK_FUNC_DATE,
        TOK_FUNC_IN,
        TOK_FEATURE,
        TOK_COMMA,   //','
        TOK_AND,
        TOK_OR,
        TOK_BIT_AND, //'&'
        TOK_BIT_OR,  //'|'
        TOK_NOT,
        TOK_EQ,
        TOK_LE,
        TOK_GR,
        TOK_LTE,
        TOK_GTE,
        TOK_NE,
        TOK_NEG,
        TOK_PLUS,
        TOK_MINUS,
        TOK_MUL,
        TOK_DIV,
        TOK_CONST_LIST,
        TOK_ATTR,  // attribute(forward index)
        TOK_TYPE_MAX
    };

public:
    ExprNode() 
        : tokenType (TOK_NONE)
        , retType(VALUE_NONE)
        , argType(VALUE_NONE)
        , leftNodeIdx(-1)
        , rightNodeIdx(-1)
    {
    }

public:
    double getDoubleVal() const
    {
        FIRTEX_ASSERT2(tokenType == TOK_INT
                       || tokenType == TOK_DOUBLE);
        return (tokenType == TOK_INT) ?
            (double)intVal : doubleVal;
    }

public:
    static inline ValueType getIntType (int64_t iValue)
    {
        return (iValue>=(int64_t)MIN_INT32 && iValue<=(int64_t)MAX_INT32 ) ?
            VALUE_INT32 : VALUE_INT64;
    }

public:
    TokenType tokenType;
    ValueType retType;
    ValueType argType;
    union
    {
        int64_t intVal;
        double doubleVal;
        std::string* strVal;
        int32_t funcIdx;
        int32_t argCount;
        ConstList* constList;
        fieldid_t attrId;
    };
    int32_t leftNodeIdx;
    int32_t rightNodeIdx;
};

class ConstList
{
public:
    typedef std::vector<int64_t> IntVector;
    typedef std::vector<double> DoubleVector;
    typedef std::vector<std::string> StringVector;

public:
    ConstList()
        : m_retType(VALUE_INT32)
    {
        m_intList.reserve(8);
        m_doubleList.reserve(8);
    }

public:
    ValueType getRetType() const
    {
        return m_retType;
    }
    
    const IntVector& getIntList() const
    {
        return m_intList;
    }

    const DoubleVector& getDoubleList() const
    {
        return m_doubleList;
    }

    const StringVector& getStringList() const
    {
        return m_strList;
    }

    void add(int64_t iValue)
    {
        if (m_retType != VALUE_DOUBLE)
        {
            m_retType = ExprNode::getIntType(iValue);
            m_intList.push_back(iValue);
        }
        else
        {
            m_doubleList.push_back((double)iValue);
        }
    }

    void add(double dbValue)
    {
        if (m_retType != VALUE_DOUBLE)
        {
            FIRTEX_ASSERT2(m_doubleList.size() == 0);
            for (size_t i = 0; i < m_intList.size(); ++i)
            {
                m_doubleList.push_back((double)m_intList[i]);
            }
            m_intList.clear();
            m_retType = VALUE_DOUBLE;
        }
        m_doubleList.push_back(dbValue);
    }

    void add(const std::string& sValue)
    {
        if (m_retType != VALUE_STRING)
        {
            FIRTEX_ASSERT2(false);
        }
        m_strList.push_back(sValue);
    }

    std::string toString() const
    {
        std::stringstream ss;
        if (m_retType == VALUE_DOUBLE)
        {
            for (size_t i = 0; i < m_doubleList.size(); ++i)
            {
                ss << m_doubleList[i];
                if (i + 1 != m_doubleList.size())
                {
                    ss << ", ";
                }
            }
        }
        else if (m_retType == VALUE_STRING)
        {
            for (size_t i = 0; i < m_intList.size(); ++i)
            {
                ss << m_intList[i];
                if (i + 1 != m_intList.size())
                {
                    ss << ", ";
                }
            }
        }
        else
        {
            for (size_t i = 0; i < m_strList.size(); ++i)
            {
                ss << m_strList[i];
                if (i + 1 != m_strList.size())
                {
                    ss << ", ";
                }
            }
        }
        return ss.str();
    }
private:
    IntVector m_intList;
    DoubleVector m_doubleList;
    StringVector m_strList;
    ValueType m_retType;
};

FX_NS_END

#endif //__FX_EXPRNODE_H
