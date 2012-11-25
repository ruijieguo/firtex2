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

#include "firtex/contrib/snowball/SnowballFilter.h"
#include "firtex/contrib/snowball/SnowballWrapper.h"
#include "firtex/utility/KeyValueParser.h"

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SnowballFilter::SnowballFilter()
    : m_pSnowball(new SnowballWrapper("english", NULL, NULL))
{
}

SnowballFilter::SnowballFilter(const SnowballFilter& src)
    : TokenFilter(src)
    , m_pSnowball(new SnowballWrapper(*(src.m_pSnowball)))
{
}

SnowballFilter::SnowballFilter(const tstring& param)
    : m_pSnowball(NULL)
{
    setParam(param);    
}

SnowballFilter::SnowballFilter(const tstring& sLanguage, const tstring& sEnc)
    : m_pSnowball(new SnowballWrapper(sLanguage.c_str(), sEnc.c_str()))
{
}

SnowballFilter::SnowballFilter(const tstring& sLanguage,
                               const tstring& sIrregularWords,
                               const tstring& sEnc,
                               TokenFilter* pPreFilter)
    : TokenFilter(pPreFilter)
    , m_pSnowball(new SnowballWrapper(sLanguage.c_str(),
                    sIrregularWords.c_str(), sEnc.c_str()))
{
}

SnowballFilter::~SnowballFilter(void)
{
    if (m_pSnowball)
    {
        delete m_pSnowball;
        m_pSnowball = NULL;
    }
}

void SnowballFilter::setParam(const tstring& sParam)
{
    KeyValueParser pp;
    if (pp.parse(sParam, TokenFilter::PARAM_SEPERATOR,
                 TokenFilter::EQUAL_MARK))
    {
        if (m_pSnowball)
        {
            delete m_pSnowball;
            m_pSnowball = NULL;
        }

        tstring lang, enc;
        if (pp.getValue(_T("language"), lang) == false)
        {
            lang = _T("english");
        }
        pp.getValue(_T("encoding"), enc);
        m_pSnowball = new SnowballWrapper(lang.c_str(), enc.empty() 
                ? NULL : enc.c_str());
    }
}

void SnowballFilter::filterInternal(TokenSourcePtr& tokenSource) const
{
    char* szText;
    size_t nLen;

    char* buffer = NULL;
    size_t nBufSize = 0;

    TokenViewPtr pInputTokens = tokenSource->getLastView();

    TokenViewPtr pOutputTokens = tokenSource->acquireCustomView(_T("stemmed_view"));

    TokenView::Iterator it = pInputTokens->iterator();
    while(it.hasNext())
    {
        const Token& token = it.next();
        szText = (char*)token.getTextValue();
        nLen = token.getTextLength();
        if (szText && nLen > 0)
        {
            if (!buffer || nBufSize < (nLen << 1))
            {
                delete[] buffer;
                nBufSize = (nLen << 1);
                buffer = new char[nBufSize];
            }
            size_t nStemLen = m_pSnowball->stemmer(buffer, nBufSize, szText, nLen);
            pOutputTokens->addToken(buffer, nStemLen, token.getPosIncrement(),
                    token.getStartOffset(), token.getEndOffset());
        }
    }

    delete[] buffer;
}

const tstring& SnowballFilter::getIdentifier() const 
{
    return SnowballFilter::Creator::instance()->getIdentifier();
}

TokenFilter* SnowballFilter::clone() const
{
    return new SnowballFilter(*this);
}

FX_NS_END

