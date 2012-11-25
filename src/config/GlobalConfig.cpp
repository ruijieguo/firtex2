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
// Created	: 2005/10/3
//
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Environment.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/MiscUtil.h"
#include "firtex/config/XMLConfigurator.h"
#include "firtex/config/Configurator.h"
#include "firtex/common/Any.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include <fstream>

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(config);
FX_NS_USE(index);
FX_NS_USE(common);

#ifdef WIN32
#ifndef STRICT
#define STRICT
#endif	
#include <windows.h>
#endif

FX_NS_DEF(config);

SETUP_STREAM_LOGGER(config, GlobalConfig);

//////////////////////////////////////////////////////////////////////////
//GlobalConfig

GlobalConfig::GlobalConfig()
{
    Storage.filesystem = "filesystem.mmap";
    Merge.strategy = "nomerge";

    Build.memory = 16; //16 MB
    Build.buildThreadCount = DEFAULT_BUILDING_THEAD_COUNT;
    Build.documentQueueSize = DEFAULT_DOCUMENT_QUEUE_SIZE;
    Build.maxIndexTermsPerDoc = 100000;

    Merge.maxAllowedOpenFiles = 1024;

    Advance.Posting.skipInterval = 8;
    Advance.Posting.maxLevel = 2;

    std::string sPath = MiscUtil::getSelfPath();
    bool bIsSo = false;
    Path p(sPath);
    if (startsWithNoCase(p.getFileName(), string("lib")))
    {
        bIsSo = true;
    }
    p.setFileName("");
    General.path = p.toString();
    p.popDirectory();
    if (bIsSo)
    {
        p.pushDirectory("firtex2");
    }
    p.pushDirectory("plugins");
    General.pluginPath = p.toString();

    p.popDirectory();
    p.pushDirectory("sysdata");
    General.dictionaryPath = p.toString();
}

GlobalConfig::~GlobalConfig(void)
{
}

//////////////////////////////////////////////////////////////////////
// GlobalConfig load

void GlobalConfig::update()
{
}

void GlobalConfig::makePathsAbsolute(const string& sBasePath)
{
    makePathAbsolute(sBasePath, General.pluginPath);
    makePathAbsolute(sBasePath, General.dictionaryPath);
}

//static
void GlobalConfig::makePathAbsolute(const string& sBasePath, string& sPath)
{
    Path p(sPath);
    p.makeDirectory();
    if (p.isRelative())
    {
        p.makeAbsolute(sBasePath);
        sPath = p.toString();
    }
}

void GlobalConfig::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator rootConf;
        
        Configurator genConf;
        General.configure(genConf);
        rootConf.configure("general", genConf.getMap());

        Configurator fsConf;
        Storage.configure(fsConf);
        rootConf.configure("storage", fsConf.getMap());

        Configurator indexConf;
        Build.configure(indexConf);
        rootConf.configure("build", indexConf.getMap());

        Configurator mergeConf;
        Merge.configure(mergeConf);
        rootConf.configure("merge", mergeConf.getMap());

        Configurator advanceConf;
        Advance.configure(advanceConf);
        rootConf.configure("advance", advanceConf.getMap());

        conf.configure("global", rootConf.getMap());
    }
    else 
    {
        Configurator::Iterator rootIt = conf.findConf("global");
        if (rootIt.hasNext())
        {
            Configurator::ConfMap confMap =
                AnyCast<Configurator::ConfMap>(rootIt.next().second);
            Configurator c(confMap);
            Configurator::Iterator it = c.findConf("general");
            if (it.hasNext())
            {
                Configurator genConf(AnyCast<Configurator::ConfMap>(it.next().second));
                General.configure(genConf);
            }

            it = c.findConf("storage");
            if (it.hasNext())
            {
                Configurator fsConf(AnyCast<Configurator::ConfMap>(it.next().second));
                Storage.configure(fsConf);
            }

            it = c.findConf("build");
            if (it.hasNext())
            {
                Configurator indexConf(AnyCast<Configurator::ConfMap>(it.next().second));
                Build.configure(indexConf);
            }

            it = c.findConf("merge");
            if (it.hasNext())
            {
                Configurator mergeConf(AnyCast<Configurator::ConfMap>(it.next().second));
                Merge.configure(mergeConf);
            }
        
            it = c.findConf("advance");
            if (it.hasNext())
            {
                Configurator advanceConf(AnyCast<Configurator::ConfMap>(it.next().second));
                Advance.configure(advanceConf);
            }
            update();
        }
    }
}

void GlobalConfig::sGeneral::configure(Configurator& conf)
{
    conf.configure("path", path, path);
    conf.configure("plugin_path", pluginPath, pluginPath);
    conf.configure("dictionary_path", dictionaryPath, dictionaryPath);
}

void GlobalConfig::sStorage::configure(Configurator& conf)
{
    conf.configure("filesystem", filesystem, filesystem);
}

void GlobalConfig::sBuild::sIndexCleaner::configure(Configurator& conf)
{
    conf.configure("strategy", strategy, "");
    conf.configure("parameter", param, "");
}

void GlobalConfig::sBuild::configure(Configurator& conf)
{
    conf.configure("memory", memory, memory);
    conf.configure("build_thread_count", buildThreadCount, buildThreadCount);
    conf.configure("document_queue_size", documentQueueSize, documentQueueSize);
    conf.configure("max_terms", maxIndexTermsPerDoc,
                   maxIndexTermsPerDoc);

    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator cleanerConf;
        IndexCleaner.configure(cleanerConf);
        conf.configure("index_cleaner", cleanerConf.getMap());
    }
    else 
    {
        Configurator::ConfMap confMap = conf.getMap();
        Configurator c(confMap);

        Configurator::Iterator it = c.findConf("index_cleaner");
        if (it.hasNext())
        {
            Configurator cleanerConf(AnyCast<Configurator::ConfMap>(it.next().second));
            IndexCleaner.configure(cleanerConf);
        }
    }
}

void GlobalConfig::sMerge::configure(Configurator& conf)
{
    conf.configure("strategy", strategy, "");
    conf.configure("param", param, "");
    conf.configure("max_allowed_open_files", maxAllowedOpenFiles, (uint32_t)1024);
}

void GlobalConfig::sAdvance::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator advConf;

        Configurator postingConf;
        Posting.configure(postingConf);
        advConf.configure("posting", postingConf.getMap());
    }
    else 
    {
        Configurator::ConfMap confMap = conf.getMap();
        Configurator c(confMap);

        Configurator::Iterator it = c.findConf("posting");
        if (it.hasNext())
        {
            Configurator postingConf(AnyCast<Configurator::ConfMap>(it.next().second));
            Posting.configure(postingConf);
        }
    }
}

void GlobalConfig::load(const std::string& sCfgPath)
{
    XMLConfigurator configurator;
    configurator.load(sCfgPath);
    configure(configurator);
    makePathsAbsolute(configurator.getDir());
}

FX_NS_END

