#include "firtex/search/FieldLengthFeatureEvaluator.h"

FX_NS_USE(index);
FX_NS_USE(queryparser);

FX_NS_DEF(search);

SETUP_LOGGER(search, FieldLengthFeatureEvaluator);

FieldLengthFeatureEvaluator::FieldLengthFeatureEvaluator(
        const LengthNormIteratorPtr& pLengthIter) 
    : ExprEvaluator(VALUE_INT32)
    , m_pLengthNormIt(pLengthIter)
{
}

FieldLengthFeatureEvaluator::~FieldLengthFeatureEvaluator() 
{
}

float FieldLengthFeatureEvaluator::evaluateFloat(const ScoreDoc& scoredDoc)
{
    return (float)doEvaluate(scoredDoc);
}

double FieldLengthFeatureEvaluator::evaluateDouble(const ScoreDoc& scoredDoc)
{
    return (double)doEvaluate(scoredDoc);
}

int32_t FieldLengthFeatureEvaluator::evaluateInt32(const ScoreDoc& scoredDoc)
{
    return doEvaluate(scoredDoc);
}

int64_t FieldLengthFeatureEvaluator::evaluateInt64(const ScoreDoc& scoredDoc)
{
    return doEvaluate(scoredDoc);
}

FX_NS_END

