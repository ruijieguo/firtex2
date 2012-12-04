#include "firtex/queryparser/DefaultQueryBuilder.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/analyzer/AnalyzerMapper.h"
#include <memory>

using namespace std;

FX_NS_USE(analyzer);
FX_NS_USE(search);
FX_NS_USE(index);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, DefaultQueryBuilder);

DefaultQueryBuilder::DefaultQueryBuilder(const QueryParser& queryParser) 
    : m_queryParser(queryParser)
{
}

DefaultQueryBuilder::~DefaultQueryBuilder() 
{
}

#define GET_FIELD_NAME(expr) expr.getFieldName().empty() ?      \
    m_queryParser.getDefaultField() : expr.getFieldName()

Query* DefaultQueryBuilder::createQuery(const TermQueryExpr& expr)
{
    const string& sField = GET_FIELD_NAME(expr);
    TokenViewPtr pTokens = tokenize(sField, expr.getValue());
    if (pTokens.isNull())
    {
        return NULL;
    }
    auto_ptr<Query> pQuery;
    TokenView::Iterator it = pTokens->iterator();
    if (it.size() == 1)
    {
        const Token& token = it.next();
        pQuery.reset(new TermQuery(new Term(sField, token.getTextValue())));
    }
    else if (it.size() > 1)
    {
        switch (m_queryParser.getDefaultOp())
        {
        case QueryParser::OP_PHRASE:
        {
            PhraseQuery* pPhraseQuery = createPhraseQuery(sField, it);
            pQuery.reset(pPhraseQuery);
        }
        case QueryParser::OP_OR:
        {
            BooleanQuery* pBoolQuery = createBooleanQuery(
                    sField, it, false, false);

            pBoolQuery->setMinShouldMatch(1);
            pQuery.reset(pBoolQuery);
        }
        default:
        {
            BooleanQuery* pBoolQuery = createBooleanQuery(
                    sField, it, true, false);
            pQuery.reset(pBoolQuery);
        }
        }//end switch
    }//end else if (it.size() > 1)

    if (pQuery.get() != NULL && (expr.getBoost() != 0))
    {
        pQuery->setBoost((float)expr.getBoost());
    }
    return pQuery.release();
}

Query* DefaultQueryBuilder::createQuery(const AnyQueryExpr& expr)
{
    return new AnyQuery();
}

Query* DefaultQueryBuilder::createQuery(const PhraseQueryExpr& expr)
{
    const string& sField = GET_FIELD_NAME(expr);
    TokenViewPtr pTokens = tokenize(sField, expr.getValue());
    if (pTokens.isNull())
    {
        return NULL;
    }
    auto_ptr<Query> pQuery;
    TokenView::Iterator it = pTokens->iterator();
    if (it.size() == 1)
    {
        const Token& token = it.next();
        pQuery.reset(new TermQuery(new Term(sField, token.getTextValue())));
    }
    else if (it.size() > 1)
    {
        PhraseQuery* pPhraseQuery = createPhraseQuery(sField, it);
        pPhraseQuery->setSlop(expr.getSlop());
        pQuery.reset(pPhraseQuery);
    }
    if ((pQuery.get() != NULL) && (expr.getBoost() != 0))
    {
        pQuery->setBoost((float)expr.getBoost());
    }    
    return pQuery.release();
}

Query* DefaultQueryBuilder::createQuery(const BooleanQueryExpr& expr)
{
    QueryExpr* pLeftNode = expr.getLeftNode();
    QueryExpr* pRightNode = expr.getRightNode();

    Query* pLeftQ = pLeftNode->accept(*this);
    BooleanClause leftClause(pLeftQ, pLeftNode->isRequired(),
                             pLeftNode->isProhibited());

    Query* pRightQ = pRightNode->accept(*this);
    BooleanClause rightClause(pRightQ, pRightNode->isRequired(),
                              pRightNode->isProhibited());

    //TODO:
    if (expr.getOpType() == QueryExpr::OP_AND)
    {
        if (!(pLeftNode->isProhibited()))
        {
            leftClause.required = true;
        }
        if (!(pRightNode->isProhibited()))
        {
            rightClause.required = true;
        }
    }
    BooleanQuery* pBoolQuery = new BooleanQuery();
    auto_ptr<BooleanQuery> boolQueryPtr(pBoolQuery);

    if (leftClause.query.isNotNull())
    {
        pBoolQuery->addClause(leftClause);
    }
    if (rightClause.query.isNotNull())
    {
        pBoolQuery->addClause(rightClause);
    }
    
    return boolQueryPtr.release();
}

Query* DefaultQueryBuilder::createQuery(const RangeQueryExpr& expr)
{
    FIRTEX_THROW(UnsupportedOpException, "Unsupport range query currently.");
    return NULL;
}

PhraseQuery* DefaultQueryBuilder::createPhraseQuery(
        const string& sField, TokenView::Iterator& it)
{
    PhraseQuery* pPhraseQuery = new PhraseQuery();
    while (it.hasNext())
    {
        const Token& token = it.next();
        pPhraseQuery->addTerm(TermPtr(new Term(sField, token.getTextValue())));
    }
    return pPhraseQuery;
}

BooleanQuery* DefaultQueryBuilder::createBooleanQuery(const string& sField,
        TokenView::Iterator& it, bool bReq, bool bProh)
{
    BooleanQuery* pBoolQuery = new BooleanQuery();
    while (it.hasNext())
    {
        const Token& token = it.next();
        QueryPtr pTermQuery(new TermQuery(new Term(sField, token.getTextValue())));
        pBoolQuery->addClause(pTermQuery, bReq, bProh);
    }
    return pBoolQuery;
}

TokenViewPtr DefaultQueryBuilder::tokenize(
        const string& sField, const string& sValue)
{
    const AnalyzerMapper* pAnalyzerMapper = m_queryParser.getAnalyzerMapper();
    const Analyzer* pAnalyzer = pAnalyzerMapper->getAnalyzerForSearcher(sField);
    if (!pAnalyzer && !(pAnalyzerMapper->isIndexed(sField)))
    {
        FX_LOG(WARN, "Invalid field name or the field is not indexed: [%s]",
               sField.c_str());
        return TokenViewPtr();
    }
    if (pAnalyzer)
    {
        return pAnalyzer->tokenize(sValue);
    }
    TokenViewPtr pTokenView(new TokenView(1, Token::TT_UNKNOWN));
    pTokenView->addToken(sValue.c_str(), sValue.length(),
                         1, 0, (int32_t)sValue.length());
    return pTokenView;
}

FX_NS_END

