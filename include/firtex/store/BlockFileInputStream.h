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
// Created	: 2010-07-31 17:20:19

#ifndef __FX_BLOCKFILEINPUTSTREAM_H
#define __FX_BLOCKFILEINPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/BlockFileSystem.h"

FX_NS_DEF(store);

class BlockFileInputStream : public InputStream
{
public:
    BlockFileInputStream(BlockFileSystem& fileSystem,
                         const BlockFileSystem::FilePtr& file);
    BlockFileInputStream(const BlockFileInputStream& src);
    ~BlockFileInputStream();

public:
    typedef BlockFileSystem::blockid_t blockid_t;

public:
    /**
     * Read bufSize bytes from file
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
    bool isEof() const ;

    /**
     * Close this input stream
     */
    void close();

    /**
     * Clone a new object
     */
    InputStream* clone() const;

protected:
    BlockFileSystem& m_fileSystem;
    BlockFileSystem::FilePtr m_pFile;
    char* m_pBlockBuffer;
    uint32_t m_nBlockSize;
    uint32_t m_nOffInBlk;
    offset_t m_nCurPos;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(BlockFileInputStream);

FX_NS_END

#endif //__FX_BLOCKFILEINPUTSTREAM_H
