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
// Created	: 2010-11-20 21:57:04

#ifndef __FX_QUERYPARSER_H
#define __FX_QUERYPARSER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/AnalyzerMapper.h"
#include "firtex/search/Query.h"

FX_NS_DEF(queryparser);

class QueryParser
{
public:
    enum OpType
    {
        OP_NONE,
        OP_AND,
        OP_OR,
        OP_PHRASE,
    };

public:
    QueryParser(const FX_NS(analyzer)::AnalyzerMapper* pAnalyzerMapper, 
                const std::string& sDefaultField, OpType defOp = OP_AND);
    ~QueryParser();

public:
    /**
     * Parse query expression
     * @param sQueryExpr query expression
     * @return query object
     * @throw SyntaxException or QueryParserException
     */
    FX_NS(search)::QueryPtr parse(const std::string& sQueryExpr);

    /**
     * Return default query operation
     */
    OpType getDefaultOp() const;

    /**
     * Return default field 
     */
    const std::string& getDefaultField() const;

    /**
     * Return analyzer mapper
     */
    const FX_NS(analyzer)::AnalyzerMapper* getAnalyzerMapper() const;

protected:
    const FX_NS(analyzer)::AnalyzerMapper* m_pAnalyzerMapper;
    std::string m_sDefField;
    OpType m_defOp;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(QueryParser);

//////////////////////////////////////////////////
inline QueryParser::OpType QueryParser::getDefaultOp() const
{
    return m_defOp;
}

inline const std::string& QueryParser::getDefaultField() const
{
    return m_sDefField;
}

inline const FX_NS(analyzer)::AnalyzerMapper*
QueryParser::getAnalyzerMapper() const
{
    return m_pAnalyzerMapper;
}

FX_NS_END

#endif //__FX_QUERYPARSER_H
