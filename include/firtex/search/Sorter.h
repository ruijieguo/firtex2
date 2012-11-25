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
// Created	: 2011-01-08 14:46:35

#ifndef __FX_SORTER_H
#define __FX_SORTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/MatchComparator.h"
#include "firtex/search/SortClause.h"
#include "firtex/index/IndexReader.h"
#include "firtex/queryparser/ExprParser.h"

FX_NS_USE(queryparser);

FX_NS_DEF(search);

class Sorter
{
public:
    static std::string SORT_BY_RELEVANCE;
    static std::string SORT_BY_INDEXORDER;
    static std::string SORT_BY_DOCID;
    static std::string REVERSE_SEPARATOR;
    static std::string SORT_ASCENDING;
    static std::string SORT_DESCENDING;

public:
    Sorter();
    ~Sorter();
    
public:
    /**
     * Setup sort comparators by sort clause
     */
    bool init(const FX_NS(index)::IndexReaderPtr& pIndexReader,
              const SortClausePtr& clause);

public:
    /**
     * Evaluate sort expression
     * @param scoredDoc scored doc to evaluate
     */
    void evaluate(ScoredDoc& scoredDoc);

    /**
     * Compare two scored docs
     */
    bool lessThan(const ScoredDoc* pDoc1, const ScoredDoc* pDoc2);

    /**
     * Return last index of expr value
     */
    uint32_t getLastExprValueIdx() const;

private:
    typedef std::vector<MatchComparator*> CompVector;
    typedef std::pair<ExprEvaluatorPtr, uint32_t> ExprPair;
    typedef std::vector<ExprPair> EvalVector;

    FX_NS(queryparser)::ExprParserPtr m_pParser;

    CompVector m_comparators;
    EvalVector m_evaluators;
    uint32_t m_lastValueIdx;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(Sorter);

/////////////////////////////////////////
//
inline void Sorter::evaluate(ScoredDoc& scoredDoc)
{
    for (EvalVector::iterator it = m_evaluators.begin();
         it != m_evaluators.end(); ++it)
    {
        uint32_t valIdx = (*it).second;
        ExprEvaluatorPtr pEval = (*it).first;
        switch(pEval->getType())
        {
        case VALUE_INT32:
            scoredDoc.getCustomValue<int32_t>(valIdx) =
                pEval->evaluateInt32(scoredDoc);

            FX_TRACE("evaluate: value Idx: [%u], value: %d",
                     valIdx, scoredDoc.getCustomValue<int32_t>(valIdx));

            break;
        case VALUE_INT64:
            scoredDoc.getCustomValue<int64_t>(valIdx) =
                pEval->evaluateInt64(scoredDoc);

            FX_TRACE("evaluate: value Idx: [%u], value: %lld",
                     valIdx, scoredDoc.getCustomValue<int64_t>(valIdx));
            break;
        case VALUE_FLOAT:
            scoredDoc.getCustomValue<float>(valIdx) =
                pEval->evaluateFloat(scoredDoc);

            FX_TRACE("evaluate: value Idx: [%u], value: %f",
                     valIdx, scoredDoc.getCustomValue<float>(valIdx));

            break;
        case VALUE_DOUBLE:
            scoredDoc.getCustomValue<double>(valIdx) =
                pEval->evaluateDouble(scoredDoc);

            FX_TRACE("evaluate: value Idx: [%u], value: %f",
                     valIdx, scoredDoc.getCustomValue<double>(valIdx));

            break;
        default:
            FIRTEX_ASSERT2(false);
            break;
        }
    }
}

inline bool Sorter::lessThan(const ScoredDoc* pDoc1, const ScoredDoc* pDoc2)
{
    int32_t compRet = 0;
    size_t nComps = m_comparators.size();
    for (size_t i = 0; i < nComps && (compRet == 0); ++i)
    {
        FX_TRACE("Sort [%d], reverse: %s", (int32_t)i, 
                 m_comparators[i]->getReverse() ? "true" : "false");
        compRet = (m_comparators[i]->getReverse())
                  ? m_comparators[i]->compare(pDoc2, pDoc1)
                  : m_comparators[i]->compare(pDoc1, pDoc2);
    }

    FX_TRACE("Compare result of Sorter: %d", compRet);
    if (compRet == 0)
    {
        return (pDoc1->getDocId() > pDoc2->getDocId());
    }
    return compRet > 0;
}

inline uint32_t Sorter::getLastExprValueIdx() const
{
    return m_lastValueIdx;
}

FX_NS_END

#endif //__FX_SORTER_H
