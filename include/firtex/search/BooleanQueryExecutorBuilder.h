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
// Created	: 2010-07-07 21:49:43

#ifndef __FX_BOOLEANQUERYEXECUTORBUILDER_H
#define __FX_BOOLEANQUERYEXECUTORBUILDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Pool.h"
#include "firtex/search/QueryExecutor.h"
#include "firtex/search/FeatureProvider.h"
#include <vector>

FX_NS_DEF(search);

class BooleanQueryExecutorBuilder
{
public:
    typedef std::vector<QueryExecutorPtr> ExecutorVector;

public:
    BooleanQueryExecutorBuilder(FeatureProviderPtr& pProvider,
                                size_t nMinShouldMatch,
                                FX_NS(utility)::PoolPtr& pPool);
    ~BooleanQueryExecutorBuilder();

public:
    /**
     * Add a sub query executor
     * @param pExe query executor
     * @param bRequired true if the doc in this executor is required
     * @param bProhibited true if the doc in this executor is prohibited
     */
    void addExecutor(QueryExecutorPtr& pExe, bool bRequired, bool bProhibited);

    /**
     * Build executor
     */
    QueryExecutorPtr buildExecutor();

private:
    QueryExecutorPtr createExecutorWithoutRequired();
    QueryExecutorPtr makeOrExecutor(ExecutorVector& scorers, size_t nMinShouldMatch);
    QueryExecutorPtr makeAndExecutor(ExecutorVector& scorers);
    QueryExecutorPtr addProhibitedExecutors(const QueryExecutorPtr& pReqExecutor);
    QueryExecutorPtr createScorerWithSomeRequired();

protected:
    ExecutorVector m_requiredExecutors;
    ExecutorVector m_optionalExecutors;
    ExecutorVector m_prohibitedExecutors;

    QueryExecutorPtr m_pQueryExecutor;

    FeatureProviderPtr m_pFeatureProvider;
    FX_NS(utility)::PoolPtr m_pPool;
    size_t m_nMinShouldMatch;
private:
    DECLARE_STREAM_LOGGER();
};

typedef FX_NS(common)::SharedPtr<BooleanQueryExecutorBuilder> BooleanQueryExecutorBuilderPtr;

//////////////////////////////////////////////////////
//
inline QueryExecutorPtr BooleanQueryExecutorBuilder::buildExecutor()
{
    return (m_requiredExecutors.size() == 0)
		? createExecutorWithoutRequired()
		: createScorerWithSomeRequired();
}

FX_NS_END

#endif //__FX_BOOLEANQUERYEXECUTOR_H
