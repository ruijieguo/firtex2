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
// Created	: 2005/11/27
//
#include "firtex/index/SingleIndexBarrelReader.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/index/MultiFieldTermReader.h"
#include "firtex/index/StoredFieldsReaderImpl.h"
#include "firtex/index/Index.h"
#include "firtex/document/Field.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index)

SETUP_STREAM_LOGGER(index, SingleIndexBarrelReader);

SingleIndexBarrelReader::SingleIndexBarrelReader(
        const FileSystemPtr& pFileSys,
        const DocumentSchema* pDocSchema,
        const ComponentBuilder* pComponentBuilder)
    : IndexBarrelReader(pFileSys, pDocSchema, pComponentBuilder)
    , m_pBarrelInfo(NULL)
    , m_pDocFilter(NULL)
{
    size_t nIndexFields = 0;
    DocumentSchema::Iterator it = pDocSchema->iterator();
    while (it.hasNext())
    {
        const FieldSchema* pFieldSchema = it.next();
        if (pFieldSchema->isIndexed())
        {
            ++nIndexFields;
        }
    }

    m_bMultiIndexFields = (nIndexFields > 1);
}

SingleIndexBarrelReader::SingleIndexBarrelReader(const SingleIndexBarrelReader& src)
    : IndexBarrelReader(src)
    , m_pLengthNormReader(src.m_pLengthNormReader)
    , m_pBarrelInfo(NULL)
    , m_pDocFilter(NULL)
    , m_bMultiIndexFields(src.m_bMultiIndexFields)
{
    if (src.m_pTermReader)
    {
        m_pTermReader.reset(src.m_pTermReader->clone());
    }
}

SingleIndexBarrelReader::~SingleIndexBarrelReader(void)
{
    close();

    m_pTermReader.reset();
    m_forwardIndexReaders.clear();
}

IndexBarrelReader* SingleIndexBarrelReader::clone() const
{
    return new SingleIndexBarrelReader(*this);
}

void SingleIndexBarrelReader::open(const BarrelInfo* pBarrelInfo,
                                   const BitVector* pDocFilter)
{
    m_pBarrelInfo = pBarrelInfo;
    m_pDocFilter = pDocFilter;

    std::string sSuffix = m_pBarrelInfo->getSuffix();

    m_pLengthNormReader.reset(new LengthNormReader(m_pFileSys));
    m_pLengthNormReader->init(sSuffix, m_pDocSchema);

    createTermReader(sSuffix);
    createForwardIndex(sSuffix);
}

void SingleIndexBarrelReader::reopen(const BarrelInfo* pBarrelInfo,
                                     const BitVector* pDocFilter)
{
    m_pBarrelInfo = pBarrelInfo;
    m_pDocFilter = pDocFilter;
}

void SingleIndexBarrelReader::createForwardIndex(const std::string& sSuffix)
{
    DocumentSchema::Iterator iter = m_pDocSchema->iterator();
    while (iter.hasNext())
    {
        const FieldSchema* pFieldSchema = iter.next();
        if(pFieldSchema->hasForwardIndex())
        {
            fieldid_t fieldId = pFieldSchema->getId();
            ForwardIndexReader* pFDReader =
                m_pComponentBuilder->buildForwardIndexReader(fieldId);
            FIRTEX_ASSERT2(pFDReader != NULL);

            if (fieldId >= (fieldid_t)m_forwardIndexReaders.size())
            {
                m_forwardIndexReaders.resize(fieldId + 1);
            }
            m_forwardIndexReaders[fieldId].reset(pFDReader);
            pFDReader->open(m_pFileSys, pFieldSchema, FORWARD_INDEX_FILEEXT, sSuffix);
        }
    }
}

void SingleIndexBarrelReader::createTermReader(const std::string& sSuffix)
{
    FileSystemPtr& pFileSys = m_pFileSys;
    if (m_bMultiIndexFields)
    {
        MultiFieldTermReader* pMultiReader =
            new MultiFieldTermReader(m_pComponentBuilder);
        m_pTermReader.reset(pMultiReader);
        pMultiReader->open(pFileSys, m_pInStreamPool, sSuffix, getDocSchema(), m_pDocFilter);
    }
    else
    {
        DocumentSchema::Iterator iter = m_pDocSchema->iterator();
        while (iter.hasNext())
        {
            const FieldSchema* pFieldSchema = iter.next();
            if (pFieldSchema->isIndexed())
            {
                TermReader* pTermReader =
                    m_pComponentBuilder->buildTermReader(pFieldSchema->getId());
                FIRTEX_ASSERT2(pTermReader != NULL);

                m_pTermReader.reset(pTermReader);
                m_pTermReader->open(pFileSys, m_pInStreamPool, sSuffix,
                        pFieldSchema, m_pDocFilter);
                break;
            }
        }
    }
}

StoredFieldsReaderPtr SingleIndexBarrelReader::createStoredFieldsReader() const
{
    StoredFieldsReaderPtr pStoredFieldsReader(
            new StoredFieldsReaderImpl(m_pDocSchema));

    pStoredFieldsReader->open(m_pInStreamPool, m_pBarrelInfo->getSuffix());
    return pStoredFieldsReader;
}

bool SingleIndexBarrelReader::isDeleted(docid_t docId) const
{
    if (m_pDocFilter)
    {
        return m_pDocFilter->test((size_t)docId);
    }
    return false;
}

bool SingleIndexBarrelReader::hasDeletions() const
{
    return (m_pDocFilter ? m_pDocFilter->any() : false);
}

df_t SingleIndexBarrelReader::getDeletedDocCount() const
{
    return m_pBarrelInfo->getDeletedDocCount();
}

TermReaderPtr SingleIndexBarrelReader::termReader(const std::string& sField) const 
{
    if (!m_pTermReader)
        return TermReaderPtr();

    if (m_bMultiIndexFields)
    {
        MultiFieldTermReaderPtr pMultiFieldTermReader = 
            std::dynamic_pointer_cast<MultiFieldTermReader>(m_pTermReader);
        FIRTEX_ASSERT2(pMultiFieldTermReader);
        return pMultiFieldTermReader->termReader(sField);
    }
    else 
    {
        if (!strCompareNoCase(m_pTermReader->getFieldSchema()->getName().c_str(), sField.c_str()))
        {
            return TermReaderPtr(m_pTermReader->clone());
        }
    }
    return TermReaderPtr();
}

TermReaderPtr SingleIndexBarrelReader::termReader() const
{
    return m_pTermReader;
}

ForwardIndexIteratorPtr
SingleIndexBarrelReader::forwardIndexReader(const std::string& sField) const
{
    FIRTEX_ASSERT2(m_pDocSchema != NULL);
    fieldid_t fieldId = m_pDocSchema->getFieldId(sField);
    if (fieldId < 0 || fieldId >= (fieldid_t)m_forwardIndexReaders.size())
    {
        return ForwardIndexIteratorPtr();
    }
    return m_forwardIndexReaders[fieldId]->iterator();
}

const ForwardIndexReader* 
SingleIndexBarrelReader::getForwardIndexReader(const std::string& sField) const
{
    FIRTEX_ASSERT2(m_pDocSchema != NULL);
    fieldid_t fieldId = m_pDocSchema->getFieldId(sField);
    if (fieldId < 0 || fieldId >= (fieldid_t)m_forwardIndexReaders.size())
    {
        return NULL;
    }
    return m_forwardIndexReaders[fieldId].get();
}

const LengthNormReader* SingleIndexBarrelReader::getLengthNormReader() const
{
    return m_pLengthNormReader.get();
}

LengthNormIteratorPtr
SingleIndexBarrelReader::lengthNorm(const std::string& sField) const
{
    return m_pLengthNormReader->lengthNorm(sField);
}

void SingleIndexBarrelReader::close()
{
    m_pLengthNormReader.reset();
}

FX_NS_END

