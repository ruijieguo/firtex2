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
// Created	: 2006/11/13
//
#include "firtex/index/primarykey/PrimaryKeyCreator.h"
#include "firtex/index/primarykey/PrimaryKeyMerger.h"
#include "firtex/index/primarykey/PrimaryKeyIndexer.h"
#include "firtex/index/primarykey/PrimaryKeyTermReader.h"
#include "firtex/document/Field.h"

FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PrimaryKeyCreator);

PrimaryKeyCreator::PrimaryKeyCreator(void)			
{
    m_identifier += _T(".primarykey");
}

PrimaryKeyCreator::~PrimaryKeyCreator(void)
{
}

FieldIndexer* PrimaryKeyCreator::createIndexer(
        const FieldSchema* pFieldSchema) const 
{
    return new PrimaryKeyIndexer(pFieldSchema);
}

FieldMerger* PrimaryKeyCreator::createMerger() const
{
    return new PrimaryKeyMerger();
}

TermReader* PrimaryKeyCreator::createTermReader() const
{
    return new PrimaryKeyTermReader();	
}

FX_NS_END
