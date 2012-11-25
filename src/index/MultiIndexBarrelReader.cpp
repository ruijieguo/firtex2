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
#include "firtex/index/MultiIndexBarrelReader.h"
#include "firtex/index/MultiTermReader.h"
#include "firtex/index/MultiStoredFieldsReader.h"
#include "firtex/index/Index.h"
#include <assert.h>

FX_NS_USE(utility);
FX_NS_USE(document);
FX_NS_USE(store);

FX_NS_DEF(index);

//////////////////////////////////////////////////////////////////////////
//MultiIndexBarrelReader::Iterator		
MultiIndexBarrelReader::Iterator::Iterator(MultiIndexBarrelReader* pMultiReader)
    : m_pMultiReader(pMultiReader)
    , m_iterator(pMultiReader->m_readers.begin())
{
}

MultiIndexBarrelReader::Iterator::Iterator(const Iterator& _iter)
    : m_pMultiReader(_iter.m_pMultiReader)
    , m_iterator(_iter.m_pMultiReader->m_readers.begin())
{
}

MultiIndexBarrelReader::Iterator::~Iterator(void)
{
    m_pMultiReader = NULL;
}

void MultiIndexBarrelReader::Iterator::reset()
{
    m_iterator = m_pMultiReader->m_readers.begin();
}

bool MultiIndexBarrelReader::Iterator::hasNext() const
{
    return (m_iterator != m_pMultiReader->m_readers.end());
}

MultiIndexBarrelReader::EntryPtr MultiIndexBarrelReader::Iterator::next()
{
    return *m_iterator++;
}

size_t MultiIndexBarrelReader::Iterator::size()const
{
    return m_pMultiReader->m_readers.size();
}

/////////////////////////////////////////////////////////////////////////
///MultiIndexBarrelReader
MultiIndexBarrelReader::MultiIndexBarrelReader(
    const FileSystemPtr& pFileSys,
    const DocumentSchema* pDocSchema,
    const ComponentBuilder* pComponentBuilder)
    : IndexBarrelReader(pFileSys, pDocSchema, pComponentBuilder)
{
}

MultiIndexBarrelReader::MultiIndexBarrelReader(const MultiIndexBarrelReader& src)
    : IndexBarrelReader(src)
    , m_pBarrelsInfo(src.m_pBarrelsInfo)
    , m_pDeletedDocFilter(src.m_pDeletedDocFilter)
{
    for (BarrelVector::const_iterator it = src.m_readers.begin();
         it != src.m_readers.end(); ++it)
    {
        m_readers.push_back((*it)->clone());
    }
}

MultiIndexBarrelReader::~MultiIndexBarrelReader()
{
    m_readers.clear();
}

IndexBarrelReader* MultiIndexBarrelReader::clone() const
{
    return new MultiIndexBarrelReader(*this);
}

void MultiIndexBarrelReader::open(const IndexVector& indices)
{
    for (IndexVector::const_iterator it = indices.begin();
         it != indices.end(); ++it)
    {
        addIndex(*it);
    }

    m_pTermReader.assign(new MultiTermReader(this));
}

void MultiIndexBarrelReader::open(const BarrelsInfoPtr& pBarrelsInfo,
                                  const DeletedDocumentFilterPtr& pDocFilter)
{
    m_pBarrelsInfo = pBarrelsInfo;
    m_pDeletedDocFilter = pDocFilter;


    BarrelsInfo::Iterator iter = m_pBarrelsInfo->iterator();
    while (iter.hasNext())
    {
        const BarrelInfo& barrelInfo = iter.next();
        if (barrelInfo.getDocCount() > 0)
        {
            docid_t baseDocdId = barrelInfo.getBaseDocId();
            const BitVector* pDocFilter = m_pDeletedDocFilter->getDocFilter(baseDocdId);

            EntryPtr pEntry = new Entry(m_pFileSys, m_pDocSchema, m_pComponentBuilder);
            pEntry->init(&barrelInfo, pDocFilter);
            //addFieldsInfo(pEntry->m_pBarrel->getFieldsInfo());
            m_readers.push_back(pEntry);
        }
    }

    m_pTermReader.assign(new MultiTermReader(this));
}

void MultiIndexBarrelReader::reopen(const BarrelsInfoPtr& pBarrelsInfo,
                                    const DeletedDocumentFilterPtr& pDocFilter)
{
    BarrelVector newReaders;
    BarrelsInfo::Iterator oldIt = m_pBarrelsInfo->iterator();
    BarrelsInfo::Iterator newIt = pBarrelsInfo->iterator();

    m_pDeletedDocFilter = pDocFilter;

    size_t cursor = 0;
    while (newIt.hasNext())
    {
        const BarrelInfo& newInfo = newIt.next();
        docid_t newBaseId = newInfo.getBaseDocId();
        const BitVector* pOneFilter =
            m_pDeletedDocFilter->getDocFilter(newBaseId);

        if (oldIt.hasNext())
        {
            const BarrelInfo& oldInfo = oldIt.next();
            if (oldInfo.getCommitId() == newInfo.getCommitId())
            {
                EntryPtr pEntry = m_readers[cursor];
                pEntry->m_pBarrelInfo = &newInfo;
                pEntry->m_pBarrel->reopen(&newInfo, pOneFilter);

                //addFieldsInfo(pEntry->m_pBarrel->getFieldsInfo());
                newReaders.push_back(pEntry);
                ++cursor;
                continue;
            }
            ++cursor;
        }

        if (newInfo.getDocCount() > 0)
        { 
            // Skip deletion only barrel
            EntryPtr pEntry = new Entry(m_pFileSys, m_pDocSchema, m_pComponentBuilder);
            pEntry->init(&newInfo, pOneFilter);

            newReaders.push_back(pEntry);
        }
    }
    m_pBarrelsInfo = pBarrelsInfo;

    m_readers.swap(newReaders);

    m_pTermReader.assign(new MultiTermReader(this));
}

void MultiIndexBarrelReader::addIndex(const Index* pIndex)
{
    //TODO: m_pDeletedDocFilter is incorrect
    BarrelsInfoPtr pBarrelsInfo(new BarrelsInfo);
    FileSystemPtr pFileSys = pIndex->getFileSystem();
    pBarrelsInfo->read(pFileSys);
    
    BarrelsInfo::Iterator iter = pBarrelsInfo->iterator();

    while (iter.hasNext())
    {
        const BarrelInfo& barrelInfo = iter.next();
        if(barrelInfo.getDocCount() > 0)
        {
            docid_t baseDocdId = barrelInfo.getBaseDocId();
            const DeletedDocumentFilter* pDelDocFilter = pIndex->getDeletedDocFilter();
            const BitVector* pDocFilter = pDelDocFilter->getDocFilter(baseDocdId);

            FileSystemPtr pFileSys = pIndex->getFileSystem();
            EntryPtr pEntry = new Entry(pFileSys, pIndex->getDocSchema(),
                    m_pComponentBuilder);
            pEntry->init(&barrelInfo, pDocFilter);

            //addFieldsInfo(pEntry->m_pBarrel->getFieldsInfo());
            m_readers.push_back(pEntry);
        }
    }
}

StoredFieldsReaderPtr MultiIndexBarrelReader::createStoredFieldsReader() const
{
    if (m_readers.size() == 0)
    {
        return StoredFieldsReaderPtr();
    }

    MultiStoredFieldsReaderPtr pMultiReader(new MultiStoredFieldsReader(m_pBarrelsInfo));
    
    for (BarrelVector::const_iterator iter = m_readers.begin();
         iter != m_readers.end(); ++iter)
    {
        pMultiReader->addRead((*iter)->m_pBarrel->createStoredFieldsReader());
    } 
    return pMultiReader;
}

TermReaderPtr MultiIndexBarrelReader::termReader() const
{
    return m_pTermReader;
}

TermReaderPtr MultiIndexBarrelReader::termReader(const tstring& sField) const
{
    assert(false);
    return TermReaderPtr();
}

LengthNormIteratorPtr 
MultiIndexBarrelReader::lengthNorm(const tstring& sField) const
{
    BarrelVector::const_iterator iter = m_readers.begin();
    EntryPtr pEntry = (*iter);
    LengthNormIteratorPtr pLengthNorm = pEntry->m_pBarrel->lengthNorm(sField);
    if (pLengthNorm.isNull())
    {
        return pLengthNorm;
    }
    iter++;
    
    while (iter != m_readers.end())
    {
        pEntry = (*iter);
        const LengthNormReader* pNormReader = pEntry->m_pBarrel->getLengthNormReader();
        pLengthNorm->addReader(pNormReader->getLengthNorm(sField),
                               pEntry->m_pBarrelInfo->getBaseDocId());
        iter++;
    }
    return pLengthNorm;
}

ForwardIndexIteratorPtr
MultiIndexBarrelReader::forwardIndexReader(const tstring& sField) const
{
    BarrelVector::const_iterator iter = m_readers.begin();
    EntryPtr pEntry = (*iter);
    ForwardIndexIteratorPtr pFdIndexIter =
        pEntry->m_pBarrel->forwardIndexReader(sField);
    if (pFdIndexIter.isNull())
    {
        return pFdIndexIter;
    }
    iter++;
    
    while (iter != m_readers.end())
    {
        pEntry = (*iter);
        pFdIndexIter->addReader(pEntry->m_pBarrel->getForwardIndexReader(sField),
                                pEntry->m_pBarrelInfo->getBaseDocId());
        iter++;
    }
    return pFdIndexIter;
}

MultiIndexBarrelReader::Iterator MultiIndexBarrelReader::iterator()
{
    return MultiIndexBarrelReader::Iterator(this);
}

FX_NS_END

