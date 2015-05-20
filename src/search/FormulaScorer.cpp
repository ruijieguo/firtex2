#include "firtex/search/FormulaScorer.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/queryparser/ExprParser.h"
#include "firtex/queryparser/DefaultExprEvaluatorBuilder.h"
#include "firtex/search/FeatureContext.h"

FX_NS_USE(utility);
FX_NS_USE(index);
FX_NS_USE(queryparser);

FX_NS_DEF(search);

SETUP_LOGGER(search, FormulaScorer);

FormulaScorer::FormulaScorer() 
{
}

FormulaScorer::~FormulaScorer() 
{
}

void FormulaScorer::beginQuery(const FeatureProviderPtr& pProvider,
                               const QueryFeature& queryFeature)
{
    IndexReaderPtr pIndexReader = pProvider->getIndexFeature().getIndexReader();
    
    ExprParser parser(pIndexReader);
    FeatureContext featureCtx(pProvider.get(), &queryFeature);
    DefaultExprEvaluatorBuilder exprBuilder(parser, pIndexReader, &featureCtx);

//    parser.parse();
}
    
void FormulaScorer::score(MatchedDocSet& matchedDocs)
{
}

void FormulaScorer::endQuery()
{
}

void FormulaScorer::setParameter(const std::string& sParam)
{
    KeyValueParser kvParser;
    if (!kvParser.parse(sParam, ",", "="))
    {
        FX_LOG(ERROR, "Parse formula parameter : [%s] FAILED", sParam.c_str());
        return;
    }
    if (!kvParser.getValue("formula", m_sFormula))
    {
        FX_LOG(ERROR, "Extract formula from [%s] FAILED", sParam.c_str());
        return;
    }
}

FX_NS_END

