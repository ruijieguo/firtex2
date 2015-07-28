#include "firtex/contrib/highlighter/QueryScorer.h"
#include "firtex/contrib/highlighter/QueryTermExtractor.h"
#include "firtex/search/Query.h"
#include <memory>


FX_NS_USE(search);
FX_NS_USE(index);
FX_NS_USE(analyzer);

FX_NS_DEF(highlight);

QueryScorer::QueryScorer(const Query* pQuery)
  : m_pCurrentTextFragment(NULL)
  , m_fTotalScore(0.0f)
  , m_fMaxTermWeight(0.0f)
{
    Query::TermVector terms;
    pQuery->terms(terms);
    if (terms.size() > 0)
    {
        m_sFieldName = terms[0]->getField();
        init(QueryTermExtractor::getTerms(pQuery, false, m_sFieldName));
    }
    else 
    {
        m_sFieldName = _T("TEXT");
        init(QueryTermExtractor::getTerms(pQuery, false, ""));
    }			
}

QueryScorer::QueryScorer(const Query* pQuery, const tchar* fieldName)
    : m_sFieldName(fieldName)
    , m_pCurrentTextFragment(NULL)
    , m_fTotalScore(0.0f)
    , m_fMaxTermWeight(0.0f)
{
    WeightedTermsPtr pTerms = QueryTermExtractor::getTerms(pQuery, false, m_sFieldName);
    init(pTerms);
}	

QueryScorer::QueryScorer(const Query* pQuery, IndexReader* pReader, const tchar* fieldName)
    : m_sFieldName(fieldName)
    , m_pCurrentTextFragment(NULL)
    , m_fTotalScore(0.0f)
    , m_fMaxTermWeight(0.0f)
{
    init(QueryTermExtractor::getIdfWeightedTerms(pQuery, pReader, m_sFieldName)); 
}

QueryScorer::QueryScorer(const WeightedTermsPtr& pWeightedTerms)
  : m_pCurrentTextFragment(NULL)
  , m_fTotalScore(0.0f)
  , m_fMaxTermWeight(0.0f)
{			
    init(pWeightedTerms);
}

QueryScorer::~QueryScorer()
{
    TermSet::iterator iter = m_uniqueTermsInFragment.begin();
    while (iter != m_uniqueTermsInFragment.end())
    {
        delete (*iter);
        iter++;
    }
    m_uniqueTermsInFragment.clear();

    m_termsToFind.clear();		
}

void QueryScorer::init(const WeightedTermsPtr& pWeightedTerms)
{
    WeightedTermPtr pWT;
    WeightedTermPtr pExistingTerm;
    for (size_t i = 0; i < pWeightedTerms->size(); i++)
    {
        pWT = (*pWeightedTerms)[i];
        if(m_sFieldName.empty())
            m_sFieldName = pWT->getTerm()->getField();
        WTermMap::iterator iter = m_termsToFind.find(pWT->getTerm());
        if(iter != m_termsToFind.end())
        {
            pExistingTerm = iter->second;
        }
        else 
        {
            pExistingTerm.reset();
        }

        if( (!pExistingTerm) || (pExistingTerm->getWeight() < pWT->getWeight()))
        {
            if(pExistingTerm)
            {
                m_termsToFind.erase(iter);
            }
            //if a term is defined more than once, always use the highest scoring weight
            m_termsToFind.insert(make_pair(pWT->getTerm(), pWT));
            m_fMaxTermWeight = max(m_fMaxTermWeight,pWT->getWeight());
        }
    }
}

void QueryScorer::startFragment(TextFragment* pNewFragment)
{			
    m_pCurrentTextFragment = pNewFragment;
    m_fTotalScore = 0;
}

float QueryScorer::getTokenScore(const Term* pTerm)
{
    Term* pTerm2 = const_cast<Term*>(pTerm);
    WTermMap::const_iterator iter = m_termsToFind.find(pTerm2);
    if(iter == m_termsToFind.end())
    {
        //not a query term
        return 0;
    }
    WeightedTermPtr pQueryTerm = iter->second;
    //found a query term - is it unique in this doc?
    if(m_uniqueTermsInFragment.find(pTerm2) == m_uniqueTermsInFragment.end())
    {
        m_fTotalScore += pQueryTerm->getWeight();
        m_uniqueTermsInFragment.insert(pTerm2->clone());
    }			
    return pQueryTerm->getWeight();
}

float QueryScorer::getFragmentScore() const
{
    return m_fTotalScore;		
}

void QueryScorer::allFragmentsProcessed()
{
    //this class has no special operations to perform at end of processing
}

float QueryScorer::getMaxTermWeight() const
{
    return m_fMaxTermWeight;
}

FX_NS_END

