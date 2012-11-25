//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2005/11/20
//

#ifndef __FX_TOKENVIEW_H
#define __FX_TOKENVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/StringPool.h"
#include "firtex/utility/Charset.h"
#include "firtex/utility/Iterator.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/Token.h"

FX_NS_DEF(analyzer);

class TokenView
{
public:
    const static size_t MAX_TOKENS = 10000;
    const static size_t DEFAULT_TOKEN_LENGTH = 8;

    typedef Token::TokenType TokenType;

    typedef std::vector<Token> TokenVector;

public:
    class Iterator
    {
    public:
        Iterator(const TokenView* pTokens);
        Iterator(const Iterator& _iter);
        ~Iterator();

    public:
        /**
         * reset the iterator
         */
        void reset()
        {
            m_nCurToken = 0;
        }

        bool hasNext() const
        {
            return (m_nCurToken < m_pTokens->m_tokens.size());
        }

        const Token& next()
        {
            const Token& token = m_pTokens->m_tokens[m_nCurToken];
            ++m_nCurToken;
            return token;
        }

        size_t size() const
        {
            return m_pTokens->m_tokens.size();
        }

    protected:
        const TokenView* m_pTokens;
        size_t m_nCurToken;
    };

public:
    TokenView();
    TokenView(size_t nMaxTokens, TokenType defTokenType = Token::TT_UNKNOWN);
    virtual ~TokenView(void);

private:
    TokenView(const TokenView& clone);

public:
    /**
     * Return iterator of tokens
     * @return iterator 
     */
    Iterator iterator() const;

public:
    /**
     * get number of tokens
     * @return number of tokens  
     */
    size_t getNumTokens() const;

    /**
     * Reserve space
     */
    void reserve(size_t nMaxTokens);

    /** 
     * clear tokens 
     */
    void clear();

    /** 
     * get token type
     * @return token type
     */
    TokenType getTokenType() const {return m_defaultType;};

    /**
     * set token type 
     * @param[in] t token type
     */
    void setTokenType(TokenType t) {m_defaultType = t;}

public:
    /**
     * add a token
     * @param pszToken token text which ended with '\0'
     * @return true if success, otherwise false
     */
    inline void addToken(const char* pszToken);

    /**
     * add a token
     * @param[in] pszToken token text which ended with '\0'
     * @param[in] nPosIncrement position increment
     * @param[in] nStartOffset start offset of token
     * @param[in] nEndOffset end offset of token
     * @param[in] tokenType token type
     */
    inline void addToken(const char* pszToken, int32_t nPosIncrement,
                         int32_t nStartOffset, int32_t nEndOffset,
                         TokenType tokenType = Token::TT_UNKNOWN);

    /**
     * add a token
     * @param[in] pszToken token text
     * @param[in] nTokenLen length of token
     * @param[in] tokenType token type
     */
    inline void addToken(const char* pszToken, size_t nTokenLen,
                         TokenType tokenType = Token::TT_UNKNOWN);

    /**
     * add a text token
     * @param[in] pszToken token text
     * @param[in] nTokenLen length of token
     * @param[in] nPosIncrement position increment
     * @param[in] nStartOffset start offset of token
     * @param[in] nEndOffset end offset of token
     * @param[in] tokenType token type
     */
    inline void addToken(const char* pszToken, size_t nTokenLen,
                         int32_t nPosIncrement, int32_t nStartOffset,
                         int32_t nEndOffset,
                         TokenType tokenType = Token::TT_UNKNOWN);

protected:
    FX_NS(utility)::StringPool m_strPool;
    TokenVector m_tokens;
    TokenType m_defaultType;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(TokenView);


//////////////////////////////////////////////////////////////////////////
//inline Function
inline size_t TokenView::getNumTokens() const 
{
    return m_tokens.size();
}

inline void TokenView::addToken(const char* pszToken)
{
    size_t len = FX_NS(utility)::strLength(pszToken);
    const char* pTokenStr = m_strPool.addString(pszToken, len);

    m_tokens.push_back(Token(pTokenStr));
}

inline void TokenView::addToken(const char* pszToken,
                                int32_t nPosIncrement, 
                                int32_t nStartOffset,
                                int32_t nEndOffset,
                                TokenType tokenType)
{
    size_t len = FX_NS(utility)::strLength(pszToken);
    const char* pTokenStr = m_strPool.addString(pszToken, len);

    m_tokens.push_back(Token(const_cast<char*>(pTokenStr), len, true,
                             nPosIncrement, nStartOffset, nEndOffset,
                             tokenType));
}

inline void TokenView::addToken(const char* pszToken, size_t nTokenLen,
                                TokenType tokenType)
{
    const char* pTokenStr = m_strPool.addString(pszToken, nTokenLen);

    m_tokens.push_back(Token(const_cast<char*>(pTokenStr), nTokenLen, true,
                             1, 0, 0, tokenType));
}

inline void TokenView::addToken(const char* pszToken, size_t nTokenLen, 
                                int32_t nPosIncrement, int32_t nStartOffset,
                                int32_t nEndOffset, TokenType tokenType)
{
    const char* pTokenStr = m_strPool.addString(pszToken, nTokenLen);

    m_tokens.push_back(Token(const_cast<char*>(pTokenStr), nTokenLen, true,
                             nPosIncrement, nStartOffset, nEndOffset,
                             tokenType));
}

inline void TokenView::reserve(size_t nMaxTokens)
{
    m_tokens.reserve(nMaxTokens);
}

FX_NS_END

#endif
