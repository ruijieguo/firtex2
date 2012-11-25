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
// Created	: 2006/5/9
//
#include "firtex/extension/plugin/AnalyzerPlugins.h"
#include "firtex/extension/plugin/PluginFactory.h"
#include "firtex/extension/plugin/Plugins.h"
#include "firtex/extension/plugin/ComTokenView.h"
#include "firtex/extension/plugin/ComTokenSource.h"
#include "firtex/extension/com/Com.h"
#include "firtex/extension/com/ComPtr.h"

FX_NS_USE(com);
FX_NS_USE(analyzer);
using namespace std;

FX_NS_DEF(plugin);

SETUP_STREAM_LOGGER(plugin, AnalyzerPlugin);

//////////////////////////////////////////////////////////////////////////
//AnalyzerPlugin
AnalyzerPlugin::AnalyzerPlugin(IAnalyzerPlugin* pAnalyzerInterface)
    : m_pAnalyzerInterface(pAnalyzerInterface)
    , m_pAnalyzedTokenVectorInterface(NULL)
    , m_pAnalyzedTokenVector(NULL)
    , m_pUnAnalyzedTokenVectorInterface(NULL)
    , m_pLastUnAnalyzedTokenVector(NULL)
{
    m_pAnalyzerInterface->AddRef();
}

AnalyzerPlugin::~AnalyzerPlugin()
{
    m_pAnalyzerInterface->Release();
    m_pAnalyzerInterface = NULL;

    if(m_pUnAnalyzedTokenVectorInterface)
    {
        delete m_pUnAnalyzedTokenVectorInterface;
        m_pUnAnalyzedTokenVectorInterface = NULL;
    }
}	

Analyzer* AnalyzerPlugin::clone() const
{
    FIRTEX_ASSERT2(false);
    return NULL;
}

void AnalyzerPlugin::doTokenize(TokenSourcePtr& tokenSource) const
{
	ComTokenSource comTokenSrc(const_cast<FX_NS(analyzer)::TokenSource*>(tokenSource.get()));
    if (m_pAnalyzerInterface->tokenize(comTokenSrc.getInterface()) != FX_S_OK)
    {
        FX_LOG(ERROR, "Tokenize FAILED.");
    }
}

const tstring AnalyzerPlugin::getIdentifier() const
{
    FX_BSTR id;
    m_pAnalyzerInterface->getIdentifier(&id);
    
    ComStr comStr;
    comStr.attach(id);

    tstring sId;
    comStr.copyTo(sId);
    
    return sId;
}

//////////////////////////////////////////////////////////////////////////
//AnalyzerPlugins
AnalyzerPlugins::AnalyzerPlugins(void)
{
}

AnalyzerPlugins::~AnalyzerPlugins(void)
{
    clear();
}

Analyzer* AnalyzerPlugins::getAnalyzer(const tchar* analyzerName)
{
    AnalyzerPlugin* pAnalyzer = NULL;
    map<tstring,IAnalyzerPlugin*>::iterator iter = m_pluginMap.find(analyzerName);
    if(iter != m_pluginMap.end())
    {
        pAnalyzer = new AnalyzerPlugin(iter->second);
        return pAnalyzer;
    }
    FX_CLSID	clsid;
    IAnalyzerPlugin* pInterface = NULL;
    if(FX_NS(plugin)::Plugins::instance()->lookupCLSID(analyzerName,clsid))
    {
        bool ret = PluginFactory::instance()->createInstance(clsid, NULL,
                IAnalyzerPlugin::iid, (void**)&pInterface);
        if(ret)
        {
            m_pluginMap.insert(make_pair((tstring)analyzerName, pInterface));
            pAnalyzer = new AnalyzerPlugin(pInterface);
            return pAnalyzer;
        }
    }
    return NULL;
}		

bool AnalyzerPlugins::loadPlugin(const PluginPtr& pPlugin)
{
    IAnalyzerPlugin* pInterface = NULL;
    bool ret = PluginFactory::instance()->createInstance(pPlugin, NULL,
            IAnalyzerPlugin::iid, (void**)&pInterface);

    if (ret)
    {
        m_pluginMap.insert(make_pair(pPlugin->getIdentifier(), pInterface));				
        return true;
    }
    return false;
}

void AnalyzerPlugins::clear()
{
    map<tstring,IAnalyzerPlugin*>::iterator iter = m_pluginMap.begin();
    while(iter != m_pluginMap.end())
    {
        iter->second->Release();				
        iter++;
    }
    m_pluginMap.clear();
}

FX_NS_END
