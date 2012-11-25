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
// Created	: 2011-01-08 15:05:31

#ifndef __FX_MATCHCOMPARATOR_H
#define __FX_MATCHCOMPARATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ScoredDoc.h"

FX_NS_DEF(search);

class MatchComparator
{
public:
    MatchComparator(bool bReverse) : m_bReverse(bReverse) {}
    virtual ~MatchComparator() {}

public:
    /**
     * Compare two scored docs
     */
    virtual int32_t compare(const ScoredDoc* pDoc1, const ScoredDoc* pDoc2) = 0;

public:
    void setReverse(bool bReverse) 
    { 
        m_bReverse = bReverse;
    }

    bool getReverse() const
    {
        return m_bReverse;
    }

private:
    bool m_bReverse;
};

class RelevanceComparator : public MatchComparator
{
public:
    RelevanceComparator(bool bReverse) : MatchComparator(bReverse) {}

public:
    int32_t compare(const ScoredDoc* pDoc1, const ScoredDoc* pDoc2)
    {
        FX_TRACE("Relevance compare: [docId: %d, score: %.2f], "
                 "[docId: %d, score: %.2f]", pDoc1->getDocId(),
                 pDoc1->getScore(), pDoc2->getDocId(), pDoc2->getScore());

        if (pDoc1->getScore() > pDoc2->getScore())
        {
            return -1;
        }
        else if (pDoc1->getScore() < pDoc2->getScore()) 
        {
            return 1;
        }
        return 0;
    }

private:
    DECLARE_STREAM_LOGGER();
};

class IndexOrderComparator : public MatchComparator
{
public:
    IndexOrderComparator(bool bReverse) : MatchComparator(bReverse) {}

public:
    int32_t compare(const ScoredDoc* pDoc1, const ScoredDoc* pDoc2)
    {
        FX_TRACE("IndexOrder compare: [%d], [%d]",
                 pDoc1->getDocId(), pDoc2->getDocId());
        if (pDoc1->getDocId() > pDoc2->getDocId())
        {
            return -1;
        }
        else if (pDoc1->getDocId() < pDoc2->getDocId())
        {
            return 1;
        }
        return 0;
    }

private:
    DECLARE_STREAM_LOGGER();
};

template<typename T>
class ExprComparator : public MatchComparator
{
public:
    typedef T ValueType;

public:
    ExprComparator(bool bReverse, uint32_t uValueIdx)
        : MatchComparator(bReverse)
        , m_uValueIdx(uValueIdx)
    {
    }

public:
    int32_t compare(const ScoredDoc* pDoc1, const ScoredDoc* pDoc2)
    {
        ValueType v1 = pDoc1->getCustomValue<ValueType>(m_uValueIdx);
        ValueType v2 = pDoc2->getCustomValue<ValueType>(m_uValueIdx);

        FX_TRACE("ExprComparator: [%d]->[%d], [%d]->[%d]",
                 pDoc1->getDocId(), (int32_t)v1,
                 pDoc2->getDocId(), (int32_t)v2);

        if (v1 > v2)
        {
            return -1;
        }
        else if (v1 < v2)
        {
            return 1;
        }
        return 0;
    }

private:
    uint32_t m_uValueIdx;

private:
    DECLARE_STREAM_LOGGER();
};

SETUP_STREAM_LOGGER_TEMP(search, ExprComparator);


typedef ExprComparator<int32_t> Int32ExprComparator;
typedef ExprComparator<int64_t> Int64ExprComparator;
typedef ExprComparator<float> FloatExprComparator;
typedef ExprComparator<double> DoubleExprComparator;

FX_NS_END

#endif //__FX_MATCHCOMPARATOR_H
