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
// Created	: 2006/7/3
//
#include "firtex/common/StdHeader.h"
#include "firtex/utility/DirectoryIterator.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/extension/collection/DirCollection.h"
#include "firtex/extension/collection/DirectoryFileFetcher.h"
#include "firtex/extension/collection/DefaultFileFilter.h"
#include "firtex/extension/processor/PlainDocumentProcessor.h"
#include "firtex/extension/processor/DocumentProcessorFactory.h"
#include "firtex/extension/processor/MultiTypeDocumentProcessor.h"

FX_NS_USE(utility);
FX_NS_USE(index);
FX_NS_USE(document);
FX_NS_USE(processor);

using namespace std;

FX_NS_DEF(collection)

SETUP_STREAM_LOGGER(collection, DirCollection);

DirCollection::DirCollection()
{
}

DirCollection::~DirCollection()
{
}

FileFetcher* DirCollection::createFileFetcher()
{
    DirectoryFileFetcher* pFetcher = new DirectoryFileFetcher();
    pFetcher->init(getConfigure()->Collection.source, getFileFilter());
    return pFetcher;
}

DocumentProcessor* DirCollection::createDocumentProcessor()
{
    return m_pDocProcessor->clone();
}

void DirCollection::doInit(const CollectionConfigurePtr& pConfigure)
{
    initProcessors(pConfigure->Collection.docTypes);
}

void DirCollection::doInitParam(const std::string& sParam)
{
}

FileFilter* DirCollection::createFileFilter(const std::string& sParam)
{
    DefaultFileFilter* pFileFilter = new DefaultFileFilter(
            getConfigure()->Collection.source);
    if (!pFileFilter->init(sParam))
    {
        delete pFileFilter;
        return NULL;
    }
    return pFileFilter;
}

void DirCollection::initProcessors(const string& sDocType)
{
    if (sDocType.empty())
    {
        m_pDocProcessor = new PlainDocumentProcessor();
    }
    else
    {
        StringTokenizer st(sDocType, ",", StringTokenizer::TOKEN_IGNORE_EMPTY | 
                           StringTokenizer::TOKEN_TRIM);
        if (st.getNumTokens() == 1)
        {
            DocumentProcessor* pProcessor = 
                DocumentProcessorFactory::instance()->createProcessorByFileExtension(st[0]);
            if (!pProcessor)
            {
                FX_LOG(ERROR, "No processor for [%s].", st[0].c_str());
            }
            else
            {
                m_pDocProcessor.assign(pProcessor);
            }
        }
        else 
        {
            MultiTypeDocumentProcessor* pMultiTypeProcessor = new MultiTypeDocumentProcessor;
            for (size_t i = 0; i < st.getNumTokens(); ++i)
            {
                DocumentProcessor* pProcessor = 
                    DocumentProcessorFactory::instance()->createProcessorByFileExtension(st[i]);
                if (!pProcessor)
                {
                    FX_LOG(ERROR, "No processor for [%s].", st[i].c_str());
                }
                else
                {
                    DocumentProcessorPtr pProcessorPtr(pProcessor);
                    pMultiTypeProcessor->addProcessor(st[i], pProcessorPtr);
                }
            }
            m_pDocProcessor.assign(pMultiTypeProcessor);
        }
    }
    m_pDocProcessor->init(getConfigure()->getDocSchema().get(),
                          getConfigure()->getTemplate().get());
}

FX_NS_END

