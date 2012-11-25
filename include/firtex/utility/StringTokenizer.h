#ifndef __STRINGTOKENIZER_H
#define __STRINGTOKENIZER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/Exception.h"
#include <vector>
#include <cstddef>

FX_NS_DEF(utility)

/**
 * @class StringTokenizer
 * @brief A string tokenizer that splits a string into
 * tokens, which are separated by separator characters.
 */
class StringTokenizer
{
public:
    /// ignore empty tokens
    const static int32_t TOKEN_IGNORE_EMPTY = 1; 
    /// remove leading and trailing whitespace from tokens
    const static int32_t TOKEN_TRIM         = 2;
    /// leave the token as it is
    const static int32_t TOKEN_LEAVE_AS     = 4;

    typedef int32_t Option;
    typedef std::vector<tstring> TokenVector;
    typedef TokenVector::const_iterator Iterator;

public:
    /**
     * Splits the given string into tokens which are separated by the separator
     * characters given in <i>sSep</i>
     * @param sUnsepTokens tokens which are expected to be separated by sSep.
     * @param sSep separator characters
     * @param opt options using to spearate tokens
     */
    StringTokenizer(const tstring& sUnsepTokens, const tstring& sSep,
                    Option opt = TOKEN_LEAVE_AS);

    /**
     * Splits the given string into tokens which are separated by the separator
     * characters given in <i>sSep</i>
     * @param sUnsepTokens tokens which are expected to be separated by sSep.
     * @param length length of sUnsepTokens
     * @param sSep separator characters
     * @param opt options using to spearate tokens
     */
    StringTokenizer(const char* sUnsepTokens, size_t length, const tstring& sSep,
                    Option opt = TOKEN_LEAVE_AS);

    StringTokenizer();
    ~StringTokenizer();
public:
    /**
     * Splits the given string into tokens which are separated by the separator
     * characters given in <i>sSep</i>
     * @param sUnsepTokens tokens which are expected to be separated by sSep.
     * @param sSep separator characters
     * @param opt options using to spearate tokens
     * @return number of tokens have been separated
     */
    size_t tokenize(const tstring& sUnsepTokens, const tstring& sSep,
                    Option opt = TOKEN_LEAVE_AS);

    /**
     * Splits the given string into tokens which are separated by the separator
     * characters given in <i>sSep</i>
     * @param sUnsepTokens tokens which are expected to be separated by sSep.
     * @param length length of sUnsepTokens
     * @param sSep separator characters
     * @param opt options using to spearate tokens
     * @return number of tokens have been separated
     */
    size_t tokenize(const char* sUnsepTokens, size_t length,
                    const tstring& sSep, Option opt);

    /**
     * sort tokens
     */
    void sort();
    
    /**
     * get begin/end const iterator
     */
    inline Iterator begin() const;
    inline Iterator end() const;

    inline const tstring& operator [] (size_t index) const;

    /**
     * get number of tokens
     */
    inline size_t getNumTokens() const;

private:
    inline static bool isSpace(tchar ch);
     
    void addToken(const tstring& sToken, Option opt);     

    void addToken(const char* str, size_t length, Option opt);

private:
    StringTokenizer(const StringTokenizer&);
    StringTokenizer& operator = (const StringTokenizer&);

private:
    TokenVector m_tokens;

private:
    DECLARE_STREAM_LOGGER();
};

///////////////////////////////////////////////////////////
// inlines
//
inline bool StringTokenizer::isSpace(tchar ch)
{
    return (ch > 0 && std::isspace(ch));
}

inline StringTokenizer::Iterator StringTokenizer::begin() const
{
    return m_tokens.begin();
}

inline StringTokenizer::Iterator StringTokenizer::end() const
{
    return m_tokens.end();
}

inline const tstring& StringTokenizer::operator [] (std::size_t index) const
{
    if (index >= m_tokens.size()) 
        FIRTEX_THROW(OutOfRangeException, "idx = [%u], size = [%u]",
                     (uint32_t)index, (uint32_t)m_tokens.size());
    return m_tokens[index];
}

inline size_t StringTokenizer::getNumTokens() const
{
    return m_tokens.size();
}

inline void StringTokenizer::addToken(const char* str, size_t length, Option opt)
{
    addToken(tstring(str, length), opt);
}

FX_NS_END

#endif 
