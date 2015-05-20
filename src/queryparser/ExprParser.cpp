#include "firtex/queryparser/ExprParser.h"
#include "firtex/queryparser/ExprLexer.h"
#include "firtex/queryparser/DefaultExprEvaluatorBuilder.h"
#include "firtex/queryparser/ExprOptimizer.h"
#include "ExprBisonParser.h"
#include "firtex/index/IndexReader.h"
#include "firtex/analyzer/DateTimeAnalyzer.h"

using namespace std;
using namespace fx_bison;

FX_NS_USE(search);
FX_NS_USE(document);
FX_NS_USE(index);
FX_NS_USE(analyzer);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, ExprParser);

ExprParser::FuncDecs ExprParser::FUNCTIONS[] =
{
    { "DATE",	1, FUNC_DATE,    VALUE_INT64},
    { "NOW",	0, FUNC_NOW,     VALUE_INT64},

    { "ABS",	1, FUNC_ABS,     VALUE_NONE},
    { "CEIL",	1, FUNC_CEIL,    VALUE_NONE},
    { "FLOOR",	1, FUNC_FLOOR,   VALUE_NONE},
    { "SIN",	1, FUNC_SIN,     VALUE_NONE},
    { "COS",	1, FUNC_COS,     VALUE_NONE},
    { "LN",	1, FUNC_LN,      VALUE_NONE},
    { "LOG2",	1, FUNC_LOG2,    VALUE_NONE},
    { "LOG10",	1, FUNC_LOG10,   VALUE_NONE},
    { "EXP",	1, FUNC_EXP,     VALUE_NONE},  //10
    { "SQRT",	1, FUNC_SQRT,    VALUE_NONE},
    { "INT64",	1, FUNC_INT64,   VALUE_INT64}, // type-enforcer special as-if-function
    { "INT32",	1, FUNC_INT32,   VALUE_INT32}, // type-enforcer special as-if-function
    { "DOUBLE",	1, FUNC_DOUBLE,  VALUE_DOUBLE},// type-enforcer special as-if-function
    { "FLOAT",	1, FUNC_FLOAT,   VALUE_FLOAT}, // type-enforcer special as-if-function

    { "MIN",	2, FUNC_MIN,     VALUE_NONE},
    { "MAX",	2, FUNC_MAX,     VALUE_NONE},
    { "POW",	2, FUNC_POW,     VALUE_NONE},
    { "IDIV",	2, FUNC_IDIV,    VALUE_NONE},        

    { "IF",	3, FUNC_IF,      VALUE_NONE},           //20
    { "MADD",	3, FUNC_MADD,    VALUE_NONE},
    { "MUL3",	3, FUNC_MUL3,    VALUE_NONE},        

    { "IN",    -1, FUNC_IN,      VALUE_INT32},

    { "DIST",   4, FUNC_DIST,    VALUE_FLOAT},

    //feature functions
    {"field_length", 1, FEATURE_FIELD_LENGTH, VALUE_INT32},
    {"field_avg_length", 1, FEATURE_FIELD_AVG_LENGTH, VALUE_INT32},    
    {"doc_avg_length", 0, FEATURE_DOC_AVG_LENGTH, VALUE_INT32},    
    {"doc_count", 0, FEATURE_DOC_COUNT, VALUE_INT32},    
    
    {"query_word_count", 0, FEATURE_QUERY_WORD_COUNT, VALUE_INT32},
    {"query_hit_count", 0, FEATURE_QUERY_HIT_COUNT, VALUE_INT32},
    {"bm25", 0, FEATURE_BM25, VALUE_FLOAT},    
    {"field_bm25", 1, FEATURE_FIELD_BM25, VALUE_FLOAT},    
};

ValueType ExprParser::RET_TYPES[ExprNode::TOK_TYPE_MAX] = 
{
    VALUE_NONE, //TOK_NONE
    VALUE_INT32, //TOK_INT
    VALUE_DOUBLE, //TOK_DOUBLE
    VALUE_NONE, //TOK_IDENT
    VALUE_NONE, //TOK_QUOTED_STRING
    VALUE_NONE, //TOK_FUNC
    VALUE_NONE, //TOK_FUNC_IN
    VALUE_NONE, //TOK_COMMA
    VALUE_INT32, //TOK_AND
    VALUE_INT32, //TOK_OR
    VALUE_INT64, //TOK_BIT_AND
    VALUE_INT64, //TOK_BIT_OR
    VALUE_INT32, //TOK_NOT
    VALUE_INT32, //TOK_EQ
    VALUE_INT32, //TOK_LE
    VALUE_INT32, //TOK_GR
    VALUE_INT32, //TOK_LTE
    VALUE_INT32, //TOK_GTE
    VALUE_INT32, //TOK_NE
    VALUE_NONE, //TOK_NEG
    VALUE_NONE, //TOK_PLUS
    VALUE_NONE, //TOK_MINUS
    VALUE_NONE, //TOK_MUL
    VALUE_FLOAT, //TOK_DIV
    VALUE_NONE, //TOK_CONST_LIST
    VALUE_NONE, //TOK_ATTR
};

ExprParser::RetAndArgType ExprParser::RET_AND_ARG_TYPES[VALUE_TYPE_MAX][VALUE_TYPE_MAX];

ExprParser::Inializer::Inializer()
{
#define DEFINE_RET_AND_ARG_TYPE(_leftType, _rightType, _retType, _argType) \
    ExprParser::RET_AND_ARG_TYPES[_leftType][_rightType].retType = _retType; \
    ExprParser::RET_AND_ARG_TYPES[_leftType][_rightType].argType = _argType

    DEFINE_RET_AND_ARG_TYPE(VALUE_INT32, VALUE_INT32, VALUE_INT32, VALUE_INT32);
    DEFINE_RET_AND_ARG_TYPE(VALUE_INT32, VALUE_INT64, VALUE_INT64, VALUE_INT64);
    DEFINE_RET_AND_ARG_TYPE(VALUE_INT32, VALUE_FLOAT, VALUE_FLOAT, VALUE_FLOAT);
    DEFINE_RET_AND_ARG_TYPE(VALUE_INT32, VALUE_DOUBLE, VALUE_DOUBLE, VALUE_DOUBLE);

    DEFINE_RET_AND_ARG_TYPE(VALUE_INT64, VALUE_INT32, VALUE_INT64, VALUE_INT64);
    DEFINE_RET_AND_ARG_TYPE(VALUE_INT64, VALUE_INT64, VALUE_INT64, VALUE_INT64);
    DEFINE_RET_AND_ARG_TYPE(VALUE_INT64, VALUE_FLOAT, VALUE_FLOAT, VALUE_FLOAT);
    DEFINE_RET_AND_ARG_TYPE(VALUE_INT64, VALUE_DOUBLE, VALUE_DOUBLE, VALUE_DOUBLE);

    DEFINE_RET_AND_ARG_TYPE(VALUE_FLOAT, VALUE_INT32, VALUE_FLOAT, VALUE_FLOAT);
    DEFINE_RET_AND_ARG_TYPE(VALUE_FLOAT, VALUE_INT64, VALUE_FLOAT, VALUE_FLOAT);
    DEFINE_RET_AND_ARG_TYPE(VALUE_FLOAT, VALUE_FLOAT, VALUE_FLOAT, VALUE_FLOAT);
    DEFINE_RET_AND_ARG_TYPE(VALUE_FLOAT, VALUE_DOUBLE, VALUE_DOUBLE, VALUE_DOUBLE);

    DEFINE_RET_AND_ARG_TYPE(VALUE_DOUBLE, VALUE_INT32, VALUE_DOUBLE, VALUE_DOUBLE);
    DEFINE_RET_AND_ARG_TYPE(VALUE_DOUBLE, VALUE_INT64, VALUE_DOUBLE, VALUE_DOUBLE);
    DEFINE_RET_AND_ARG_TYPE(VALUE_DOUBLE, VALUE_FLOAT, VALUE_DOUBLE, VALUE_DOUBLE);
    DEFINE_RET_AND_ARG_TYPE(VALUE_DOUBLE, VALUE_DOUBLE, VALUE_DOUBLE, VALUE_DOUBLE);

    DEFINE_RET_AND_ARG_TYPE(VALUE_INT32, VALUE_NONE, VALUE_INT32, VALUE_INT32);
    DEFINE_RET_AND_ARG_TYPE(VALUE_INT64, VALUE_NONE, VALUE_INT64, VALUE_INT64);
    DEFINE_RET_AND_ARG_TYPE(VALUE_FLOAT, VALUE_NONE, VALUE_FLOAT, VALUE_FLOAT);
    DEFINE_RET_AND_ARG_TYPE(VALUE_DOUBLE, VALUE_NONE, VALUE_DOUBLE, VALUE_DOUBLE);

#undef DEFINE_RET_AND_ARG_TYPE
}

//////////////////////////////////////////
//
ExprParser::ExprParser(const IndexReaderPtr& pIndexReader)
    : m_pIndexReader(pIndexReader)
    , m_iRootNode(-1)
    , m_bTraceScanning(false)
    , m_bTraceParsing(false)
{
    m_nodes.reserve(1024);

    initStaticData();
}

ExprParser::~ExprParser() 
{
}

ExprEvaluatorPtr ExprParser::parse(std::istream& input, 
                                   ExprEvaluatorBuilder* pExprBuilder)
{
    ostringstream oss;
    ExprLexer lexer(&input, &oss);
    lexer.set_debug(m_bTraceScanning);

    ExprBisonParser parser(lexer, *this);
    parser.set_debug_level(m_bTraceParsing);
    if (parser.parse() != 0)
    {
        return ExprEvaluatorPtr();
    }

    FX_TRACE("Before optimize: [%s]", toString().c_str());
    ExprOptimizer optimizer(m_nodes);
    optimizer.optimize(m_iRootNode);
    FX_TRACE("After optimize: [%s]", toString().c_str());
    if (pExprBuilder)
    {
        return pExprBuilder->createExpr(m_nodes[m_iRootNode]);
    }
    else 
    {
        DefaultExprEvaluatorBuilder builder(*this, m_pIndexReader);
        return builder.createExpr(m_nodes[m_iRootNode]);
    }
}

ExprEvaluatorPtr ExprParser::parse(const std::string& input, 
                                   ExprEvaluatorBuilder* pExprBuilder)
{
    istringstream iss(input);
    return parse(iss, pExprBuilder);
}

int32_t ExprParser::addIntNode(int64_t iValue)
{
    FX_TRACE("Add Int node: %lld", iValue);
    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);

    ExprNode& node = m_nodes[size];
    node.tokenType = ExprNode::TOK_INT;
    node.retType = ExprNode::getIntType(iValue);
    node.intVal = iValue;
    return (int32_t)size;
}

int32_t ExprParser::addFloatNode(double fValue)
{
    FX_TRACE("Add Float node: %.2f", fValue);
    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);

    ExprNode& node = m_nodes[size];
    node.tokenType = ExprNode::TOK_DOUBLE;
    node.retType = VALUE_FLOAT;
    node.doubleVal = fValue;
    return (int32_t)size;
}

int32_t ExprParser::addAttrNode(const std::string& sAttrName)
{
    if (m_pIndexReader.isNull())
    {
        return -1;
    }
    const DocumentSchema* pSchema = m_pIndexReader->getDocSchema();
    FIRTEX_ASSERT2(pSchema != NULL);
    
    const FieldSchema* pFieldSchema = pSchema->getSchema(sAttrName);
    if (!pFieldSchema || !(pFieldSchema->hasForwardIndex()))
    {
        FX_LOG(ERROR, "No forward index built on field: [%s].", sAttrName.c_str());
        return -1;
    }

    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);
    ExprNode& node = m_nodes[size];
    node.tokenType = ExprNode::TOK_ATTR;
    node.attrId = pFieldSchema->getId();

    switch(pFieldSchema->getValueType())
    {
    case FieldType::INT32:
        node.retType = node.argType = VALUE_INT32;
        break;
    case FieldType::UINT32:
        node.retType = VALUE_INT32;
        node.argType = VALUE_UINT32;
        break;
    case FieldType::INT64:
        node.retType = node.argType = VALUE_INT64;
        break;
    case FieldType::UINT64:
        node.retType = VALUE_INT64;
        node.argType = VALUE_UINT64;
        break;
    case FieldType::FLOAT:
        node.retType = node.argType = VALUE_FLOAT;
        break;
    case FieldType::DOUBLE:
        node.retType = node.argType = VALUE_DOUBLE;
        break;
    default:
        FX_LOG(ERROR, "Unsupport field type: [%s].", sAttrName.c_str());
        m_nodes.resize(size);
        return -1;
        break;
    }

    return (int32_t)size;
}

int32_t ExprParser::addOpNode(TokenType iOp, int32_t iLeft, int32_t iRight)
{
    FX_TRACE("Add op node: op: [%d], left: [%d], right: [%d]",
             iOp, iLeft, iRight);
    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);
    ExprNode& node = m_nodes[size];
    node.tokenType = iOp;

    ValueType typeOfLeft = (iLeft == -1) ? VALUE_NONE 
                           : m_nodes[iLeft].retType;
    ValueType typeOfRight = (iRight == -1) ? VALUE_NONE 
                            : m_nodes[iRight].retType;

    FX_TRACE("Add op node: op[%d], leftType: [%d], rightType: [%d]",
             iOp, typeOfLeft, typeOfRight);

    node.retType = RET_TYPES[iOp];
    if (node.retType == VALUE_NONE)
    {
        node.retType = RET_AND_ARG_TYPES[typeOfLeft][typeOfRight].retType;
    }
    node.argType = RET_AND_ARG_TYPES[typeOfLeft][typeOfRight].argType;

    if (node.retType == VALUE_NONE)
    {
        node.retType = node.argType;
    }

    node.argCount = 0;
    if (iOp == ExprNode::TOK_COMMA)
    {
        if (iLeft>=0)
        {
            node.argCount += (m_nodes[iLeft].tokenType == ExprNode::TOK_COMMA) ?
                             m_nodes[iLeft].argCount : 1;
        }
        if (iRight >= 0)
        {
            node.argCount += (m_nodes[iRight].tokenType == ExprNode::TOK_COMMA) ?
                             m_nodes[iRight].argCount : 1;
        }
    }
    node.leftNodeIdx = iLeft;
    node.rightNodeIdx = iRight;
    
    FX_TRACE("Add expr node: op: [%d], retType: [%d], argType: [%d]",
             iOp, node.retType, node.argType);

    return (int32_t)size;
}

int32_t ExprParser::addFeatureNode(const string& sFuncName, 
                                   int32_t iLeft, int32_t iRight)
{
    int32_t idx = getFuncIdx(sFuncName);
    if (idx == -1)
    {
        FX_LOG(ERROR, "Function: [%s] is not supported", sFuncName.c_str());
        return -1;
    }
    return addFuncNode(idx, iLeft, iRight, ExprNode::TOK_FEATURE);
}

int32_t ExprParser::addFuncNode(int32_t iFunc, int32_t iLeft, int32_t iRight, 
                                ExprNode::TokenType tokenType)
{
    FX_TRACE("Add func node: funcIdx[%d], left[%d], right[%d]", iFunc, iLeft, iRight);

    FIRTEX_ASSERT2(iFunc >= 0 && iFunc < (int32_t)MAX_FUNCTIONS);

    if (iRight < 0)
    {
        int32_t iExpectedArgc = FUNCTIONS[iFunc].argCount;
        int32_t iArgc = 0;
        if (iLeft >=0)
        {
            iArgc = (m_nodes[iLeft].tokenType == ExprNode::TOK_COMMA) 
                    ? m_nodes[iLeft].argCount : 1;
        }
        if (iExpectedArgc < 0)
        {
            if (iArgc < -iExpectedArgc)
            {
                FX_LOG(ERROR, "Function [%s()] called with [%d] arguments, "
                       "at least [%d] arguments expected", 
                       FUNCTIONS[iFunc].name, iArgc, -iExpectedArgc);
                return -1;
            }
        } 
        else if (iArgc != iExpectedArgc)
        {
            FX_LOG(ERROR, "Function [%s()] called with [%d] arguments, "
                   "[%d] arguments expected", FUNCTIONS[iFunc].name, 
                   iArgc, iExpectedArgc);
            return -1;
        }
    }

    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);
    ExprNode & node = m_nodes[size];
    node.tokenType = tokenType;
    node.funcIdx = iFunc;
    node.leftNodeIdx = iLeft;
    node.rightNodeIdx = iRight;

    ValueType typeOfLeft = (iLeft == -1) ? VALUE_NONE 
                           : m_nodes[iLeft].retType;
    ValueType typeOfRight = (iRight == -1) ? VALUE_NONE 
                            : m_nodes[iRight].retType;
    FX_TRACE("Add func node: funcIdx[%d], leftType: [%d], rightType: [%d]",
             iFunc, typeOfLeft, typeOfRight);

    // deduce type
    node.argType = RET_AND_ARG_TYPES[typeOfLeft][typeOfRight].argType;
    node.retType = FUNCTIONS[iFunc].retType;
    if (node.retType == VALUE_NONE)
    {
        node.retType = RET_AND_ARG_TYPES[typeOfLeft][typeOfRight].retType;
    }

    FX_TRACE("Add expr node: func: [%d], retType: [%d], argType: [%d]",
             iFunc, node.retType, node.argType);
    return (int32_t)size;
}

int32_t ExprParser::addDateFuncNode(int32_t iFunc, const string& sDateStr)
{
    FX_TRACE("Add func node: funcIdx[%d], date: [%s]", iFunc, sDateStr.c_str());
    FIRTEX_ASSERT2(iFunc >= 0 && iFunc < (int32_t)MAX_FUNCTIONS);
    
    int64_t iDateVal = DateTimeAnalyzer::parse(sDateStr);
    if (iDateVal <= 0)
    {
        FX_LOG(ERROR, "Invalid date format: [%s]", sDateStr.c_str());
        return -1;
    }
    
    int32_t iLeft = addIntNode(iDateVal);
    return addFuncNode(iFunc, iLeft);    
}

int32_t ExprParser::addConstListNode(int64_t iValue)
{
    FX_TRACE("Add const list node: %lld", iValue);
    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);

    ExprNode& node = m_nodes[size];
    node.tokenType = ExprNode::TOK_CONST_LIST;
    node.constList = new ConstList();
    node.constList->add(iValue);

    return (int32_t)size;
}

int32_t ExprParser::addConstListNode(double dbValue)
{
    FX_TRACE("Add const list node: %f", dbValue);
    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);

    ExprNode& node = m_nodes[size];
    node.tokenType = ExprNode::TOK_CONST_LIST;
    node.constList = new ConstList();
    node.constList->add(dbValue);

    return (int32_t)size;
}

int32_t ExprParser::addConstListNode(const string& strValue)
{
    FX_TRACE("Add const list node: %s", strValue.c_str());
    size_t size = m_nodes.size();
    m_nodes.resize(size + 1);

    ExprNode& node = m_nodes[size];
    node.tokenType = ExprNode::TOK_CONST_LIST;
    node.constList = new ConstList();
    node.constList->add(strValue);

    return (int32_t)size;
}

void ExprParser::appendToConstList(int32_t iNode, int64_t iValue)
{
    FX_TRACE("Append to const list: node[%d], value[%lld]", iNode, iValue);
    m_nodes[iNode].constList->add(iValue);
}

void ExprParser::appendToConstList(int32_t iNode, double dbValue)
{
    FX_TRACE("Append to const list: node[%d], value[%f]", iNode, dbValue);
    m_nodes[iNode].constList->add(dbValue);    
}

void ExprParser::appendToConstList(int32_t iNode, const string& strValue)
{
    FX_TRACE("Append to const list: node[%d], value[%s]", 
             iNode, strValue.c_str());
    m_nodes[iNode].constList->add(strValue);
}

void ExprParser::fail(const class location& l, const std::string& m)
{
    std::stringstream ss;
    ss << l << ": " << m;
    m_sErrorMsg = ss.str();
}

string ExprParser::toString() const
{
    if (m_iRootNode)
    {
        return ExprParser::toString(m_iRootNode, m_nodes);
    }
    return "";
}

//static 
string ExprParser::toString(int32_t iNodeIdx, const NodeVector& nodes)
{
    stringstream ss;
    toString(iNodeIdx, nodes, ss);
    return ss.str();
}

//static 
void ExprParser::toString(int32_t iNodeIdx,
                          const NodeVector& nodes,
                          std::stringstream& ss)
{
    const ExprNode& exprNode = nodes[iNodeIdx];
    toString(exprNode.tokenType, ss);
    if (exprNode.tokenType == ExprNode::TOK_FUNC)
    {
        ss << getFunc(exprNode.funcIdx).name;
    }
    ss << "(";
    if (exprNode.leftNodeIdx >= 0)
    {
        toString(exprNode.leftNodeIdx, nodes, ss);
    }
    if (exprNode.rightNodeIdx >= 0)
    {
        ss << ", ";
        toString(exprNode.rightNodeIdx, nodes, ss);
    }
    switch(exprNode.tokenType)
    {
    case ExprNode::TOK_INT:
        ss << exprNode.intVal;
        break;
    case ExprNode::TOK_DOUBLE:
        ss << exprNode.doubleVal;
        break;
    case ExprNode::TOK_CONST_LIST:
        ss << exprNode.constList->toString();
        break;
    case ExprNode::TOK_ATTR:
        ss << exprNode.attrId;
        break;
    default:
//        ss << "unknow";
        break;
    }
    
    ss << ")";
}

//static
void ExprParser::toString(ExprNode::TokenType type, std::stringstream& ss)
{
    switch(type)
    {
    case ExprNode::TOK_NONE:
        ss << "NONE";
        break;
    case ExprNode::TOK_INT:
        ss << "INT";
        break;
    case ExprNode::TOK_DOUBLE:
        ss << "DOUBLE";
        break;
    case ExprNode::TOK_IDENT:
        ss << "IDENT";
        break;
    case ExprNode::TOK_QUOTED_STRING:
        ss << "STRING";
        break;
    case ExprNode::TOK_FUNC:
        break;
    case ExprNode::TOK_FUNC_IN:
        ss << "IN";
        break;
    case ExprNode::TOK_COMMA:
        ss << "COMMA";
        break;
    case ExprNode::TOK_AND:
        ss << "AND";
        break;
    case ExprNode::TOK_OR:
        ss << "OR";
        break;
    case ExprNode::TOK_BIT_AND:
        ss << "AND";
        break;
    case ExprNode::TOK_BIT_OR:
        ss << "OR";
        break;
    case ExprNode::TOK_NOT:
        ss << "NOT";
        break;
    case ExprNode::TOK_EQ:
        ss << "EQ";
        break;
    case ExprNode::TOK_LE:
        ss << "LE";
        break;
    case ExprNode::TOK_GR:
        ss << "GR";
        break;
    case ExprNode::TOK_LTE:
        ss << "LTE";
        break;
    case ExprNode::TOK_GTE:
        ss << "GTE";
        break;
    case ExprNode::TOK_NE:
        ss << "NE";
        break;
    case ExprNode::TOK_NEG:
        ss << "NEG";
        break;
    case ExprNode::TOK_PLUS:
        ss << "PLUS";
        break;
    case ExprNode::TOK_MINUS:
        ss << "MINUS";
        break;
    case ExprNode::TOK_MUL:
        ss << "MUL";
        break;
    case ExprNode::TOK_DIV:
        ss << "DIV";
        break;
    case ExprNode::TOK_CONST_LIST:
        ss << "CONST_LIST";
        break;
    case ExprNode::TOK_ATTR:
        ss << "ATTR";
        break;
    default:
        ss << "UNKNOW";
        break;
    }
}

FX_NS_END

