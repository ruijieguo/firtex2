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
// Created      : 2005/11/20

#include "firtex/analyzer/StandardAnalyzer.h"
#include "firtex/analyzer/StandardTokenizer.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/analyzer/UnigramSegmenter.h"
#include "firtex/analyzer/MaxForwardSegmenter.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/File.h"

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, StandardAnalyzer);

tstring StandardAnalyzer::PARAM_ENCODE = _T("encode");
tstring StandardAnalyzer::PARAM_ALGORITHM = _T("algorithm");

StandardAnalyzer::StandardAnalyzer()
    : m_pSegmenter(NULL)
    , m_eSegAlgorithm(SA_MAX_FORWARD)
    , m_sEncodeName("utf-8")
{
}

StandardAnalyzer::StandardAnalyzer(const StandardAnalyzer& src)
    : Analyzer(src)
    , m_pSegmenter(NULL)
    , m_eSegAlgorithm(src.m_eSegAlgorithm)
    , m_sEncodeName(src.m_sEncodeName)
{
    if (src.m_pSegmenter)
    {
        m_pSegmenter = src.m_pSegmenter->clone();
    }
}

StandardAnalyzer::~StandardAnalyzer(void)
{
    delete m_pSegmenter;
    m_pSegmenter = NULL;
}

void StandardAnalyzer::init()
{
    tstring dict = getCoreDictPath(GLOBAL_CONF().General.dictionaryPath);
    File f(dict);
    if (!f.exists())
    {
        FX_LOG(ERROR, _T("Core dictionary: [%s] not found"), dict.c_str());
        FIRTEX_THROW(FileIOException, _T("Load dictionary FAILED."));
        return;
    }

    init(dict);
}

void StandardAnalyzer::init(const tstring& sDictPath)
{
    TokenizerPtr pTok;
    if (m_sEncodeName == "gbk" || m_sEncodeName == "gb2312")
    {
        pTok = new GBKStandardTokenizer();
    }
    else
    {
        pTok = new UTF8StandardTokenizer();
    }

    if (m_eSegAlgorithm == SA_UNIGRAM)
    {
        m_pSegmenter = new UnigramSegmenter(pTok);
    }
    else 
    {
        m_pSegmenter = new MaxForwardSegmenter(pTok);
    }

    try
    {
        m_pSegmenter->init(sDictPath);
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, _T("Load dictionary [%s] FAILED: [%s]"),
               sDictPath.c_str(), e.what().c_str());
        throw;
    }
}

void StandardAnalyzer::doTokenize(TokenSourcePtr& tokenSource) const
{
    FIRTEX_ASSERT2(m_pSegmenter != NULL);

    TokenViewPtr pInTokenView = tokenSource->getLastView();
    TokenViewPtr pOutTokenView = tokenSource->acquireCustomView(_T("standard_view"));
    pOutTokenView->reserve(getMaxTokensToAnalyze());
    pOutTokenView->setTokenType(Token::TT_CWORD);
     
    TokenView::Iterator iter = pInTokenView->iterator();
    while(iter.hasNext())
    {
        const Token& token = iter.next();
        m_pSegmenter->segment(pOutTokenView,
                              token.getTextValue(),
                              token.getTextLength());
    }
}

void StandardAnalyzer::setParam(const tstring& sParam)
{
    KeyValueParser kvParser;
    bool ret = kvParser.parse(sParam, ";", "=");
    if (!ret)
    {
        FX_LOG(ERROR, "Invalid analyzer parameter: [%s]",
               sParam.c_str());
    }
    else
    {
        tstring sValue;
        if (kvParser.getValue(PARAM_ALGORITHM, sValue))
        {
            if (!strCompareNoCase(sValue.c_str(), "max_forward"))
            {
                m_eSegAlgorithm = SA_MAX_FORWARD;
            }
            else if (!strCompareNoCase(sValue.c_str(), "unigram"))
            {
                m_eSegAlgorithm = SA_UNIGRAM;
            }
            else
            {
                FX_LOG(ERROR, "Invalid parameter: [algorithm=%s]",
                       sValue.c_str());
                m_eSegAlgorithm = SA_MAX_FORWARD;
            }
        }
        
        if (kvParser.getValue(PARAM_ENCODE, sValue))
        {
            if ((!strCompareNoCase(sValue.c_str(), "utf-8"))
                || (!strCompareNoCase(sValue.c_str(), "gbk")) 
                || (!strCompareNoCase(sValue.c_str(), "gb2312")))
            {
                m_sEncodeName = toLower(sValue);
            }
            else
            {
                FX_LOG(ERROR, "Invalid parameter: [encode=%s], "
                       "using default [encode=utf-8]",
                       sValue.c_str());
                m_sEncodeName = toLower(sValue);
            }
        }
    }
}

const tstring StandardAnalyzer::getIdentifier() const 
{
    return StandardAnalyzer::Creator::instance()->getIdentifier();
}

Analyzer* StandardAnalyzer::clone() const
{
    return new StandardAnalyzer(*this);
}

FX_NS_END

