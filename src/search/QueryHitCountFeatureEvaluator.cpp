#include "firtex/search/QueryHitCountFeatureEvaluator.h"
#include "firtex/search/QueryFeature.h"

FX_NS_USE(queryparser);
FX_NS_DEF(search);

SETUP_LOGGER(search, QueryHitCountFeatureEvaluator);

QueryHitCountFeatureEvaluator::QueryHitCountFeatureEvaluator(
        const FeatureContext* pFeatureCtx) 
    : ExprEvaluator(VALUE_INT32)
{
    const QueryFeature* pQueryFeature = pFeatureCtx->queryFeature;
    m_termIdx.reserve(pQueryFeature->size());

    for (size_t i = 0; i < pQueryFeature->size(); ++i)
    {
        const TermQueryFeature& tqFeature = (*pQueryFeature)[i];
        if (!tqFeature.isProhibited())
        {
            m_termIdx.push_back(i);
        }
    }
}

QueryHitCountFeatureEvaluator::~QueryHitCountFeatureEvaluator() 
{
}

float QueryHitCountFeatureEvaluator::evaluateFloat(const ScoreDoc& scoredDoc)
{
    return (float)doEvaluate((const MatchedDoc&)scoredDoc);
}

double QueryHitCountFeatureEvaluator::evaluateDouble(const ScoreDoc& scoredDoc)
{
    return (double)doEvaluate((const MatchedDoc&)scoredDoc);
}

int32_t QueryHitCountFeatureEvaluator::evaluateInt32(const ScoreDoc& scoredDoc)
{
    return doEvaluate((const MatchedDoc&)scoredDoc);
}

int64_t QueryHitCountFeatureEvaluator::evaluateInt64(const ScoreDoc& scoredDoc)
{
    return doEvaluate((const MatchedDoc&)scoredDoc);
}

FX_NS_END

