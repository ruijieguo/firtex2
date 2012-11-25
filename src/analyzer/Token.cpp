#include "firtex/analyzer/Token.h"

using namespace std;

FX_NS_USE(index);

FX_NS_DEF(analyzer);

Token::Token(void) 
    : m_nStartOffset(0)
    , m_nEndOffset(0)
    , m_nPosIncrement(1)
    , m_tokenType(TT_UNKNOWN)
    , m_hintId(INVALID_HINTID)
{
}

Token::Token(const char* text)
    : m_text(text)
    , m_nStartOffset(0)
    , m_nEndOffset(0)
    , m_nPosIncrement(1)
    , m_tokenType(TT_CWORD)
    , m_hintId(INVALID_HINTID)
{
}

Token::Token(const string& text)
    : m_text(text)
    , m_nStartOffset(0)
    , m_nEndOffset(0)
    , m_nPosIncrement(1)
    , m_tokenType(TT_CWORD)
    , m_hintId(INVALID_HINTID)
{
}

Token::Token(const Token& clone)
    : m_text(clone.m_text)
{
    m_nStartOffset = clone.m_nStartOffset;
    m_nEndOffset = clone.m_nEndOffset;
    m_nPosIncrement = clone.m_nPosIncrement;
    m_tokenType = clone.m_tokenType;
    m_hintId = clone.m_hintId;
}

Token::Token(const char* text, int32_t nPosIncrement,
             int32_t nStartOffset, int32_t nEndOffset)
    : m_text(text)
    , m_nStartOffset(nStartOffset)
    , m_nEndOffset(nEndOffset)
    , m_nPosIncrement(nPosIncrement)
    , m_tokenType(TT_CWORD)
    , m_hintId(INVALID_HINTID)
{
}

Token* Token::clone() const
{
    return new Token(*this);
}

FX_NS_END
