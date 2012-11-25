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
// Created	: 2010-11-13 23:01:41

#ifndef __FX_EXPRLEXER_H
#define __FX_EXPRLEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <sstream>

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer ExprFlexLexer
#include "FlexLexer.h"
#endif

#include "firtex/queryparser/ExprParser.h"

FX_NS_DEF(queryparser);
class ExprLexer;
FX_NS_END

// #ifdef PARSER_HEADER_H
// #undef PARSER_HEADER_H
// #endif

#include "queryparser/ExprBisonParser.hh"

FX_NS_DEF(queryparser);

class ExprLexer : public ExprFlexLexer
{
public:
    ExprLexer(std::istream *input, std::ostream *output);
    virtual ~ExprLexer();

public:
    typedef fx_bison::ExprBisonParser::token token;
    typedef fx_bison::ExprBisonParser::token_type token_type;
    typedef fx_bison::ExprBisonParser::semantic_type semantic_type;
    typedef fx_bison::ExprBisonParser::location_type location_type;

public:
    virtual token_type lex(semantic_type* yylval, location_type* yylloc);

public:
    void setDebug(bool debug);
    
private:
    std::ostringstream oss;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_EXPRLEXER_H
