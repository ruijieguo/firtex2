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

#ifndef __FX_STANDARDTOKENIZER_H
#define __FX_STANDARDTOKENIZER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/TokenView.h"
#include "firtex/analyzer/UTF8TokenLexer.h"

#undef yyFlexLexerOnce
#include "firtex/analyzer/GBKTokenLexer.h"

#include "firtex/analyzer/Tokenizer.h"
#include <sstream>

FX_NS_DEF(analyzer);

template <typename LexerT>
class StandardTokenizer : public Tokenizer
{
public:
    typedef LexerT LexerType;

public:
    class Lexer : public LexerWrapper
    {
    public:
        Lexer(const char* szText, size_t length)
            : m_input(std::string(szText, length))
        {
            m_pLexer = new LexerType(&m_input, &m_oss);
        }
        ~Lexer()
        {
            delete m_pLexer;
        }

    public:
        Token::TokenType lex(const char* &pText, size_t &length)
        {
            return m_pLexer->lex(pText, length);
        }

    private:
        ostringstream m_oss;
        istringstream m_input;
        LexerType* m_pLexer;
    };

public:
    StandardTokenizer();
    ~StandardTokenizer();

public:
    /** 
     * Scan input text stream and tokenize to small tokens.
     * @param pTokenView result segmented tokens
     * @param szText text need to segment
     * @param length of text in bytes
     */
    void tokenize(TokenViewPtr& pTokenView, const char* szText,
                  size_t length) const;

    /** 
     * Scan input text stream and tokenize to small tokens.
     * @param pTokenView result segmented tokens
     * @param sText text to segment
     * @param nStartOffOfToken start offset of token
     */
    void tokenize(TokenViewPtr& pTokenView,
                  int32_t& nPosInc, const std::string& sText,
                  int32_t nStartOffOfToken) const;

    /**
     * Set tracing flags
     * @param bTraceScanning output scanner tracing message if true
     */
    void setTraceFlag(bool bTraceScanning);

public:
    LexerWrapper* createLexer(const char* szText,
                              size_t length) const;

public:
    /**
     * Return error message if parse failed.
     */
    std::string getErrorMessage() const;

public:
    /** 
     * General error handling.
     */
    void fail(const std::string& m);

    /**
     * Finalize the parsing
     */
    void finalize(int32_t iRootIdx);

private:
    std::string m_sErrorMsg;
    std::string m_sStreamName;

    bool m_bTraceScanning;

private:
    DECLARE_STREAM_LOGGER();
};

///////////////////////////////////////////
SETUP_STREAM_LOGGER_TEMP(analyzer, StandardTokenizer);

template <typename LexerT>
StandardTokenizer<LexerT>::StandardTokenizer()
    : m_bTraceScanning(false)
{
}

template <typename LexerT>
StandardTokenizer<LexerT>::~StandardTokenizer() 
{
}


template <typename LexerT>
void StandardTokenizer<LexerT>::tokenize(TokenViewPtr& pTokenView,
        int32_t& nPosInc, const std::string& sText,
        int32_t nStartOffOfToken) const
{
    ostringstream oss;
    istringstream input(sText);
    LexerType lexer(&input, &oss);
    lexer.setDebug(m_bTraceScanning);

    const char* szToken = NULL;
    size_t nTokenLen = 0;

    size_t pos = nStartOffOfToken;
    int32_t inc = nPosInc;
    Token::TokenType tokenType = Token::TT_MAX;
    while ((tokenType = lexer.lex(szToken, nTokenLen)) != Token::TT_MAX)
    {
        if (!szToken)
        {
            pos += nTokenLen;
            if (tokenType != Token::TT_WHITESPACE)
            {
                ++inc;
            }
            continue;
        }
        pTokenView->addToken(szToken, nTokenLen, inc, 
                             (int32_t)pos, (int32_t)(pos + nTokenLen));
        pos += nTokenLen;
        
        inc = 1;
    }
    nPosInc = inc;
}

template <typename LexerT>
void StandardTokenizer<LexerT>::tokenize(TokenViewPtr& pTokenView,
        const char* szText, size_t length) const
{
    string str(szText, length);
    Tokenizer::tokenize(pTokenView, str);
}

///////////////////////////////////////////
//inline
template <typename LexerT>
inline void StandardTokenizer<LexerT>::fail(const std::string& m)
{
    m_sErrorMsg = m;
}

template <typename LexerT>
inline std::string StandardTokenizer<LexerT>::getErrorMessage() const
{
    return m_sErrorMsg;
}

template <typename LexerT>
inline void StandardTokenizer<LexerT>::setTraceFlag(bool bTraceScanning)
{
    m_bTraceScanning = bTraceScanning;
}

template <typename LexerT>
inline typename StandardTokenizer<LexerT>::LexerWrapper* 
StandardTokenizer<LexerT>::createLexer(const char* szText, size_t length) const
{
    return new Lexer(szText, length);
}

typedef StandardTokenizer<UTF8TokenLexer> UTF8StandardTokenizer;
typedef StandardTokenizer<GBKTokenLexer> GBKStandardTokenizer;

FX_NS_END

#endif //__FX_STANDARDTOKENIZER_H
