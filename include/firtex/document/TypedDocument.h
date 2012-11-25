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
// Created	: 2011-02-24 22:50:30

#ifndef __FX_TYPEDDOCUMENT_H
#define __FX_TYPEDDOCUMENT_H

#include <vector>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/DocumentSchema.h"

FX_NS_DEF(document);

template <typename T>
class TypedDocument
{
public:
    typedef T FieldType;

public:
    typedef std::vector<FieldType*> FieldVector;

public:
    class Iterator
    {
    public:
        Iterator(const FieldVector& fields)
            : m_fields(fields)
            , m_nIterator(0)
        {
        }

        Iterator(const Iterator& _iter)
            : m_fields(_iter.m_fields)
            , m_nIterator(_iter.m_nIterator)
        {
        }

        virtual ~Iterator() {}

    public:
        /**
         * Reset the iterator
         */
        void reset()
        {
            m_nIterator = 0;            
        }

        /**
         * @return true for not, false for yes
         */
        bool hasNext() const
        {
            for (size_t i = m_nIterator; i < m_fields.size(); ++i)
            {
                if (m_fields[i])
                {
                    return true;
                }
            }
            return false;

        }

        /**
         * retrieval current element
         * @return current element
         */
        const FieldType* next()
        {
            while (m_nIterator < m_fields.size())
            {
                if (m_fields[m_nIterator])
                {
                    return m_fields[m_nIterator++];
                }
                ++m_nIterator;
            }
            FIRTEX_ASSERT2(false);
            return NULL;
        }

        /**
         * number of elements
         * @return number of elements
         */
        size_t size() const
        {
            size_t nCount = 0;
            for (size_t i = 0; i < m_fields.size(); ++i)
            {
                if (m_fields[i])
                {
                    ++nCount;
                }
            }
            return nCount;
        }

    private:
        const FieldVector& m_fields;
        size_t m_nIterator;
    };

public:
    TypedDocument(const DocumentSchema* pDocSchema);
    ~TypedDocument();

public:
    /**
     * get/set document Id
     */
    void setDocId(docid_t did) const; 
    docid_t getDocId() const;

    /**
     * Add a field with specified fieldId
     * @return an available field from field pool
     */
    FieldType* addField(fieldid_t id);

public:
    /**
     * Get document schema
     * @return reference to document schema
     */
    const DocumentSchema* getDocSchema() const;

    /**
     * Return field by field name
     * @param sFieldName field name
     * @return field object
     */
    const FieldType* getField(const tstring& sFieldName) const;

    /**
     * Return field by field id
     * @param fieldId field id
     * @return field object
     */
    const FieldType* getField(fieldid_t fieldId) const;

    /**
     * clear all fields
     */
    void clear();

public:
    /**
     * Return const iterator of document fields
     * @return iterator 
     */
    Iterator iterator() const;

protected:
    FieldVector& getFields();
    FieldVector& getFieldPool();
    
private:
    /**
     * Init the doument by specified document schema
     */
    void init(const DocumentSchema* pDocSchema);

private:
    const DocumentSchema* m_pDocSchema;

    mutable docid_t m_docId;    ///document Id

    FieldVector m_fields;
    FieldVector m_fieldPool;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////////////////////////////////////
//Docuemnt inline functions
template <typename T>
TypedDocument<T>::TypedDocument(const DocumentSchema* pDocSchema)
    : m_docId(INVALID_DOCID)
{
    init(pDocSchema);
}

template <typename T>
TypedDocument<T>::~TypedDocument()
{
    for (size_t i = 0; i < m_fields.size(); i++)
    {
        if (m_fields[i])
        {
            delete m_fields[i];
        }
    }

    for (size_t i = 0; i < m_fieldPool.size(); i++)
    {
        if (m_fieldPool[i])
        {
            delete m_fieldPool[i];
        }
    }

    m_fields.clear();
    m_fieldPool.clear();
}

template <typename T>
void TypedDocument<T>::init(const DocumentSchema* pDocSchema)
{
    FIRTEX_ASSERT2(pDocSchema);

    m_pDocSchema = pDocSchema;

    m_fields.resize(m_pDocSchema->getNumFields(), NULL);
    m_fieldPool.resize(m_pDocSchema->getNumFields(), NULL);

    DocumentSchema::Iterator iter = m_pDocSchema->iterator();
    while (iter.hasNext())
    {
        const FieldSchema* pFieldSchema = iter.next();
        if (pFieldSchema->getId() != INVALID_FIELDID)
        {
            FieldType* pField = new FieldType(pFieldSchema);
            m_fieldPool[pFieldSchema->getId()] = pField;
        }
    }
}

template <typename T>
const typename TypedDocument<T>::FieldType*
TypedDocument<T>::getField(const tstring& sFieldName) const
{
    if (m_pDocSchema)
    {
        const FieldSchema* pFieldSchema = m_pDocSchema->getSchema(sFieldName);
        if (pFieldSchema)
        {
            fieldid_t fid = pFieldSchema->getId();
            return getField(fid);
        }
    }
    return NULL;
}

template <typename T>
void TypedDocument<T>::clear()
{
    for (size_t i = 0; i < m_fields.size(); i++)
    {
        FieldType* pField = m_fields[i];
        if (pField)
        {
            pField->clear();
            FIRTEX_ASSERT2(!m_fieldPool[i]);
            m_fieldPool[i] = pField;
            m_fields[i] = NULL;
        }
    }
}

template <typename T>
inline typename TypedDocument<T>::Iterator
TypedDocument<T>::iterator() const
{
    return Iterator(m_fields);
}

template <typename T>
inline void TypedDocument<T>::setDocId(docid_t did) const 
{
    m_docId = did;
}

template <typename T>
inline docid_t TypedDocument<T>::getDocId() const 
{
    return m_docId;
}

template <typename T>
inline const typename TypedDocument<T>::FieldType*
TypedDocument<T>::getField(fieldid_t fieldId) const
{
    if (fieldId < 0 && fieldId >= (fieldid_t)m_fields.size())
    {
        return NULL;
    }
    return m_fields[fieldId];
}

template <typename T>
inline typename TypedDocument<T>::FieldType*
TypedDocument<T>::addField(fieldid_t fieldId)
{
    FIRTEX_ASSERT((fieldId >=0 && fieldId < (fieldid_t)m_fields.size()),
                  "Field id is out of range");

    FieldType* pField = m_fields[fieldId];
    if (!pField)
    {
        pField = m_fieldPool[fieldId];
        m_fields[fieldId] = pField;
        m_fieldPool[fieldId] = NULL;
    }
    FIRTEX_ASSERT2(pField);
    
    pField->clear();
    return pField;
}

template <typename T>
inline const DocumentSchema* TypedDocument<T>::getDocSchema() const 
{
    return m_pDocSchema;
}

template <typename T>
inline typename TypedDocument<T>::FieldVector&
TypedDocument<T>::getFields()
{
    return m_fields;
}

template <typename T>
inline typename TypedDocument<T>::FieldVector& 
TypedDocument<T>::getFieldPool()
{
    return m_fieldPool;
}

FX_NS_END

#endif //__FX_TYPEDDOCUMENT_H
