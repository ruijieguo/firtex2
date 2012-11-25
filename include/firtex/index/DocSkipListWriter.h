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
// Created	: 2010-08-10 22:40:54

#ifndef __FX_DOCSKIPLISTWRITER_H
#define __FX_DOCSKIPLISTWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TypedSkipListWriter.h"
#include "firtex/index/DocSkipPoint.h"

FX_NS_DEF(index);

class DocSkipListWriter : public TypedSkipListWriter<DocSkipPoint>
{
public:
    DocSkipListWriter(uint32_t nMaxLevels,
                      FX_NS(utility)::Pool* pPostingPool,
                      FX_NS(utility)::Pool* pBufferPool);
    ~DocSkipListWriter();

public:
    /**
     * Add a skip point
     * @param docOff offset of doc-list
     */
    void addSkipPoint(docid_t docId, ctf_t cumSumOfTf, offset_t docOff)
    {
        TypedSkipListWriter<DocSkipPoint>::addSkipPoint(DocSkipPoint(docId, cumSumOfTf, docOff));
    }

    /**
     * Create skip list reader
     *
     * @return reader instance
     */
    TypedSkipListReader<DocSkipPoint>* createReader() const;

    /**
     * Write skip data to byte slice
     * @param nLevel level to write
     * @param sliceWriter slice writer
     * @param pKeys key array
     * @param pValues value array
     * @param pChildPointers child pointer array, only valid when nLevel > 0
     * @param nSize size of array
     * @param bSeal seal the last record if true
     */
    void writeRecord(uint32_t nLevel,
                     ByteSliceOutputStream& sliceWriter,
                     const DocSkipPoint* pValues,
                     const offset_t* pChildPointers,
                     uint32_t nSize, bool bSeal);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DOCSKIPLISTWRITER_H
