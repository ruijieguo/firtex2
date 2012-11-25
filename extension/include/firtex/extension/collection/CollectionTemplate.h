//
// Copyright(C) 2005--2009 Ruijie Guo
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/7
//

#ifndef __COLLECTIONTEMPLATE_H
#define __COLLECTIONTEMPLATE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/utility/String.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/config/Configurable.h"
#include <vector>

FX_NS_DEF(document)
class DocumentSchema;
FX_NS_END

FX_NS_DEF(collection);

/**
 * @class CollectionTemplate
 * @brief Describe the collection's template which is used
 * to define the format of collection
 *
 * Example:
 * 
 * To describe collection: 
 *
 * <DOC>
 *    <field1>field1 content</field1>
 *    <field2>field2 content</field2>
 *    <field3>field3 content</field3>
 * </DOC>
 * 
 * Note: field1 is indexed as TITLE field, 
 *       field2 and field3 are indexed as BODY field
 * 
 * <template>
 *     <doc_tag>DOC</doc_tag>
 *     <!-- optional, indicate the field is embedded in doc tags(trec) -->
 *     <embedded_field>CONTENT</embedded_field>
 *     <tags>
 *          <tag>
 *              <name>field1</name>
 *              <!-- optional -->
 *              <field_name>TITLE</field_name>
 *          </tag>
 *          <tag>
 *              <name>field2</name>
 *              <!-- optional -->
 *              <field_name>BODY</field_name>
 *              <!-- optional -->
 *              <alias>TEXT</alias>
 *          </tag>
 *          <tag>
 *              <name>field3</name>
 *              <!-- optional -->
 *              <field_name>BODY</field_name>
 *          </tag>
 *     </tags>
 * </template>
 * 
 */
class CollectionTemplate : public FX_NS(config)::Configurable
{
public:
    class Tag : public FX_NS(config)::Configurable
    {
    public:
        Tag() : m_nId ( -1 ){}
        Tag(const tstring& name, const tstring& sFieldName, const tstring& sAlias) 
            : m_nId ( -1 )
            , m_sTagName(name)
            , m_sFieldName(sFieldName)
            , m_sAlias(sAlias)
        {}

        Tag(const Tag& other) 
            : m_nId (other.m_nId)
            , m_sTagName(other.m_sTagName)
            , m_sFieldName(other.m_sFieldName)
            , m_sAlias(other.m_sAlias)
        {}

    public:
        void configure(FX_NS(config)::Configurator& conf);

    public:
        const tstring& getTagName()const {return m_sTagName;}
        const tstring& getAlias() const{return m_sAlias;}
        const tstring& getFieldName() const{return m_sFieldName;}
        fieldid_t getId() const {return m_nId;}
        void setId(fieldid_t id) { m_nId = id;}

    protected:
        fieldid_t m_nId;
        tstring m_sTagName;
        tstring m_sFieldName;
        tstring m_sAlias;

        friend class CollectionTemplate;
    };

    typedef std::vector<CollectionTemplate::Tag> TagVector;

public:
    class Iterator 
    {
    public:
        Iterator(const CollectionTemplate* pTemplate);
        Iterator(const Iterator& _iter);
        virtual ~Iterator(void);
    public:
        /**
         * reset the iterator
         */
        void reset();

        /**
         * whether reach to end or not
         * @return true for not, false for yes
         */
        bool hasNext() const;

        /**
         * retrieval current element
         * @return current element
         */
        const CollectionTemplate::Tag& next();

        /**
         * number of elements
         * @return number of elements
         */
        size_t size()const;
    private:
        CollectionTemplate* m_pCollectionTemplate;
        TagVector::const_iterator m_iterator;
    };

public:
    CollectionTemplate(void);
    CollectionTemplate(const CollectionTemplate& other);
    ~CollectionTemplate(void);

public:
    void configure(FX_NS(config)::Configurator& conf);

public:
    /** get tag item iterator */
    CollectionTemplate::Iterator iterator() const;

    /** get tag count */
    size_t getNumTags() const {return m_vTags.size();}

    /** get root tag name */
    const tstring& getDocTag() const {return m_sDocTag;}
    void setDocTag(const tstring& sDocTag) {m_sDocTag = sDocTag;}

    /** get embedded field */
    const tstring& getEmbeddedField() const {return m_sEmbeddedField;}

    void addTag(const tstring& sTagName, const tstring& sFieldName = _T(""),
                const tstring& sAlias = _T(""));

    /**
     * make sure that the schema id of tag is correct
     * @param pSchema document schema
     */
    void makeSure(const FX_NS(document)::DocumentSchema* pSchema);

private:
    void makeSureTag(Tag& tag, const FX_NS(document)::DocumentSchema* pSchema);

private:
    tstring m_sDocTag;
    tstring m_sEmbeddedField;
    TagVector m_vTags;
};

DEFINE_TYPED_PTR(CollectionTemplate);

FX_NS_END

#endif
