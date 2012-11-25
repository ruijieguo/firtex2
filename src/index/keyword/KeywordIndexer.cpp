//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-12-25 18:34:23

#include "firtex/index/keyword/KeywordIndexer.h"
#include "firtex/index/keyword/InMemKeywordTermReader.h"
#include <limits>

FX_NS_DEF(index);

SETUP_LOGGER(index, KeywordIndexer);

TermReaderPtr KeywordIndexer::termReader() const
{
    InMemKeywordTermReader* pTermReader = new InMemKeywordTermReader();
    TermReaderPtr pTermReaderPtr(pTermReader);
    pTermReader->init(this->getFieldSchema(), this->m_pPostingTable, NULL);
    return pTermReaderPtr;
}

FX_NS_END

