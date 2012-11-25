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
// Created	: 2011-04-01 22:39:56

#ifndef __FX_TOKENIZER_H
#define __FX_TOKENIZER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(analyzer);

class Tokenizer
{
public:
    Tokenizer() {}
    virtual ~Tokenizer() {}

public:
    class LexerWrapper
    {
    public:
        virtual ~LexerWrapper() {}
    public:
        virtual Token::TokenType lex(const char* &pText, size_t &length) = 0;
    };

public:
    /** 
     * Scan input text stream and tokenize to small tokens.
     * @param pTokenView result segmented tokens
     * @param szText text need to segment
     * @param length of text in bytes
     */
    virtual void tokenize(TokenViewPtr& pTokenView, const char* szText,
                          size_t length) const = 0;

    /** 
     * Scan input text stream and tokenize to small tokens.
     * @param pTokenView result segmented tokens
     * @param sText text to segment
     * @param nStartOffOfToken start offset of token
     */
    virtual void tokenize(TokenViewPtr& pTokenView,
                          int32_t& nPosInc, const std::string& sText,
                          int32_t nStartOffOfToken) const = 0;

    /**
     * Create an lexer
     */
    virtual LexerWrapper* createLexer(const char* szText,
            size_t length) const = 0;

public:
    /** 
     * Scan input text stream and tokenize to small tokens.
     * @param pTokenView result segmented tokens
     * @param sText text to segment
     */
    void tokenize(TokenViewPtr& pTokenView, const std::string& sText) const;
};

DEFINE_TYPED_PTR(Tokenizer);

///////////////////////////////////
//
inline void Tokenizer::tokenize(TokenViewPtr& pTokenView,
                                const std::string& sText) const
{
    int32_t nPosInc = 0;
    tokenize(pTokenView, nPosInc, sText, 0);
}

FX_NS_END

#endif //__FX_TOKENIZER_H
