#ifndef _WEIGHTEDTERM_H
#define _WEIGHTEDTERM_H

#include "firtex/index/Term.h"

FX_NS_DEF(highlight)

class WeightedTerm
{
public:
    WeightedTerm(float fWeight, const FX_NS(index)::Term* pTerm)
        : m_fWeight(fWeight)
        , m_pTerm(pTerm->clone())
    {
    }

    ~WeightedTerm(void)
    {
        if(m_pTerm)
        {
            delete m_pTerm;
            m_pTerm = NULL;
        }
    }

    /**
     * @return the term value (stemmed)
     */
    const FX_NS(index)::Term* getTerm()
    {
        return m_pTerm;
    }

    /**
     * @return the weight associated with this term
     */
    float getWeight() const
    {
        return m_fWeight;
    }

    /**
     * @param term the term value 
     */
    void setTerm(const FX_NS(index)::Term* pTerm)
    {
        if(m_pTerm)
            delete m_pTerm;
        m_pTerm = pTerm->clone();
    }

    /**
     * @param weight the weight associated with this term
     */
    void setWeight(float fWeight)
    {
        m_fWeight = fWeight;
    }

    void setField(const std::string& sFieldName)
    {
        if (m_pTerm)
        {
            m_pTerm->setField(sFieldName);
        }
    }

protected:
    float m_fWeight;
    FX_NS(index)::Term*	m_pTerm;

    friend class QueryTermExtractor;
};

DEFINE_TYPED_PTR(WeightedTerm);

typedef std::vector<WeightedTermPtr> WeightedTerms;

DEFINE_TYPED_PTR(WeightedTerms);

FX_NS_END

#endif
