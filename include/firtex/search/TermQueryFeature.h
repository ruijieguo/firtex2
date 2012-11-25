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
// Created	: 2010-07-12 20:50:03

#ifndef __FX_TERMQUERYFEATURE_H
#define __FX_TERMQUERYFEATURE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/Term.h"
#include "firtex/index/TermMeta.h"
#include "firtex/index/TermPostingIterator.h"

FX_NS_DEF(search);

class TermQueryFeature
{
public:
    // query type
    enum QueryType
    {
        QT_NONE,    //empty
        QT_TERM,    //term query
        QT_PHRASE,  //phrase query
        QT_AND,     //and query
        QT_OR,      //or query
        QT_REQOPT,  //required and optional query
        QT_REQPROH  //required and prohibited query
    };
    
    // modifier type
    enum ModType
    {
        MOD_NONE,  //no modifier
        MOD_NOT,   //prohibited
        MOD_REQ,   //required
    };

public:
    TermQueryFeature() 
        : m_pTermMeta(NULL)
        , m_fBoost(1.0) 
        , m_uNumSuccessors(0)
        , m_queryType(QT_NONE)
        , m_modifier(MOD_NONE)
    {
    }

    ~TermQueryFeature() {}

public:
    /**
     * Return document frequency
     */
    df_t getDocFreq() const;

    /**
     * Return total term frequency
     */
    ctf_t getCTF() const;

    /**
     * Return term query boost
     */
    float getBoost() const;

    /**
     * Return query term
     */
    const FX_NS(index)::Term* getTerm() const;

    /**
     * Set query term
     */
    void setTerm(const FX_NS(index)::TermPtr& pTerm);

    /**
     * Set term meta
     */
    void setTermMeta(const FX_NS(index)::TermMeta* pTermMeta);

    /**
     * Set boost value of query
     */
    void setBoost(float fBoost);

    /**
     * Set/get query type
     */
    QueryType getQueryType() const {return m_queryType;}
    void setQueryType(QueryType qt) { m_queryType = qt;}

    /**
     * Set/get required modifier
     */
    bool isRequired() const {return m_modifier == MOD_REQ;}
    void setRequired(bool bReq);

    /**
     * Set/get prohibited modifier
     */
    bool isProhibited() const {return m_modifier == MOD_NOT;}
    void setProhibited(bool bProh);

    /**
     * Set/get number of successors
     */
    size_t getNumSuccessors() const {return m_uNumSuccessors;}
    void setNumSuccessors(size_t uNumSuc) {m_uNumSuccessors = uNumSuc;}

protected:
    FX_NS(index)::TermPtr m_pTerm;
    const FX_NS(index)::TermMeta* m_pTermMeta;
    float m_fBoost;
    
    size_t m_uNumSuccessors;
    QueryType m_queryType;
    ModType m_modifier;
};

////////////////////////////////////////////////////
//
inline df_t TermQueryFeature::getDocFreq() const
{
    return m_pTermMeta ? m_pTermMeta->getDocFreq() : 0;
}

inline ctf_t TermQueryFeature::getCTF() const
{
    return m_pTermMeta ? m_pTermMeta->getCTF() : 0;
}

inline float TermQueryFeature::getBoost() const
{
    return m_fBoost;
}

inline const FX_NS(index)::Term* TermQueryFeature::getTerm() const
{
    return m_pTerm.get();
}

inline void TermQueryFeature::setTerm(const FX_NS(index)::TermPtr& pTerm)
{
    m_pTerm = pTerm;
}

inline void TermQueryFeature::setTermMeta(const FX_NS(index)::TermMeta* pTermMeta)
{
    m_pTermMeta = pTermMeta;
}

inline void TermQueryFeature::setBoost(float fBoost)
{
    m_fBoost = fBoost;
}

inline void TermQueryFeature::setRequired(bool bReq) 
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

inline void TermQueryFeature::setProhibited(bool bProh) 
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

FX_NS_END

#endif //__FX_TERMQUERYFEATURE_H
