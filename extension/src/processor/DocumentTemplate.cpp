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
#include "firtex/extension/processor/DocumentTemplate.h"
#include "firtex/utility/Number.h"
#include "firtex/document/DocumentSchema.h"

FX_NS_USE(utility);
FX_NS_USE(config)
FX_NS_USE(document)
FX_NS_USE(common)

FX_NS_DEF(processor);

SETUP_LOGGER(processor, DocumentTemplate);

DocumentTemplate::DocumentTemplate(void)
{
}

DocumentTemplate::DocumentTemplate(const DocumentTemplate& other)
{
    m_sDocTag = other.m_sDocTag;
    for (TagVector::const_iterator iter = other.m_tags.begin();
        iter != m_tags.end(); iter++)
    {
        m_tags.push_back(*iter);
    }
}

DocumentTemplate::~DocumentTemplate(void)
{
}

void DocumentTemplate::addMeta(const std::string& sMetaName,
                               const std::string& sFieldName)
{
    m_metadata.push_back(Meta(sMetaName, sFieldName));
}

void DocumentTemplate::addTag(const std::string& sTagName,
                              const std::string& sFieldName,
                              const std::string& sAlias)
{
    m_tags.push_back(DocumentTemplate::Tag(sTagName, sFieldName, sAlias));
}

void DocumentTemplate::makeSure(const DocumentSchema* pSchema)
{
    if (!pSchema)
    {
        return;
    }

    for(Metadata::iterator iter = m_metadata.begin();
        iter != m_metadata.end(); iter++)
    {
        makeSureMeta(*iter, pSchema);
    }
	
    for(TagVector::iterator iter = m_tags.begin();
        iter != m_tags.end(); iter++)
    {
        makeSureTag(*iter, pSchema);
    }
}

void DocumentTemplate::makeSureMeta(Meta& meta, const DocumentSchema* pSchema)
{
    const FieldSchema* pFieldSchema = 
        pSchema->getSchema(meta.getFieldName().c_str());
    if (!pFieldSchema)
    {
        FX_LOG(ERROR, "No field: [%s] in schema match the meta: [%s]", 
               meta.getFieldName().c_str(), meta.getMetaName().c_str());
        FIRTEX_THROW(InvalidConfigException, "No field: [%s] in schema match "
                     "the meta: [%s]", meta.getFieldName().c_str(),
                     meta.getMetaName().c_str());
    }
    
    meta.setId(pFieldSchema->getId());
}

void DocumentTemplate::makeSureTag(DocumentTemplate::Tag& tag, 
                                     const DocumentSchema* pSchema)
{
    const FieldSchema* pFieldSchema = 
        pSchema->getSchema(tag.getTagName().c_str());
    if (!pFieldSchema)
    {
        pFieldSchema = pSchema->getSchema(tag.getFieldName().c_str());
    }
    if (pFieldSchema)
    {
        tag.setId(pFieldSchema->getId());
    }
}

DocumentTemplate::Iterator DocumentTemplate::iterator() const
{
    return DocumentTemplate::Iterator(this);
}

void DocumentTemplate::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator rootConf;
        rootConf.configure("doc_tag", m_sDocTag, "");
        rootConf.configure("embedded_field", m_sEmbeddedField, "");

        Configurator metadataConf;
        for (Metadata::iterator it = m_metadata.begin(); 
             it != m_metadata.end(); ++it)
        {
            Configurator tagConf;
            (*it).configure(tagConf);
            metadataConf.configure("meta", tagConf.getMap());
        }
        rootConf.configure("metadata", metadataConf.getMap());
    
        Configurator tagsConf;
        for (TagVector::iterator it = m_tags.begin(); 
             it != m_tags.end(); ++it)
        {
            Configurator tagConf;
            (*it).configure(tagConf);
            tagsConf.configure("tag", tagConf.getMap());
        }
        rootConf.configure("tags", tagsConf.getMap());
        
        conf.configure("template", rootConf.getMap());
    }
    else
    {
        m_sDocTag.clear();
        m_tags.clear();

        Configurator::Iterator it = conf.findConf("template");
        if (it.hasNext())
        {
            Configurator rootConf(AnyCast<Configurator::ConfMap>(it.next().second));
            rootConf.configure("doc_tag", m_sDocTag, "");
            rootConf.configure("embedded_field", m_sEmbeddedField, "");

            Configurator::Iterator it1 = rootConf.findConf("metadata");
            if (it1.hasNext())
            {
                Configurator metadataConf(AnyCast<Configurator::ConfMap>(it1.next().second));
                Configurator::Iterator it3 = metadataConf.iterator();
                while (it3.hasNext())
                {
                    Configurator::KeyValuePair kv = it3.next();
                    if (kv.first == "meta")
                    {
                        Configurator tagConf(AnyCast<Configurator::ConfMap>(kv.second));
                        DocumentTemplate::Meta meta;
                        meta.configure(tagConf);
                        m_metadata.push_back(meta);
                    }
                }
            }
            
            Configurator::Iterator it2 = rootConf.findConf("tags");
            if (it2.hasNext())
            {
                Configurator tagsConf(AnyCast<Configurator::ConfMap>(it2.next().second));
                Configurator::Iterator it3 = tagsConf.iterator();
                while (it3.hasNext())
                {
                    Configurator::KeyValuePair kv = it3.next();
                    if (kv.first == "tag")
                    {
                        Configurator tagConf(AnyCast<Configurator::ConfMap>(kv.second));
                        DocumentTemplate::Tag tag;
                        tag.configure(tagConf);
                        m_tags.push_back(tag);
                    }
                }
            }
        }
    }
}

void DocumentTemplate::Tag::configure(Configurator& conf)
{
    conf.configure("name", m_sTagName, "");
    conf.configure("field_name", m_sFieldName, "");
    conf.configure("alias", m_sAlias, "");
}

void DocumentTemplate::Meta::configure(Configurator& conf)
{
    conf.configure("name", m_sMetaName, "");
    conf.configure("field_name", m_sFieldName, "");
}

//////////////////////////////////////////////////////////////////////////
//DocumentTemplate::Iterator
DocumentTemplate::Iterator::Iterator(const DocumentTemplate* pDocumentTemplate)
    : m_pDocumentTemplate( const_cast<DocumentTemplate*> (pDocumentTemplate))	
{
    m_iterator = m_pDocumentTemplate->m_tags.begin();
}

DocumentTemplate::Iterator::Iterator(const Iterator& _iter)
    : m_pDocumentTemplate(_iter.m_pDocumentTemplate)
    , m_iterator(_iter.m_iterator)
{
}

DocumentTemplate::Iterator::~Iterator(void)
{
    m_pDocumentTemplate = NULL;
}

void DocumentTemplate::Iterator::reset()
{
    m_iterator = m_pDocumentTemplate->m_tags.begin();
}

bool DocumentTemplate::Iterator::hasNext() const
{
    return (m_iterator != m_pDocumentTemplate->m_tags.end());
}

const DocumentTemplate::Tag& DocumentTemplate::Iterator::next()
{
    return *m_iterator++;
}

size_t DocumentTemplate::Iterator::size()const
{
    return m_pDocumentTemplate->m_tags.size();
}

FX_NS_END
