//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: GuoRuijie
// Email	: ruijieguo@gmail.com
// Created	: 2005/11/20
//
#include "firtex/analyzer/Analyzer.h"
#include "firtex/document/Field.h"
#include "firtex/analyzer/AnalyzerFactory.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/analyzer/LowercaseFilter.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/config/GlobalConfig.h"

FX_NS_USE(utility);
FX_NS_USE(config);

FX_NS_DEF(analyzer);

Analyzer::Analyzer()
    : m_pAnteTokenFilter(NULL)
    , m_pPostTokenFilter(NULL)
    , m_nMaxTokensToAnalyze(GLOBAL_CONF().Build.maxIndexTermsPerDoc)
{		
}

Analyzer::Analyzer(const Analyzer& src)
    : m_pAnteTokenFilter(NULL)
    , m_pPostTokenFilter(NULL)
    , m_nMaxTokensToAnalyze(src.m_nMaxTokensToAnalyze)
{
    if (src.m_pAnteTokenFilter)
    {
        m_pAnteTokenFilter = src.m_pAnteTokenFilter->clone();
    }
    if (src.m_pPostTokenFilter)
    {
        m_pPostTokenFilter = src.m_pPostTokenFilter->clone();
    }
}

Analyzer::~Analyzer(void)
{				
    if (m_pPostTokenFilter)
    {
        delete m_pPostTokenFilter;
        m_pPostTokenFilter = NULL;
    }
    if (m_pAnteTokenFilter)
    {
        delete m_pAnteTokenFilter;
        m_pAnteTokenFilter = NULL;
    }
}
			
void Analyzer::attachPostFilter(TokenFilter* pPostTokenFilter)
{
    if (m_pPostTokenFilter)
    {
        m_pPostTokenFilter->attachFilter(pPostTokenFilter);
    }
    else 
    {
        m_pPostTokenFilter = pPostTokenFilter;
    }
}

TokenFilter* Analyzer::detachPostFilter()
{
    TokenFilter* pPostFilter = m_pPostTokenFilter;
    m_pPostTokenFilter = NULL;
    return pPostFilter;
}

void Analyzer::attachAnteFilter(TokenFilter* pAnteTokenFilter)
{
    if (m_pAnteTokenFilter)
        m_pAnteTokenFilter->attachFilter(pAnteTokenFilter);
    else 
        m_pAnteTokenFilter = pAnteTokenFilter;
}

TokenFilter* Analyzer::detachAnteFilter()
{
    TokenFilter* pAnteFilter = m_pAnteTokenFilter;
    m_pAnteTokenFilter = NULL;
    return pAnteFilter;
}

TokenFilter* Analyzer::detachAnteFilter(const std::string& szIdentifier)
{
    TokenFilter* pAnteFilter = m_pAnteTokenFilter;
    if (pAnteFilter)
    {
        if (startsWithNoCase(pAnteFilter->getIdentifier(), szIdentifier))
        {            
            m_pAnteTokenFilter = NULL;
            return pAnteFilter;
        }
        else
            return pAnteFilter->detachFilter(szIdentifier);
    }
    return NULL;
}					

TokenViewPtr Analyzer::tokenize(const char* szText, size_t length) const
{
    TokenSourcePtr tokenSource(new TokenSource);
    tokenSource->setOriginalView(szText, length);
    return tokenize(tokenSource);
}

TokenViewPtr Analyzer::tokenize(TokenSourcePtr& tokenSource) const
{
    if (m_pAnteTokenFilter)
        m_pAnteTokenFilter->filter(tokenSource);
    doTokenize(tokenSource);
    if (m_pPostTokenFilter)
        m_pPostTokenFilter->filter(tokenSource);
    return tokenSource->getLastView();
}

void Analyzer::setParam(const std::string& pszParam)
{
}

FX_NS_END
