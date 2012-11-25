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
// Created      : 2007/3/6
//

#include <stdlib.h>
#include <map>
#include "firtex/contrib/snowball/SnowballWrapper.h"
#include "stemmer/include/libstemmer.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(analyzer);

SnowballWrapper::SnowballWrapper()
    : m_sLanguage("english")
{
}

SnowballWrapper::SnowballWrapper(const SnowballWrapper& src)
    : m_sLanguage(src.m_sLanguage)
    , m_sEncode(src.m_sEncode)
    , m_sIrregularWords(src.m_sIrregularWords)
{
    m_irregularWords = src.m_irregularWords;
}

SnowballWrapper::SnowballWrapper(const tchar* szLanguage)
    : m_sLanguage(szLanguage)
{
}

SnowballWrapper::SnowballWrapper(const tchar* szLanguage, const tchar* szEnc)
    : m_sLanguage(szLanguage)
    , m_sEncode(szEnc)
{
}

SnowballWrapper::SnowballWrapper(const tchar* szLanguage,
                                 const tchar* szIrregularWords,
                                 const tchar* szEnc)
    : m_sLanguage(szLanguage)
{
    if (szIrregularWords)
    {
        m_sIrregularWords = szIrregularWords;
    }
    if (szEnc)
    {
        m_sEncode = szEnc;
    }
}

SnowballWrapper::~SnowballWrapper()
{
}

size_t SnowballWrapper::stemmer(char* szStemmedBuf, size_t nBufSize,
                                const char* szTextBuf, size_t nTextLen) const

{
    if (nTextLen <= 0)
        return 0;
    
    struct sb_stemmer* pStemmer = sb_stemmer_new(m_sLanguage.c_str(),
            m_sEncode.empty() ? NULL : m_sEncode.c_str());
    if (!pStemmer)
    {
        return 0;
    }

    char* pStopPos = szStemmedBuf + nBufSize - 1;

    bool bFlagEnglish = true;
    int i;
    register char* pWord;
    int  nWordLen;
    char* pStemmerStart = szStemmedBuf;
    register char* pStemmedBufStart = pStemmerStart;
    register char *pStemmedBufEnd;
    register const char* pWordStart;
    register const char* pWordEnd;

    pWordStart = szTextBuf;
    const char* pTextEnd = szTextBuf + nTextLen;
    while(pWordStart < pTextEnd)
    {
        while(pWordStart < pTextEnd && !((*pWordStart >= 'a' && *pWordStart <= 'z') 
                        || (*pWordStart >= 'A' && *pWordStart <= 'Z')))
        {
            *pStemmedBufStart = *pWordStart;
            pWordStart++;
            pStemmedBufStart++;
        }
        if (pWordStart >= pTextEnd)
            break;

        pWordEnd = pWordStart + 1;
        while((pWordEnd < (pTextEnd)) && ((*pWordEnd >= 'a' && *pWordEnd <= 'z') 
                        || (*pWordEnd >= 'A' && *pWordEnd <= 'Z')))
        {
            pWordEnd++;
        }

        pStemmedBufEnd = pStemmedBufStart;
        for (i=0; i < pWordEnd - pWordStart; i++)
        {
            if (pWordStart[i] >= 'A' && pWordStart[i] <= 'Z')
                *pStemmedBufEnd = 'a'-'A' + pWordStart[i];
            else
            {
                if (!(pWordStart[i] >= 'a' && pWordStart[i] <= 'z'))
                    bFlagEnglish = false;
                *pStemmedBufEnd = pWordStart[i];
            }
            pStemmedBufEnd++;
        }

        if (bFlagEnglish)
        {
            if (pWordEnd != pWordStart)
            {
                if (*(pWordEnd-1) != 'y')
                {
                    pWord = (char*)sb_stemmer_stem(pStemmer,
                            (const sb_symbol*)pStemmedBufStart,
                            (int)(pStemmedBufEnd - pStemmedBufStart));
                    nWordLen = sb_stemmer_length(pStemmer);
                    for (i = 0; i < nWordLen && pStemmedBufStart < pStopPos; i++)
                    {
                        *pStemmedBufStart++ = pWord[i];
                    }
                }
                else
                {
                    pStemmedBufStart = pStemmedBufEnd;
                }
            }
        }
        else
        {
            pStemmedBufStart = pStemmedBufEnd;
        }
        pWordStart = pWordEnd;
    }

    *pStemmedBufStart = 0;
    sb_stemmer_delete(pStemmer);

    return pStemmedBufStart - pStemmerStart;
}

FX_NS_END

