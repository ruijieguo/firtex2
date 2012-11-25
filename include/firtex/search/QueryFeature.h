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
// Created	: 2010-07-12 21:24:51

#ifndef __FX_QUERYFEATURE_H
#define __FX_QUERYFEATURE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/TermQueryFeature.h"
#include <vector>

FX_NS_DEF(search);

class QueryFeature
{
public:
    QueryFeature();
    QueryFeature(size_t nNumTerms);
    virtual ~QueryFeature();

public:
    /**
     * Number of terms
     */
    size_t size() const;

    /**
     * Reserve capacity
     */
    void reserve(size_t nCapacity);

    /**
     * operator []
     */
    TermQueryFeature& operator [] (size_t n);
    const TermQueryFeature& operator [] (size_t n) const;

    /**
     * Return next []
     */
    TermQueryFeature& next();

protected:
    typedef TermQueryFeature* Vector;
    Vector m_featureArray;
    size_t m_nCapacity;
    size_t m_nSize;
};

/////////////////////////////////////////////////////
//
inline size_t QueryFeature::size() const
{
    return m_nSize;
}

inline TermQueryFeature& QueryFeature::operator [] (size_t n)
{
    if (n >= m_nCapacity)
    {
        reserve(n + 1);
    }
    if (n >= m_nSize)
    {
        m_nSize = (n + 1);
    }
    return m_featureArray[n];
}

inline const TermQueryFeature& QueryFeature::operator [] (size_t n) const
{
    FIRTEX_ASSERT2(n < m_nSize);
    return m_featureArray[n];
}

inline TermQueryFeature& QueryFeature::next()
{
    FIRTEX_ASSERT2(m_nSize < m_nCapacity);
    return m_featureArray[m_nSize++];
}

FX_NS_END

#endif //__FX_QUERYFEATURE_H
