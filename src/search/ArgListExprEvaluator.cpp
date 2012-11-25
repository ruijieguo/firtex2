#include "firtex/search/ArgListExprEvaluator.h"

FX_NS_USE(queryparser);
FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, ArgListExprEvaluator);

ArgListExprEvaluator::ArgListExprEvaluator(
        ExprEvaluator* pLeft, ExprEvaluator* pRight)
    : ExprEvaluator(VALUE_NONE)
{
    addArg(pLeft);
    addArg(pRight);
}

ArgListExprEvaluator::~ArgListExprEvaluator() 
{
    for (ArgVector::iterator it = m_arguments.begin();
         it != m_arguments.end(); ++it)
    {
        delete *it;
    }
    m_arguments.clear();
}

void ArgListExprEvaluator::addArg(ExprEvaluator* pExprEval)
{
    m_arguments.push_back(pExprEval);
}

float ArgListExprEvaluator::evaluateFloat(const ScoredDoc& scoredDoc)
{
    FIRTEX_ASSERT2(false);
    return 0.0f;
}

double ArgListExprEvaluator::evaluateDouble(const ScoredDoc& scoredDoc)
{
    FIRTEX_ASSERT2(false);
    return 0.0f;
}

int32_t ArgListExprEvaluator::evaluateInt32(const ScoredDoc& scoredDoc)
{
    FIRTEX_ASSERT2(false);
    return -1;
}

int64_t ArgListExprEvaluator::evaluateInt64(const ScoredDoc& scoredDoc)
{
    FIRTEX_ASSERT2(false);
    return -1;
}

FX_NS_END

