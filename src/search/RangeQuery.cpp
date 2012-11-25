#include "firtex/search/RangeQuery.h"
#include "firtex/search/BooleanQuery.h"
#include "firtex/search/TermQuery.h"

FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, RangeQuery);

RangeQuery::RangeQuery(const TermPtr& pLowerTerm, const TermPtr& pUpperTerm,
                       bool bInclusive)
    : m_pLowerTerm(pLowerTerm)
    , m_pUpperTerm(pUpperTerm)
    , m_bInclusive(bInclusive)
{
}

RangeQuery::RangeQuery(const RangeQuery& src)
    : m_pLowerTerm(src.m_pLowerTerm)
    , m_pUpperTerm(src.m_pUpperTerm)
    , m_bInclusive(src.m_bInclusive)
{
}

RangeQuery::~RangeQuery() 
{
}

tstring RangeQuery::getIdentifier() const
{
    return _T("RangeQuery");
}

size_t RangeQuery::size() const
{
    return 0;
}

QueryExecutorPtr RangeQuery::createExecutor(IndexReaderPtr& pIndexReader,
        FeatureProviderPtr& pProvider, PoolPtr& pPool) const
{
    TermReaderPtr pTermReader = pIndexReader->termReader();
    TermIteratorPtr pTermIterator =
        pTermReader->termIterator(m_pLowerTerm.get(), m_pUpperTerm.get());
    if (pTermIterator.isNull())
    {
        return QueryExecutorPtr();
    }

    BooleanQueryPtr query(new BooleanQuery());

    const Term* lastTerm = NULL;
    if (!m_bInclusive)
    {
        if (pTermIterator->hasNext())
        {
            lastTerm = pTermIterator->next().term;
            if (!lastTerm->compare(m_pLowerTerm.get()) && pTermIterator->hasNext())
            {
                //skip lower term
                pTermIterator->next();
            }
        }
    }

    while (pTermIterator->hasNext()) 
    {
        lastTerm = pTermIterator->next().term;
        if (!m_bInclusive && m_pUpperTerm.isNotNull() 
            && !lastTerm->compare(m_pUpperTerm.get()))
        {
            continue;
        }
        TermQueryPtr tq(new TermQuery(lastTerm->clone()));
        tq->setBoost(getBoost());
        query->addClause(tq, false, false);
    }

    if (query->size() == 0)
    {
        return QueryExecutorPtr();
    }
    return query->createExecutor(pIndexReader, pProvider, pPool);
}

Query* RangeQuery::clone() const
{
    return new RangeQuery(*this);
}

tstring RangeQuery::toString() const
{
    stringstream ss;
    ss << _T("RangeQuery: ");
    if (m_bInclusive)
    {
        ss << _T("[");
    }
    else 
    {
        ss << _T("(");
    }

    if (m_pLowerTerm.isNotNull())
    {
        ss << m_pLowerTerm->toString();
    }
    ss << _T(" TO ");

    if (m_pUpperTerm.isNotNull())
    {
        ss << m_pUpperTerm->toString();
    }

    if (m_bInclusive)    
    {
        ss << _T("]");
    }
    else 
    {
        ss << _T(")");
    }
    return ss.str();
}

void RangeQuery::terms(TermVector& terms) const
{
    return;
}

FX_NS_END

