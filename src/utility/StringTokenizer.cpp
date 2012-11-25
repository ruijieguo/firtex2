#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/StringUtils.h"
#include <cctype>
#include <algorithm>

using namespace std;
FX_NS_DEF(utility);

StringTokenizer::StringTokenizer()
{
}
                                 
StringTokenizer::StringTokenizer(const tstring& sUnsepTokens,
                                 const tstring& sSep, Option opt)
{
    tokenize(sUnsepTokens, sSep, opt);
}

StringTokenizer::StringTokenizer(const char* sUnsepTokens, size_t length,
                                 const tstring& sSep, Option opt)
{
    tokenize(sUnsepTokens, length, sSep, opt);
}

StringTokenizer::~StringTokenizer()
{
}

size_t StringTokenizer::tokenize(const tstring& sUnsepTokens,
                                 const tstring& sSep, Option opt)
{
    size_t n = 0, nOld = 0;
    while (n != tstring::npos)
    {
        n = sUnsepTokens.find(sSep, n);
        if (n != tstring::npos)
        {
            if (n != nOld)
            {
                addToken(sUnsepTokens.substr(nOld, n - nOld), opt);
            }
            else
            {
                addToken(_T(""), opt);
            }
                        
            n += sSep.length();
            nOld = n;
        }
    }
    addToken(sUnsepTokens.substr(nOld, sUnsepTokens.length() - nOld), opt);
    return m_tokens.size();
}

size_t StringTokenizer::tokenize(const char* sUnsepTokens, size_t length,
                                 const tstring& sSep, Option opt)
{
    char* pCursor = (char*)sUnsepTokens;
    char* pCursorPre = pCursor;
    while (pCursor != NULL)
    {
        pCursor = strFind(pCursor, sSep.c_str());
        if (pCursor != NULL)
        {
            if (pCursor != pCursorPre)
            {
                addToken(pCursorPre, pCursor - pCursorPre, opt);
            }
            else
            {
                addToken(_T(""), opt);
            }
                        
            pCursor += sSep.length();
            pCursorPre = pCursor;
        }
    }
    addToken(pCursorPre, (sUnsepTokens + length - pCursorPre), opt);
    return m_tokens.size();
}

bool StringComparator(const string& s1, const string& s2)
{
    return (s1 > s2);
}

void StringTokenizer::sort()
{
    std::sort(m_tokens.begin(), m_tokens.end(), StringComparator);
}

void StringTokenizer::addToken(const tstring& sToken, Option opt)
{
    size_t nLength = sToken.length();        
    if (opt & TOKEN_LEAVE_AS)
    {
        if ( !(opt & TOKEN_IGNORE_EMPTY))
        {
            m_tokens.push_back(sToken);
        }
        else if (nLength > 0)
        {
            m_tokens.push_back(sToken);
        }
    }
    else if (opt & TOKEN_TRIM)
    {
        size_t n = 0;
        while(n < nLength && isSpace(sToken[n]))
        {
            n++;
        }
        size_t n2 = nLength;                    
        while(n2 > n && isSpace(sToken[n2 - 1]))
        {
            n2--;
        }                        
        if (n2 > n)
        {
            m_tokens.push_back(sToken.substr(n, n2 - n));
        }
        else if ( !(opt & TOKEN_IGNORE_EMPTY))
        {
            m_tokens.push_back(_T(""));//insert an empty token
        }                    
    }
    else if (nLength > 0)
    {
        m_tokens.push_back(sToken);
    }
}

FX_NS_END
