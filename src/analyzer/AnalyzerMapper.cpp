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
// Email	: ruijieguo@gmail.com
// Created	: 2008/12/28
//
#include "firtex/analyzer/AnalyzerMapper.h"
#include "firtex/analyzer/AnalyzerFactory.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/analyzer/FilterFactory.h"

FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, AnalyzerMapper);

AnalyzerMapper::AnalyzerMapper()
    : m_pDocSchema(NULL)
{
}

AnalyzerMapper::AnalyzerMapper(const AnalyzerMapper& src)
    : m_pDocSchema(src.m_pDocSchema)
{
    for (AnalyzerIdMap::const_iterator it1 = src.m_analyzerForIndexById.begin();
         it1 != src.m_analyzerForIndexById.end(); ++it1)
    {
        AnalyzerPtr pAnalyzer(it1->second->clone());
        m_analyzerForIndexById.insert(make_pair(it1->first, pAnalyzer));
        const FieldSchema* pFieldSchema = m_pDocSchema->getSchema(it1->first);
        FIRTEX_ASSERT2(pFieldSchema != NULL);
        m_analyzerForIndex.insert(make_pair(pFieldSchema->getName(), pAnalyzer));
    }

    for (AnalyzerIdMap::const_iterator it2 = src.m_analyzerForSearchById.begin();
         it2 != src.m_analyzerForSearchById.end(); ++it2)
    {
        AnalyzerPtr pAnalyzer(it2->second->clone());
        m_analyzerForSearchById.insert(make_pair(it2->first, it2->second->clone()));
        const FieldSchema* pFieldSchema = m_pDocSchema->getSchema(it2->first);
        FIRTEX_ASSERT2(pFieldSchema != NULL);
        m_analyzerForSearch.insert(make_pair(pFieldSchema->getName(), pAnalyzer));
    }
}

AnalyzerMapper::~AnalyzerMapper()
{
    clear();	
}

void AnalyzerMapper::init(const DocumentSchema* pDocSchema)
{
    m_pDocSchema = pDocSchema;
    map(pDocSchema);
}

void AnalyzerMapper::map(const DocumentSchema* pDocSchema)
{
    DocumentSchema::Iterator iter = pDocSchema->iterator();
    const FieldSchema* pFieldSchema = NULL;
    while (iter.hasNext())
    {
        pFieldSchema = iter.next();
        const FieldType* pFieldType = pFieldSchema->getFieldType();
        if (pFieldSchema->isAnalyzed())
        {			
            const FieldType::IndexType& indexType = pFieldType->getIndexType();
            const FieldType::Analyzer* pAnalyzer = &(indexType.analyzer);
            if (pAnalyzer->isEmpty())
            {
                pAnalyzer = &(pFieldType->getForwardIndexType().analyzer);
            }
            FIRTEX_ASSERT2(!pAnalyzer->isEmpty());

            AnalyzerPtr pAnalyzerPtr = createAnalyzer(*pAnalyzer);

            map(pFieldSchema->getId(), pFieldSchema->getName(), pAnalyzerPtr,
                m_analyzerForIndex, m_analyzerForIndexById);
        }
        if (pFieldType)
        {
            const FieldType::SearchType& searchType = pFieldType->getSearchType();
            const FieldType::Analyzer& analyzerForSearch = searchType.analyzer;
            if (!analyzerForSearch.identifier.empty())
            {
                AnalyzerPtr pAnalyzer = createAnalyzer(analyzerForSearch);

                map(pFieldSchema->getId(), pFieldSchema->getName(), pAnalyzer,
                    m_analyzerForSearch, m_analyzerForSearchById);
            }            
        }
    }
}

Analyzer* AnalyzerMapper::createAnalyzer(const FieldType::Analyzer& analyzer)
{
    Analyzer* pAnalyzer = createAnalyzer(analyzer.identifier,
            analyzer.parameter);

    TokenFilter* pFilter = NULL;
    if (!analyzer.anteFilter.empty() 
        && (pFilter = createFilters(analyzer.anteFilter)))
    {
        pAnalyzer->attachAnteFilter(pFilter);
    }

    if (!analyzer.postFilter.empty() 
        && (pFilter = createFilters(analyzer.postFilter)))
    {
        pAnalyzer->attachPostFilter(pFilter);
    }
    return pAnalyzer;
}

void AnalyzerMapper::mapForSearcher(fieldid_t fid, const std::string& sFieldName,
                                    const AnalyzerPtr& pAnalyzer)
{
    map(fid, sFieldName, pAnalyzer,
        m_analyzerForSearch, m_analyzerForSearchById);
}

void AnalyzerMapper::map(fieldid_t fid, const std::string& sFieldName,
                         const AnalyzerPtr& pAnalyzer)
{
    map(fid, sFieldName, pAnalyzer, m_analyzerForIndex, m_analyzerForIndexById);
}

void AnalyzerMapper::map(fieldid_t fid, const std::string& sFieldName, 
                         const AnalyzerPtr& pAnalyzer, AnalyzerMap& nameMap,
                         AnalyzerIdMap& idMap)
{
    AnalyzerIdMap::iterator iter = idMap.find(fid);
    if (iter != idMap.end())
    {
        FX_LOG(WARN, _T("The field analyzer of field: [%d] already exist, "
                        "over write it."), fid);
        idMap.erase(iter);
        AnalyzerMap::iterator iter2 = nameMap.find(sFieldName);
        if (iter2 != nameMap.end())
        {
            nameMap.erase(iter2);
        }
    }
    idMap.insert(make_pair(fid, pAnalyzer));
    nameMap.insert(make_pair(sFieldName, pAnalyzer));
}

const Analyzer* AnalyzerMapper::getAnalyzer(fieldid_t fid) const
{
    AnalyzerIdMap::const_iterator it = m_analyzerForIndexById.find(fid);
    if (it != m_analyzerForIndexById.end())
    {
        return it->second.get();
    }
    return NULL;
}

const Analyzer* AnalyzerMapper::getAnalyzer(const std::string& sFieldName) const
{
    AnalyzerMap::const_iterator iter = m_analyzerForIndex.find(sFieldName);
    if (iter != m_analyzerForIndex.end())
    {
        return iter->second.get();
    }
    return NULL;
}

const Analyzer* AnalyzerMapper::getAnalyzerForSearcher(fieldid_t fid) const
{
    AnalyzerIdMap::const_iterator it = m_analyzerForSearchById.find(fid);
    if (it != m_analyzerForSearchById.end())
    {
        return it->second.get();
    }
    return getAnalyzer(fid);
}

AnalyzerPtr AnalyzerMapper::getAnalyzer(fieldid_t fid)
{
    AnalyzerIdMap::iterator it = m_analyzerForIndexById.find(fid);
    if (it != m_analyzerForIndexById.end())
    {
        return it->second;
    }
    return AnalyzerPtr();
}

AnalyzerPtr AnalyzerMapper::getAnalyzerForSearcher(fieldid_t fid)
{
    AnalyzerIdMap::iterator it = m_analyzerForSearchById.find(fid);
    if (it != m_analyzerForSearchById.end())
    {
        return it->second;
    }
    return getAnalyzer(fid);
}

const Analyzer* AnalyzerMapper::getAnalyzerForSearcher(const std::string& sFieldName) const
{
    AnalyzerMap::const_iterator iter = m_analyzerForSearch.find(sFieldName);
    if (iter != m_analyzerForSearch.end())
    {
        return iter->second.get();
    }
    return getAnalyzer(sFieldName);
}

void AnalyzerMapper::clear()
{
    m_analyzerForIndexById.clear();
    m_analyzerForIndex.clear();

    m_analyzerForSearchById.clear();
    m_analyzerForSearch.clear();

    m_pDocSchema = NULL;
}

TokenFilter* AnalyzerMapper::createFilters(const std::string& sFilterStr)
{
    StringTokenizer st(sFilterStr, ";", StringTokenizer::TOKEN_IGNORE_EMPTY 
                       | StringTokenizer::TOKEN_TRIM);
    TokenFilter* pPrevFilter = NULL;
    TokenFilter* pFilter = NULL;
    for (size_t i = 0; i < st.getNumTokens(); ++i)
    {
        std::string sFilter = st[i];
        std::string sIdent, sParam;
        std::string::size_type pos = sFilter.find(',', 0);
        if (pos != std::string::npos)
        {
            sIdent = sFilter.substr(0, pos);
            sParam = sFilter.substr(pos + 1);
        } 
        else 
        {
            sIdent = sFilter;
        }
        
        pFilter = FilterFactory::instance()->createFilter(sIdent, sParam);
        if (!pFilter)
        {
            FIRTEX_THROW(BadParameterException, "Create filter: [%s] FAILED.", sIdent.c_str());
        }

        if (pPrevFilter)
        {
            pFilter->attachFilter(pPrevFilter);
        }
        pPrevFilter = pFilter;
    }
    return pPrevFilter;
}

AnalyzerMapper* AnalyzerMapper::clone() const
{
    return new AnalyzerMapper(*this);
}

Analyzer* AnalyzerMapper::createAnalyzer(
        const string& sIdent, const string& sParam)
{
    Analyzer* pAnalyzer = AnalyzerFactory::instance()->createAnalyzer(
            sIdent, sParam);
    if (!pAnalyzer)
    {
        FIRTEX_THROW(UnsupportedException, "Unsupported analyzer: [%s]",
                     sIdent.c_str());
    }

    return pAnalyzer;
}

FX_NS_END
