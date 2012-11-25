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
// Created	: 2010-07-31 21:47:59

#ifndef __FX_OUTPUTSTREAM_H
#define __FX_OUTPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"

FX_NS_DEF(store);

class OutputStream
{
public:
    virtual ~OutputStream() {}

public:
    /**
     * Write bufSize bytes to file
     * @param buffer bytes to write
     * @param bufSize number of bytes to write
     * @throw throw FileIOException if write failed. 
     */
    virtual void write(const void* buffer, size_t bufSize) = 0;
    
    /**
     * Read data from input stream and feed to this ouput stream
     * @param inStream input stream instance 
     * @param size number of bytes to feed
     */
    virtual void write(InputStream& inStream, offset_t size = (offset_t)(-1)) = 0;

    /**
     * Commit data to another output stream
     * @param outStream output stream instance
     */
    virtual void copyTo(OutputStream& outStream) = 0;
    
    /**
     * Flush all in-memory data to file system
     * @throw throw FileIOException if flush failed.
     */
    virtual void flush() = 0;

    /**
     * Seek to specified position in the file
     * @param pos new position to seek
     * @throw throw FileIOException if failed.
     */
    virtual void seek(offset_t pos) = 0;

    /**
     * Return the current position
     */
    virtual offset_t tell() const = 0;

    /**
     * Close this output stream
     */
    virtual void close() = 0;

public:
    virtual void writeString(const std::string& str)
    {
        writeInt32((int32_t)str.length());
        write(str.c_str(), str.length());
    }

public:
    inline void writeByte(uint8_t value);
    inline void writeInt32(int32_t value);
    inline void writeInt64(int64_t value);
    inline void writeVInt32(int32_t value);
    inline void writeVInt64(int64_t value);
};

DEFINE_TYPED_PTR(OutputStream);

///////////////////////////////////////////////////////////
inline void OutputStream::writeByte(uint8_t value)
{
    write((const void*)&value, sizeof(uint8_t));
}

inline void OutputStream::writeInt32(int32_t value)
{
    uint8_t buffer[4];
    buffer[0] = (uint8_t)((value) >> 24);
    buffer[1] = (uint8_t)((value >> 16));
    buffer[2] = (uint8_t)((value >> 8));
    buffer[3] = (uint8_t)(value);
    write((const void*)buffer, sizeof(buffer));
}

inline void OutputStream::writeInt64(int64_t value)
{
    writeInt32((int32_t)(value >> 32));
    writeInt32((int32_t)value);
}

inline void OutputStream::writeVInt32(int32_t value)
{
    uint8_t buffer[5];
    uint32_t ui = (uint32_t)value;
    size_t i = 0;
    while ((ui & ~0x7F) != 0) 
    {
        buffer[i++] = (uint8_t)((ui & 0x7f) | 0x80);
        ui >>= 7;
    }
    buffer[i] = (uint8_t)ui;
    write((const void*)buffer, (i + 1) * sizeof(uint8_t));
}

inline void OutputStream::writeVInt64(int64_t value)
{
    uint8_t buffer[9];
    uint64_t ui = value;
    size_t i = 0;
    while ((ui & ~0x7F) != 0) 
    {
        buffer[i++] = (uint8_t)((ui & 0x7f) | 0x80);
        ui >>= 7;
    }
    buffer[i] = (uint8_t)ui;
    write((const void*)buffer, (i + 1) * sizeof(uint8_t));
}

FX_NS_END

#endif //__FX_OUTPUTSTREAM_H
