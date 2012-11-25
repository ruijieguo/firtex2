//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2007/4/23
//

#ifndef __FX_MMAPFILE_H
#define __FX_MMAPFILE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/File.h"
#include "firtex/common/IO.h"
#include <algorithm>
#include <cstddef>

FX_NS_DEF(utility);

class MMapFileImpl;

class MMapFile
{
public:
    typedef FX_OS_FILE_DESCRIPTOR FileHandle;

public:
    enum AccessMode
    {
        AM_UNKNOW = 0,
        AM_READ,
        AM_WRITE
    };

    /**
     * Default constructor creates an unmapped MMapFile object.
     */
    MMapFile();

    /** Creates a MMapFile object by copying another one */
    ~MMapFile();


public:
    /**
     * Creates or connects to a mapped file with the given name.
     * @param sFileName file name to map. 
     * @param mode file access mode, AM_READ for read AM_WRITE for write.
     * @param pAddrHit specifying the desired start address of the shared
     *        memory area. 
     *        Whether the hint is actually honored is, however, up to the system.
     *        Windows platform will generally ignore the hint.
     * @param bServer if false, the mapped file will be unlinked by 
     *        calling shm_unlink (on POSIX platforms) 
     *        when the MMapFile object is destroyed. 
     *        The bServer parameter is ignored on Windows platforms.
     */
    void open(const std::string& sFileName, std::size_t size, AccessMode mode,
              const void* pAddrHint = 0, bool bServer = true);

    /**
     * Maps the whole file into a shared memory segment.
     * @param file file object
     * @param pAddrHit specifying the desired start address of 
     *        the shared memory area. 
     *        Whether the hint is actually honored is, however,
     *        up to the system. 
     *        Windows platform will generally ignore the hint.
     */
    void open(const File& file, AccessMode mode, 
              const void* pAddrHint = 0);

    /// Map a opened file handle
    void open(const FileHandle& fileHandle,
              AccessMode mode,
              size_t nMapSize,
              const void* pAddrHint = 0);

    /**
     * Close the opened file
     */
    void close();

public:
    /** Assigns another MMapFile object.*/
    MMapFile& operator = (const MMapFile& other);

    /** Swaps the MMapFile object with another one.*/
    void swap(MMapFile& other);

    /** 
     * Returns the start address of the mapped file.
     * @return begin address of mapped file, will be 
     *         NULL for illegal mapped file.
     */
    char* begin() const;

    /** 
     * Returns the one-past-end end address of the shared memory segment. 
     * @return end address of mapped file, will be NULL 
     *         for illegal mapped file.
     */
    char* end() const;

    /**
     * Return the file size 
     */
    int64_t length() const;

    /**
     * Return file name
     */
    std::string getFileName() const;

private:
    /** Creates a MMapFile object by copying another one */
    MMapFile(const MMapFile& other) {}

private:
    MMapFileImpl* m_pImpl;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(MMapFile);

//////////////////////////////////////////////////////////////
// inlines
//
inline void MMapFile::swap(MMapFile& other)
{
    using std::swap;
    swap(m_pImpl, other.m_pImpl);
}

FX_NS_END

#endif
