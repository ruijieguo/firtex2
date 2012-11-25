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
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2007/3/6
//

#ifndef _SNOWBALLWRAPPER_H
#define _SNOWBALLWRAPPER_H

#include <string.h>
#include <map>
#include "firtex/common/StdHeader.h"

struct sb_stemmer;

FX_NS_DEF(analyzer);

class SnowballWrapper
{
public:
    SnowballWrapper();
    SnowballWrapper(const SnowballWrapper& src);

    /**
     * constructor
     * @param szLanguage language, NULL for English
     */
    SnowballWrapper(const tchar* szLanguage);

    /**
     * constructor
     * @param szLanguage language, NULL for English
     * @szEnc encode,  NULL for utf-8
     */
    SnowballWrapper(const tchar* szLanguage, const tchar* szEnc);

    /**
     * constructor
     * @param szLanguage language, NULL for English
     * @szIrregularWords irregular words file
     * @szEnc encode, NULL for utf-8
     */
    SnowballWrapper(const tchar* szLanguage,
                    const tchar* szIrregularWords,
                    const tchar* szEnc);
    ~SnowballWrapper();

public:
    /**
     * stemmer a buffer
     * @param szStemmedBuf store stemmed string
     * @param szBufSize size of szStemmedBuf
     * @param nTextLen length of szTextBuff
     * @return size of stemmed bytes
     */
    size_t stemmer(char* szStemmedBuf, size_t szBufSize,
                   const char* szTextBuf, size_t nTextLen) const;

protected:
    typedef std::map<std::string, std::string> WordMap;
    tstring m_sLanguage;
    tstring m_sEncode;
    WordMap m_irregularWords;

    tstring m_sIrregularWords;
};

///////////////////////////////////////////////////////////////////
///inline function

FX_NS_END

#endif
