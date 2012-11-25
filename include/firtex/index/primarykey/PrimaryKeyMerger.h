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
// Created	: 2005/12/3
//
#ifndef __FX_PRIMARYKEYMERGER_H
#define __FX_PRIMARYKEYMERGER_H

#include "firtex/common/StdHeader.h"
#include <vector>
#include "firtex/index/FieldMerger.h"
#include "firtex/index/primarykey/PrimaryKeyTermReader.h"
#include "firtex/store/InputStream.h"
#include "firtex/index/TermInfo.h"

FX_NS_DEF(index)

class PrimaryKeyMerger : public FieldMerger
{
public:
    typedef PrimaryKeyTermReader::PostingTable PostingTable;
    typedef PrimaryKeyTermReader::PostingTablePtr PostingTablePtr;

public:
    PrimaryKeyMerger();
    virtual ~PrimaryKeyMerger();

public:
    /**
     * Merge multi index barrel of one field to one barrel
     * @param mergeFieldInfos field infos of multi index barrel
     * @param fieldMeta merged field meta
     */
    void merge(const IndexMergeInfos& mergeFieldInfos, 
               FieldMeta& fieldMeta);

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
    void mergeTerms(const MergingTermVector& terms) {}

    /**
     * Give dirived class a chance to know merging is ended
     */
    void endMerge();

private:
    void mergeWith(const IndexMergeInfo& mergeFieldInfo,
                   const PostingTablePtr& postingTable);

protected:
    FX_NS(store)::OutputStreamPtr m_pPkOutStream;
    PostingTable m_postingTable;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
