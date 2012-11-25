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
// Created	: 2010-07-31 13:44:48

#ifndef __FX_INPUTSTREAM_H
#define __FX_INPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(store);

class InputStream
{
public:
    InputStream() {}
    virtual ~InputStream() {}

public:
    /**
     * Read bufSize bytes from file
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    virtual size_t read(void* buffer, size_t bufSize) = 0;

    /**
     * Read bytes from file. If the reading bytes can be hold in buffer, 
     * memory copy operation is avoid.
     *
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    virtual size_t readMayCopy(void*& buffer, size_t bufSize) = 0;

    /**
     * Seek to specified position in the file
     * @param pos new position to seek
     * @return position actually sought
     * @throw throw FileIOException if failed.
     */
    virtual void seek(offset_t pos) = 0;

    /**
     * Read bufSize bytes from file at specified position
     * @param pos position to read from
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    virtual size_t seekAndRead(offset_t pos, void* buffer, size_t bufSize) = 0;

    /**
     * Return the current seek position
     * @throw throw FileIOException if read failed.
     */
    virtual offset_t tell() const = 0;

    /**
     * Get size of file
     */
    virtual offset_t getSize() const = 0;
    
    /**
     * Return the absolute path of the input stream
     * 
     * @return absolute path of stream
     */
    virtual std::string getFilePath() const = 0;

    /**
     * End of file or not
     */
    virtual bool isEof() const = 0;

    /**
     * Close this input stream
     */
    virtual void close() = 0;

    /**
     * Clone a new object
     */
    virtual InputStream* clone() const = 0;

public:
    inline uint8_t readByte();
    inline int32_t readInt32();
    inline int64_t readInt64();
    inline int32_t readVInt32();
    inline int64_t readVInt64();
};

DEFINE_TYPED_PTR(InputStream);

/////////////////////////////////////////////////////
//
inline uint8_t InputStream::readByte()
{
    uint8_t value[1];
    uint8_t* temp = value;
    read((void*&)temp, sizeof(uint8_t));
    return temp[0];
}

inline int32_t InputStream::readInt32()
{
    uint8_t b1 = readByte();
    uint8_t b2 = readByte();
    uint8_t b3 = readByte();
    uint8_t b4 = readByte();
    return (((b1 & 0xff) << 24) | ((b2 & 0xff) << 16) 
             | ((b3 & 0xff) << 8) | (b4 & 0xff));
}

inline int64_t InputStream::readInt64()
{
    int32_t v1 = readInt32();
    int32_t v2 = readInt32();
    return (((int64_t)v1) << 32) | (v2 & 0xFFFFFFFFL);
}

inline int32_t InputStream::readVInt32()
{
    uint8_t b = readByte();
    int32_t i = b & 0x7F;
    for (int32_t shift = 7; (b & 0x80) != 0; shift += 7) 
    {
        b = readByte();
        i |= (b & 0x7FL) << shift;
    }
    return i;
}

inline int64_t InputStream::readVInt64()
{
    uint8_t b = readByte();
    int64_t i = b & 0x7F;
    for (int32_t shift = 7; (b & 0x80) != 0; shift += 7)
    {
        b = readByte();
        i |= (b & 0x7FLL) << shift;
    }
    return i;
}

FX_NS_END

#endif //__FX_INPUTSTREAM_H
