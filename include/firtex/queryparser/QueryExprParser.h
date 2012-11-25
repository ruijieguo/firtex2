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
// Created	: 2010-11-14 13:30:05

#ifndef __FX_QUERYEXPRPARSER_H
#define __FX_QUERYEXPRPARSER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/queryparser/QueryExpr.h"

namespace fx_bison
{
class location;
};

FX_NS_DEF(queryparser);

class QueryExprParser
{
public:
    QueryExprParser(const std::string& sDefField);
    ~QueryExprParser();

public:
    /** 
     * Invoke the scanner and parser for a stream.
     * @param input input stream
     * @param sName stream name for error messages
     * @return root node of query expression if successfully parsed
     */
    QueryExprPtr parse(std::istream& input,
                       const std::string& sName = "StreamInput");

    /** 
     * Invoke the scanner and parser on an input string.
     * @param input input string
     * @param sName stream name for error messages
     * @return root node of query expression if successfully parsed
     */
    QueryExprPtr parse(const std::string& input,
                       const std::string& sName = "StringInput");
    
    /**
     * Set tracing flags
     * @param bTraceScanning output scanner tracing message if true
     * @param bTraceParsing output bison parser tracing message if true
     */
    void setTraceFlag(bool bTraceScanning, bool bTraceParsing);

    /**
     * Return default field
     */
    const std::string& getDefaultField() const;

    /** 
     * Error handling with associated line number.
     */
    void fail(const fx_bison::location& l, const std::string& m);

    /** 
     * General error handling.
     */
    void fail(const std::string& m);

    /**
     * Return error message if parse failed.
     */
    std::string getErrorMessage() const;

public:
    void finalize(QueryExpr* pExpr);
    QueryExpr::OpType getDefaultOp() const;

protected:
   // friend class QueryExprBisonParser;

private:
    std::string m_sDefaultField;
    QueryExprPtr m_pRootExpr;
    std::string m_sErrorMsg;

    // Stream name (file or input stream) used for error messages.
    std::string m_sStreamName;

    bool m_bTraceScanning;
    bool m_bTraceParsing;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////
//
inline void QueryExprParser::fail(const std::string& m)
{
    m_sErrorMsg = m;
}

inline void QueryExprParser::finalize(QueryExpr* pExpr)
{
    m_pRootExpr = pExpr;
}

inline const std::string& QueryExprParser::getDefaultField() const
{
    return m_sDefaultField;
}

inline QueryExpr::OpType QueryExprParser::getDefaultOp() const
{
    return QueryExpr::OP_AND;
}

inline std::string QueryExprParser::getErrorMessage() const
{
    return m_sErrorMsg;
}

inline void QueryExprParser::setTraceFlag(bool bTraceScanning, bool bTraceParsing)
{
    m_bTraceScanning = bTraceScanning;
    m_bTraceParsing = bTraceParsing;
}

FX_NS_END

#endif //__FX_QUERYEXPRPARSER_H
