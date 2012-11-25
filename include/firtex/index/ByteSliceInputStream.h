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
// Created	: 2010-09-11 20:35:58

#ifndef __FX_BYTESLICEINPUTSTREAM_H
#define __FX_BYTESLICEINPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/ByteSlice.h"
#include "firtex/store/InputStream.h"

FX_NS_DEF(index);

class ByteSliceInputStream : public FX_NS(store)::InputStream
{
public:
    ByteSliceInputStream(ByteSlice* pHeadSlice, bool bDestroySlice, size_t nSize = (size_t)-1);
    ByteSliceInputStream(const ByteSliceInputStream& src);
    ~ByteSliceInputStream();

public:
    /**
     * Read bufSize bytes from file
     *
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    size_t read(void* buffer, size_t bufSize);

    /**
     * Read bytes from file. If the reading bytes can be hold in buffer, 
     * memory copy operation is avoid.
     *
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    size_t readMayCopy(void*& buffer, size_t bufSize);

    /**
     * Seek to specified position in the file
     * @param pos new position to seek
     * @return position actually sought
     * @throw throw FileIOException if failed.
     */
    void seek(offset_t pos);

    /**
     * Read bufSize bytes from file at specified position
     * @param pos position to read from
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    size_t seekAndRead(offset_t pos, void* buffer, size_t bufSize);

    /**
     * Return the current seek position
     * @throw throw FileIOException if read failed.
     */
    offset_t tell() const;

    /**
     * Get size of file
     */
    offset_t getSize() const;

    /**
     * Return the absolute path of the input stream
     * 
     * @return absolute path of stream
     */
    std::string getFilePath() const;

    /**
     * End of file or not
     */
    bool isEof() const;

    /**
     * Close this input stream
     */
    void close();

    /**
     * Clone a new object
     */
    FX_NS(store)::InputStream* clone() const;

protected:
    ByteSlice* m_pHeadSlice;
    size_t m_nTotalSize;

    ByteSlice* m_pCurSlice;
    uint32_t m_nCurPosInSlice;
    offset_t m_nCurPos;
    bool m_bDestroySlice;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(ByteSliceInputStream);

FX_NS_END

#endif //__FX_BYTESLICEINPUTSTREAM_H
