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
// Created	: 2010-11-14 13:49:18

#ifndef __FX_QUERYEXPR_H
#define __FX_QUERYEXPR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);
class Query;
FX_NS_END

FX_NS_DEF(queryparser);
class QueryBuilder;

struct NumberNode
{
public:
    std::string m_sValue;
    union
    {
        int64_t m_i64Val;
        float m_fVal;
    };
};

class QueryExpr
{
public:
    enum OpType
    {
        OP_NONE,
        OP_AND,
        OP_OR,
    };

    enum ModType
    {
        MOD_NONE,//no modifier
        MOD_NOT, //prohibited
        MOD_REQ, //required
    };

public:
    QueryExpr();
    QueryExpr(const std::string& sField);
    virtual ~QueryExpr();

public:
    bool isRequired() const {return m_modifier == MOD_REQ;}
    bool isProhibited() const {return m_modifier == MOD_NOT;}

    void setRequired(bool bReq);
    void setProhibited(bool bProh);

    virtual void setFieldName(const std::string& sField);

public:
    static std::string* toString(int64_t value);

public:
    /**
     * Transfer to string format
     */
    virtual std::string toString() const = 0;

    /**
     * Accept visitor to create query
     */
    virtual FX_NS(search)::Query* accept(QueryBuilder& builder) = 0;

protected:
    std::string m_sFieldName;
    ModType m_modifier;
};

DEFINE_TYPED_PTR(QueryExpr);

class TermQueryExpr : public QueryExpr
{
public:
    TermQueryExpr(const std::string& sField, std::string* pValue);
    TermQueryExpr(std::string* pValue);
    ~TermQueryExpr(); 

public:
    void setBoost(float fBoost);

    const std::string& getFieldName() const;
    const std::string& getValue() const;
    float getBoost() const;

public:
    std::string toString() const;
    FX_NS(search)::Query* accept(QueryBuilder& builder);

protected:
    std::string* m_pValue;
    float m_fBoost;
    ModType m_modifier;
};

class PhraseQueryExpr : public TermQueryExpr
{
public:
    PhraseQueryExpr(const std::string& sField, std::string* pValue,
                    int32_t nSlop = 0)
        : TermQueryExpr(sField, pValue)
        , m_nSlop(nSlop)
    {
    }

    PhraseQueryExpr(std::string* pValue, int32_t nSlop = 0)
        : TermQueryExpr(pValue)
        , m_nSlop(nSlop)
    {
    }

public:
    FX_NS(search)::Query* accept(QueryBuilder& builder);
    std::string toString() const;
    int32_t getSlop() const {return m_nSlop;}

private:
    int32_t m_nSlop;
};

class RangeQueryExpr : public TermQueryExpr
{
public:
    RangeQueryExpr(const std::string& sField, std::string* pLowerValue,
                   bool bInclusiveLower, std::string* pUpperValue,
                   bool bInclusiveUpper);

    RangeQueryExpr(std::string* pLowerValue, bool bInclusiveLower,
                   std::string* pUpperValue, bool bInclusiveUpper);

    ~RangeQueryExpr();

public:
    static std::string* toString(int64_t value);

public:
    std::string getLowerValue() const {return m_pLowerValue ? *m_pLowerValue : "";}
    std::string getUpperValue() const {return m_pUpperValue ? *m_pUpperValue : "";}

    std::string toString() const;
    FX_NS(search)::Query* accept(QueryBuilder& builder);
    
private:
    std::string* m_pLowerValue;
    std::string* m_pUpperValue;
    bool m_bInclusiveLower;
    bool m_bInclusiveUpper;
};

class BooleanQueryExpr : public QueryExpr
{
public:
    BooleanQueryExpr(QueryExpr* pExpr1, QueryExpr* pExpr2, QueryExpr::OpType op);
    ~BooleanQueryExpr();

public:
    QueryExpr* getLeftNode() const {return m_pLeftExpr;}
    QueryExpr* getRightNode() const {return m_pRightExpr;}
    QueryExpr::OpType getOpType() const { return m_operation;}

    void setFieldName(const std::string& sField);

    std::string toString() const;
    FX_NS(search)::Query* accept(QueryBuilder& builder);

private:
    QueryExpr* m_pLeftExpr;
    QueryExpr* m_pRightExpr;
    QueryExpr::OpType m_operation;
};

///////////////////////////////////////
//
inline void QueryExpr::setRequired(bool bReq) 
{
    if (bReq)
    {
        m_modifier = MOD_REQ;
    }
    else if (m_modifier == MOD_REQ)
    {
        m_modifier = MOD_NONE;
    }
}

inline void QueryExpr::setProhibited(bool bProh) 
{
    if (bProh)
    {
        m_modifier = MOD_NOT;
    }
    else if (m_modifier == MOD_NOT)
    {
        m_modifier = MOD_NONE;
    }
}

inline void TermQueryExpr::setBoost(float fBoost)
{
    m_fBoost = fBoost;
}

inline const std::string& TermQueryExpr::getFieldName() const
{
    return m_sFieldName;
}

inline const std::string& TermQueryExpr::getValue() const
{
    return *m_pValue;
}

inline float TermQueryExpr::getBoost() const
{
    return m_fBoost;
}

FX_NS_END

#endif //__FX_QUERYEXPR_H
