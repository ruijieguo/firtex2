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
// Created	: 2010-07-31 21:46:17

#ifndef __FX_BLOCKFILEOUTPUTSTREAM_H
#define __FX_BLOCKFILEOUTPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/OutputStream.h"
#include "firtex/store/BlockFileSystem.h"

FX_NS_DEF(store);

class BlockFileOutputStream : public OutputStream
{
public:
    typedef BlockFileSystem::blockid_t blockid_t;

public:
    BlockFileOutputStream(BlockFileSystem& fileSystem,
                          const BlockFileSystem::FilePtr& file);
    ~BlockFileOutputStream();

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
    void write(InputStream& inStream, offset_t size = (offset_t)(-1));

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

protected:
    inline void flushBlock();
    inline void updateFileSize();

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

/////////////////////////////////////////////////
//
inline void BlockFileOutputStream::flushBlock()
{
    if (m_nOffInBlk > 0)
    {
        if (m_nOffInBlk < m_nBlockSize)
        {
            memset(m_pBlockBuffer + m_nOffInBlk, 0, m_nBlockSize - m_nOffInBlk);
        }
        blockid_t blockId = m_nCurPos / m_nBlockSize;
        m_fileSystem.writeBlock(m_pFile, blockId, m_pBlockBuffer);
        m_nOffInBlk = 0;
        m_nCurPos = (offset_t)(++blockId) * m_nBlockSize;
    }
}

inline void BlockFileOutputStream::updateFileSize()
{
    if (m_nCurPos > m_pFile->fileActualSize)
    {
        m_pFile->fileActualSize = m_nCurPos;
    }
}

FX_NS_END

#endif //__FX_BLOCKFILEOUTPUTSTREAM_H
