//
// Copyright(C) 2005--2008 Guo Ruijie. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn ruijieguo@gmail.com
// Created	: 2008/12/4
//
#include "firtex/index/ComponentBuilder.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/ComponentCreatorFactory.h"
#include "firtex/index/ForwardIndexFactory.h"
#include "firtex/index/ForwardIndexMerger.h"
#include "firtex/utility/Exception.h"
#include <sstream>

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, ComponentBuilder);

ComponentBuilder::ComponentBuilder()
{
}

ComponentBuilder::~ComponentBuilder()
{
}

void ComponentBuilder::init(const DocumentSchemaPtr& pDocSchema)
{
    DocumentSchema::Iterator iter = pDocSchema->iterator();
    while (iter.hasNext())
    {
        const FieldSchema* pFieldSchema = iter.next();
        if (pFieldSchema->isIndexed() || pFieldSchema->hasForwardIndex())
        {
            if (pFieldSchema->isIndexed())
            {
                const FieldType* pFieldType = pFieldSchema->getFieldType();
                const FieldType::IndexType& indexType = pFieldType->getIndexType();
                if (!indexType.identifier.empty())
                {				
                    map(pFieldSchema->getId(), indexType.identifier);
                }
                else 
                {
                    FIRTEX_THROW(InvalidConfigException, _T("Identifier of "
                                    "index for field: [%s] is empty."),
                            pFieldSchema->getName().c_str());
                }
            }
            if (pFieldSchema->hasForwardIndex())
            {
                const FieldType* pFieldType = pFieldSchema->getFieldType();
                const FieldType::ForwardIndexType& fdIndexType = pFieldType->getForwardIndexType();
                if (!fdIndexType.identifier.empty())
                {				
                    map(pFieldSchema->getId(), fdIndexType.identifier);
                }
                else 
                {
                    FIRTEX_THROW(InvalidConfigException, _T("Identifier of forward "
                                    "index for field: [%s] is empty"), 
                            pFieldSchema->getName().c_str());
                }
            }
        }
    }
}

void ComponentBuilder::map(fieldid_t fieldId, const tstring& sIdentifier)
{
    bool bAlreadyExist = false;
    if (startsWithNoCase(sIdentifier, tstring(_T("indexer"))))
    {
        Map::iterator it = m_indexerMap.find(fieldId);
        if (it != m_indexerMap.end())
        {
            bAlreadyExist = true;
        }
        else
        {
            m_indexerMap.insert(make_pair(fieldId, sIdentifier));
        }
    }
    else if(startsWithNoCase(sIdentifier, tstring(_T("fdindex"))))
    {
        Map::iterator it = m_forwardIndexMap.find(fieldId);
        if (it != m_forwardIndexMap.end())
        {
            bAlreadyExist = true;
        }
        else 
        {
            m_forwardIndexMap.insert(make_pair(fieldId, sIdentifier));
        }
    }
    else 
    {
        FIRTEX_THROW(BadParameterException, _T("Invalid component identifier: [%s]"), 
                     sIdentifier.c_str());
    }
    if (bAlreadyExist)
    {
        FIRTEX_THROW(AlreadyExistException, _T("Map field: [%d] to [%s] FAILED: "
                        "the entry is already exist."), fieldId, sIdentifier.c_str());
    }
}

FieldIndexer* ComponentBuilder::buildIndexer(fieldid_t fieldId, 
        const FieldSchema* pFieldSchema) const
{
    const FieldComponentCreator* pCreator = getFieldComponentCreator(fieldId);
    return pCreator->createIndexer(pFieldSchema);
}

FieldMerger* ComponentBuilder::buildMerger(fieldid_t fieldId) const
{
    const FieldComponentCreator* pCreator = getFieldComponentCreator(fieldId);
    return pCreator->createMerger();
}

TermReader*	ComponentBuilder::buildTermReader(fieldid_t fieldId) const
{
    const FieldComponentCreator* pCreator = getFieldComponentCreator(fieldId);
    return pCreator->createTermReader();
}

ForwardIndexWriter* ComponentBuilder::buildForwardIndexWriter(fieldid_t fieldId) const
{
    Map::const_iterator it = m_forwardIndexMap.find(fieldId);
    if (it != m_forwardIndexMap.end())
    {
        ForwardIndexWriter* pForwardIndexWriter =
            ForwardIndexFactory::instance()->createForwardIndexWriter(it->second);
        if (!pForwardIndexWriter)
        {
            FIRTEX_THROW(NotFoundException, _T("Create forward index of [%s] FAILED"), 
                         it->second.c_str());
        }
        return pForwardIndexWriter;
    }
    FIRTEX_THROW(NotFoundException, _T("Not found forward index writer for [%d]"), fieldId);
    return NULL;
}

ForwardIndexReader* ComponentBuilder::buildForwardIndexReader(fieldid_t fieldId) const
{
    Map::const_iterator it = m_forwardIndexMap.find(fieldId);
    if (it != m_forwardIndexMap.end())
    {
        ForwardIndexReader* pForwardIndexReader =
            ForwardIndexFactory::instance()->createForwardIndexReader(it->second);
        if (!pForwardIndexReader)
        {
            FIRTEX_THROW(NotFoundException, _T("Create forward index of [%s] FAILED"),
                         it->second.c_str());
        }
        return pForwardIndexReader;
    }
    
    FIRTEX_THROW(NotFoundException, _T("Not found forward index reader for [%d]"), fieldId);
    return NULL;
}

ForwardIndexMerger* ComponentBuilder::buildForwardIndexMerger(fieldid_t fieldId) const
{
    Map::const_iterator it = m_forwardIndexMap.find(fieldId);
    if (it != m_forwardIndexMap.end())
    {
        ForwardIndexMerger* pForwardIndexMerger =
            ForwardIndexFactory::instance()->createForwardIndexMerger(it->second);
        if (!pForwardIndexMerger)
        {
            FIRTEX_THROW(NotFoundException, _T("Create forward index of [%s]"), 
                         it->second.c_str());
        }
        return pForwardIndexMerger;
    }
    FIRTEX_THROW(NotFoundException, _T("Not found forward index Merger for [%d]"), fieldId);
    return NULL;
}

const FieldComponentCreator* 
ComponentBuilder::getFieldComponentCreator(fieldid_t fieldId) const
{
    Map::const_iterator it = m_indexerMap.find(fieldId);
    if (it != m_indexerMap.end())
    {
        FieldComponentCreator* pCreator = 
            ComponentCreatorFactory::instance()->getCreator(it->second);
        if (!pCreator)
        {
            FIRTEX_THROW(NotFoundException, _T("Not found comonent creator of [%s]"), it->second.c_str());
        }
        return pCreator;
    }
    return NULL;
}

FX_NS_END
    
