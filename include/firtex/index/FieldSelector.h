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
// Created : 2011-09-24 10:38:16

#ifndef __FX_FIELDSELECTOR_H
#define __FX_FIELDSELECTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/index/FieldFilter.h"
#include <map>

FX_NS_DEF(index);

class FieldSelector
{
public:
    struct Field
    {
        Field(const FX_NS(document)::FieldSchema* s,
              const FieldFilterPtr& f = FieldFilterPtr())
            : fieldSchema(s)
            , fieldFilter(f)
        {
        }

        fieldid_t getFieldId() const
        {
            return fieldSchema->getId();
        }

        const std::string& getFieldName() const
        {
            return fieldSchema->getName();
        }

        bool hasFilter() const
        {
            return fieldFilter.isNotNull();
        }

        bool filter(const std::string& sOrgValue, std::string& sNewValue) const
        {
            if (fieldFilter.isNotNull())
            {
                return fieldFilter->filter(sOrgValue, sNewValue);
            }
            return false;
        }

        const FX_NS(document)::FieldSchema* fieldSchema;
        FieldFilterPtr fieldFilter;
    };
    DEFINE_TYPED_PTR(Field);

public:
    FieldSelector(const FX_NS(document)::DocumentSchema* pDocSchema,
                  bool bSelectStoredFields, bool bSelectForwardIndexFields)
        : m_pDocSchema(pDocSchema)
    {
        FX_NS(document)::DocumentSchema::Iterator it = m_pDocSchema->iterator();
        while (it.hasNext())
        {
            const FX_NS(document)::FieldSchema* pFieldSchema = it.next();

            if (bSelectStoredFields && pFieldSchema->isStored())
            {
                m_fields.insert(std::make_pair(pFieldSchema->getId(),
                                FieldPtr(new Field(pFieldSchema))));
            }
            if (bSelectForwardIndexFields && pFieldSchema->hasForwardIndex())
            {
                m_fields.insert(std::make_pair(pFieldSchema->getId(),
                                FieldPtr(new Field(pFieldSchema))));
            }
        }
    }

    FieldSelector(const FX_NS(document)::DocumentSchema* pDocSchema)
        : m_pDocSchema(pDocSchema)
    {
    }

    ~FieldSelector() {}

public:
    typedef std::map<fieldid_t, FieldPtr> FieldMap;

    class Iterator
    {
    public:
        Iterator(const FieldMap& fields)
            : m_fields(fields)
            , m_iterator(m_fields.begin())
        {
        }

    public:
        bool hasNext() const
        {
            return (m_iterator != m_fields.end());
        }

        const FieldPtr& next()
        {
            const FieldPtr& p = m_iterator->second;
            ++m_iterator;
            return p;
        }

        size_t size() const
        {
            return m_fields.size();
        }

    private:
        const FieldMap& m_fields;
        FieldMap::const_iterator m_iterator;
    };

public:
    /**
     * Add one field to select
     * 
     * @param sFieldName field name
     * @param pFieldFilter field filter of this field
     * 
     * @return true if added
     */
    bool addField(const std::string& sFieldName, 
                  const FieldFilterPtr& pFieldFilter = FieldFilterPtr())
    {
        const FX_NS(document)::FieldSchema* pFieldSchema = m_pDocSchema->getSchema(sFieldName);
        if (!pFieldSchema)
        {
            return false;
        }
        
        fieldid_t fieldId = pFieldSchema->getId();
        if (m_fields.find(fieldId) != m_fields.end())
        {
            return false;
        }
        
        m_fields.insert(std::make_pair(fieldId, 
                        FieldPtr(new Field(pFieldSchema, pFieldFilter))));
        return true;
    }

    /// Return iterator for iteration of selected fields
    Iterator iterator() const
    {
        return Iterator(m_fields);
    }

    size_t size() const
    {
        return m_fields.size();
    }
        
private:
    const FX_NS(document)::DocumentSchema* m_pDocSchema;

    FieldMap m_fields;
};

FX_NS_END

#endif //__FX_FIELDSELECTOR_H
