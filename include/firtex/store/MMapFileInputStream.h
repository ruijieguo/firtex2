//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-09 22:09:47

#ifndef __FX_MMAPFILEINPUTSTREAM_H
#define __FX_MMAPFILEINPUTSTREAM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/MMapFileSystem.h"
#include "firtex/utility/MMapFile.h"

FX_NS_DEF(store);

class MMapFileInputStream : public InputStream
{
public:
    MMapFileInputStream(MMapFileSystem& fileSystem,
                        const BlockFileSystem::FilePtr& pFile,
                        const FX_NS(utility)::MMapFilePtr& pMMapFile);
    MMapFileInputStream(const MMapFileInputStream& src);
    ~MMapFileInputStream();

public:
    /**
     * Read bytes from file
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    virtual size_t read(void* buffer, size_t bufSize);

    /**
     * Read bytes from file without memory copy operation
     *
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    virtual size_t readMayCopy(void*& buffer, size_t bufSize);

    /**
     * Seek to specified position in the file
     * @param pos new position to seek
     * @return position actually sought
     * @throw throw FileIOException if failed.
     */
    virtual void seek(offset_t pos);

    /**
     * Read bufSize bytes from file at specified position
     * @param pos position to read from
     * @param buffer bytes stored to
     * @param bufSize number of bytes to read
     * @return number of bytes actually read
     * @throw throw FileIOException if read failed.
     */
    virtual size_t seekAndRead(offset_t pos, void* buffer, size_t bufSize);

    /**
     * Return the current seek position
     * @throw throw FileIOException if read failed.
     */
    virtual offset_t tell() const;

    /**
     * Get size of file
     */
    virtual offset_t getSize() const;
    
    /**
     * Return the absolute path of the input stream
     * 
     * @return absolute path of stream
     */
    virtual std::string getFilePath() const;

    /**
     * End of file or not
     */
    virtual bool isEof() const;

    /**
     * Close this input stream
     */
    virtual void close();

    /**
     * Clone a new object
     */
    virtual InputStream* clone() const;

private:
    MMapFileSystem& m_fileSystem;
    BlockFileSystem::FilePtr m_pFile;
    FX_NS(utility)::MMapFilePtr m_pMMapFile;

    offset_t m_nCurPos;
    char* m_pBegin;
    char* m_pEnd;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(MMapFileInputStream);

FX_NS_END

#endif //__FX_MMAPFILEINPUTSTREAM_H
