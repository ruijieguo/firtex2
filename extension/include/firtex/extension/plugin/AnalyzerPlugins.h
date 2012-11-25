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
#ifndef __FX_ANALYZERPLUGINS_H
#define __FX_ANALYZERPLUGINS_H

#include <map>
#include <string>
#include "firtex/analyzer/Analyzer.h"
#include "firtex/extension/plugin/Plugin.h"
#include "FirteXCOM.h"

FX_NS_DEF(plugin);
	
class Plugin;
class AnalyzerPlugin : public FX_NS(analyzer)::Analyzer
{
public:
    AnalyzerPlugin(IAnalyzerPlugin* pAnalyzerInterface);
    virtual ~AnalyzerPlugin();

public:			
    /**
     * get identifier of analyzer
     * @return the identifier
     */
    const tstring getIdentifier() const;

    /**
     * Clone a new object
     */
    FX_NS(analyzer)::Analyzer* clone() const;

protected:
    void doTokenize(FX_NS(analyzer)::TokenSourcePtr& tokenSource) const;

protected:
    IAnalyzerPlugin* m_pAnalyzerInterface;
    ITokenView* m_pAnalyzedTokenVectorInterface;
    FX_NS(analyzer)::TokenView*	m_pAnalyzedTokenVector;

    ITokenView* m_pUnAnalyzedTokenVectorInterface;
    FX_NS(analyzer)::TokenView* m_pLastUnAnalyzedTokenVector;

private:
    DECLARE_STREAM_LOGGER();
};

class AnalyzerPlugins
{
public:
    AnalyzerPlugins(void);
    ~AnalyzerPlugins(void);
public:
    FX_NS(analyzer)::Analyzer* getAnalyzer(const tchar* analyzerName);

    /**
     * load a analyzer plugin
     * @param pPlugin the plugin 
     * @return the plugin is loaded or not
     */
    bool loadPlugin(const PluginPtr& pPlugin);
protected:
    /**
     * clear all plugins
     */
    void clear();

protected:
    std::map<tstring, IAnalyzerPlugin*> m_pluginMap;

};

FX_NS_END

#endif
