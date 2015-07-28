#include "firtex/contrib/highlighter/QueryTermExtractor.h"
#include "firtex/search/Query.h"
#include "firtex/index/IndexReader.h"
#include "firtex/search/BooleanQuery.h"
#include <cmath>

using namespace std;
FX_NS_USE(search);
FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(highlight)

QueryTermExtractor::QueryTermExtractor(void)
{

}
QueryTermExtractor::~QueryTermExtractor(void)
{

}

WeightedTermsPtr QueryTermExtractor::getTerms(const Query* pQuery) 
{
    return getTerms(pQuery,false);
}

WeightedTermsPtr QueryTermExtractor::getIdfWeightedTerms(const Query* pQuery,
        IndexReader* pReader, const string& fieldName) 
{
    WeightedTermsPtr pWTs = getTerms(pQuery, false, fieldName);
    int32_t totalNumDocs = pReader->getDocCount();
    WeightedTermPtr pWT;
    for (size_t i = 0; i < pWTs->size(); i++)
    {
        try
        {
            pWT = (*pWTs)[i];

            TermReaderPtr pTermReader = pReader->termReader();
            TermPostingIteratorPtr pPostIter = pTermReader->seek(pWT->getTerm());
            df_t docFreq = pPostIter->getTermMeta().getDocFreq();
            //IDF algorithm taken from DefaultSimilarity class
            float idf = (float)(std::log((float)totalNumDocs/(double)(docFreq + 1)) + 1.0);
            pWT->m_fWeight *= idf;
        } 
        catch (FX_NS(utility)::FirteXException&)
        {
            //ignore 
        }
    }
    return pWTs;
}

WeightedTermsPtr QueryTermExtractor::getTerms(const Query* pQuery,
        bool prohibited, const string& fieldName) 
{
    WeightedTermsPtr pWTs(new WeightedTerms());
    getTerms(pQuery, pWTs, prohibited,fieldName);
    return pWTs;
}

WeightedTermsPtr QueryTermExtractor::getTerms(const Query* pQuery, bool prohibited) 
{
    Query::TermVector terms;
    pQuery->terms(terms);
    if (terms.size() > 0)
    {
        return getTerms(pQuery, prohibited, terms[0]->getField().c_str());
    }
    return WeightedTermsPtr();
}	

void QueryTermExtractor::getTerms(const Query* pQuery, WeightedTermsPtr& pWTs,
                                  bool prohibited, const string& fieldName) 
{
    if (!strCompare(pQuery->getIdentifier().c_str(), _T("BooleanQuery")))
    {
        getTermsFromBooleanQuery((BooleanQuery*)pQuery, pWTs, prohibited, fieldName);
    }
    else
    {
        Query::TermVector terms;
        pQuery->terms(terms);
        TermPtr pTerm;
        for (size_t i = 0;i < terms.size(); i++)
        {
            pTerm = terms[i];
            if ((fieldName.empty()) || (pTerm->getField() == fieldName))
            {
                WeightedTermPtr pWT(new WeightedTerm(pQuery->getBoost(), pTerm.get()));
                pWTs->push_back(pWT);
            }
            else 
            {
                WeightedTermPtr pWT(new WeightedTerm(pQuery->getBoost(), pTerm.get()));
                pWT->setField(fieldName);
                pWTs->push_back(pWT);
            }
        }
    }
}

void QueryTermExtractor::getTermsFromBooleanQuery(const BooleanQuery* pQuery,
        WeightedTermsPtr& pWTs, bool prohibited, const string& fieldName)
{
    const BooleanClauseVector& queryClauses = pQuery->getClauses();

    BooleanClauseVector::const_iterator iter = queryClauses.begin();
    while (iter != queryClauses.end())
    {
        BooleanClause clause = *iter;
        if (prohibited || !(clause.prohibited))
        {
            getTerms(clause.query.get(), pWTs,prohibited, fieldName);
        }
        iter++;
    }
}

FX_NS_END
