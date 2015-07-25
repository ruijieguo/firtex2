#include "firtex/queryparser/QueryParser.h"
#include "firtex/queryparser/QueryExprParser.h"
#include "firtex/queryparser/DefaultQueryBuilder.h"

using namespace std;
FX_NS_USE(analyzer);
FX_NS_USE(search);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, QueryParser);

QueryParser::QueryParser(const AnalyzerMapper* pAnalyzerMapper, 
                         const string& sDefaultField, OpType defOp)
    : m_pAnalyzerMapper(pAnalyzerMapper)
    , m_sDefField(sDefaultField)
    , m_defOp(defOp)
{
    if (m_sDefField.empty())
    {
        m_sDefField = pAnalyzerMapper->getDefaultField();
    }
}

QueryParser::~QueryParser() 
{
}

QueryPtr QueryParser::parse(const string& sQueryExpr)
{
    QueryExprParser exprParser(m_sDefField);
    QueryExprPtr pExpr = exprParser.parse(sQueryExpr);
    if (pExpr.isNull())
    {
        FIRTEX_THROW(SyntaxException, "Bad query syntax: [%s]", 
                     exprParser.getErrorMessage().c_str());
    }
    DefaultQueryBuilder builder(*this);

    Query* pQuery = pExpr->accept(builder);
    return QueryPtr(pQuery);
}

FX_NS_END

