#include "firtex/search/BooleanQuery.h"
#include "firtex/search/BooleanQueryExecutorBuilder.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, BooleanQuery);

BooleanQuery::BooleanQuery() 
    : m_nMinShouldMatch(0)
{
}

BooleanQuery::BooleanQuery(const BooleanQuery& src)
    : m_clauses(src.m_clauses)
    , m_nMinShouldMatch(src.m_nMinShouldMatch)
{
}

BooleanQuery::~BooleanQuery() 
{
}

void BooleanQuery::addClause(const QueryPtr& pQuery, bool bRequired, bool bProhibited)
{
    if (m_clauses.size() >= MAX_CLAUSE_COUNT)
    {
        FIRTEX_THROW(TooManyElementException, _T("Too many clauses"));
    }
    m_clauses.push_back(BooleanClause(pQuery, bRequired, bProhibited));
}

void BooleanQuery::addClause(const BooleanClause& clause)
{
    if (m_clauses.size() >= MAX_CLAUSE_COUNT)
    {
        FIRTEX_THROW(TooManyElementException, _T("Too many clauses"));
    }
    m_clauses.push_back(clause);
}

tstring BooleanQuery::getIdentifier() const
{
    return _T("BooleanQuery");
}

size_t BooleanQuery::size() const
{
    size_t size = 0;
    for (BooleanClauseVector::const_iterator it = m_clauses.begin();
         it != m_clauses.end(); ++it)
    {
        size += (*it).query->size();
    }
    return size;
}

QueryExecutorPtr BooleanQuery::createExecutor(IndexReaderPtr& pIndexReader,
        FeatureProviderPtr& pProvider,
        PoolPtr& pPool) const
{
    BooleanQueryExecutorBuilderPtr pExeBuilder(new BooleanQueryExecutorBuilder(
                    pProvider, m_nMinShouldMatch, pPool));

    for (BooleanClauseVector::const_iterator it = m_clauses.begin();
         it != m_clauses.end(); ++it)
    {
        const BooleanClause& clause = (*it);
        QueryExecutorPtr pSubExe = clause.query->createExecutor(
                pIndexReader, pProvider, pPool);
        if (pSubExe)
        {
            pExeBuilder->addExecutor(pSubExe, clause.required, clause.prohibited);
        }
        else if(clause.required)
        {
            //break;
            return QueryExecutorPtr();
        }
    }
    return pExeBuilder->buildExecutor();
}

Query* BooleanQuery::clone() const
{
    return new BooleanQuery(*this);
}

tstring BooleanQuery::toString() const
{
    stringstream ss;
    ss << _T("BooleanQuery: [");
    for (BooleanClauseVector::const_iterator it = m_clauses.begin();
         it != m_clauses.end(); ++it)
    {
        if (it->required)
        {
            ss << "+";
        }
        else if(it->prohibited)
        {
            ss << "-";
        }
        ss << (*it).query->toString();
        if ((it + 1) != m_clauses.end())
        {
            ss << _T(", ");
        }
    }
    ss << _T("]");
    return ss.str();
}

void BooleanQuery::terms(TermVector& terms) const
{
    for (BooleanClauseVector::const_iterator it = m_clauses.begin();
         it != m_clauses.end(); ++it)
    {
        (*it).query->terms(terms);
    }
}

FX_NS_END

