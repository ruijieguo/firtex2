//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-08-22 17:19:15

#ifndef __FX_POSTINGMERGER_H
#define __FX_POSTINGMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/FieldMerger.h"

FX_NS_DEF(index);

class PostingMerger
{
public:
    PostingMerger() {}
    virtual ~PostingMerger() {}

public:
    /**
     * Merge posting of a barrel
     * @param terms terms to merge
     */
    offset_t merge(const MergingTermVector& terms)
    {
        beginMerge(terms);
        for (MergingTermVector::const_iterator it = terms.begin();
             it != terms.end(); ++it)
        {
            doMerge(*it);
        }
        return endMerge();
    }

    /**
     * Will be invoked before merging postings
     */
    virtual void beginMerge(const MergingTermVector& terms) = 0;

    /**
     * Merge one term 
     */
    virtual void doMerge(const MergingTerm* pTerm) = 0;

    /**
     * Will be invoked after merging
     */
    virtual offset_t endMerge() = 0;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_POSTINGMERGER_H
