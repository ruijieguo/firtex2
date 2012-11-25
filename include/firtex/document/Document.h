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
// Created	: 2005/11/20
//

#ifndef __FX_DOCUMENT_H
#define __FX_DOCUMENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/document/DocumentType.h"
#include "firtex/document/Field.h"
#include "firtex/document/TypedDocument.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/Charset.h"
#include "firtex/utility/StringUtils.h"

FX_NS_DEF(document);

class Document : public TypedDocument<Field>
{
public :
    typedef TypedDocument<Field>::Iterator Iterator;
    typedef TypedDocument<Field>::FieldVector FieldVector;

    enum Action
    {
        AT_NONE = 0,
        AT_ADD  = 1,
        AT_DELETE = 2,
        AT_UPDATE = 3,
    };


public:
    Document(const DocumentSchema* pDocSchema)
        : TypedDocument<Field>(pDocSchema)
        , m_action(AT_NONE)
        , m_fBoost(1.0)
        , m_docType(doc_type::UNKNOWN)
        , m_primaryKeyId(INVALID_DOCID)
    {
        DocumentSchema::Iterator it = pDocSchema->iterator();
        while (it.hasNext())
        {
            const FieldSchema* pFieldSchema = it.next();
            if (!FX_NS(utility)::strCompareNoCase(pFieldSchema->getTypeName().c_str(),
                            "PRIMARY_KEY"))
            {
                m_primaryKeyId = pFieldSchema->getId();
                break;
            }
        }
    }

    ~Document() {}

public:
    /// Set action type
    void setAction(Action a) const;
    
    /// Return action type
    Action getAction() const;

    /**
     * Get/set boost value of the document, 
     * A boost value is used for boosting the score of one document
     * The default value is 1.0
     */
    float getBoost() const;
    void setBoost(float fBoost);
    
    /**
     * get/set the document type
     */
    doctype_t getDocType() const;
    void setDocType(doctype_t t);

    /// Return the primary key of document
    const char* getPrimaryKey() const;

    /// Return the primary key of document
    void setPrimaryKey(const std::string& sPrimKey);

public:
    /**
     * Add field value according to the field id,
     * @param id field id,
     * @param bNoCopy copy string value or not
     */
    inline void addField(fieldid_t id, const std::string& value);
    inline void addField(fieldid_t id, const char* value);
    inline void addField(fieldid_t id, const char* value, size_t len);
    inline void addField(fieldid_t id, char* value, size_t len, bool bNoCopy);
    inline Field* addField(fieldid_t id);

private:
    void setBoost(FieldVector& fields, float fBoost);

private:
    mutable Action m_action;
    float m_fBoost;             ///boost value of document
    doctype_t m_docType;        ///document type

    fieldid_t m_primaryKeyId;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(Document);

//////////////////////////////////////////////////////////////////////////
//Docuemnt inline functions
inline void Document::setAction(Action a) const
{
    m_action = a;
}
    
inline Document::Action Document::getAction() const
{
    return m_action;
}


inline doctype_t Document::getDocType() const 
{
    return m_docType;
}

inline void Document::setDocType(doctype_t t) 
{
    m_docType = t;
}

inline void Document::addField(fieldid_t id, const std::string& value)
{
    Field* pField = TypedDocument<Field>::addField(id);
    pField->setValue(value);
}

inline void Document::addField(fieldid_t id, const char* value, size_t len)
{
    Field* pField = TypedDocument<Field>::addField(id);
    pField->setValue(value, len);
}

inline void Document::addField(fieldid_t id, const char* value)
{
    Field* pField = TypedDocument<Field>::addField(id);
    pField->setValue(value);
}

inline void Document::addField(fieldid_t id, char* value, size_t len, bool bNoCopy)
{
    Field* pField = TypedDocument<Field>::addField(id);
    pField->setValue(value, len, bNoCopy);
}

inline Field* Document::addField(fieldid_t id)
{
    return TypedDocument<Field>::addField(id);
}

inline float Document::getBoost() const 
{
    return m_fBoost;
}

inline const char* Document::getPrimaryKey() const
{
    if (m_primaryKeyId != INVALID_FIELDID)
    {
        const Field* pField = TypedDocument<Field>::getField(m_primaryKeyId);
        if (pField)
        {
            return pField->getValue().c_str();
        }
    }
    static std::string EMPTY_STRING;
    return EMPTY_STRING.c_str();
}

inline void Document::setPrimaryKey(const std::string& sPrimKey)
{
    if (m_primaryKeyId != INVALID_FIELDID)
    {
        addField(m_primaryKeyId, sPrimKey);
    }
}

FX_NS_END

#endif
