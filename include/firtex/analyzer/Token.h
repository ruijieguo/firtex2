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
#ifndef __FX_TOKEN_H
#define __FX_TOKEN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/utility/String.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/Term.h"

FX_NS_DEF(analyzer);

class Token
{
public:
    typedef FX_NS(utility)::StringBase<char> String;

    enum TokenType
    {
        TT_UNKNOWN = 0,
        TT_NUMBER,      //< number
        TT_DATE,        //< Date time
        TT_ALPHA,
        TT_CWORD,       //< Chinese word
        TT_STRING,      //< String
        TT_PUNCT,       //< Punctuation symbol
        TT_WHITESPACE,  //< whitespace
        TT_LINE_BREAK,   //< line break (\n \r)
        TT_MAX,         //< Number of attribute
    };

public:
    Token();
    Token(const char* text);
    Token(const std::string& text);
    Token(const Token& clone);
    Token(const char* text, int32_t nPosIncrement, 
          int32_t nStartOffset, int32_t nEndOffset);
    Token(char* text, size_t nTextLen, bool bNoCopy,
          int32_t nPosIncrement, int32_t nStartOffset,
          int32_t nEndOffset, TokenType tokenType);

    virtual ~Token() {}

public:
    /** 
     * get token attribute type
     * @return token attribute type
     */
    inline TokenType getTokenType() const;

    /**
     * set token attribute type
     * @param t attribute type
     */
    inline void setTokenType(TokenType t);

    /**
     * Return text value of token
     * @return text content of token
     */
    inline const char* getTextValue() const;

    /**
     * Return length of text
     * @return length of text
     */
    inline size_t getTextLength()const;

    /**
     * set text value of token
     * @param text text string
     */
    inline void setTextValue(const char* text);

    /**
     * set text value of token
     * @param text text string
     */
    inline void setTextValue(const char* text, size_t length);

    /**
     * set text value of token
     * @param text text string
     */
    inline void setTextValue(char* text, size_t length, bool bNoCopy);

    /**
     * Set token attribute
     */
    inline void setAttribute(int32_t nPosInc, int32_t nStartOff,
                             int32_t nEndOff);

    /**
     * get start offset of token
     * @return the offset
     */
    inline int32_t getStartOffset() const;

    /**
     * set start offset of token
     * @param nStartOffset start offset
     */
    inline void setStartOffset(int32_t nStartOffset);

    /**
     * get end offset of token 
     * @return the offset
     */
    inline int32_t getEndOffset() const;

    /**
     * set end offset of token
     * @param nEndOffset end offset of token
     */

    inline void setEndOffset(int32_t nEndOffset);

    /**
     * get position increment of token
     * @return position increment of token
     */
    inline int32_t getPosIncrement() const;

    /**
     * set position increment of token
     * @param nPositionIncrement position increment of token
     */
    inline void setPosIncrement(int32_t nPosInc);

    /**
     * Return hind id of token
     */
    inline int64_t getHintId() const;

    /**
     * Set hint id of token
     */
    inline void setHintId(int64_t hintId);

    /**
     * Clear the token
     */
    void clear();

    /**
     * clone a new object
     */
    virtual Token* clone() const;

private:
    String m_text;                  //< text content
    int32_t m_nStartOffset;         //< Start offset of token
    int32_t m_nEndOffset;           //< End offset of token
    int32_t m_nPosIncrement;   //< Position increment
    
    TokenType m_tokenType;            //< Token attribute type
    int64_t m_hintId;               //< The hint Id of the token
};

DEFINE_TYPED_PTR(Token);

//////////////////////////////////////////////////////////////////////////
///inline functions
inline Token::Token(char* text, size_t nTextLen, bool bNoCopy,
                    int32_t nPosIncrement, int32_t nStartOffset,
                    int32_t nEndOffset, TokenType tokenType)
    : m_text(text, nTextLen, bNoCopy)
    , m_nStartOffset(nStartOffset)
    , m_nEndOffset(nEndOffset)
    , m_nPosIncrement(nPosIncrement)
    , m_tokenType(tokenType)
    , m_hintId(INVALID_HINTID)

{
}

inline const char* Token::getTextValue() const
{
    return m_text.c_str();
}

inline size_t Token::getTextLength() const
{
    return m_text.length();
}

inline void Token::setTextValue(const char* text)
{
    m_text.assign(text);
}

inline void Token::setTextValue(const char* text, size_t length)
{
    m_text.assign(text, length);
}

inline void Token::setTextValue(char* text, size_t length, bool bNoCopy)
{
    m_text.assign(text, length, bNoCopy);
}

inline Token::TokenType Token::getTokenType() const 
{
    return m_tokenType;
}

inline void Token::setTokenType(TokenType t)
{
    m_tokenType = t;
}

inline void Token::setAttribute(int32_t nPosInc,
                                int32_t nStartOff,
                                int32_t nEndOff)
{
    m_nPosIncrement = nPosInc;
    m_nStartOffset = nStartOff;
    m_nEndOffset = nEndOff;
}

inline int32_t Token::getStartOffset() const 
{
    return m_nStartOffset;
}

inline void Token::setStartOffset(int32_t nStartOffset)
{
    m_nStartOffset = nStartOffset;
}

inline int32_t Token::getEndOffset() const 
{
    return m_nEndOffset;
}

inline void Token::setEndOffset(int32_t nEndOffset)
{
    m_nEndOffset = nEndOffset;
}

inline int32_t Token::getPosIncrement() const 
{
    return m_nPosIncrement;
}

inline void Token::setPosIncrement(int32_t nPosInc)
{
    m_nPosIncrement = nPosInc;
}

inline int64_t Token::getHintId() const
{
    return m_hintId;
}

inline void Token::setHintId(int64_t hintId)
{
    m_hintId = hintId;
}

inline void Token::clear()
{
    m_text.clear();

    m_nStartOffset = 0;
    m_hintId = INVALID_HINTID;
}

FX_NS_END

#endif
