//
// Copyright(C) 2005--2009 Ruijie Guo
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/7
//

#ifndef __FX_DOCUMENTTEMPLATE_H
#define __FX_DOCUMENTTEMPLATE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/Logger.h"
#include "firtex/utility/String.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/config/Configurable.h"
#include <vector>

FX_NS_DEF(document)
class DocumentSchema;
FX_NS_END

FX_NS_DEF(processor);

/**
 * @class DocumentTemplate
 * @brief Describe the document's template which is used
 * to define the format of document
 *
 * Example:
 * 
 * To describe document: 
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
 *
 *     <metadata>
 *          <meta>
 *              <name>URL</name>
 *              <field_name>URL</field_name>
 *          </meta>
 *     </metadata>
 *
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
class DocumentTemplate : public FX_NS(config)::Configurable
{
public:
    class Tag : public FX_NS(config)::Configurable
    {
    public:
        Tag() : m_nId ( -1 ){}
        Tag(const std::string& name, const std::string& sFieldName,
            const std::string& sAlias) 
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
        const std::string& getTagName()const {return m_sTagName;}
        const std::string& getAlias() const{return m_sAlias;}
        const std::string& getFieldName() const{return m_sFieldName;}
        fieldid_t getId() const {return m_nId;}
        void setId(fieldid_t id) { m_nId = id;}

    protected:
        fieldid_t m_nId;
        std::string m_sTagName;
        std::string m_sFieldName;
        std::string m_sAlias;

        friend class DocumentTemplate;
    };
    typedef std::vector<DocumentTemplate::Tag> TagVector;

    class Meta : public FX_NS(config)::Configurable
    {
    public:
        Meta() : m_id(INVALID_FIELDID) {}
        Meta(const std::string& sMetaName,
             const std::string& sFieldName) 
            : m_id(INVALID_FIELDID)
            , m_sMetaName(sMetaName)
            , m_sFieldName(sFieldName)
        {}

    public:
        const std::string& getMetaName()const {return m_sMetaName;}
        const std::string& getFieldName() const{return m_sFieldName;}
        fieldid_t getId() const {return m_id;}
        void setId(fieldid_t id) { m_id = id;}

    public:
        void configure(FX_NS(config)::Configurator& conf);

    protected:
        fieldid_t m_id;
        std::string m_sMetaName;
        std::string m_sFieldName;
    };
    typedef std::vector<Meta> Metadata;

public:
    class Iterator 
    {
    public:
        Iterator(const DocumentTemplate* pTemplate);
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
        const DocumentTemplate::Tag& next();

        /**
         * number of elements
         * @return number of elements
         */
        size_t size()const;

    private:
        DocumentTemplate* m_pDocumentTemplate;
        TagVector::const_iterator m_iterator;
    };

public:
    DocumentTemplate(void);
    DocumentTemplate(const DocumentTemplate& other);
    ~DocumentTemplate(void);

public:
    void configure(FX_NS(config)::Configurator& conf);

public:
    /** get tag item iterator */
    Iterator iterator() const;

    /** get tag count */
    size_t getNumTags() const {return m_tags.size();}

    /** get root tag name */
    const std::string& getDocTag() const {return m_sDocTag;}
    void setDocTag(const std::string& sDocTag) {m_sDocTag = sDocTag;}

    /** get embedded field */
    const std::string& getEmbeddedField() const {return m_sEmbeddedField;}
    void setEmbeddedField(const std::string& sEmField) { m_sEmbeddedField = sEmField;}

    /** get metadata */
    const Metadata& getMetadata() const {return m_metadata;}

    void addMeta(const std::string& sMetaName,
                 const std::string& sFieldName);

    /// Return tag
    const Tag& getTag(size_t nTagIdx) const;

    void addTag(const std::string& sTagName,
                const std::string& sFieldName = "",
                const std::string& sAlias = "");

    /**
     * make sure that the schema id of tag is correct
     * @param pSchema document schema
     */
    void makeSure(const FX_NS(document)::DocumentSchema* pSchema);

private:
    void makeSureMeta(Meta& meta, const FX_NS(document)::DocumentSchema* pSchema);
    void makeSureTag(Tag& tag, const FX_NS(document)::DocumentSchema* pSchema);

private:
    std::string m_sDocTag;
    std::string m_sEmbeddedField;
    TagVector m_tags;
    Metadata m_metadata;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(DocumentTemplate);

////////////////////////////////////////////////
//
inline const DocumentTemplate::Tag& DocumentTemplate::getTag(size_t nTagIdx) const
{
    FIRTEX_ASSERT2(nTagIdx < m_tags.size());
    return m_tags[nTagIdx];
}

FX_NS_END

#endif
