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

#ifndef __FX_GBKTOKENLEXER_H
#define __FX_GBKTOKENLEXER_H

#include <sstream>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/Token.h"

//#if !defined(yyFlexLexerOnce) || ! defined(GBKTokenFlexLexer)
#ifndef yyFlexLexerOnce
#undef yyFlexLexer
#define yyFlexLexer GBKTokenFlexLexer
#include "FlexLexer.h"
//#define yyFlexLexerOnce
//#define GBKTokenFlexLexerOnce
#endif

FX_NS_DEF(analyzer);

class GBKTokenLexer : public GBKTokenFlexLexer
{
public:
    typedef Token::TokenType TokenType;

public:
    GBKTokenLexer(std::istream *input, std::ostream *output);
    virtual ~GBKTokenLexer();

public:
    virtual TokenType lex(const char* &pText, size_t &length);

public:
    void setDebug(bool debug);
    
private:
    std::ostringstream oss;

private:
    DECLARE_STREAM_LOGGER();
};

//#undef yyFlexLexer
//#define yyFlexLexerOnce

FX_NS_END

#endif //__FX_GBKTOKENLEXER_H
