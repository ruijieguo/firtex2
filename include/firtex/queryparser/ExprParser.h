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
// Created	: 2011-01-02 23:49:35

#ifndef __FX_EXPRPARSER_H
#define __FX_EXPRPARSER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include "firtex/queryparser/ExprNode.h"
#include "firtex/queryparser/ExprEvaluatorBuilder.h"
#include <sstream>

namespace fx_bison
{
class location;
class ExprBisonParser;
};

FX_NS_DEF(index);
class IndexReader;
DEFINE_TYPED_PTR(IndexReader);
FX_NS_END

FX_NS_DEF(queryparser);

class ExprParser
{
public:
    typedef ExprNode::TokenType TokenType;

    typedef std::vector<ExprNode> NodeVector;
    
public:
    //Supported functions
    enum FuncType
    {
	FUNC_DATE,
	FUNC_NOW,

	FUNC_ABS,
	FUNC_CEIL,
	FUNC_FLOOR,
	FUNC_SIN,
	FUNC_COS,
	FUNC_LN,
	FUNC_LOG2,
	FUNC_LOG10,
	FUNC_EXP,
	FUNC_SQRT,
	FUNC_INT64,
	FUNC_INT32,
	FUNC_DOUBLE,
	FUNC_FLOAT,

	FUNC_MIN,
	FUNC_MAX,
	FUNC_POW,
        FUNC_IDIV,

        FUNC_IF,
        FUNC_MADD,
        FUNC_MUL3,

        FUNC_INTERVAL,
        FUNC_IN,
        FUNC_BITDOT,

        FUNC_DIST,

        //feature functions
        FEATURE_FIELD_LENGTH,
        FEATURE_FIELD_AVG_LENGTH,
        FEATURE_DOC_AVG_LENGTH,
        FEATURE_DOC_COUNT,

        FEATURE_QUERY_WORD_COUNT,
        FEATURE_QUERY_HIT_COUNT,
        FEATURE_BM25,
        FEATURE_FIELD_BM25,
        MAX_FUNCTIONS,
    };

    struct FuncDecs
    {
        const char* name;
        uint32_t argCount;
        FuncType funcType;
        ValueType retType;
    };

public:
    ExprParser(const FX_NS(index)::IndexReaderPtr& pIndexReader);
    ~ExprParser();

public:
    /** 
     * Invoke the scanner and parser for a stream.
     * @param input input stream
     * @param pExprBuilder expression evaluator builder
     * @return expression evaluator if successfully parsed
     */
    FX_NS(search)::ExprEvaluatorPtr parse(std::istream& input,
            ExprEvaluatorBuilder* pExprBuilder = NULL);

    /** 
     * Invoke the scanner and parser on an input string.
     * @param input input string
     * @param pExprBuilder expression evaluator builder
     * @return expression evaluator if successfully parsed
     */
    FX_NS(search)::ExprEvaluatorPtr parse(const std::string& input,
            ExprEvaluatorBuilder* pExprBuilder = NULL);

    /**
     * Clear parsed result
     */
    void clear();

    /**
     * Set tracing flags
     * @param bTraceScanning output scanner tracing message if true
     * @param bTraceParsing output bison parser tracing message if true
     */
    void setTraceFlag(bool bTraceScanning, bool bTraceParsing);

    /**
     * Return expression node by node index
     */
    const ExprNode& getExprNode(int32_t iNodeIdx) const;

    /**
     * Print parse result to string
     */
    std::string toString() const;

public:
    /**
     * Return function index
     */
    static int32_t getFuncIdx(const std::string& sFuncName);

    /**
     * Get function by function index
     */
    static const FuncDecs& getFunc(int32_t funcIdx);

    /**
     * Return max functons supported
     */
    static size_t getMaxFunctions();

    /**
     * Print specified nodes to string 
     */
    static std::string toString(int32_t iNodeIdx, const NodeVector& nodes);

public:
    /**
     * Return error message if parse failed.
     */
    std::string getErrorMessage() const;

public:
    int32_t addIntNode(int64_t iValue);
    int32_t addFloatNode(double fValue);
    int32_t addAttrNode(const std::string& sAttrName);
    int32_t addOpNode(TokenType iOp, int32_t iLeft, int32_t iRight);
    int32_t addFuncNode(int32_t iFunc, int32_t iLeft, int32_t iRight = -1, 
                        ExprNode::TokenType tokenType = ExprNode::TOK_FUNC);
    int32_t addFeatureNode(const std::string& sFuncName, int32_t iLeft, 
                           int32_t iRight = -1);
    int32_t addConstListNode(int64_t iValue);
    int32_t addConstListNode(double dbValue);
    int32_t addConstListNode(const std::string& strValue);
    void appendToConstList(int32_t iNode, int64_t iValue);
    void appendToConstList(int32_t iNode, double iValue);
    void appendToConstList(int32_t iNode, const std::string& strValue);
    int32_t addDateFuncNode(int32_t iFunc, const std::string& sDateStr);

    /** 
     * Error handling with associated line number.
     */
    void fail(const fx_bison::location& l, const std::string& m);

    /** 
     * General error handling.
     */
    void fail(const std::string& m);

    /**
     * Finalize the parsing
     */
    void finalize(int32_t iRootIdx);

private:
    static void toString(int32_t iNodeIdx, const NodeVector& nodes,
                         std::stringstream& ss);
    static void toString(ExprNode::TokenType type,
                         std::stringstream& ss);

private:
    FX_NS(index)::IndexReaderPtr m_pIndexReader;

    NodeVector m_nodes;
    int32_t m_iRootNode;
    std::string m_sErrorMsg;

    bool m_bTraceScanning;
    bool m_bTraceParsing;

private:
    struct Inializer
    {
        Inializer();
    };

    static void initStaticData()
    {
        static Inializer STATIC_DATA_INIALIZER;
    }

    struct RetAndArgType
    {
        ValueType retType;
        ValueType argType;
    };

    static ValueType RET_TYPES[ExprNode::TOK_TYPE_MAX];
    static RetAndArgType RET_AND_ARG_TYPES[VALUE_TYPE_MAX][VALUE_TYPE_MAX];

    static FuncDecs FUNCTIONS[MAX_FUNCTIONS];

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(ExprParser);

///////////////////////////////////////////
inline void ExprParser::fail(const std::string& m)
{
    m_sErrorMsg = m;
}

inline void ExprParser::finalize(int32_t iRootIdx)
{
    m_iRootNode = iRootIdx;
}

inline std::string ExprParser::getErrorMessage() const
{
    return m_sErrorMsg;
}

inline void ExprParser::setTraceFlag(bool bTraceScanning, bool bTraceParsing)
{
    m_bTraceScanning = bTraceScanning;
    m_bTraceParsing = bTraceParsing;
}

//static 
inline int32_t ExprParser::getFuncIdx(const std::string& sFuncName)
{
    for (size_t i = 0; i < sizeof(FUNCTIONS) / sizeof(FUNCTIONS[0]); ++i)
    {
        if (sFuncName == FUNCTIONS[i].name)
        {
            return (int32_t)i;
        }
    }
    return -1;
}

//static
inline const ExprParser::FuncDecs& ExprParser::getFunc(int32_t funcIdx)
{
    FIRTEX_ASSERT2(funcIdx >= 0 && funcIdx < (int32_t)MAX_FUNCTIONS);
    return FUNCTIONS[funcIdx];
}

//static 
inline size_t ExprParser::getMaxFunctions()
{
    return MAX_FUNCTIONS;
}

inline const ExprNode& ExprParser::getExprNode(int32_t iNodeIdx) const
{
    FIRTEX_ASSERT2(iNodeIdx >= 0 && iNodeIdx < (int32_t)m_nodes.size());
    return m_nodes[iNodeIdx];
}

inline void ExprParser::clear()
{
    m_nodes.clear();
    m_iRootNode = -1;
}

FX_NS_END

#endif //__FX_EXPRPARSER_H
