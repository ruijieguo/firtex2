

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
// Email	: ruijieguo@gmail.com
// Created	: 2008/12/4
//
#ifndef __ComponentBuilder_H
#define __ComponentBuilder_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Logger.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/document/DocumentSchema.h"
#include <map>

FX_NS_DEF(index);

class FieldIndexer;
class FieldMerger;
class TermReader;
class ForwardIndexWriter;
class ForwardIndexReader;
class ForwardIndexMerger;

class ComponentBuilder
{
    typedef std::map<fieldid_t, tstring> Map;

public:
    ComponentBuilder();
    virtual ~ComponentBuilder();

public:
    /**
     * Initialize 
     */
    void init(const FX_NS(document)::DocumentSchemaPtr& pDocSchema);

    /**
     * Map a field id to a component identifier
     * @param fieldId field id
     * @param sIdentifier identifier of component 
     */
    void map(fieldid_t fieldId, const tstring& sIdentifier);

    /**
     * Build field indexer component
     *
     * @param fieldId field id
     * @param pFieldSchema field info for indexer
     * @return field indexer component, NULL if failed.
     */
    FieldIndexer* buildIndexer(fieldid_t fieldId,
                               const FX_NS(document)::FieldSchema* pFieldSchema) const;

    /** 
     * Build field merger component
     * @param fieldId field id
     * @return field merger component
     */
    FieldMerger* buildMerger(fieldid_t fieldId) const;

    /** 
     * Build term reader component
     * @param fieldId field id
     * @return term reader component
     */
    TermReader*	buildTermReader(fieldid_t fieldId) const;

    /**
     * Build forward index component writer by field id
     * @param fieldId field id
     */
    ForwardIndexWriter* buildForwardIndexWriter(fieldid_t fieldId) const;

    /**
     * Build forward index component reader by field id
     * @param fieldId field id
     */
    ForwardIndexReader* buildForwardIndexReader(fieldid_t fieldId) const;

    /**
     * Build forward index component reader by field id
     * @param fieldId field id
     */
    ForwardIndexMerger* buildForwardIndexMerger(fieldid_t fieldId) const;

    /**
     * Return field component crateor of specified field
     */
    const FieldComponentCreator* getFieldComponentCreator(fieldid_t fieldId) const;

private:			
    Map m_indexerMap;
    Map m_forwardIndexMap;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(ComponentBuilder);

FX_NS_END

#endif
