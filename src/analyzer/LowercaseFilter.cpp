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
// Email	: ruijieguo@gmail.com
// Created	: 2007/3/8
//

#include "firtex/analyzer/LowercaseFilter.h"

FX_NS_DEF(analyzer)

SETUP_STREAM_LOGGER(analyzer, LowercaseFilter);

LowercaseFilter::LowercaseFilter(void)
{
}

LowercaseFilter::LowercaseFilter(const LowercaseFilter& src)
    : TokenFilter(src)
{
    
}

LowercaseFilter::LowercaseFilter(TokenFilter* pPreFilter)
    :TokenFilter(pPreFilter)
{
}

LowercaseFilter::~LowercaseFilter(void)
{
}

const tstring& LowercaseFilter::getIdentifier() const 
{
    return LowercaseFilter::Creator::instance()->getIdentifier();
}

void LowercaseFilter::filterInternal(TokenSourcePtr& tokenSource) const
{
    char* szText;
    size_t nLen;
    char cDelt = 'a' - 'A';
    register char* pBegin;
    char* pEnd ;
    TokenViewPtr pTokens = tokenSource->getLastView();
    TokenView::Iterator iter = pTokens->iterator();
    while(iter.hasNext())
    {				
        const Token& token = iter.next();
        szText = (char*)token.getTextValue();
        nLen = token.getTextLength();
        if(szText && nLen > 0)
        {
            pBegin = szText;
            pEnd = szText + nLen;
            while (pBegin < pEnd)
            {
                if(*pBegin >= 'A' && *pBegin <= 'Z')
                    *pBegin += cDelt;
                pBegin++;
            }
        }
    }
}

TokenFilter* LowercaseFilter::clone() const
{
    return new LowercaseFilter(*this);
}

FX_NS_END

