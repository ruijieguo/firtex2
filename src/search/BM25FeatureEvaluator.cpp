#include "firtex/search/BM25FeatureEvaluator.h"

FX_NS_USE(queryparser);

FX_NS_DEF(search);

SETUP_LOGGER(search, BM25FeatureEvaluator);

BM25FeatureEvaluator::BM25FeatureEvaluator(const FeatureContext* pFeatureCtx,
                                           const std::string& sField) 
    : ExprEvaluator(VALUE_FLOAT)
{
    m_scorer.init(pFeatureCtx->featureProvider, *pFeatureCtx->queryFeature, sField);
}

BM25FeatureEvaluator::~BM25FeatureEvaluator() 
{
}

float BM25FeatureEvaluator::evaluateFloat(const ScoreDoc& scoredDoc)
{
    const DocScoreFeature& docScoreFeature = 
        ((const MatchedDoc&)scoredDoc).scoreFeature();
    return (float)m_scorer.score(scoredDoc.getDocId(), docScoreFeature);
}

double BM25FeatureEvaluator::evaluateDouble(const ScoreDoc& scoredDoc)
{
    const DocScoreFeature& docScoreFeature = 
        ((const MatchedDoc&)scoredDoc).scoreFeature();
    return (double)m_scorer.score(scoredDoc.getDocId(), docScoreFeature);
}

int32_t BM25FeatureEvaluator::evaluateInt32(const ScoreDoc& scoredDoc)
{
    const DocScoreFeature& docScoreFeature = 
        ((const MatchedDoc&)scoredDoc).scoreFeature();
    return (int32_t)m_scorer.score(scoredDoc.getDocId(), docScoreFeature);
}

int64_t BM25FeatureEvaluator::evaluateInt64(const ScoreDoc& scoredDoc)
{
    const DocScoreFeature& docScoreFeature = 
        ((const MatchedDoc&)scoredDoc).scoreFeature();
    return (int64_t)m_scorer.score(scoredDoc.getDocId(), docScoreFeature);
}

FX_NS_END

