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
// Email	: ruijieguo@gmail.com
// Created	: 2010/01/13
//
#ifndef __PRIMARYKEYTERMITERATOR_H
#define __PRIMARYKEYTERMITERATOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermIterator.h"
#include "PrimaryKeyTermReader.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/utility/HashMap.h"
#include "firtex/index/primarykey/PrimaryKeyPostingIterator.h"

FX_NS_DEF(index)

class PrimaryKeyTermReader;

class PrimaryKeyTermIterator :	public TermIterator
{
public:
    typedef FX_NS(utility)::HashMap<uint64_t, docid_t> PostingTable;
    typedef FX_NS(common)::SharedPtr<PostingTable> PostingTablePtr;

public:
    PrimaryKeyTermIterator();
    virtual ~PrimaryKeyTermIterator(void);

public:
    /**
     * Inialize
     */
    void init(const PostingTablePtr& pPostTable);

    /**
     * Check if the iterator has next element or not
     */
    bool hasNext();
	
    /**
     * Return the next element
     */
    TermEntry next();

    /**
     * Return number of terms
     */
    int64_t size() const;

private:
    PostingTablePtr m_pPostingTable;
    PostingTable::Iterator m_termIterator;

    UInt64Term m_curTerm;
    docid_t m_curDocId;
    PrimaryKeyPostingIterator* m_pCurPostIterator;
    TermPostingIteratorPtr m_pIteratorPtr;
};

FX_NS_END

#endif
