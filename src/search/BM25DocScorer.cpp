#include "firtex/search/BM25DocScorer.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_LOGGER(search, BM25DocScorer);

BM25DocScorer::BM25DocScorer() 
{
}

BM25DocScorer::~BM25DocScorer() 
{
    clear();
}

void BM25DocScorer::init(const FeatureProvider* pProvider,
                         const QueryFeature& queryFeature,
                         const std::string& sField)
{
    const IndexFeature& indexFeature = pProvider->getIndexFeature();

    for (size_t i = 0; i < queryFeature.size(); ++i)
    {
        const TermQueryFeature& tqFeature = queryFeature[i];
        FIRTEX_ASSERT2(tqFeature.getNumSuccessors() < queryFeature.size() - i);

        const Term* pTerm = tqFeature.getTerm();
        if (!tqFeature.isProhibited() 
            && (sField.empty() || !strCompareNoCase(pTerm->getField().c_str(), sField.c_str())))
        {
            BM25Weight w;
            w.init(indexFeature, &tqFeature);
            
            BM25TermScorer* pDocScore = new BM25TermScorer(indexFeature, w, 
                    &tqFeature, (uint32_t)i);
            m_termScorers.push_back(pDocScore);
        }
    }
}

void BM25DocScorer::clear()
{
    for (TermScorerVect::iterator it = m_termScorers.begin();
         it != m_termScorers.end(); ++it)
    {
        delete *it;
    }
    m_termScorers.clear();
}


FX_NS_END

