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
// Created	: 2005/11/9
//
#include "firtex/extension/collection/Collection.h"
#include "firtex/extension/collection/CollectionFactory.h"
#include "firtex/analyzer/AnalyzerFactory.h"
#include "firtex/document/DocumentType.h"
#include "firtex/extension/plugin/Plugins.h"
#include "firtex/extension/collection/CollectionTemplate.h"
#include "firtex/common/Logger.h"
#include "firtex/config/XMLConfigurator.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/KeyValueParser.h"
#include <memory>
#include <sstream>

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(config);
FX_NS_USE(document);
FX_NS_USE(common);
FX_NS_USE(index);
FX_NS_USE(thread);
FX_NS_USE(processor);

FX_NS_DEF(collection);

///////////////////////////////////////////////////
///Collection

SETUP_STREAM_LOGGER(collection, Collection);

Collection::Collection()
    : m_bStop(false)
{
    FX_NS(plugin)::Plugins::instance();
}

Collection::~Collection()
{
    stop();
}

void Collection::init(const CollectionConfigurePtr& pConfigure)
{
    sanityCheck(pConfigure);

    m_pConfigure = pConfigure;

    if (!pConfigure->Collection.parameter.empty())
    {
        initParam(pConfigure->Collection.parameter);
    }

    if (m_pConfigure->getTemplate().isNotNull())
    {
        m_pConfigure->getTemplate()->makeSure(m_pConfigure->getDocSchema().get());
    }

    m_pInputDocQueue = new InputDocQueue(m_pConfigure->Collection.inputDocQueueSize);

    Index::AccessMode am = Index::WRITE;
    if (!strCompareNoCase(m_pConfigure->Collection.buildMode.c_str(), _T("batch") ))
        am = Index::WRITE;
    else if (!strCompareNoCase(m_pConfigure->Collection.buildMode.c_str(), _T("append")))
        am = Index::APPEND;

    m_pIndex = new Index();
    m_pIndex->open(m_pConfigure->Collection.target, am, m_pConfigure->getDocSchema().get());
    m_pWriter = m_pIndex->acquireWriter();
    if (m_pWriter.isNull())
    {
        FIRTEX_THROW(NullPointerException, _T("Acquire index writer FAILED."));
    }
    if (m_pConfigure->Collection.autoCommit.maxDocs != 0
        || m_pConfigure->Collection.autoCommit.maxTime != 0)
    {
        CommitCondition cond;
        cond.maxDocs = m_pConfigure->Collection.autoCommit.maxDocs;
        cond.maxTime = m_pConfigure->Collection.autoCommit.maxTime;
        m_pWriter->setAutoCommit(cond);
    }

    m_pWriter->setEncoding(m_pConfigure->Collection.encoding);

    doInit(m_pConfigure);
}

void Collection::sanityCheck(const CollectionConfigurePtr& pConfigure)
{
    FIRTEX_ASSERT((pConfigure.isNotNull()),
                  _T("Invalid argument:Collection.buildIndex()"));

    if (pConfigure->Collection.source.empty())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Source collection path is invalid."));
    }

    if (pConfigure->Collection.target.empty())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Location of indexes is invalid."));
    }

    if (strCompareNoCase(pConfigure->Collection.buildMode.c_str(), _T("batch"))
        && strCompareNoCase(pConfigure->Collection.buildMode.c_str(), _T("append")))
    {
        FIRTEX_THROW(BadParameterException, "Invalid index build mode: [%s], "
                     "option: [batch/append].", pConfigure->Collection.buildMode.c_str());
    }
}

void Collection::scan(bool bBlock)
{
    m_pFileFetcher = createFileFetcher();
    if (m_pFileFetcher.isNull())
    {
        FIRTEX_THROW(UnImplementException, "Create file fetcher FAILED.");
    }
    
    if (bBlock)
    {
        doScan();
        waitStop();
    }
    else
    {
        m_pScanThread = new Thread();
        m_pScanThread->start(*this);
    }
}

void Collection::stop()
{
    m_bStop = true;
    waitStop();
}

void Collection::waitStop()
{
    if (m_pScanThread.isNotNull())
    {
        m_pScanThread->join();
        m_pScanThread.reset();
    }

    if (m_pThreadPool.isNotNull())
    {
        for (size_t i = 0; i < m_processors.size(); ++i)
        {
            m_pInputDocQueue->enqueue(RawDocumentPtr());
        }

        m_pThreadPool->joinAll();
        m_pThreadPool.reset();
    }

    if (m_pIndex.isNotNull())
    {
        m_pIndex->close();
        m_pIndex.reset();
    }
}

void Collection::run()
{
    doScan();
}

void Collection::doScan()
{
    initProcessThreads();

    while (!m_bStop)
    {
        RawDocumentPtr pRawDoc = new RawDocument();
        if (m_pFileFetcher->fetchNext(pRawDoc))
        {
            m_pInputDocQueue->enqueue(pRawDoc);
        }
        else
        {
            break;
        }
    }
}

void Collection::initProcessThreads()
{
    m_pThreadPool = new ThreadPool(m_pConfigure->Collection.processThreadCount,
                                   m_pConfigure->Collection.processThreadCount);

    for (size_t i = 0; i < m_pThreadPool->capacity(); ++i)
    {
        ProcessorRunnerPtr pRunner = new ProcessorRunner(*this);
        m_pThreadPool->start(*pRunner);
        m_processors.push_back(pRunner);
    }
}

void Collection::doProcess()
{
    DocumentProcessorPtr pDocProcessor = createDocumentProcessor();

    DocumentSource source(m_pConfigure->getDocSchema().get());
    while (true)
    {
        source.reset();
        RawDocumentPtr pRawDoc = m_pInputDocQueue->waitDequeue();
        if (pRawDoc.isNotNull())
        {
            source.setRawDocument(pRawDoc);
            do 
            {
                pDocProcessor->process(source);
                DocumentPtr pDoc = source.stealLastDocument();
                if (pDoc.isNotNull() && pDoc->getAction() != Document::AT_NONE)
                {
                    m_pWriter->addDocument(pDoc);
                    if (m_pObserver.isNotNull())
                    {
                        m_pObserver->step();
                    }
                }
            } while (source.toBeContinued());
        }
        else 
        {
            if (m_pObserver.isNotNull())
            {
                m_pObserver->complete();
            }
            break;
        }
    }
}

void Collection::setObserver(const ProgressObserverPtr& pObserver)
{
    m_pObserver = pObserver;
}

CollectionPtr Collection::buildIndex(const std::string& sConfFile,
                                     const ProgressObserverPtr& pProgressObserver,
                                     bool bBlock)
{
    CollectionConfigurePtr pConfigure = new CollectionConfigure();
    XMLConfigurator xmlConf;
    xmlConf.load(sConfFile);
    pConfigure->configure(xmlConf);
    makePathAbsolute(xmlConf.getDir(), pConfigure->Collection.source);
    makePathAbsolute(xmlConf.getDir(), pConfigure->Collection.target);
    GLOBAL_CONF().makePathsAbsolute(xmlConf.getDir());

    return buildIndex(pConfigure, pProgressObserver, bBlock);
}

CollectionPtr Collection::buildIndex(const CollectionConfigurePtr& pConfigure,
                                     const ProgressObserverPtr& pProgressObserver, 
                                     bool bBlock)
{
    FIRTEX_ASSERT((pConfigure.isNotNull()),
                  _T("Invalid argument:Collection.buildIndex()"));

    CollectionPtr pCol = CollectionFactory::instance()->createCollection(
            pConfigure->Collection.identifier);
    if (pCol.isNull())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Create collection : [%s] FAILED"),
                     pConfigure->Collection.identifier.c_str());
    }
    if (pProgressObserver.isNotNull())
    {
        pCol->setObserver(pProgressObserver);
    }
    pCol->init(pConfigure);
    pCol->scan(bBlock);
    return pCol;
}

CollectionPtr Collection::buildCollection(const string& sConfFile)
{
    CollectionConfigurePtr pConfigure(new CollectionConfigure());
    try
    {
        XMLConfigurator loader;
        loader.load(sConfFile);
        pConfigure->configure(loader);
        
        makePathAbsolute(loader.getDir(), pConfigure->Collection.source);
        makePathAbsolute(loader.getDir(), pConfigure->Collection.target);
        GLOBAL_CONF().makePathsAbsolute(loader.getDir());
    }
    catch(const FirteXException& fe)
    {
        FX_LOG(ERROR, "Parse Configure file: [%s], reson: [%s]",
               sConfFile.c_str(), fe.what().c_str());
        throw;
    }
    CollectionPtr pCol = CollectionFactory::instance()->createCollection(
            pConfigure->Collection.identifier);
    if (pCol.isNull())
    {
        return pCol;
    }
    pCol->init(pConfigure);
    return pCol;
}

void Collection::makePathAbsolute(const std::string& sBasePath, std::string& sPath)
{
    Path p(sPath);
    p.makeDirectory();
    if (p.isRelative())
    {
        p.makeAbsolute(sBasePath);
        sPath = p.toString();
    }
}

void Collection::initParam(const std::string& sParam)
{
    FileFilter* pFileFilter = createFileFilter(sParam);
    if (pFileFilter)
    {
        m_pFileFilter.assign(pFileFilter);
    }

    doInitParam(sParam);
}

FX_NS_END
