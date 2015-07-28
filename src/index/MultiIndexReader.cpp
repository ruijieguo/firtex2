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
// Created	: 2007/10/03
//

#include "firtex/index/MultiIndexReader.h"
#include "firtex/index/MultiIndex.h"
#include "firtex/index/SingleIndexBarrelReader.h"
#include "firtex/index/MultiIndexBarrelReader.h"

FX_NS_USE(utility);
FX_NS_DEF(index)

SETUP_STREAM_LOGGER(index, MultiIndexReader);
	
MultiIndexReader::MultiIndexReader(MultiIndex* pIndex)
    : m_pIndex(pIndex)
{
    m_pBarrelsInfo = pIndex->getBarrelsInfo();
}

MultiIndexReader::~MultiIndexReader(void)
{
}

void MultiIndexReader::open()
{
    IndexReader::open(m_pIndex->getIndexBarrelKeeper());

    createBarrelReader();
}

void MultiIndexReader::createBarrelReader()
{
    //TODO:

    // FIRTEX_ASSERT((m_pIndex != NULL), "The index has not opened yet.");

    // size_t bc = m_pBarrelsInfo->getBarrelCount();
    // if(bc == 1)
    // {
    //     const BarrelInfo& lastBarrel = m_pBarrelsInfo->getLastBarrel();
    //     const IndexVector& indexVector = m_pIndex->getIndexVector();
    //     FIRTEX_ASSERT2(indexVector.size() > 0);

    //     const DeletedDocumentFilter* pDelDocFilter =
    //         indexVector[0]->getDeletedDocFilter();
    //     const BitVector* pDocFilter =
    //         pDelDocFilter->getDocFilter(lastBarrel.getBaseDocId());
    //     SingleIndexBarrelReader* pSingleReader  = new SingleIndexBarrelReader(
    //         indexVector[0]->getFileSystem(), indexVector[0]->getDocSchema(),
    //         indexVector[0]->getComponentBuilder());
    //     m_pBarrelReader.reset(pSingleReader);
    //     pSingleReader->open(&lastBarrel, pDocFilter);
    // }
    // else if(bc > 1)
    // {
    //     const IndexVector& indexVector = m_pIndex->getIndexVector();
    //     FIRTEX_ASSERT2(indexVector.size() > 0);

    //     MultiIndexBarrelReader* pMultiIndexReader;
    //     m_pBarrelReader = pMultiIndexReader = new MultiIndexBarrelReader(
    //         indexVector[0]->getFileSystem(), indexVector[0]->getDocSchema(),
    //         indexVector[0]->getComponentBuilder());
    //     pMultiIndexReader->open(indexVector);
    // }
    // else 
    // {
    //     FIRTEX_THROW(IndexCollapseException, _T("The index barrel number is 0."));
    // }
}

FX_NS_END

