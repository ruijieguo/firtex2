//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-01-04 22:35:42

#ifndef __FX_EXPROPTIMIZER_H
#define __FX_EXPROPTIMIZER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/queryparser/ExprNode.h"
#include <vector>

FX_NS_DEF(queryparser);

class ExprOptimizer
{
public:
    typedef std::vector<ExprNode> NodeVector;

public:
    ExprOptimizer(NodeVector& nodes);
    ~ExprOptimizer();

public:
    /**
     * Optimize expression node
     * @param iNodeIdx nodex index in node array
     */
    void optimize(int32_t iNodeIdx);

protected:
    bool optimizeArithmetic(ExprNode* pRoot,
                            ExprNode* pLeft,
                            ExprNode* pRight);

protected:
    static bool isAddSub(const ExprNode& node);
    static bool isArithmetic(const ExprNode& node);
    static bool isConst(const ExprNode& node);
    static double doubleVal(const ExprNode& node);
    template<typename T>
    static void swap(T& left, T& right);    

private:
    NodeVector& m_nodes;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////////////
//
//static
inline bool ExprOptimizer::isAddSub(const ExprNode& node)
{
    return (node.tokenType == ExprNode::TOK_PLUS 
            || node.tokenType == ExprNode::TOK_MINUS);
}

//static 
inline bool ExprOptimizer::isArithmetic(const ExprNode& node)
{
    ExprNode::TokenType tokenType = node.tokenType;
    return (tokenType == ExprNode::TOK_PLUS 
            || tokenType == ExprNode::TOK_MINUS 
            || tokenType == ExprNode::TOK_MUL 
            || tokenType == ExprNode::TOK_DIV);
}

//static 
inline bool ExprOptimizer::isConst(const ExprNode& node)
{
    return (node.tokenType == ExprNode::TOK_INT 
            || node.tokenType == ExprNode::TOK_DOUBLE);
}

//static 
inline double ExprOptimizer::doubleVal(const ExprNode& node)
{
    FIRTEX_ASSERT2(isConst(node));
    return (node.tokenType == ExprNode::TOK_INT ?
            (double)node.intVal : node.doubleVal);
}
        
//static 
template<typename T>
inline void ExprOptimizer::swap(T& left, T& right)
{
    T tmp = left;
    left = right;
    right = tmp;
}

FX_NS_END

#endif //__FX_EXPROPTIMIZER_H
