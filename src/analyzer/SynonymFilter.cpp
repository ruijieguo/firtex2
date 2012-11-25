//
// Copyright(C) 2005--2009 FirteX Develop Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/17
//
#include "firtex/analyzer/SynonymFilter.h"

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, SynonymFilter);

SynonymFilter::SynonymFilter(void)			
{
}

SynonymFilter::SynonymFilter(const SynonymFilter& src)
    : TokenFilter(src)
    , m_synonyms(src.m_synonyms)
{
}

SynonymFilter::SynonymFilter(TokenFilter* pPreFilter)
    : TokenFilter(pPreFilter)
{
}

SynonymFilter::~SynonymFilter(void)
{
}

void SynonymFilter::loadSynonyms(const tstring& szSynFile)
{
    m_synonyms.close();
    m_synonyms.load(szSynFile);
}

void SynonymFilter::filterInternal(TokenSourcePtr& tokenSource) const
{
    TokenViewPtr pTokenView = tokenSource->getLastView();
    
    if (!pTokenView 
        || (pTokenView->getTokenType() != Token::TT_CWORD
            && pTokenView->getTokenType() != Token::TT_STRING))
    {
        return;
    }

    TokenViewPtr pSynTokens = tokenSource->acquireCustomView(_T("synonym_view"));

    char* szText;
    TokenView::Iterator iter = pTokenView->iterator();
    while(iter.hasNext())
    {
        const Token& token = iter.next();
        szText = (char*)token.getTextValue();
        if (szText)
        {
            pSynTokens->addToken(token.getTextValue(), token.getTextLength(),
                    token.getPosIncrement(), token.getStartOffset(),
                    token.getEndOffset());
            SynonymMap::Group* pGroup = m_synonyms.getSynonyms(szText);
            if (pGroup)
            {
                for (SynonymMap::Group::iterator it = pGroup->begin();
                    it != pGroup->end(); ++it)
                {
                    const std::string& str = *it;
                    pSynTokens->addToken(str.c_str(), str.length(), (int32_t)0,
                            token.getStartOffset(), token.getEndOffset());
                    pSynTokens->setTokenType(token.getTokenType());
                }
            }
        }
    }
}

void SynonymFilter::setParam(const tstring& sParam)
{
    loadSynonyms(sParam);
}

TokenFilter* SynonymFilter::clone() const
{
    return new SynonymFilter(*this);
}

FX_NS_END
