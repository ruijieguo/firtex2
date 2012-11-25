#ifndef _QUERYSCORER_H
#define _QUERYSCORER_H

#include "firtex/common/StdHeader.h"
#include "Scorer.h"
#include "firtex/index/Term.h"
#include "WeightedTerm.h"
#include <map>
#include <set>

FX_NS_DEF(search);
class Query;
FX_NS_END

FX_NS_DEF(index);
class IndexReader;
FX_NS_END

FX_NS_DEF(highlight);

class QueryScorer :  public Scorer
{
protected:
    struct ltterm
    {
        bool operator()(const FX_NS(index)::Term* t1, const FX_NS(index)::Term* t2) const
        {
            return t1->compare(t2) < 0;
        }
    };

    typedef std::map<const FX_NS(index)::Term*, WeightedTermPtr, ltterm> WTermMap;
    typedef std::set<FX_NS(index)::Term*> TermSet;

public:
    /**
     * 
     * @param query a Lucene query (ideally rewritten using query.rewrite 
     * before being passed to this class and the searcher)
     */
    QueryScorer(const FX_NS(search)::Query* pQuery);

    /**
     * 
     * @param query a Lucene query (ideally rewritten using query.rewrite 
     * before being passed to this class and the searcher)
     * @param fieldName the Field name which is used to match Query terms
     */
    QueryScorer(const FX_NS(search)::Query* pQuery, const tchar* fieldName);

    /**
     * @param query a Lucene query (ideally rewritten using query.rewrite 
     * before being passed to this class and the searcher)
     * @param reader used to compute IDF which can be used to a) score selected fragments better 
     * b) use graded highlights eg set font color intensity
     * @param fieldName the field on which Inverse Document Frequency (IDF) calculations are based
     */
    QueryScorer(const FX_NS(search)::Query* pQuery,
                FX_NS(index)::IndexReader* pReader,
                const tchar* fieldName);

    QueryScorer(const WeightedTermsPtr& pWeightedTerms);

    virtual ~QueryScorer(void);

public:		
    void startFragment(TextFragment* pNewFragment);
    float getTokenScore(const FX_NS(index)::Term* pTerm);
    float getFragmentScore() const;
    void allFragmentsProcessed();

    /**
     * 
     * @return The highest weighted term (useful for passing to GradientFormatter to set
     * top end of coloring scale.  
     */
    float getMaxTermWeight() const;

    const tstring& getFieldName() const { return m_sFieldName;}

private:
    void init(const WeightedTermsPtr& pWeightedTerms);

protected:
    tstring m_sFieldName;
    TextFragment* m_pCurrentTextFragment;
    TermSet m_uniqueTermsInFragment;
    float m_fTotalScore;
    float m_fMaxTermWeight;
    WTermMap m_termsToFind;
};

FX_NS_END

#endif
