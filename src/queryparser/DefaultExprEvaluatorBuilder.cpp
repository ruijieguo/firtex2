#include "firtex/queryparser/DefaultExprEvaluatorBuilder.h"
#include "firtex/queryparser/ExprParser.h"
#include "firtex/search/ArgListExprEvaluator.h"
#include "firtex/search/ConstExprEvaluator.h"
#include "firtex/search/BinaryExprEvaluator.h"
#include "firtex/search/UnaryExprEvaluator.h"
#include "firtex/search/TernaryExprEvaluator.h"
#include "firtex/search/AttrExprEvaluator.h"
#include "firtex/search/InExprEvaluator.h"
#include "firtex/index/IndexReader.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexIterator.h"

FX_NS_USE(search);
FX_NS_USE(index);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, DefaultExprEvaluatorBuilder);

DefaultExprEvaluatorBuilder::DefaultExprEvaluatorBuilder(ExprParser& parser,
        const IndexReaderPtr& pIndexReader) 
    : m_parser(parser)
    , m_pIndexReader(pIndexReader)
{
}

DefaultExprEvaluatorBuilder::~DefaultExprEvaluatorBuilder() 
{
}

ExprEvaluator* DefaultExprEvaluatorBuilder::createExpr(const ExprNode& exprNode)
{
#define CASE_CREATE_EVALUATOR(prefix)                                   \
    if (exprNode.argType == ExprNode::VALUE_INT32) return new prefix##Int32ExprEvaluator(pLeft, pRight); \
    else if (exprNode.argType == ExprNode::VALUE_INT64) return new prefix##Int64ExprEvaluator(pLeft, pRight); \
    else return new prefix##FloatExprEvaluator(pLeft, pRight);

    bool bSkipLeft = false;
    bool bSkipRight = false;
    if (exprNode.tokenType == ExprNode::TOK_FUNC)
    {
        ExprParser::FuncType eFunc = ExprParser::getFunc(exprNode.funcIdx).funcType;
        if (eFunc == ExprParser::FUNC_GEODIST || eFunc == ExprParser::FUNC_IN)
        {
            bSkipLeft = true;
        }
        if (eFunc == ExprParser::FUNC_IN)
        {
            bSkipRight = true;
        }
    }

    ExprEvaluator* pLeft = NULL;
    if (!bSkipLeft && exprNode.leftNodeIdx != -1)
    {
        pLeft = createExpr(m_parser.getExprNode(exprNode.leftNodeIdx));
    }
    
    ExprEvaluator* pRight = NULL;
    if (!bSkipRight && exprNode.rightNodeIdx != -1)
    {
        pRight = createExpr(m_parser.getExprNode(exprNode.rightNodeIdx));
    }

    switch (exprNode.tokenType)
    {
    case ExprNode::TOK_ATTR:
        return createAttrEvaluator(exprNode); break;

    case ExprNode::TOK_DOUBLE:
        if (exprNode.retType == VALUE_FLOAT)
        {
            return new ConstFloatExprEvaluator((float)exprNode.doubleVal);
        }
        else 
        {
            return new ConstDoubleExprEvaluator(exprNode.doubleVal);
        }
        break;

    case ExprNode::TOK_INT:
        if (exprNode.retType == VALUE_INT32)
        {
            return new ConstInt32ExprEvaluator((int32_t)exprNode.intVal);
        }
        else if (exprNode.retType == VALUE_INT64)
        {
            return new ConstInt64ExprEvaluator(exprNode.intVal);
        }
        else
        {
            return new ConstFloatExprEvaluator(float(exprNode.intVal));
        }
        break;

    case ExprNode::TOK_PLUS:
        return new AddExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_MINUS:
        return new SubExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_MUL:
        return new MulExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_DIV:
        return new DivExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_BIT_AND:
        return new BitAndExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_BIT_OR:
        return new BitOrExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_LE:
        return new LTExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_GR:
        return new GTExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_LTE:
        return new LTEExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_GTE:
        return new GTEExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_EQ:
        return new EQExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_NE:
        return new NEExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_AND:
        return new AndExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_OR:
        return new OrExprEvaluator(exprNode.retType, pLeft, pRight); break;
    case ExprNode::TOK_NOT:
        return new NotExprEvaluator(exprNode.retType, pLeft); break;

    case ExprNode::TOK_COMMA:
        return new ArgListExprEvaluator(pLeft, pRight); break;
    case ExprNode::TOK_NEG:
        FIRTEX_ASSERT2(pRight == NULL);
        return new NegExprEvaluator(exprNode.retType, pLeft); break;

    case ExprNode::TOK_FUNC:
        return createFuncExprEvaluator(exprNode, pLeft); break;

    default:
        FIRTEX_ASSERT2(0);//unhandled token type
        break;
    }

    delete pLeft;
    delete pRight;

    return NULL;
}

ExprEvaluator* DefaultExprEvaluatorBuilder::createFuncExprEvaluator(
        const ExprNode& exprNode, ExprEvaluator* pLeftEva)
{
    ArgListExprEvaluator::ArgVector args;
    if (pLeftEva)
    {
        swapArglist(pLeftEva, args);
    }

    ExprParser::FuncType eFunc =
        ExprParser::getFunc(exprNode.funcIdx).funcType;
    switch (eFunc)
    {
    case ExprParser::FUNC_DATE: //optimized, should not be here
        FIRTEX_ASSERT2(false); break; 

    case ExprParser::FUNC_NOW: //optimized, should not be here
        FIRTEX_ASSERT2(false); break; 

    case ExprParser::FUNC_ABS:
        return new AbsExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_CEIL:
        return new CeilExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_FLOOR:
        return new FloorExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_SIN:
        return new SinExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_COS:
        return new CosExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_LN:
        return new LnExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_LOG2:
        return new Log2ExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_LOG10:
        return new Log10ExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_EXP:
        return new ExpExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_SQRT:
        return new SqrtExprEvaluator(exprNode.retType, args[0]);
    case ExprParser::FUNC_INT64:
        return args[0];
    case ExprParser::FUNC_INT32:
        return new Int32ExprEvaluator(exprNode.retType, args[0]);

    case ExprParser::FUNC_MIN:
        return new MinExprEvaluator(exprNode.retType, args[0], args[1]);
    case ExprParser::FUNC_MAX:
        return new MaxExprEvaluator(exprNode.retType, args[0], args[1]);
    case ExprParser::FUNC_POW:
        return new PowExprEvaluator(exprNode.retType, args[0], args[1]);
    case ExprParser::FUNC_IDIV:
        return new IDivExprEvaluator(exprNode.retType, args[0], args[1]);

    case ExprParser::FUNC_IF:
        return new IfExprEvaluator(exprNode.retType, args[0], args[1], args[2]);
    case ExprParser::FUNC_MADD:
        return new MAddExprEvaluator(exprNode.retType, args[0], args[1], args[2]);
    case ExprParser::FUNC_MUL3:
        return new Mul3ExprEvaluator(exprNode.retType, args[0], args[1], args[2]); 
    case ExprParser::FUNC_IN:
        return createInNode(exprNode);
    default:
        FIRTEX_ASSERT2(false); break;
    }
        
    return NULL;

#undef CASE_CREATE_EVALUATOR
}

//static 
void DefaultExprEvaluatorBuilder::swapArglist(ExprEvaluator* pLeftEva, 
        ArgListExprEvaluator::ArgVector& args)
{
    if (!pLeftEva || !pLeftEva->isArgList() )
    {
        args.push_back(pLeftEva);
        return;
    }

    ArgListExprEvaluator* pArgEva =
        dynamic_cast<ArgListExprEvaluator*>(pLeftEva);
    FIRTEX_ASSERT2(pArgEva);
    swap(args, pArgEva->getArgList());

    delete pLeftEva;
}

ExprEvaluator* DefaultExprEvaluatorBuilder::createAttrEvaluator(
        const ExprNode& exprNode)
{
    if (m_pIndexReader.isNull())
    {
        return NULL;
    }
    ForwardIndexIteratorPtr pAttrIter =
        m_pIndexReader->forwardIndexReader(exprNode.attrId);
    if (pAttrIter.isNull())
    {
        FX_LOG(ERROR, "Create forward index iterator for field: [%d] FAILED",
               (int32_t)exprNode.attrId);
        return NULL;
    }
    
    switch(exprNode.retType)
    {
    case VALUE_INT32:
    {
        if (exprNode.argType == VALUE_INT32)
        {
            Int32ForwardIndexIteratorPtr pInt32AttrIt = 
                pAttrIter.cast<Int32ForwardIndexIterator>();
            FIRTEX_ASSERT2(pInt32AttrIt.isNotNull());
            return new AttrExprEvaluator<int32_t>(pInt32AttrIt);
        }
        else if (exprNode.argType == VALUE_UINT32)
        {
            UInt32ForwardIndexIteratorPtr pUInt32AttrIt = 
                pAttrIter.cast<UInt32ForwardIndexIterator>();
            FIRTEX_ASSERT2(pUInt32AttrIt.isNotNull());
            return new AttrExprEvaluator<int32_t,
                UInt32ToInt32AttrExprEvaluatorCastTraits<int32_t> >(pUInt32AttrIt);
        }
        break;
    }

    case VALUE_INT64:
    {
        if (exprNode.argType == VALUE_INT64)
        {
            Int64ForwardIndexIteratorPtr pInt64AttrIt = 
                pAttrIter.cast<Int64ForwardIndexIterator>();
            FIRTEX_ASSERT2(pInt64AttrIt.isNotNull());
            return new AttrExprEvaluator<int64_t>(pInt64AttrIt);
        }
        else if (exprNode.argType == VALUE_UINT64)
        {
            UInt64ForwardIndexIteratorPtr pUInt64AttrIt = 
                pAttrIter.cast<UInt64ForwardIndexIterator>();
            FIRTEX_ASSERT2(pUInt64AttrIt.isNotNull());
            return new AttrExprEvaluator<int64_t,
                UInt64ToInt64AttrExprEvaluatorCastTraits<int64_t> >(pUInt64AttrIt);
        }
        break;
    }

    case VALUE_FLOAT:
    {
        FloatForwardIndexIteratorPtr pFloatAttrIt = 
            pAttrIter.cast<FloatForwardIndexIterator>();
        FIRTEX_ASSERT2(pFloatAttrIt.isNotNull());
        return new AttrExprEvaluator<float>(pFloatAttrIt);
        break;
    }
    case VALUE_DOUBLE:
    {
        DoubleForwardIndexIteratorPtr pDoubleAttrIt = 
            pAttrIter.cast<DoubleForwardIndexIterator>();
        FIRTEX_ASSERT2(pDoubleAttrIt.isNotNull());
        return new AttrExprEvaluator<double>(pDoubleAttrIt);
        break;
    }
    default:
        FIRTEX_ASSERT2(false); break;
    }
    
    return NULL;
}

ExprEvaluator* DefaultExprEvaluatorBuilder::createInNode(
        const ExprNode& exprNode)
{
    const ExprNode& rightNode = m_parser.getExprNode(exprNode.rightNodeIdx);
    if (rightNode.tokenType != ExprNode::TOK_CONST_LIST)
    {
        FX_LOG(ERROR, "IN() arguments must be constants except the first onde");
        return NULL;
    }
    const ConstList* pConstList = rightNode.constList;
    ExprEvaluator* pLeftEval = createExpr(
            m_parser.getExprNode(exprNode.leftNodeIdx));
    if (!pLeftEval)
    {
        return NULL;
    }
    switch(pConstList->getRetType())
    {
    case VALUE_INT32:
        return new Int32InExprEvaluator(pLeftEval, pConstList); break;
    case VALUE_INT64:
        return new Int64InExprEvaluator(pLeftEval, pConstList); break;
    case VALUE_FLOAT:
        return new FloatInExprEvaluator(pLeftEval, pConstList); break;
    case VALUE_DOUBLE:
        return new DoubleInExprEvaluator(pLeftEval, pConstList); break;
    default:
        FIRTEX_ASSERT2(false);
        break;
    }
    return NULL;
}

FX_NS_END

