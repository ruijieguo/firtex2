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
#include "firtex/common/StdHeader.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/document/BuiltInFieldDefinition.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/config/Configurator.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Number.h"

#define FORWARD_INDEX_SUFFIX "_WITH_FDINDEX"

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(config);
FX_NS_USE(common);

FX_NS_DEF(document);

//////////////////////////////////////////////////////////////////////////
//DocumentSchema::Iterator		
DocumentSchema::Iterator::Iterator(const DocumentSchema* pDocumentSchema)
    : m_pDocumentSchema(pDocumentSchema)
    , m_iterator(pDocumentSchema->m_fields.begin())
{
}

DocumentSchema::Iterator::Iterator(const Iterator& _iter)
    : m_pDocumentSchema(_iter.m_pDocumentSchema)
    , m_iterator(_iter.m_iterator)
{
}

DocumentSchema::Iterator::~Iterator(void)
{
    m_pDocumentSchema = NULL;
}

void DocumentSchema::Iterator::reset()
{
    m_iterator = m_pDocumentSchema->m_fields.begin();
}

bool DocumentSchema::Iterator::hasNext() const
{
    return (m_iterator != m_pDocumentSchema->m_fields.end());
}

const FieldSchema* DocumentSchema::Iterator::next()
{
    return *m_iterator++;
}

size_t DocumentSchema::Iterator::size()const
{
    return m_pDocumentSchema->m_fields.size();
}

//////////////////////////////////////////////////////////////////////////
//
DocumentSchema::DocumentSchema(void)
    : m_pFieldDefinition(NULL)
{
}

DocumentSchema::DocumentSchema(const DocumentSchema& src)
{
    FieldSchemaVector::const_iterator iter = src.m_fields.begin();
    while (iter != src.m_fields.end())
    {
        addField(*(*iter));
        iter++;
    }
    if (src.m_pFieldDefinition)
    {
        m_pFieldDefinition = new FieldDefinition(*(src.m_pFieldDefinition));
    }
    else 
    {
        m_pFieldDefinition = NULL;
    }
}

DocumentSchema::~DocumentSchema(void)
{
    clear();
}

fieldid_t DocumentSchema::addUnIndexedField(const string& sName, const string& sType)
{
    if (sName.empty())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Field schema name is empty."));
    }

    const FieldSchema* pFieldSchema = getSchema(sName);
    if (!pFieldSchema)
    {
        return addField(sName, sType, true, false);
    }
    return pFieldSchema->getId();
}

fieldid_t DocumentSchema::addTextField(const string& sName, bool bStore)
{
    if (sName.empty())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Field schema name is empty."));
    }
    const FieldSchema* pFieldSchema = getSchema(sName);
    if (!pFieldSchema)
    {
        return addField(sName, _T("TEXT"), bStore, false);
    }
    return pFieldSchema->getId();
}

fieldid_t DocumentSchema::addUnStoredField(
        const string& sName, const string& sType)
{
    if (sName.empty())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Field schema name is empty."));
    }
    const FieldSchema* pFieldSchema = getSchema(sName);
    if (!pFieldSchema)
    {
        return addField(sName, sType, false, false);
    }
    return pFieldSchema->getId();
}

fieldid_t DocumentSchema::addSortableField(
        const string& sName, FieldType::ValueType vt, bool bStore)
{
    if (sName.empty())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Field schema name is empty."));
    }

    const FieldType* pFieldType = NULL;
    if (m_pFieldDefinition)
    {
        pFieldType = m_pFieldDefinition->getForwardIndexFieldType(vt);
    }
    
    if (!pFieldType)
    {
        pFieldType = BuiltInFieldDefinition::instance()->getForwardIndexFieldType(vt);
    }

    if (!pFieldType)
    {
        FIRTEX_THROW(IllegalArgumentException, _T("No forward index field defined."));
    }
    
    FieldSchema* pFieldSchema = getSchema(sName);
    if (!pFieldSchema)
    {
        pFieldSchema = new FieldSchema(sName, pFieldType, bStore);
        pFieldSchema->setId((fieldid_t)m_fields.size());
        m_fields.push_back(pFieldSchema);
    }
    else 
    {
        pFieldSchema->setFieldType(pFieldType);
        pFieldSchema->setStore(bStore);
    }
    return pFieldSchema->getId();
}

fieldid_t DocumentSchema::addField(const string& sName, const string& sType,
                                   bool bStore, bool bSortAndFilterale,
                                   float fBoost)
{
    if (sName.empty())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Field schema name is empty."));
    }

    const FieldType* pFieldType = NULL;
    if (!sType.empty())
    {
        pFieldType = getFieldType(sType, bSortAndFilterale);
        FIRTEX_ASSERT2(pFieldType != NULL);
    }

    FieldSchema* pFieldSchema = getSchema(sName);
    if (!pFieldSchema)
    {
        pFieldSchema = new FieldSchema(sName, pFieldType, bStore, fBoost);
        pFieldSchema->setId((fieldid_t)m_fields.size());
        m_fields.push_back(pFieldSchema);
    }
    else 
    {
        pFieldSchema->setFieldType(pFieldType);
        pFieldSchema->setStore(bStore);
        pFieldSchema->setBoost(fBoost);
    }
    return pFieldSchema->getId();
}

fieldid_t DocumentSchema::addField(const FieldSchema& fieldSchema)
{
    FieldSchema* pFieldSchema = new FieldSchema(fieldSchema);
    pFieldSchema->setId((fieldid_t)m_fields.size());
    m_fields.push_back(pFieldSchema);
    return pFieldSchema->getId();
}

int32_t DocumentSchema::mergeSchema(const DocumentSchema& schema)
{
    const FieldSchema* pFieldSchema = NULL;
    int32_t nNumAdded = 0;
    DocumentSchema::Iterator iter = schema.iterator();
    while (iter.hasNext())
    {
        pFieldSchema = iter.next();
        if (this->getSchema(pFieldSchema->getName()) == NULL)
        {
            this->addField(*pFieldSchema);
            nNumAdded++;
        }
    }
    return nNumAdded;
}

const FieldSchema* DocumentSchema::getSchema(const string& sName) const
{
    if (sName.empty())
    {
        return NULL;
    }

    FieldSchemaVector::const_iterator iter = m_fields.begin();
    while (iter != m_fields.end())
    {
        if (!strCompareNoCase((*iter)->m_sName, sName))        
        {
            return (*iter);
        }
        iter++;
    }
    return NULL;
}

FieldSchema* DocumentSchema::getSchema(const string& sName)
{
    if (sName.empty())
    {
        return NULL;
    }

    FieldSchemaVector::iterator iter = m_fields.begin();
    while (iter != m_fields.end())
    {
        if (!strCompareNoCase((*iter)->m_sName, sName))
        {
            return (*iter);
        }
        iter++;
    }
    return NULL;
}

const FieldSchema* DocumentSchema::getSchema(fieldid_t fieldId) const 
{
    FieldSchemaVector::const_iterator iter = m_fields.begin();
    while (iter != m_fields.end())
    {
        if ((*iter)->getId() == fieldId)
            return (*iter);
        iter++;
    }
    return NULL;
}

void DocumentSchema::clear()
{
    FieldSchemaVector::iterator iter = m_fields.begin();
    while (iter != m_fields.end())
    {
        delete (*iter);
        iter++;
    }
    m_fields.clear();
    if (m_pFieldDefinition)
    {
        delete m_pFieldDefinition;
        m_pFieldDefinition = NULL;
    }
}

DocumentSchema::Iterator DocumentSchema::iterator() const
{
    return DocumentSchema::Iterator(this);
}

bool DocumentSchema::isEqual(const DocumentSchema& schema) const
{
    if (m_fields.size() < schema.m_fields.size())
    {
        //this is a hack way...
        //TODO: 
        return false;
    }
    FieldSchemaVector::const_iterator it2;
    for(FieldSchemaVector::const_iterator iter = m_fields.begin(), 
                                           it2 = schema.m_fields.begin();
        it2 != schema.m_fields.end(); iter++, it2++)
    {
        if (!((*iter)->isEqual((*(*it2))) ) )
        {
            return false;
        }
    }
    return true;
}

DocumentSchema& DocumentSchema::operator = (const DocumentSchema& right)
{
    clear();

    FieldSchemaVector::const_iterator iter = right.m_fields.begin();
    while (iter != right.m_fields.end())
    {
        m_fields.push_back(new FieldSchema(*(*iter)));
        iter++;
    }
    return *this;
}

void DocumentSchema::setFieldDefinition(const FieldDefinition& fieldDef)
{
    if (m_pFieldDefinition)
    {
        delete m_pFieldDefinition;
    }
    m_pFieldDefinition = new FieldDefinition(fieldDef);
}

void DocumentSchema::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator rootConf;
        if (m_pFieldDefinition)
        {
            m_pFieldDefinition->configure(rootConf);
        }

        Configurator fieldsConf;
        fieldsConf.configure("default_field", m_sDefField);
        for (FieldSchemaVector::const_iterator it = m_fields.begin();
             it != m_fields.end(); ++it)
        {
            Configurator fieldConf;
            (*it)->configure(fieldConf);
            fieldsConf.configure(_T("field"),  fieldConf.getMap());
        }
        rootConf.configure(_T("fields"), fieldsConf.getMap());
        
        conf.configure(_T("index_schema"), rootConf.getMap());
    }
    else 
    {
        clear();
    
        Configurator::Iterator it = conf.findConf(_T("index_schema"));
        if (it.hasNext())
        {
            Configurator rootConf(AnyCast<Configurator::ConfMap>(it.next().second));
            Configurator::Iterator it2 = rootConf.findConf(_T("type_define"));
            if (it2.hasNext())
            {
                m_pFieldDefinition = new FieldDefinition();
                m_pFieldDefinition->configure(rootConf);
            }

            it2 = rootConf.findConf(_T("fields"));
            if (it2.hasNext())
            {
                Configurator fieldsConf(AnyCast<Configurator::ConfMap>(
                                it2.next().second));
                
                it = fieldsConf.iterator();
                while (it.hasNext())
                {
                    Configurator::KeyValuePair kv = it.next();
                    if (kv.first == "default_field")
                    {
                        m_sDefField = AnyCast<std::string>(kv.second);
                    } 
                    else if (kv.first == "field")
                    {
                        FieldSchema fs;
                        Configurator fieldConf(AnyCast<Configurator::ConfMap>(kv.second));
                        fs.configure(fieldConf);
                        if (!fs.getTypeName().empty())
                        {
                            const FieldType* pFieldType = getFieldType(fs.getTypeName(), fs.isFilterable());
                            fs.setFieldType(pFieldType);
                        }
                        addField(fs);
                    }//endif
                }//end while
            }//end if
        }
    }
}

const FieldType* DocumentSchema::getFieldType(const string& sTypeName,
        bool bSortAndFilterable)
{
    const FieldType* pFieldType = NULL;
    if (m_pFieldDefinition)
    {
        pFieldType = m_pFieldDefinition->getFieldType(sTypeName);
    }
    
    if (!pFieldType)
    {
        pFieldType = BuiltInFieldDefinition::instance()->getFieldType(sTypeName);
    }

    if (!pFieldType)
    {
        FIRTEX_THROW(UnsupportedException,
                     _T("Unsupport field type: [%s]"), sTypeName.c_str());
    }
    
    if (bSortAndFilterable && !pFieldType->hasForwardIndex())
    {
        string str = pFieldType->getName() + FORWARD_INDEX_SUFFIX;
        if (m_pFieldDefinition)
        {
            const FieldType* pFieldType2 = m_pFieldDefinition->getFieldType(str);
            if (pFieldType2)
            {
                return pFieldType2;
            }
        }
        FieldType sortFieldType(*pFieldType);
        sortFieldType.setName(str);
        sortFieldType.setForwardIndex(bSortAndFilterable);
        if (!m_pFieldDefinition)
        {
            m_pFieldDefinition = new FieldDefinition();
        }
        m_pFieldDefinition->addFieldType(sortFieldType);
        pFieldType = m_pFieldDefinition->getFieldType(str);
    }
    return pFieldType;
}

FX_NS_END
