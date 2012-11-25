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
#ifndef _PLUGINS_H
#define _PLUGINS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "firtex/extension/com/Com.h"
#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Logger.h"
#include "firtex/extension/plugin/DocumentProcessorPlugins.h"
#include "firtex/extension/plugin/Plugin.h"
#include "firtex/extension/plugin/AnalyzerPlugins.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/thread/SynchronizedObject.h"
#include "firtex/utility/Iterator.h"
#include "firtex/utility/Singleton.h"
#include "firtex/thread/TSS.h"

FX_NS_DEF(plugin)
	
class Plugins : public FX_NS(utility)::Singleton<Plugins>
{
public:
    typedef std::vector<PluginPtr> PluginVector;

protected:
    Plugins(void);
    ~Plugins(void);

public:
    bool lookupCLSID(const tstring& identifier, FX_NS(com)::FX_CLSID& pCLSID) const;
    bool lookupEnable(FX_NS(com)::FX_REFCLSID pCLSID, bool bDefault) const;
    bool lookupInstall(FX_NS(com)::FX_REFCLSID pCLSID) const;

public:
    /**
     * find plugin by file name
     * @param fielname file name of plugin
     * @return plugin
     */
    PluginPtr findByFileName(const tstring& filename) const;

    /**
     * @param clisid CLSID of plugin
     * @return plugin object
     */
    PluginPtr findByCLSID(FX_NS(com)::FX_CLSID clsid) const;
        
protected:
    /**
     * clear plugins
     */
    void clear();

    /**
     * enumerate plugins configured in plugins/config.xml
     */
    void enumerate();

public:
    class Iterator
    {
    public:
        Iterator(const Plugins* pPlugins)
            : m_pPlugins(pPlugins)
        {
        }
    public:
        /**
         * reset the iterator
         */
        void reset(){m_iterator = m_pPlugins->m_plugins.begin();}

        /**
         * whether reach to end or not
         * @return true for not, false for yes
         */
        bool hasNext() const{return ( m_iterator != m_pPlugins->m_plugins.end() );}

        /**
         * retrieval current element
         * @return current element
         */
        PluginPtr next() {return *m_iterator++;}

        /**
         * number of elements
         * @return number of elements
         */
        size_t size() const {return m_pPlugins->m_plugins.size();}

    protected:
        const Plugins* m_pPlugins;

        PluginVector::const_iterator m_iterator;
    };	
public:
    /**
     * get iterator of plugins
     */
    Plugins::Iterator iterator() const;

public:
    DocumentProcessorPlugins ProcessorPlugin;
    AnalyzerPlugins AnalyzerPlugin;

protected:
    PluginVector m_plugins;

    DECLARE_LAZY_SINGLETON(Plugins);

private:
    DECLARE_STREAM_LOGGER();
};		
//////////////////////////////////////////////////////////////////////////
//

FX_NS_END

#endif
