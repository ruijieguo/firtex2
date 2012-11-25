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

#ifndef __FX_POSSKIPLISTWRITER_H
#define __FX_POSSKIPLISTWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TypedSkipListWriter.h"

FX_NS_DEF(index);

class PosSkipListWriter : public TypedSkipListWriter<offset_t>
{
public:
    PosSkipListWriter(uint32_t nMaxLevels,
                      FX_NS(utility)::Pool* pPostingPool,
                      FX_NS(utility)::Pool* pBufferPool);
    ~PosSkipListWriter();

public:
    /**
     * Add a skip point
     * @param posOff offset of pos-list
     */
    void addSkipPoint(offset_t posOff)
    {
        TypedSkipListWriter<offset_t>::addSkipPoint(posOff);
    }

    /**
     * Create skip list reader
     *
     * @return reader instance
     */
    TypedSkipListReader<offset_t>* createReader() const;

    /**
     * Write skip data to byte slice
     * @param nLevel level to write
     * @param sliceWriter slice writer
     * @param pPoints point array
     * @param pChildPointers child pointer array, only valid when nLevel > 0
     * @param nSize size of array
     * @param bSeal seal the last record if true
     */
    void writeRecord(uint32_t nLevel,
                     ByteSliceOutputStream& sliceWriter,
                     const offset_t* pPoints,
                     const offset_t* pChildPointers,
                     uint32_t nSize, bool bSeal);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_POSSKIPLISTWRITER_H
