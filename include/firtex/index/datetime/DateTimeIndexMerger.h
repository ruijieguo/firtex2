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
// Created	: 2010-08-22 15:44:12

#ifndef __FX_DATETIMEINDEXMERGER_H
#define __FX_DATETIMEINDEXMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/FieldMerger.h"
#include "firtex/index/TypedDictWriter.h"
#include "firtex/index/PosPostingMerger.h"

FX_NS_DEF(index);

class DateTimeIndexMerger : public FieldMerger
{
public:
    typedef TypedDictWriter<int64_t, offset_t> DictWriter;
    DEFINE_TYPED_PTR(DictWriter);

public:
    DateTimeIndexMerger();
    ~DateTimeIndexMerger();

protected:
    /**
     * Give dirived class a chance to know merging is begin
     * @param mergeFieldInfos field infos of multi index barrel
     */
    void beginMerge(const IndexMergeInfos& mergeFieldInfos);

    /**
     * Merge some terms inn different barrel
     * @param terms terms vectory to merge
     */
    void mergeTerms(const MergingTermVector& terms);

    /**
     * Give dirived class a chance to know merging is ended
     */
    void endMerge();

private:
    FX_NS(store)::OutputStreamPtr m_pDictOutStream;
    FX_NS(store)::OutputStreamPtr m_pDocOutStream;
    DictWriterPtr m_pDictWriter;

    int64_t m_nTotalTerms;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(DateTimeIndexMerger);

FX_NS_END

#endif //__FX_DATETIMEINDEXMERGER_H
