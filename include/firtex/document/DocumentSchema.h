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
// Created	: 2006/6/27
// Modified	: 2009/11/15
//

#ifndef _DOCUMENTSCHEMA_H
#define _DOCUMENTSCHEMA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Iterator.h"
#include "firtex/config/Configurable.h"
#include "firtex/document/FieldType.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/document/FieldDefinition.h"
#include <vector>
#include <map>

FX_NS_DEF(document);

class DocumentSchema : public FX_NS(config)::Configurable
{
    typedef std::vector<FieldSchema*> FieldSchemaVector;

public:
    class Iterator
    {
    public:
        Iterator(const DocumentSchema* pDocumentSchema);
        Iterator(const Iterator& _iter);
        virtual ~Iterator(void);
    public:
        void reset();
        bool hasNext() const;
        const FieldSchema* next();
        size_t size()const;

    private:
        const DocumentSchema* m_pDocumentSchema;
        FieldSchemaVector::const_iterator m_iterator;
    };

public:
    DocumentSchema(void);
    DocumentSchema(const DocumentSchema& src);
    virtual ~DocumentSchema(void);

public:
    void configure(FX_NS(config)::Configurator& conf);

public:
    /**
     * Add unindexed field
     * @param sName field name
     * @param sType field index type
     * @return field id of added field
     */
    fieldid_t addUnIndexedField(const std::string& sName, const std::string& sType = "");

    /**
     * Add text field
     * @param sName field name
     * @param bStore store or not
     * @return field id of added field
     */
    fieldid_t addTextField(const std::string& sName,
                           bool bStore = false);

    /**
     * add unstored field
     * @param sName field name
     * @param sType field type previous defined
     * @return field id of added field
     */
    fieldid_t addUnStoredField(const std::string& sName, const std::string& sType);

    /**
     * Add sortable field
     * @param sName field name
     * @param vt value type of field
     * @param store store or not
     */
    fieldid_t addSortableField(const std::string& sName,
                               FieldType::ValueType vt,
                               bool bStore = false);

    /**
     * add a field
     * @param sName field name
     * @param sType field type previous defined
     * @param store store or not
     * @param bSortAndFilterale if true forward index
     *  will be created for this field
     */
    fieldid_t addField(const std::string& sName, const std::string& sType,
                       bool bStore, bool bSortAndFilterale = false,
                       float fBoost = 1.0);

    /**
     * add text item
     * @param item schema item
     * @return schema id
     */
    fieldid_t addField(const FieldSchema& fieldSchema);

    /**
     * set/get default field
     */
    void setDefaultField(const std::string& sDefField);
    std::string getDefaultField() const;

public:
    /**
     * add items
     * @param schema schema
     * @return number of added items
     */
    int32_t mergeSchema(const DocumentSchema& schema);

    /**
     * Get a schema by name for read only
     * @param name item name 
     */
    const FieldSchema* getSchema(const std::string& sName) const;

    /**
     * Get a schema by name for read or update
     * @param name item name 
     */
    FieldSchema* getSchema(const std::string& sName);

    /**
     * get item by id
     * @param fieldId schema id
     * @return schema item
     */
    const FieldSchema* getSchema(fieldid_t fieldId) const;

    /**
     * Get fieldId by field name
     */
    fieldid_t getFieldId(const std::string& sFieldName) const;

public:
    /**
     * get schema item count 
     */
    size_t getNumFields() const;

    /**
     * get item by position
     * @param idx position
     * @return schema item
     */
    const FieldSchema* operator[] (const size_t idx) const;

    /**
     * get schema item iterator 
     */
    DocumentSchema::Iterator iterator() const;

    /**
     * assign operator
     */
    DocumentSchema& operator = (const DocumentSchema& right);

    /**
     * Clear all field schema 
     */
    void clear();

    /** 
     * is equal
     * @return true if equal otherwise false
     */
    bool isEqual(const DocumentSchema& schema) const;

    /**
     * Set field type definition which define how to index a field
     */
    void setFieldDefinition(const FieldDefinition& fieldDef);

private:
    const FieldType* getFieldType(const std::string& sTypeName,
                                  bool bSortAndFilterable);

private:
    std::string m_sDefField;
    FieldSchemaVector m_fields;
    FieldDefinition* m_pFieldDefinition;

    friend class Iterator;
};

DEFINE_TYPED_PTR(DocumentSchema);

/////////////////////////////////////////////

inline fieldid_t DocumentSchema::getFieldId(const std::string& sFieldName) const
{
    const FieldSchema* pFieldSchema = getSchema(sFieldName);
    if (pFieldSchema)
    {
        return pFieldSchema->getId();
    }
    return INVALID_FIELDID;
}

inline size_t DocumentSchema::getNumFields() const 
{
    return (size_t)m_fields.size();
}

inline const FieldSchema* DocumentSchema::operator[](const size_t idx) const
{
    if (idx == size_t(-1) || idx >= m_fields.size())
        return NULL;
    return m_fields[idx];
}

inline void DocumentSchema::setDefaultField(const std::string& sDefField)
{
    m_sDefField = sDefField;
}

inline std::string DocumentSchema::getDefaultField() const
{
    return m_sDefField;
}


FX_NS_END

#endif
