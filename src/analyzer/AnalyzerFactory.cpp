//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2005/11/20
//
#include "firtex/config/GlobalConfig.h"
#include "firtex/analyzer/AnalyzerFactory.h"
#include "firtex/analyzer/StandardAnalyzer.h"
#include "firtex/analyzer/DateTimeAnalyzer.h"
#include "firtex/analyzer/NumericAnalyzer.h"
#include "firtex/analyzer/StandardStopFilter.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/File.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/analyzer/LowercaseFilter.h"
#include "firtex/analyzer/KeywordAnalyzer.h"
#include "firtex/analyzer/SingleWordAnalyzer.h"

#include <memory>

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, AnalyzerFactory);

AnalyzerFactory::AnalyzerFactory(void)
{
    registerCreator(StandardAnalyzer::Creator::instance());
    registerCreator(SingleWordAnalyzer::Creator::instance());
    registerCreator(DateTimeAnalyzer::Creator::instance());
    registerCreator(NumericAnalyzerCreator<int32_t>::instance());
    registerCreator(NumericAnalyzerCreator<int64_t>::instance());
    registerCreator(KeywordAnalyzer::Creator::instance());
}

AnalyzerFactory::~AnalyzerFactory(void)
{
}

Analyzer* AnalyzerFactory::createAnalyzer(const std::string& sIdentifier,
        const std::string& sParam)
{
    SCOPED_LOCK();
    Analyzer* pAnalyzer = NULL;
    Analyzer::Creator* pCreator = getCreator(sIdentifier);
    if (pCreator)
    {
        pAnalyzer = pCreator->create();
    }
    if (!pAnalyzer)
    {
        FX_LOG(ERROR, _T("Create analyzer[%s] FAILED"), sIdentifier.c_str());
        return NULL;
    }

    if (!sParam.empty())
    {
        pAnalyzer->setParam(sParam);
    }

    pAnalyzer->init();

    return pAnalyzer;
}

void AnalyzerFactory::registerCreator(Analyzer::Creator* pCreator)
{
    SCOPED_LOCK();
    std::string str = pCreator->getIdentifier();
    getCreatorMap().insert(make_pair(str,pCreator));
}

Analyzer::Creator* AnalyzerFactory::getCreator(const std::string& sIdentifier)
{
    CreatorMap& am = getCreatorMap();
    CreatorMap::iterator iter = am.find(sIdentifier);
    if (iter != am.end())
        return iter->second;
    return NULL;
}

AnalyzerList AnalyzerFactory::getRegisteredAnalyzers()
{
    SCOPED_LOCK();
    AnalyzerList  al;
    CreatorMap& am = getCreatorMap();
    CreatorMap::iterator iter = am.begin();
    while(iter != am.end())
    {
        al.push_back(iter->first);
        iter++;
    }
    return al;
}

FX_NS_END
