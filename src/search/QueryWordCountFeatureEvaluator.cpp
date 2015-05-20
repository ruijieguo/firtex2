#include "firtex/search/QueryWordCountFeatureEvaluator.h"

FX_NS_USE(queryparser);
FX_NS_DEF(search);

SETUP_LOGGER(search, QueryWordCountFeatureEvaluator);

QueryWordCountFeatureEvaluator::QueryWordCountFeatureEvaluator(const QueryFeature& queryFeature) 
    : ExprEvaluator(VALUE_INT32)
    , m_nWordCount(0)
{
    for (size_t i = 0; i < queryFeature.size(); ++i)
    {
        const TermQueryFeature& tqFeature = queryFeature[i];
        if (!tqFeature.isProhibited())
        {
            ++m_nWordCount;
        }
    }
}

QueryWordCountFeatureEvaluator::~QueryWordCountFeatureEvaluator() 
{
}

float QueryWordCountFeatureEvaluator::evaluateFloat(const ScoreDoc& scoredDoc)
{
    return (float)m_nWordCount;
}

double QueryWordCountFeatureEvaluator::evaluateDouble(const ScoreDoc& scoredDoc)
{
    return (double)m_nWordCount;
}

int32_t QueryWordCountFeatureEvaluator::evaluateInt32(const ScoreDoc& scoredDoc)
{
    return m_nWordCount;
}

int64_t QueryWordCountFeatureEvaluator::evaluateInt64(const ScoreDoc& scoredDoc)
{
    return m_nWordCount;
}

FX_NS_END

