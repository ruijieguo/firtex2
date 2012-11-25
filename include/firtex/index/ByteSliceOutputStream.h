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
// Created	: 2008-02-25 19:40:53

#ifndef __FX_BYTESLICEOUTPUTSTREAM_H
#define __FX_BYTESLICEOUTPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/OutputStream.h"
#include "firtex/store/InputStream.h"
#include "firtex/utility/Pool.h"
#include "firtex/index/ByteSlice.h"

FX_NS_DEF(index);

class ByteSliceOutputStream : public FX_NS(store)::OutputStream
{
public:
    static size_t ALLOC_STRATEGY;
    static size_t LOWER_LIMIT;
    static size_t UPPER_LIMIT;
    static float INC_RATIO;

    const static size_t ALLOC_CONST = 0x01;
    const static size_t ALLOC_EXP = 0x02;
    const static size_t ALLOC_EXPLIMIT = 0x03;

    const static size_t DEFAULT_EXCEPTION_ALLOC_SIZE = 32;

public:
    ByteSliceOutputStream(FX_NS(utility)::Pool* pPool);
    ~ByteSliceOutputStream();

public:
    /**
     * Write bufSize bytes to file
     * @param buffer bytes to write
     * @param bufSize number of bytes to write
     * @throw throw FileIOException if write failed. 
     */
    void write(const void* buffer, size_t bufSize);

    /**
     * Read data from input stream and feed to this ouput stream
     * @param inStream input stream instance 
     * @param size number of bytes to feed
     */
    void write(FX_NS(store)::InputStream& inStream, offset_t size = (offset_t)(-1));
    
    /**
     * Flush all in-memory data to file system
     * @throw throw FileIOException if flush failed.
     */
    void flush();

    /**
     * Seek to specified position in the file
     * @param pos new position to seek
     * @throw throw FileIOException if failed.
     */
    void seek(offset_t pos);

    /**
     * Return the current position
     */
    offset_t tell() const;

    /**
     * Close this output stream
     */
    void close();

public:

    /**
     * Commit data to another output stream, etc. file system output stream
     * @param outStream output stream instance
     */
    void copyTo(FX_NS(store)::OutputStream& outStream);

    /**
     * Reset for reuse
     */
    void reset();

    /**
     * Return head byte slice of the writer
     */
    inline ByteSlice* getHeadSlice() const;

    /** Return memory pool */
    inline FX_NS(utility)::Pool* allocatePool();

protected:
    /**
     * Allocate a new slice from mem pool
     */
    void newSlice();

    /**
     * Return the next slice size
     * @param nCurSize current slice size
     */
    inline size_t getSliceSize(size_t nCurSize) const;

protected:
    ByteSlice* m_pHeadSlice;	///Posting list header
    ByteSlice* m_pTailSlice;	///Posting list tail
    size_t m_nTotalSize;	///Total size
    uint8_t* m_pWriteUpto;	

    FX_NS(utility)::Pool* m_pPool;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(ByteSliceOutputStream);

//////////////////////////////////////////////////////
//


inline ByteSlice* ByteSliceOutputStream::getHeadSlice() const
{
    return m_pHeadSlice;
}

inline FX_NS(utility)::Pool* ByteSliceOutputStream::allocatePool()
{
    return m_pPool;
}

inline size_t ByteSliceOutputStream::getSliceSize(size_t nCurSize) const
{
    if (nCurSize == 0)
    {
        return LOWER_LIMIT;
    }
	
    size_t nNewSize = 0;

    if (ALLOC_STRATEGY == ALLOC_EXPLIMIT)
    {
        nNewSize = std::min(UPPER_LIMIT, std::max(LOWER_LIMIT,
                        (size_t)(nCurSize*(INC_RATIO - 1) + 0.5)));
    }
    else if (ALLOC_STRATEGY == ALLOC_EXP)
    {
        nNewSize = std::max(LOWER_LIMIT, (size_t)(nCurSize*(INC_RATIO - 1) + 0.5));
    }
    else if (ALLOC_STRATEGY == ALLOC_CONST)
    {
        nNewSize = LOWER_LIMIT;
    }
    return nNewSize;
}

FX_NS_END

#endif //__FX_BYTESLICEOUTPUTSTREAM_H
