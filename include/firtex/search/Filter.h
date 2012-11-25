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
// Created	: 2011-01-09 16:33:33

#ifndef __FX_FILTER_H
#define __FX_FILTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexReader.h"
#include "firtex/queryparser/ExprParser.h"
#include "firtex/search/FilterClause.h"
#include "firtex/search/ScoredDoc.h"
#include "firtex/search/ExprEvaluator.h"

FX_NS_DEF(search);

class Filter
{
public:
    Filter();
    ~Filter();

public:
    /**
     * Initialize filter
     */
    bool init(const FX_NS(index)::IndexReaderPtr& pIndexReader,
              const FilterClausePtr& clause);

    /**
     * Filter a document
     * @return return true if filter the document
     */
    bool isFiltered(const ScoredDoc& scoredDoc);

private:
    FX_NS(queryparser)::ExprParserPtr m_pParser;
    ExprEvaluatorPtr m_pEvaluator;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(Filter);

/////////////////////////////////////////////
//
inline bool Filter::isFiltered(const ScoredDoc& scoredDoc)
{
    return (m_pEvaluator->evaluateInt64(scoredDoc) != 0);
}

FX_NS_END

#endif //__FX_FILTER_H
