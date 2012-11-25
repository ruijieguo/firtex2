//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/12
//

#include "firtex/analyzer/KeywordAnalyzer.h"
#include "firtex/utility/StringTokenizer.h"

FX_NS_USE(utility);

FX_NS_DEF(analyzer)

SETUP_STREAM_LOGGER(analyzer, KeywordAnalyzer);

KeywordAnalyzer::KeywordAnalyzer()
{
    setMaxTokensToAnalyze(MAX_TOKEN_TO_ANALYZE);
}

KeywordAnalyzer::KeywordAnalyzer(const KeywordAnalyzer& src)
    : Analyzer(src)
{
}

KeywordAnalyzer::~KeywordAnalyzer(void)
{
}

const tstring KeywordAnalyzer::getIdentifier() const 
{
    return KeywordAnalyzer::Creator::instance()->getIdentifier();
};

void KeywordAnalyzer::doTokenize(TokenSourcePtr& tokenSource) const
{
    TokenViewPtr pInputTokens = tokenSource->getLastView();
    TokenViewPtr pOutputTokens = tokenSource->acquireCustomView(_T("keyword_view"));
    pOutputTokens->reserve(getMaxTokensToAnalyze());
    pOutputTokens->setTokenType(Token::TT_STRING);

    TokenView::Iterator it = pInputTokens->iterator();
    while (it.hasNext())
    {
        const Token& token = it.next();

        const char* pText = token.getTextValue();
        size_t length = token.getTextLength();

        pOutputTokens->addToken(pText, length);
    }
}

Analyzer* KeywordAnalyzer::clone() const
{
    return new KeywordAnalyzer(*this);
}

FX_NS_END

