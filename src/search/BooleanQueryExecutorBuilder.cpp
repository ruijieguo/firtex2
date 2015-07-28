#include "firtex/search/BooleanQueryExecutorBuilder.h"
#include "firtex/search/ReqProhQueryExecutor.h"
#include "firtex/search/ReqOptQueryExecutor.h"
#include "firtex/search/AndQueryExecutor.h"
#include "firtex/search/OrQueryExecutor.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(utility);
FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, BooleanQueryExecutorBuilder);

BooleanQueryExecutorBuilder::BooleanQueryExecutorBuilder(
        FeatureProviderPtr& pProvider, size_t nMinShouldMatch, PoolPtr& pPool)
    : m_pFeatureProvider(pProvider)
    , m_pPool(pPool)
    , m_nMinShouldMatch(nMinShouldMatch)
{
}

BooleanQueryExecutorBuilder::~BooleanQueryExecutorBuilder() 
{
}

void BooleanQueryExecutorBuilder::addExecutor(QueryExecutorPtr& pExe,
        bool bRequired, bool bProhibited)
{
    if (bRequired) 
    {
        if (bProhibited) 
        {
            FIRTEX_THROW(IllegalArgumentException,
                         "A query cannot be required and prohibited in boolean clause.");
        }
        m_requiredExecutors.push_back(pExe);
    }
    else if (bProhibited) 
    {
        m_prohibitedExecutors.push_back(pExe);
    }
    else 
    {
        m_optionalExecutors.push_back(pExe);
    }
}

QueryExecutorPtr BooleanQueryExecutorBuilder::createExecutorWithoutRequired()
{
    if (m_optionalExecutors.size() == 0) // no optional excutors
    {
        return NULL;
    } 
    else
    { 
        size_t nOptRequired = (m_nMinShouldMatch < 1) ? 1 : m_nMinShouldMatch;
        if (m_optionalExecutors.size() < nOptRequired) // optional executors are not enough
        {
            return NULL;
        } 
        else //m_optionalExecutors.size() >= nOptRequired
        {
            QueryExecutorPtr pReqExecutor;
            if (m_optionalExecutors.size() > nOptRequired)
            {
                pReqExecutor = makeOrExecutor(m_optionalExecutors, nOptRequired);
            }
            else //m_optionalExecutors.size() == nOptRequired
            {
                if (m_optionalExecutors.size() == 1)
                {
                    pReqExecutor = m_optionalExecutors[0];
                    m_optionalExecutors.clear();
                }
                else
                {
                    pReqExecutor = makeAndExecutor(m_optionalExecutors);
                }
            }		
            return addProhibitedExecutors(pReqExecutor);
        }
    }
}

QueryExecutorPtr BooleanQueryExecutorBuilder::makeOrExecutor(
        ExecutorVector& scorers, size_t nMinShouldMatch)
{
    OrQueryExecutor* pExecutor = new OrQueryExecutor(
            m_pFeatureProvider, nMinShouldMatch, m_pPool);

    for (ExecutorVector::const_iterator it = scorers.begin();
         it != scorers.end(); ++it)
    {
        pExecutor->addExecutor(*it);
    }
    scorers.clear();
    return QueryExecutorPtr(pExecutor);
}

QueryExecutorPtr BooleanQueryExecutorBuilder::makeAndExecutor(ExecutorVector& scorers)
{
    AndQueryExecutor* pExecutor = new AndQueryExecutor(m_pFeatureProvider, m_pPool);
    for (ExecutorVector::const_iterator it = scorers.begin();
         it != scorers.end(); ++it)
    {
        pExecutor->addExecutor(*it);
    }
    scorers.clear();
    return QueryExecutorPtr(pExecutor);
}

QueryExecutorPtr BooleanQueryExecutorBuilder::addProhibitedExecutors(const QueryExecutorPtr& pReqExecutor)
{
    if (m_prohibitedExecutors.size() == 0)
    {
        return pReqExecutor;
    }
    else 
    {
        QueryExecutorPtr pExe;
        if (m_prohibitedExecutors.size() == 1)
        {
            pExe = m_prohibitedExecutors[0];
            m_prohibitedExecutors.clear();
        }
        else 
        {
            OrQueryExecutor* pOrExe = new OrQueryExecutor(m_pFeatureProvider, 0, m_pPool);
            pExe.reset(pOrExe);
            for (ExecutorVector::const_iterator it = m_prohibitedExecutors.begin();
                 it != m_prohibitedExecutors.end(); ++it)
            {
                pOrExe->addExecutor(*it);
            }
            m_prohibitedExecutors.clear();
        }
        return QueryExecutorPtr(new ReqProhQueryExecutor(m_pFeatureProvider,
                        m_pPool, pReqExecutor, pExe));
    }		
}

QueryExecutorPtr BooleanQueryExecutorBuilder::createScorerWithSomeRequired()
{			
    if (m_optionalExecutors.size() < m_nMinShouldMatch)
    {
        return NULL; 
    }
    else if (m_optionalExecutors.size() == m_nMinShouldMatch) 
    {
        QueryExecutorPtr pReqExe;
        if (m_requiredExecutors.size() > 1)
        {
            pReqExe = makeAndExecutor(m_requiredExecutors);
        }
        else 
        {
            pReqExe = m_requiredExecutors[0];
            m_requiredExecutors.clear();
        }
        return addProhibitedExecutors(pReqExe);
    } 
    else // m_optionalExecutors.size() > m_nMinShouldMatch, and at least one required scorer
    { 
        QueryExecutorPtr pReqExecutor;
        if (m_requiredExecutors.size() == 1)
        {
            pReqExecutor = m_requiredExecutors[0];
            m_requiredExecutors.clear();
        }
        else 
        {
            pReqExecutor = makeAndExecutor(m_requiredExecutors);
        }
		
        if (m_nMinShouldMatch > 0)
        {	
            QueryExecutorPtr pOrExe = makeOrExecutor(m_optionalExecutors, m_nMinShouldMatch);
            AndQueryExecutorPtr pAndExe(new AndQueryExecutor(m_pFeatureProvider, m_pPool));
            pAndExe->addExecutor(pReqExecutor);
            pAndExe->addExecutor(pOrExe);
            return addProhibitedExecutors(pAndExe);
        } 
        else // m_nMinShouldMatch == 0 
        {	
            QueryExecutorPtr pExe;
            if (m_optionalExecutors.size() == 1) 
            {
                pExe = m_optionalExecutors[0];
                m_optionalExecutors.clear();
            }
            else 
            {
                pExe = makeOrExecutor(m_optionalExecutors, 1);
            }
            return QueryExecutorPtr(new ReqOptQueryExecutor(m_pFeatureProvider, m_pPool,
                            addProhibitedExecutors(pReqExecutor), pExe));
        }
    }
}

FX_NS_END

