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
#include "firtex/analyzer/FilterFactory.h"
#include "firtex/analyzer/StandardStopFilter.h"
#include "firtex/analyzer/SynonymFilter.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/analyzer/LowercaseFilter.h"
#include <memory>

#ifdef HAVE_SNOWBALL
#include "firtex/contrib/snowball/Snowball.h"
#endif

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, FilterFactory);

FilterFactory::FilterFactory(void)
{
    registerCreator(StandardStopFilter::Creator::instance());
    registerCreator(LowercaseFilter::Creator::instance());
    registerCreator(SynonymFilter::Creator::instance());
#ifdef HAVE_SNOWBALL
    registerCreator(SnowballFilter::Creator::instance());
#endif
}

FilterFactory::~FilterFactory(void)
{
}

TokenFilter* FilterFactory::createFilter(const tstring& sIdentifier,const tstring& sParam)
{
    SCOPED_LOCK();
    TokenFilter* pTokenFilter = NULL;
    TokenFilter::Creator* pCreator = getCreator(sIdentifier);
    if(pCreator)
    {
        pTokenFilter = pCreator->create();
    }
    if(!pTokenFilter)
    {
        FX_STREAM_LOG(ERROR) << _T("Create filter failed:") 
                             << sIdentifier << FIRTEX_ENDL;
        return NULL;
    }
    if (!sParam.empty())
    {
        pTokenFilter->setParam(sParam);
    }

    return pTokenFilter;
}

void FilterFactory::registerCreator(TokenFilter::Creator* pCreator)
{
    SCOPED_LOCK();
    tstring str = pCreator->getIdentifier();
    getCreatorMap().insert(make_pair(str,pCreator));
}

TokenFilter::Creator* FilterFactory::getCreator(const tstring& _identifier)
{
    tstring str = _identifier;
    CreatorMap& am = getCreatorMap();
    CreatorMap::iterator iter = am.find(str);
    if(iter != am.end())
        return iter->second;
    return NULL;
}

FilterFactory::FilterList FilterFactory::getRegisteredFilters()
{
    SCOPED_LOCK();
    FilterList  tl;
    CreatorMap& am = getCreatorMap();
    CreatorMap::iterator iter = am.begin();
    while(iter != am.end())
    {
        tl.push_back(iter->first);
        iter++;
    }
    return tl;
}

FX_NS_END
