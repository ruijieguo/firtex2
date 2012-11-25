#include "firtex/search/PhraseQuery.h"
#include "firtex/search/PhraseQueryExecutor.h"
#include <sstream>

using namespace std;
FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, PhraseQuery);

PhraseQuery::PhraseQuery() 
    : m_nSlop(0)
{
}

PhraseQuery::PhraseQuery(const PhraseQuery& src) 
{
    m_terms.assign(src.m_terms.begin(), src.m_terms.end());
    m_nSlop = src.m_nSlop;
}

PhraseQuery::~PhraseQuery() 
{
}

tstring PhraseQuery::getIdentifier() const
{
    return _T("PhraseQuery");
}

size_t PhraseQuery::size() const
{
    return m_terms.size();
}

QueryExecutorPtr PhraseQuery::createExecutor(IndexReaderPtr& pIndexReader,
                                             FeatureProviderPtr& pProvider,
                                             PoolPtr& pPool) const
{
    FIRTEX_ASSERT2(m_terms.size() > 1);

    TermReaderPtr pTermReader = pIndexReader->termReader();
    size_t size = m_terms.size();
    PhraseQueryExecutorPtr pExe = new PhraseQueryExecutor(
            this, pProvider, pPool);

    vector<TermPostingIteratorPtr> termPosVec;
    termPosVec.reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        TermPostingIteratorPtr pPostIter = pTermReader->seek(m_terms[i].get());
        if (pPostIter.isNull())
        {
            return QueryExecutorPtr();
        }
        pExe->addPosting(pPostIter);
    }
    pExe->setSlop(m_nSlop);
    return pExe;
}

void PhraseQuery::addTerm(const TermPtr& pTerm)
{
    m_terms.push_back(pTerm);
}

Query* PhraseQuery::clone() const
{
    return new PhraseQuery(*this);
}

void PhraseQuery::terms(TermVector& terms) const
{
    terms.insert(terms.end(), m_terms.begin(), m_terms.end());
}

tstring PhraseQuery::toString() const
{
    stringstream ss;
    ss << _T("PhraseQuery: [");
    for (TermVector::const_iterator it = m_terms.begin();
         it != m_terms.end(); ++it)
    {
        ss << (*it)->toString();
        if ((it + 1) != m_terms.end())
        {
            ss << _T(", ");
        }
    }
    ss << _T("]");
    return ss.str();
}

FX_NS_END

