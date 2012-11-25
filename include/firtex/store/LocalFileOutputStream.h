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
// Created	: 2010-12-25 19:44:47

#ifndef __FX_LOCALFILEOUTPUTSTREAM_H
#define __FX_LOCALFILEOUTPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/BinaryFile.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/OutputStream.h"

FX_NS_DEF(store);

class LocalFileOutputStream : public OutputStream
{
public:
    typedef enum
    {
        APPEND	= 1,
        CREATE	= 2
    }OpenMode;

public:
    LocalFileOutputStream();
    ~LocalFileOutputStream();

public:
    /**
     * Open file on local file system
     * @param sFileName file name
     * @param access mode: append or create
     * @throw throw FileIOException if failed
     */
    void open(const std::string& sFileName, OpenMode om);

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
    void write(InputStream& inStream, offset_t size = (offset_t)(-1));

    /**
     * Write string to file
     * @param string to write
     * @throw throw FileIOException if write failed. 
     */
    void writeString(const std::string& str);

    /**
     * Commit data to another output stream
     * @param outStream output stream instance
     */
    void copyTo(OutputStream& outStream);
    
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

private:
    FX_NS(utility)::BinaryFile m_fileWriter;
};

DEFINE_TYPED_PTR(LocalFileOutputStream);

FX_NS_END

#endif //__FX_LOCALFILEOUTPUTSTREAM_H
