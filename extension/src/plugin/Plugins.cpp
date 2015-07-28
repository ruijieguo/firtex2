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
// Created	: 2006/7/9
//
#include "firtex/config/GlobalConfig.h"
#include "firtex/extension/plugin/Plugins.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"

FX_NS_USE(utility);
FX_NS_USE(com);
FX_NS_USE(utility);

FX_NS_DEF(plugin)

SETUP_STREAM_LOGGER(plugin, Plugins);

Plugins::Plugins(void)
{
    enumerate();
}

Plugins::~Plugins(void)
{
    clear();
}

void Plugins::enumerate()
{
    //plugin.xml∏Ò Ω£∫
    // <plugins>
    //	 <plugin>			
    //		<identifier>identifier</identifier>
    //		<clsid></clsid>
    //		<name></name>			
    //		<progid></progid>
    //		<versionindependentprogid></versionindependentprogid>
    //		<filename></filename>	
    //		<enable>true/false</enable>
    //	 </plugin>
    // </plugins>

    try
    {
        Path p(GLOBAL_CONF().General.pluginPath);
        p.makeDirectory();
        p.setFileName(_T("plugins.xml"));
        
        File f(p);
        if (!f.exists())
        {
            return;
        }

        XMLDocumentWrapper xmlDoc;
        xmlDoc.parseFile(p.toString());

        FX_LOG(INFO, "Load plugins configure: [%s]", p.toString().c_str());

        XMLNodeWrapperPtr pRootNode = xmlDoc.firstNode("plugins");
        if (!pRootNode)
        {
            FX_STREAM_LOG(WARN) << _T("Invalid plugins configure file: ")
                                << p.toString() << FIRTEX_ENDL;
            return;
        }

        for (XMLNodeWrapperPtr pNode = pRootNode->firstNode("plugin"); 
             pNode; pNode = pNode->nextSibling("plugin"))
        {
            PluginPtr pPlugin(new Plugin());
            if (pPlugin->load(pNode))
            {
                if (!pPlugin->isEnabled())
                {
                    FX_STREAM_LOG(INFO) << _T("Plugin: [") << pPlugin->getName() << _T("] ")
                                        << _T("is loaded, but disabled.") << FIRTEX_ENDL;
                    continue;
                }
                    
                if(startsWithNoCase(pPlugin->getIdentifier(), tstring(_T("processor"))))
                {

                    if (!ProcessorPlugin.loadPlugin(pPlugin))
                    {
                        FX_STREAM_LOG(WARN) << _T("Load processor plugin FAILED:") 
                                            << pPlugin->getName() << FIRTEX_ENDL;
                        continue;
                    }
                }
                else if(startsWithNoCase(pPlugin->getIdentifier(), tstring(_T("analyzer"))))
                {
                    if(!AnalyzerPlugin.loadPlugin(pPlugin))
                    {
                        FX_STREAM_LOG(WARN) << _T("Install analyzer plugin failed:")
                                            << pPlugin->getName() << FIRTEX_ENDL;
                        continue;
                    }
                }
                else 
                {
                    FX_STREAM_LOG(WARN) << _T("Unkown plugin:") 
                                        << pPlugin->getName() << FIRTEX_ENDL;
                    continue;
                }			
                pPlugin->m_bInstalled = true;
                FX_STREAM_LOG(INFO) << _T("Load plugin success:") << pPlugin->getName() << FIRTEX_ENDL;
                m_plugins.push_back(pPlugin);
            }
        }				
    }
    catch (const FirteXException& e)
    {
        FX_LOG(WARN, "Load plugins FAILED: [%s]", e.what().c_str());
    }
}

void Plugins::clear()
{
    m_plugins.clear();
}

bool Plugins::lookupCLSID(const tstring& identifier, FX_CLSID& pCLSID) const
{	
    PluginPtr pPlugin;
    PluginVector::const_iterator iter = m_plugins.begin();
    while (iter != m_plugins.end())
    {
        pPlugin = *iter;
        if (endsWithNoCase(pPlugin->m_identifier, identifier) )
        {
            pCLSID = pPlugin->m_clsid;
            return pPlugin->m_bEnable;
        }
        iter++;
    }
    return false;
}

bool Plugins::lookupEnable(FX_REFCLSID pCLSID, bool bDefault) const
{
    PluginPtr pPlugin;
    PluginVector::const_iterator iter = m_plugins.begin();
    while (iter != m_plugins.end())
    {
        pPlugin = *iter;
        if( FX_IsEqualCLSID(pCLSID,pPlugin->m_clsid) )
        {					
            return pPlugin->m_bEnable;
        }
        iter++;
    }			
    return bDefault;
}

bool Plugins::lookupInstall(FX_REFCLSID pCLSID) const
{
    PluginPtr pPlugin;
    PluginVector::const_iterator iter = m_plugins.begin();
    while (iter != m_plugins.end())
    {
        pPlugin = *iter;
        if( FX_IsEqualCLSID(pCLSID, pPlugin->m_clsid) )
        {					
            return pPlugin->m_bInstalled;
        }
        iter++;
    }			
    return false;
}

PluginPtr Plugins::findByFileName(const tstring& filename) const
{
    PluginVector::const_iterator iter = m_plugins.begin();
    PluginPtr pPlugin;
    while(iter!=m_plugins.end())
    {
        pPlugin = *iter;
        if(pPlugin->m_filename == filename)
            return (*iter);
        iter++;
    }
    return PluginPtr();
}

PluginPtr Plugins::findByCLSID(FX_CLSID clsid) const
{
    PluginVector::const_iterator iter = m_plugins.begin();
    PluginPtr pPlugin;
    while(iter!=m_plugins.end())
    {
        pPlugin = *iter;
        if( FX_IsEqualCLSID(clsid, pPlugin->m_clsid) )
        {
            return (*iter);
        }
        iter++;
    }
    return PluginPtr();
}

Plugins::Iterator Plugins::iterator() const
{
    return Plugins::Iterator(this);
}

//////////////////////////////////////////////////////////////////////////
//Plugin



FX_NS_END
