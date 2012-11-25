//
// Copyright(C) 2005--2010 FirteX Develop Team
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 20010/01/13
//
#include "firtex/index/primarykey/PrimaryKeyTermIterator.h"
#include "firtex/index/primarykey/PrimaryKeyTermReader.h"
#include "firtex/index/TermInfo.h"
#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/index/BarrelTermPositionIterator.h"
#include "firtex/index/PostingWriter.h"

#include <sstream>
#include <limits>
using namespace std;

FX_NS_DEF(index);

PrimaryKeyTermIterator::PrimaryKeyTermIterator()
    : m_curDocId(INVALID_DOCID)
    , m_pCurPostIterator(NULL)
{
}

PrimaryKeyTermIterator::~PrimaryKeyTermIterator(void)
{			
}

void PrimaryKeyTermIterator::init(const PostingTablePtr& pPostTable)
{
    m_pPostingTable = pPostTable;
    m_curDocId = INVALID_DOCID;
    m_pCurPostIterator = new PrimaryKeyPostingIterator();
    m_pIteratorPtr = m_pCurPostIterator;
    m_termIterator = pPostTable->iterator();
}

bool PrimaryKeyTermIterator::hasNext()
{
    return m_termIterator.hasNext();
}

TermIterator::TermEntry PrimaryKeyTermIterator::next()
{
    PostingTable::KeyValueType& kv = m_termIterator.next();
    m_curDocId = kv.second;
    m_curTerm.setValue(kv.first);

    m_pCurPostIterator->setValue(m_curDocId);
   
    TermEntry entry;
    entry.term = &m_curTerm;
    entry.postingIterator = m_pIteratorPtr;
    return entry;
}		

int64_t PrimaryKeyTermIterator::size() const
{
    return m_pPostingTable->size();
}

FX_NS_END
