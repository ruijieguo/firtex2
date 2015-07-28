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
#include "firtex/index/MultiFieldTermReader.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/document/Field.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(store);
FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, MultiFieldTermReader);

MultiFieldTermReader::MultiFieldTermReader(const ComponentBuilder* pComBuilder)
    : TermReader()
    , m_pComponentBuilder(pComBuilder)
{
}

MultiFieldTermReader::~MultiFieldTermReader(void)
{
}

void MultiFieldTermReader::open(const FileSystemPtr& pFileSys,
                                const InputStreamPoolPtr& pStreamPool,
                                const std::string& sBarrel,
                                const DocumentSchema* pDocSchema,
                                const BitVector* pDocFilter)
{
    TermReader* pTermReader = NULL;
    DocumentSchema::Iterator iter = pDocSchema->iterator();
    while(iter.hasNext())
    {
        const FieldSchema* pFieldSchema = iter.next();

        if (pFieldSchema->isIndexed())///index field
        {
            pTermReader = m_pComponentBuilder->buildTermReader(pFieldSchema->getId());
            if (pTermReader)
            {
                pTermReader->open(pFileSys, pStreamPool, sBarrel, 
                        pFieldSchema, pDocFilter);
                TermReaderPtr pTmp(pTermReader);
                m_fieldsTermReaders.insert(make_pair(
                                pFieldSchema->getName(), pTmp));
            }
        }
    }
}

void MultiFieldTermReader::open(const FileSystemPtr& pFileSystem,
                                const InputStreamPoolPtr& pStreamPool,
                                const std::string& sBarrel, 
                                const FieldSchema* pFieldSchema,
                                const BitVector* pDocFilter)
{
    FIRTEX_THROW(IllegalArgumentException, _T("Unsupported operation."));
}

TermReaderPtr MultiFieldTermReader::termReader(const std::string& sField) const
{
    ReaderMap::const_iterator iter = m_fieldsTermReaders.find(sField);
    if (iter != m_fieldsTermReaders.end())
    {
        return TermReaderPtr(iter->second->clone());
    }
    return TermReaderPtr();
}

TermIteratorPtr MultiFieldTermReader::termIterator(
        const Term* pLowerTerm, const Term* pUpperTerm) const
{
    if (!pLowerTerm && !pUpperTerm)
    {
        FIRTEX_THROW(IllegalArgumentException, _T("At least one term must be non-null."));
    }
    if (pLowerTerm && pUpperTerm && pLowerTerm->getField() != pUpperTerm->getField())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Both terms must be for the same field"));
    }
    ReaderMap::const_iterator iter 
        = m_fieldsTermReaders.find(pLowerTerm ? pLowerTerm->getField()
                : pUpperTerm->getField());
    if (iter != m_fieldsTermReaders.end())
        return iter->second->termIterator(pLowerTerm, pUpperTerm);
    return NULL;
}

TermIteratorPtr MultiFieldTermReader::termIterator(const std::string& sField) const
{
    ReaderMap::const_iterator iter = m_fieldsTermReaders.find(sField);
    if (iter != m_fieldsTermReaders.end())
        return iter->second->termIterator(sField);
    return NULL;
}

TermPostingIteratorPtr MultiFieldTermReader::seek(const Term* pTerm) const
{
    std::string field = pTerm->getField();
    ReaderMap::const_iterator iter = m_fieldsTermReaders.find(field);
    if (iter != m_fieldsTermReaders.end())
    {
        return iter->second->seek(pTerm);
    }
    return TermPostingIteratorPtr();
}

TermReader* MultiFieldTermReader::clone() const
{
    MultiFieldTermReader* pReader = new MultiFieldTermReader(m_pComponentBuilder);
    ReaderMap::const_iterator iter = m_fieldsTermReaders.begin();
    while (iter != m_fieldsTermReaders.end())
    {
        TermReaderPtr pTmp(iter->second->clone());
        pReader->m_fieldsTermReaders.insert(
                make_pair(iter->first, pTmp));
        iter++;
    }
    return pReader;
}

void MultiFieldTermReader::addTermReader(const std::string& field,
        const TermReaderPtr& pTermReader)
{
    m_fieldsTermReaders.insert(make_pair(field, pTermReader));
}

FX_NS_END

