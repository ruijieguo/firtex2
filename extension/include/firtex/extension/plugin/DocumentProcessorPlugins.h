//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-09 00:06:00

#ifndef __FX_DOCUMENTPROCESSORPLUGINS_H
#define __FX_DOCUMENTPROCESSORPLUGINS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/plugin/Plugin.h"
#include "firtex/extension/plugin/DocumentProcessorPlugin.h"
#include <map>

FX_NS_DEF(plugin);

class DocumentProcessorPlugins
{
public:
    typedef std::map<std::string,IDocumentProcessorPlugin*> PluginMap;

public:
    DocumentProcessorPlugins();
    virtual~DocumentProcessorPlugins();

public:	
    /**
     * create a processor plugin by identifier
     * @param identifier identifier of processor
     * @return a processor object			 
     */
    DocumentProcessorPlugin* getProcessorByIdentifier(const std::string& identifier);

    /**
     * clear all plugins
     */
    void clear();

protected:
    /**
     * Load a processor plugin
     * @param pPlugin the plugin 
     * @return the plugin is loaded or not
     */
    bool loadPlugin(const PluginPtr& pPlugin);	

public:
    class Iterator
    {
    public:
        Iterator(DocumentProcessorPlugins* pPlugins)
            : m_pDocProcessorPlugins(pPlugins)
        {
        }
    public:
        /**
         * reset the iterator
         */
        void reset(){m_iterator = m_pDocProcessorPlugins->m_pluginMap.begin();}

        /**
         * whether reach to end or not
         * @return true for not, false for yes
         */
        bool hasNext() const {return ( m_iterator != m_pDocProcessorPlugins->m_pluginMap.end() );}

        /**
         * retrieval current element
         * @return current element
         */
        FX_NS(plugin)::IDocumentProcessorPlugin*next() {return (m_iterator++)->second;}

        /**
         * number of elements
         * @return number of elements
         */
        size_t size()const{return m_pDocProcessorPlugins->m_pluginMap.size();}

    protected:
        DocumentProcessorPlugins* m_pDocProcessorPlugins;
        PluginMap::iterator m_iterator;
    };

public:
    DocumentProcessorPlugins::Iterator iterator();

protected:
    PluginMap m_pluginMap;
    
    friend class Plugins;
    
private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_DOCUMENTPROCESSORPLUGINS_H
