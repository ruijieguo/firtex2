#include "firtex/queryparser/QueryExpr.h"
#include "firtex/queryparser/QueryBuilder.h"
#include "firtex/utility/NumberFormatter.h"
#include <sstream>

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(search);

FX_NS_DEF(queryparser);

QueryExpr::QueryExpr() 
    : m_modifier(MOD_NONE)
{
}

QueryExpr::QueryExpr(const std::string& sField)
    : m_sFieldName(sField)
{
}

QueryExpr::~QueryExpr() 
{
}

void QueryExpr::setFieldName(const std::string& sField)
{
    m_sFieldName = sField;
}

//static 
std::string* QueryExpr::toString(int64_t value)
{
    std::string* pStr = new std::string;
    NumberFormatter::append(*pStr, value);
    return pStr;
}

///TermQueryExpr
TermQueryExpr::TermQueryExpr(const std::string& sField, std::string* pValue)
    : QueryExpr(sField)
    , m_pValue(pValue)
    , m_fBoost(1)
    , m_modifier(MOD_NONE)
{
}

TermQueryExpr::TermQueryExpr(std::string* pValue)
    : m_pValue(pValue)
    , m_fBoost(1)
    , m_modifier(MOD_NONE)
{
}

TermQueryExpr::~TermQueryExpr()
{
    if (m_pValue)
    {
        delete m_pValue;
    }
}

std::string TermQueryExpr::toString() const
{
    stringstream ss;
    if (isRequired())
    {
        ss << "+";
    }
    if (isProhibited())
    {
        ss << "-";
    }
    ss << m_sFieldName << ": " << (m_pValue ? *m_pValue : "");
    if (m_fBoost != 1)
    {
        ss << "^" << m_fBoost;
    }
    return ss.str();
}

Query* TermQueryExpr::accept(QueryBuilder& builder)
{
    return builder.createQuery(*this);
}

///AnyQueryExpr
AnyQueryExpr::AnyQueryExpr()
{
}

AnyQueryExpr::~AnyQueryExpr()
{
}

std::string AnyQueryExpr::toString() const
{
    stringstream ss;
    ss << "*:*";
    return ss.str();
}

Query* AnyQueryExpr::accept(QueryBuilder& builder)
{
    return builder.createQuery(*this);
}


/////////////////////////////////////////
//PhraseQueryExpr
std::string PhraseQueryExpr::toString() const
{
    stringstream ss;
    if (isRequired())
    {
        ss << "+";
    }
    if (isProhibited())
    {
        ss << "-";
    }
    ss << m_sFieldName << ": " << "\"" << (m_pValue ? *m_pValue : "") << "\"";
    if (m_nSlop != 0)
    {
        ss << "~" << m_nSlop;
    }
    if (m_fBoost != 1)
    {
        ss << "^" << m_fBoost;
    }
    return ss.str();
}

Query* PhraseQueryExpr::accept(QueryBuilder& builder)
{
    return builder.createQuery(*this);
}


/////////////////////////////////////////
///RangeQueryExpr
RangeQueryExpr::RangeQueryExpr(const std::string& sField,
                               std::string* pLowerValue,
                               bool bInclusiveLower,
                               std::string* pUpperValue,
                               bool bInclusiveUpper)
    : TermQueryExpr(sField, NULL)
    , m_pLowerValue(pLowerValue)
    , m_pUpperValue(pUpperValue)
    , m_bInclusiveLower(bInclusiveLower)
    , m_bInclusiveUpper(bInclusiveUpper)
{
}

RangeQueryExpr::RangeQueryExpr(std::string* pLowerValue,
                               bool bInclusiveLower,
                               std::string* pUpperValue,
                               bool bInclusiveUpper)
    : TermQueryExpr( NULL)
    , m_pLowerValue(pLowerValue)
    , m_pUpperValue(pUpperValue)
    , m_bInclusiveLower(bInclusiveLower)
    , m_bInclusiveUpper(bInclusiveUpper)
{
}

RangeQueryExpr::~RangeQueryExpr()
{
    if (m_pLowerValue)
    {
        delete m_pLowerValue;
    }
    if (m_pUpperValue)
    {
        delete m_pUpperValue;
    }
}

Query* RangeQueryExpr::accept(QueryBuilder& builder)
{
    return builder.createQuery(*this);
}

std::string RangeQueryExpr::toString() const
{
    stringstream ss;
    if (isRequired())
    {
        ss << "+";
    }
    if (isProhibited())
    {
        ss << "-";
    }
    ss << getFieldName() << ": ";
    if (m_bInclusiveLower)
    {
        ss << "[" << (m_pLowerValue ? *m_pLowerValue : "");
    }
    else 
    {
        ss << "{" << (m_pLowerValue ? *m_pLowerValue : "");
    }
    ss << " TO ";
    if (m_bInclusiveUpper)
    {
        ss << (m_pUpperValue ? *m_pUpperValue : "") << "]";
    }
    else 
    {
        ss << (m_pUpperValue ? *m_pUpperValue : "") << "}";
    }
    return ss.str();
}

////////////////////////////////////////////////////////////
///BooleanQueryExpr
BooleanQueryExpr::BooleanQueryExpr(QueryExpr* pExpr1,
                                   QueryExpr* pExpr2,
                                   QueryExpr::OpType op)
    : m_pLeftExpr(pExpr1)
    , m_pRightExpr(pExpr2)
    , m_operation(op)
{
}

BooleanQueryExpr::~BooleanQueryExpr()
{
    delete m_pLeftExpr;
    delete m_pRightExpr;
}

Query* BooleanQueryExpr::accept(QueryBuilder& builder)
{
    return builder.createQuery(*this);
}

void BooleanQueryExpr::setFieldName(const std::string& sField)
{
    m_pLeftExpr->setFieldName(sField);
    m_pRightExpr->setFieldName(sField);
}

std::string BooleanQueryExpr::toString() const
{
    stringstream ss;
    if (isRequired())
    {
        ss << "+" << "<";
    }
    if (isProhibited())
    {
        ss << "-" << "<";
    }
    ss << "<" << m_pLeftExpr->toString() << ">";
    switch (m_operation)
    {
    case QueryExpr::OP_AND:
        ss << " AND ";
        break;
    case QueryExpr::OP_OR:
        ss << " OR ";
        break;
    default:
        assert(false);
    }
    ss << "<" << m_pRightExpr->toString() << ">";
    if (isRequired() || isProhibited())
    {
        ss << ">";
    }

    return ss.str();
}

FX_NS_END

