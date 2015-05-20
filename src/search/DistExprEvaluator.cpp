#include "firtex/search/DistExprEvaluator.h"

FX_NS_USE(queryparser);

FX_NS_DEF(search);

SETUP_LOGGER(search, DistExprEvaluator);

DistExprEvaluator::DistExprEvaluator(ExprEvaluator* pLat1Expr,
                                     ExprEvaluator* pLon1Expr,
                                     ExprEvaluator* pLat2Expr,
                                     ExprEvaluator* pLon2Expr)
    : ExprEvaluator(VALUE_FLOAT)
    , m_pLat1Expr(pLat1Expr)
    , m_pLon1Expr(pLon1Expr)
    , m_pLat2Expr(pLat2Expr)
    , m_pLon2Expr(pLon2Expr)
{
}

float DistExprEvaluator::evaluateFloat(const ScoreDoc& scoredDoc)
{
    return calcDistance(m_pLat1Expr->evaluateFloat(scoredDoc),
                        m_pLon1Expr->evaluateFloat(scoredDoc),
                        m_pLat2Expr->evaluateFloat(scoredDoc),
                        m_pLon2Expr->evaluateFloat(scoredDoc));
}

double DistExprEvaluator::evaluateDouble(const ScoreDoc& scoredDoc)
{
    return (double)calcDistance(m_pLat1Expr->evaluateFloat(scoredDoc),
                                m_pLon1Expr->evaluateFloat(scoredDoc),
                                m_pLat2Expr->evaluateFloat(scoredDoc),
                                m_pLon2Expr->evaluateFloat(scoredDoc));
}

int32_t DistExprEvaluator::evaluateInt32(const ScoreDoc& scoredDoc)
{
    return (int32_t)calcDistance(m_pLat1Expr->evaluateFloat(scoredDoc),
                                 m_pLon1Expr->evaluateFloat(scoredDoc),
                                 m_pLat2Expr->evaluateFloat(scoredDoc),
                                 m_pLon2Expr->evaluateFloat(scoredDoc));
}

int64_t DistExprEvaluator::evaluateInt64(const ScoreDoc& scoredDoc)
{
    return (int64_t)calcDistance(m_pLat1Expr->evaluateFloat(scoredDoc),
                                 m_pLon1Expr->evaluateFloat(scoredDoc),
                                 m_pLat2Expr->evaluateFloat(scoredDoc),
                                 m_pLon2Expr->evaluateFloat(scoredDoc));
}

FX_NS_END

