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
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/1/4
//
#include "firtex/index/MultiTermReader.h"
#include "firtex/index/MultiTermPostingIterator.h"
#include "firtex/index/MultiIndexBarrelReader.h"
#include "firtex/index/MultiTermIterator.h"
#include "firtex/config/GlobalConfig.h"

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, MultiTermReader);

MultiTermReader::MultiTermReader(void)
{
}

MultiTermReader::MultiTermReader(const MultiIndexBarrelReader* pBarrelReader)
    : m_pBarrelReader((MultiIndexBarrelReader*)pBarrelReader)
{
    loadTermReaders();
}

MultiTermReader::~MultiTermReader(void)
{
    m_pBarrelReader = NULL;
}

void MultiTermReader::open(FileSystemPtr& pFileSystem,
                           const tstring& sBarrelSuffix, 
                           const FieldSchema* pFieldSchema, 
                           const BitVector* pDocFilter)
{
    FIRTEX_THROW(UnsupportedOpException, _T("Unsupported operation."));
}

TermIteratorPtr MultiTermReader::termIterator(const Term* pLowerTerm, const Term* pUpperTerm) const
{
    if (!pLowerTerm && !pUpperTerm)
    {
        FIRTEX_THROW(IllegalArgumentException, _T("At least one term must be non-null."));
    }
    if (pLowerTerm && pUpperTerm && pLowerTerm->getField() != pUpperTerm->getField())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Both terms must be for the same field"));
    }

    tstring field = pLowerTerm ? pLowerTerm->getField() : pUpperTerm->getField();
	
    MultiTermIterator* pTermIterator = new MultiTermIterator();
    TermIteratorPtr pTmpIter(pTermIterator);
    bool bHasIterator = false;
    for (TermReaderVector::const_iterator it = m_termReaders.begin();
         it != m_termReaders.end(); ++it)
    {
        TermIteratorPtr pIt((*it).second->termIterator(pLowerTerm, pUpperTerm));
        if (!pIt.isNull())
        {
            pTermIterator->addIterator((*it).first, pIt);
            bHasIterator = true;
        }
    }
    if (!bHasIterator)
    {
        return TermIteratorPtr();
    }
    pTermIterator->init();
    return pTmpIter;
}		

TermIteratorPtr MultiTermReader::termIterator(const tstring& field) const
{			
    MultiTermIterator* pTermIterator = new MultiTermIterator();
    TermIteratorPtr pTmpIter(pTermIterator);
    bool bHasIterator = false;
    TermIteratorPtr pIt;
    for (TermReaderVector::const_iterator it = m_termReaders.begin(); it != m_termReaders.end(); ++it)
    {
        TermIteratorPtr pIt((*it).second->termIterator(field));
        if (!pIt.isNull())
        {
            pTermIterator->addIterator((*it).first, pIt);
            bHasIterator = true;
        }
    }
    if (!bHasIterator)
    {
        return TermIteratorPtr();
    }
    pTermIterator->init();
    return pTmpIter;
}

TermPostingIteratorPtr MultiTermReader::seek(const Term* pTerm) const
{
    MultiTermPostingIterator* pIter = new MultiTermPostingIterator();
    TermPostingIteratorPtr pIterPtr(pIter);
    for (TermReaderVector::const_iterator it = m_termReaders.begin();
         it != m_termReaders.end(); ++it)
    {
        TermPostingIteratorPtr pSubIter = (*it).second->seek(pTerm);
        if (pSubIter.isNotNull())
        {
            pIter->add((*it).first, pSubIter);
        }
    }
    if (pIter->size() > 0)
    {
        pIter->init();
        return pIterPtr;
    }
    return TermPostingIteratorPtr();
}

TermReader* MultiTermReader::clone() const
{
    return new MultiTermReader(this->m_pBarrelReader);
}

void MultiTermReader::loadTermReaders()
{
    MultiIndexBarrelReader::Iterator iter = m_pBarrelReader->iterator();
    while (iter.hasNext())
    {
        MultiIndexBarrelReader::EntryPtr pEntry = iter.next();
        TermReaderPtr pTermReader(pEntry->m_pBarrel->termReader());
        if(!pTermReader.isNull())
        {
            m_termReaders.push_back(make_pair(pEntry->m_pBarrelInfo, pTermReader));
        }
    }
}		

FX_NS_END

