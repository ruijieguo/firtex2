#include "firtex/queryparser/ExprOptimizer.h"
#include "firtex/queryparser/ExprParser.h"
#include "firtex/search/UnaryExprEvaluator.h"
#include "firtex/utility/Timestamp.h"
#include <math.h>

FX_NS_USE(utility);

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, ExprOptimizer);

ExprOptimizer::ExprOptimizer(NodeVector& nodes) 
    : m_nodes(nodes)
{
}

ExprOptimizer::~ExprOptimizer() 
{
}

void ExprOptimizer::optimize(int32_t iNodeIdx)
{
    FIRTEX_ASSERT2(iNodeIdx >= 0);

    FX_TRACE("Optimize: [%s]", ExprParser::toString(iNodeIdx, m_nodes).c_str());

    if (m_nodes[iNodeIdx].leftNodeIdx >= 0)
    {
        optimize(m_nodes[iNodeIdx].leftNodeIdx);
    }
    if (m_nodes[iNodeIdx].rightNodeIdx >= 0)
    {
        optimize(m_nodes[iNodeIdx].rightNodeIdx);
    }

    ExprNode* pRoot = &m_nodes[iNodeIdx];
    ExprNode* pLeft = (pRoot->leftNodeIdx >= 0) ? &m_nodes[pRoot->leftNodeIdx] : NULL;
    ExprNode* pRight = (pRoot->rightNodeIdx >= 0) ? &m_nodes[pRoot->rightNodeIdx] : NULL;

    // arithmetic expression with constants
    if (isArithmetic(*pRoot))
    {
        if (optimizeArithmetic(pRoot, pLeft, pRight))
        {
            FX_TRACE("Optimized: [%s]", ExprParser::toString(iNodeIdx, m_nodes).c_str());
            return;
        }
    }

    // madd, mul3
    // if ((pRoot->tokenType == ExprNode::TOK_PLUS || pRoot->tokenType == ExprNode::TOK_MUL)
    //     && (pLeft->tokenType == ExprNode::TOK_MUL || pRight->tokenType == ExprNode::TOK_MUL))
    // {
    //     if (pLeft->tokenType != ExprNode::TOK_MUL)
    //     {
    //         swap(pRoot->leftNodeIdx, pRoot->rightNodeIdx);
    //         swap(*pLeft, *pRight);
    //     }

    //     pLeft->tokenType = ExprNode::TOK_COMMA;

    //     int32_t iLeft = pRoot->leftNodeIdx;
    //     int32_t iRight = pRoot->rightNodeIdx;

    //     pRoot->funcIdx = (pRoot->tokenType == ExprNode::TOK_PLUS) ?
    //                      ExprParser::FUNC_MADD : ExprParser::FUNC_MUL3;
    //     pRoot->tokenType = ExprNode::TOK_FUNC;
    //     size_t size = m_nodes.size();
    //     pRoot->leftNodeIdx = size;
    //     pRoot->rightNodeIdx = -1;

    //     FIRTEX_ASSERT2(ExprParser::getFunc(pRoot->funcIdx).funcType == pRoot->funcIdx);
        
    //     // This may invalidate all pointers
    //     m_nodes.resize(size + 1);
    //     pRoot = &m_nodes[iNodeIdx];
    //     pLeft = (pRoot->leftNodeIdx >= 0) ?
    //             &m_nodes[pRoot->leftNodeIdx] : NULL;
    //     pRight = (pRoot->rightNodeIdx >= 0) ?
    //              &m_nodes[pRoot->rightNodeIdx] : NULL;
        
    //     ExprNode& argNode = m_nodes[size];
    //     argNode.tokenType = ExprNode::TOK_COMMA;
    //     argNode.leftNodeIdx = iLeft;
    //     argNode.rightNodeIdx = iRight;
    //     FX_TRACE("Optimized: [%s]", ExprParser::toString(iNodeIdx, m_nodes).c_str());
    //     return;
    // }

    // division by a constant optimize to  multiplication by inverse
    if (pRoot->tokenType == ExprNode::TOK_DIV 
        && pRight->tokenType == ExprNode::TOK_DOUBLE)
    {
        pRight->doubleVal = 1.0f / pRight->doubleVal;
        pRoot->tokenType = ExprNode::TOK_MUL;
        FX_TRACE("Optimized: [%s]", ExprParser::toString(iNodeIdx, m_nodes).c_str());
        return;
    }

    // optimize DATE('2011-12-28 21:19:01')
    if (pRoot->tokenType == ExprNode::TOK_FUNC 
        && ExprParser::getFunc(pRoot->funcIdx).funcType == ExprParser::FUNC_DATE)
    {
        FIRTEX_ASSERT2(pLeft);
        pRoot->tokenType = ExprNode::TOK_INT;
        pRoot->intVal = pLeft->intVal;
        return;
    }

    // unary function from a constant
    if (pRoot->tokenType == ExprNode::TOK_FUNC 
        && ExprParser::getFunc(pRoot->funcIdx).argCount == 1 && isConst(*pLeft))
    {
        double dbArg = pLeft->tokenType == ExprNode::TOK_DOUBLE ? 
                     pLeft->doubleVal : double(pLeft->intVal);
        switch (ExprParser::getFunc(pRoot->funcIdx).funcType)
        {
        case ExprParser::FUNC_ABS:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = fabs(dbArg);
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_CEIL:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(ceil(dbArg));
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_FLOOR:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(floor(dbArg));
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_SIN:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(sin(dbArg));
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_COS:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(cos(dbArg));
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_LN:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(log(dbArg));
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_LOG2:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(log(dbArg)*M_LOG2E);
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_LOG10:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(log(dbArg)*M_LOG10E);
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_EXP:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(exp(dbArg));
            pRoot->leftNodeIdx = -1;
            break;
        case ExprParser::FUNC_SQRT:
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
            pRoot->doubleVal = double(sqrt(dbArg));
            pRoot->leftNodeIdx = -1;
            break;
        default: 
            break;
        }
        FX_TRACE("Optimized: [%s]", ExprParser::toString(iNodeIdx, m_nodes).c_str());
        return;
    }

    // constant function (such as NOW())
    if (pRoot->tokenType == ExprNode::TOK_FUNC 
        && ExprParser::getFunc(pRoot->funcIdx).funcType == ExprParser::FUNC_NOW)
    {
        pRoot->tokenType = ExprNode::TOK_INT;
        Timestamp now;
        pRoot->intVal = (int64_t)now.time();
        FX_TRACE("Optimized: [%s]", ExprParser::toString(iNodeIdx, m_nodes).c_str());
        return;
    }

        
    FX_TRACE("Optimized: [%s]", ExprParser::toString(iNodeIdx, m_nodes).c_str());

    // case of INT32(int - attr)
    // if (pRoot->tokenType == ExprNode::TOK_FUNC
    //     && pRoot->funcIdx == ExprParser::FUNC_INT32
    //     && (pLeft->tokenType == ExprNode::TOK_ATTR_INT 
    //         || pLeft->tokenType == ExprNode::TOK_ATTR_BITS))
    // {
    //     pRoot->tokenType = ExprNode::TOK_ATTR_SINT;
    //     pRoot->m_tLocator = pLeft->m_tLocator;
    // }   
}

bool ExprOptimizer::optimizeArithmetic(ExprNode* pRoot,
                                       ExprNode* pLeft,
                                       ExprNode* pRight)
{
    // optimize (constant op constant)
    if (isConst(*pLeft) && isConst(*pRight))
    {
        //case of (int ['+ExprNode::TOK_DIV-ExprNode::TOK_DIV*'] int)
        if (pLeft->tokenType == ExprNode::TOK_INT 
            && pRight->tokenType == ExprNode::TOK_INT 
            && pRoot->tokenType  !=  ExprNode::TOK_DIV)
        {
            switch (pRoot->tokenType)
            {
            case ExprNode::TOK_PLUS: 
                pRoot->intVal = pLeft->intVal + pRight->intVal; 
                break;
            case ExprNode::TOK_MINUS:
                pRoot->intVal = pLeft->intVal - pRight->intVal;
                break;
            case ExprNode::TOK_MUL:
                pRoot->intVal = pLeft->intVal * pRight->intVal;
                break;
            default: FIRTEX_ASSERT2(false); break;
            }
            pRoot->tokenType = ExprNode::TOK_INT;
        }
        else
        {
            double dbLeft = doubleVal(*pLeft);
            double dbRight = doubleVal(*pRight);
            switch (pRoot->tokenType)
            {
            case ExprNode::TOK_PLUS: pRoot->doubleVal = dbLeft + dbRight; break;
            case ExprNode::TOK_MINUS: pRoot->doubleVal = dbLeft - dbRight; break;
            case ExprNode::TOK_MUL: pRoot->doubleVal = dbLeft * dbRight; break;
            case ExprNode::TOK_DIV: pRoot->doubleVal = dbLeft / dbRight; break;
            default: FIRTEX_ASSERT2(false); break;
            }
            pRoot->tokenType = ExprNode::TOK_DOUBLE;
        }
        pRoot->leftNodeIdx = pRoot->rightNodeIdx = -1;
        return true;
    }

    // case of (expr op const), move const to the left
    if (isConst(*pRight))
    {
        FIRTEX_ASSERT2(!isConst(*pLeft));
        swap (pRoot->leftNodeIdx, pRoot->rightNodeIdx);
        swap(*pLeft, *pRight);

        // case of (expr-const), optimize to ((-const)+expr)
        if (pRoot->tokenType == ExprNode::TOK_MINUS)
        {
            pRoot->tokenType = ExprNode::TOK_PLUS;
            if (pLeft->tokenType == ExprNode::TOK_INT)
            {
                pLeft->intVal *= -1;
            }
            else
            {
                pLeft->doubleVal *= -1;
            }
        }

        // case (expr/const), optimze to ((1/const)*expr)
        if (pRoot->tokenType == ExprNode::TOK_DIV)
        {
            pRoot->tokenType = ExprNode::TOK_MUL;
            pLeft->doubleVal = 1.0f / doubleVal(*pLeft);
            pLeft->tokenType = ExprNode::TOK_DOUBLE;
        }
    }

    // optimize compatible operations with constants
    if (isConst(*pLeft) && isArithmetic(*pRight) 
        && isAddSub(*pRoot) == isAddSub(*pRight) 
        && isConst(m_nodes[pRight->leftNodeIdx]))
    {
        ExprNode * pConst = &m_nodes[pRight->leftNodeIdx];
        ExprNode * pExpr = &m_nodes[pRight->rightNodeIdx];
        FIRTEX_ASSERT2(!isConst(*pExpr)); // must had been optimized

        // case of (left op (const op2 expr)),
        // optimize to ((left op const) op*op2 expr)
        if (isAddSub(*pRoot))
        {
            // fold consts
            int iSign = ((pRoot->tokenType == ExprNode::TOK_PLUS) ? 1 : -1);
            if (pLeft->tokenType == ExprNode::TOK_INT 
                && pConst->tokenType == ExprNode::TOK_INT)
            {
                pLeft->intVal += iSign*pConst->intVal;
            } 
            else
            {
                pLeft->doubleVal = doubleVal(*pLeft) + iSign * doubleVal(*pConst);
                pLeft->tokenType = ExprNode::TOK_DOUBLE;
            }

            // fold ops
            pRoot->tokenType = (pRoot->tokenType == pRight->tokenType) ?
                               ExprNode::TOK_PLUS : ExprNode::TOK_MINUS;

        } 
        else
        {
            // fols consts
            if (pRoot->tokenType == ExprNode::TOK_MUL 
                && pLeft->tokenType == ExprNode::TOK_INT 
                && pConst->tokenType == ExprNode::TOK_INT)
            {
                pLeft->intVal *= pConst->intVal;
            } 
            else
            {
                if (pRoot->tokenType == ExprNode::TOK_MUL)
                {
                    pLeft->doubleVal = doubleVal(*pLeft) * doubleVal(*pConst);
                }
                else
                {
                    pLeft->doubleVal = doubleVal(*pLeft) / doubleVal(*pConst);
                }
                pLeft->tokenType = ExprNode::TOK_DOUBLE;
            }

            // fold ops
            pRoot->tokenType = (pRoot->tokenType == pRight->tokenType) ?
                               ExprNode::TOK_MUL : ExprNode::TOK_DIV;
        }

        // promote expr arg
        pRoot->rightNodeIdx = pRight->rightNodeIdx;
        pRight = pExpr;
    }

    // promote children constants
    if (isArithmetic(*pLeft) && isAddSub(*pLeft) == isAddSub(*pRoot)
        && isConst (m_nodes[pLeft->leftNodeIdx]))
    {
        // case of ((const op lr) op2 right),
        // optimize to (const op (lr op2/op right))
        // constant gets promoted one level up
        int32_t iConst = pLeft->leftNodeIdx;
        pLeft->leftNodeIdx = pLeft->rightNodeIdx;
        pLeft->rightNodeIdx = pRoot->rightNodeIdx; // (c op lr) -> (lr ... r)

        switch (pLeft->tokenType)
        {
        case ExprNode::TOK_PLUS:
        case ExprNode::TOK_MUL:
            // (c + lr) op r -> c + (lr op r)
            // (c * lr) op r -> c * (lr op r)
            swap (pLeft->tokenType, pRoot->tokenType);
        break;

        case ExprNode::TOK_MINUS:
            // (c - lr) + r -> c - (lr - r)
            // (c - lr) - r -> c - (lr + r)
            pLeft->tokenType = (pRoot->tokenType == ExprNode::TOK_PLUS ?
                    ExprNode::TOK_MINUS : ExprNode::TOK_PLUS);
            pRoot->tokenType = ExprNode::TOK_MINUS;
            break;

        case ExprNode::TOK_DIV:
            // (c / lr) * r -> c * (r / lr)
            // (c / lr) / r -> c / (r * lr)
            swap(pLeft->leftNodeIdx, pLeft->rightNodeIdx);
            pLeft->tokenType = (pRoot->tokenType == ExprNode::TOK_MUL) ?
                               ExprNode::TOK_DIV : ExprNode::TOK_MUL;
            break;

        default:
            FIRTEX_ASSERT2(false); break; //unhandled op in left-const promotion
        }

        pRoot->rightNodeIdx = pRoot->leftNodeIdx;
        pRoot->leftNodeIdx = iConst;

        pLeft = &m_nodes[pRoot->leftNodeIdx];
        pRight = &m_nodes[pRoot->rightNodeIdx];
    }
    return false;
}


FX_NS_END

