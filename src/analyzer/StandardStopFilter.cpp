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
// Created	: 2005/11/20
//
#include "firtex/analyzer/StandardStopFilter.h"
#include "firtex/analyzer/StandardAnalyzer.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Exception.h"
#include "firtex/index/IndexWriter.h"
#include "firtex/utility/KeyValueParser.h"

FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, StandardStopFilter);

StandardStopFilter::StandardStopFilter()
{
}

StandardStopFilter::StandardStopFilter(const StandardStopFilter& src)
    : TokenFilter(src)
    , m_stopwords(src.m_stopwords)
{
}

void StandardStopFilter::init(const tstring& stopfile)
{
    if (loadFile(stopfile) == 0)
    {
        FX_LOG(ERROR, "Load stop word file: [%s] FAILED.", stopfile.c_str());
    }
}

StandardStopFilter::~StandardStopFilter(void)
{

}

size_t StandardStopFilter::loadFile(const tstring& filename)
{
    m_stopwords.clear();

    size_t nWords = 0;
    ifstream ifterm(filename.c_str(), ios::in|ios::binary);
    if (!ifterm.is_open()) 
    {
        return 0;
    }

    StandardAnalyzer analyzer;
    char term[128];
    while (!ifterm.eof())
    {
        ifterm.getline(term, 128);
        TokenViewPtr pTokens = analyzer.tokenize(term, strlen(term));
        if (pTokens->getNumTokens() > 1)
        {
            FX_STREAM_LOG(WARN) << "Stop word: [" << term 
                                << "] contains more than two terms" << FIRTEX_ENDL;
        }
        else if(pTokens->getNumTokens() == 1)
        {
            TokenView::Iterator it = pTokens->iterator();
            const Token& token = it.next();
            assert(token.getHintId() != INVALID_HINTID);
            m_stopwords.insert((termid_t)token.getHintId());
            ++nWords;
        }
        else 
        {
            FX_STREAM_LOG(WARN) << "Ignore stop word: [" << term 
                                << "]." << FIRTEX_ENDL;
        }
    }
    ifterm.close();

    return nWords;
}

size_t StandardStopFilter::loadWords(const tstring& sWords)
{
    m_stopwords.clear();

    size_t nWords = 0;
    StandardAnalyzer analyzer;
    analyzer.init();
    TokenViewPtr pTokens = analyzer.tokenize(sWords.c_str(), sWords.length());
    if (pTokens)
    {
        TokenView::Iterator it = pTokens->iterator();
        while (it.hasNext())
        {
            const Token& token = it.next();
            assert(token.getHintId() != INVALID_HINTID);
            m_stopwords.insert((termid_t)token.getHintId());

            ++nWords;
        }
    }
    return nWords;
}

const tstring& StandardStopFilter::getIdentifier() const 
{
    return StandardStopFilter::Creator::instance()->getIdentifier();
}

void StandardStopFilter::filterInternal(TokenSourcePtr& tokenSource) const
{
    TokenViewPtr pTokenView = tokenSource->getLastView();
    if (!!pTokenView)
    {
        TokenView::Iterator it = pTokenView->iterator();
        while (it.hasNext())
        {
            Token& token = const_cast<Token&>(it.next());
            if (m_stopwords.find((termid_t)token.getHintId()) 
                != m_stopwords.end())
            {
                token.clear();
            }
        }
    }
}

void StandardStopFilter::setParam(const tstring& sParam)
{
    KeyValueParser parser;
    if (parser.parse(sParam, TokenFilter::PARAM_SEPERATOR,
                     TokenFilter::EQUAL_MARK))
    {
        size_t nWords = 0;
        string sValue;
        if (parser.getValue("words", sValue))
        {
            nWords = loadWords(sValue);
        }
        else if (parser.getValue("file", sValue))
        {
            nWords = loadFile(sValue);
        }
        else 
        {
            return ;
        }

        if (nWords == 0)
        {
            FX_LOG(ERROR, "Load stop words: [%s] FAILED.",
                   sValue.c_str());
        }
        else
        {
            FX_LOG(INFO, "Loaded [%u] stop words.", (uint32_t)nWords);
        }

    }
}

TokenFilter* StandardStopFilter::clone() const
{
    return new StandardStopFilter(*this);
}

FX_NS_END
