//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2005/11/20
//
#include "firtex/common/StdHeader.h"
#include "firtex/analyzer/TokenView.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, TokenView);

///////////////////////////////////////////////////////////
//TokenView::Iterator

TokenView::Iterator::Iterator(const TokenView* pTokens)
    : m_pTokens(pTokens)
    , m_nCurToken(0)
{
}

TokenView::Iterator::Iterator(const Iterator& _iter)
    : m_pTokens(_iter.m_pTokens)
    , m_nCurToken(_iter.m_nCurToken)
{
}

TokenView::Iterator::~Iterator(void)
{
    m_pTokens = NULL;
}

/////////////////////////////////////////////////////////////////////
///TokenView
TokenView::TokenView(void)
    : m_strPool(TokenView::MAX_TOKENS * DEFAULT_TOKEN_LENGTH)
    , m_defaultType(Token::TT_CWORD)
{
    m_tokens.reserve(TokenView::MAX_TOKENS);
}

TokenView::TokenView(size_t nMaxTokens, TokenType tokenType)
    : m_strPool(nMaxTokens * DEFAULT_TOKEN_LENGTH)
    , m_defaultType(tokenType)
{
    m_tokens.reserve(TokenView::MAX_TOKENS);
}

TokenView::~TokenView(void)
{
    clear();
}

void TokenView::clear()
{
    m_tokens.clear();
    m_strPool.clear();
}

TokenView::Iterator TokenView::iterator() const
{
    return TokenView::Iterator(this);
}

FX_NS_END
