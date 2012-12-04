//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (firtex.sourceforge.net)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-11-13 23:01:41

#ifndef __FX_DATETIMELEXER_H
#define __FX_DATETIMELEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <sstream>

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer DateTimeFlexLexer
#include "FlexLexer.h"
#endif

FX_NS_DEF(analyzer);
class DateTimeLexer;
FX_NS_END

#include "analyzer/DateTimeBisonParser.h"

FX_NS_DEF(analyzer);

class DateTimeLexer : public DateTimeFlexLexer
{
public:
    DateTimeLexer(std::istream *input, std::ostream *output);
    virtual ~DateTimeLexer();

public:
    typedef fx_bison::DateTimeBisonParser::token token;
    typedef fx_bison::DateTimeBisonParser::token_type token_type;
    typedef fx_bison::DateTimeBisonParser::semantic_type semantic_type;
    typedef fx_bison::DateTimeBisonParser::location_type location_type;

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

#endif //__FX_LEXER_H
