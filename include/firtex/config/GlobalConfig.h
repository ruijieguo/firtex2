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
// Created      : 2005/12/3
//

#ifndef __GLOBALCONFIGURATOR_H
#define __GLOBALCONFIGURATOR_H

#include <list>
#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/config/Configurable.h"
#include "firtex/config/Configurator.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/Singleton.h"

#define GLOBAL_CONF() (*(FX_NS(config)::GlobalConfig::instance()))

FX_NS_DEF(config)

class Configurator;

class GlobalConfig : public Configurable,
                     public FX_NS(utility)::Singleton<GlobalConfig>
{
public:
    const static size_t DEFAULT_BUILDING_THEAD_COUNT = 8;
    const static size_t DEFAULT_DOCUMENT_QUEUE_SIZE = 500;

public:
    GlobalConfig(void);
    ~GlobalConfig(void);
    
public:
    /**
     * Load configure from file
     */
    void load(const std::string& sCfgPath);
    void configure(Configurator& conf);

    /**
     * Make configured paths absolute
     */
    void makePathsAbsolute(const std::string& sBasePath);

public:
    struct sGeneral : public Configurable
    {
        std::string path;        ///path of the program
        std::string pluginPath;  ///path of plug-ins
        std::string dictionaryPath;

    public:
        void configure(Configurator& conf);
    };
    sGeneral General;

    struct sStorage : public Configurable
    {
        std::string filesystem;
        void configure(Configurator& conf);
    };
    sStorage Storage;

    struct sBuild : public Configurable
    {
        int64_t memory;                /// Memory pool size (in MB) for indexing
        int32_t buildThreadCount;      /// Number of threads for index building
        int32_t documentQueueSize;     /// Document queue size for index building
        int32_t maxIndexTermsPerDoc;   /// Max index words of each document
        
        struct sIndexCleaner : public Configurable
        {
            std::string strategy;
            std::string param;

        public:
            void configure(Configurator& conf);
        };
        sIndexCleaner IndexCleaner;

    public:
        void configure(Configurator& conf);
    };
    sBuild Build;

    struct sMerge : public Configurable 
    {
        std::string strategy;      ///index merge strategy
        std::string param;         ///index merge parameter

        uint32_t maxAllowedOpenFiles;
    public:
        void configure(Configurator& conf);
    };
    sMerge Merge;

    struct sSearch : public Configurable 
    {
        /// refresh timer in seconds, <=0 means no refresh
        int32_t refresh_timer;

    public:
        void configure(Configurator& conf)
        {
            conf.configure("refresh_timer", refresh_timer, 0);
        }
    };
    sSearch Search;

    struct sAdvance  : public Configurable
    {
        struct sPosting  : public Configurable
        {
            int32_t skipInterval;   ///skip interval
            int32_t maxLevel;       ///max skip level

        public:
            void configure(Configurator& conf)
            {
                conf.configure(_T("skip_interval"), skipInterval, skipInterval);
                conf.configure(_T("maxLevel"), maxLevel, maxLevel);
            }
        };
        sPosting Posting;

    public:
        void configure(Configurator& conf);
    };
    sAdvance Advance;

protected:
    /** do some after load config parameter */
    void update();

private:
    static void makePathAbsolute(const std::string& sBasePath,
                                 std::string& sPath);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
