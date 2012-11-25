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
// Created	: 2010-07-22 23:23:20

#ifndef __FX_BLOCKFILESYSTEM_H
#define __FX_BLOCKFILESYSTEM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/IO.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/store/FileSystem.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/OutputStream.h"
#include "firtex/thread/FastMutex.h"
#include <string>
#include <set>
#include <list>
#include <map>

FX_NS_DEF(store);

class BlockFileSystem : public FileSystem
{
public:
    DECLARE_FILESYSTEM_CREATOR(BlockFileSystem, "block");

public:
    const static uint32_t LOGICAL_BLOCK_FACTOR = 1;

    const static uint32_t DEFAULT_BLOCK_SIZE = 32 * 1024; //4M

public:
    typedef FX_OS_OFFSET blockid_t;
    typedef FX_OS_FILE_DESCRIPTOR FileHandle;

    using FileSystem::MMapFile;
    using FileSystem::MMapFilePtr;

public:
    typedef std::map<std::string, offset_t> FileSizeMap;

    struct File
    {
    public:
        File(const std::string& sName, FileHandle fd, bool ro) 
            : totalBlocks(1)
            , fileName(sName)
            , fileHandle(fd)
            , fileActualSize(0)
            , readOnly(ro)
        {
        }

    public:
        // The number of blocks consumed by this file, PLUS 1.
        uint64_t totalBlocks;

        // File name relative to file system path
        std::string fileName;

        // File descriptor of this file
        FileHandle fileHandle;
        
        // actual 
        offset_t fileActualSize;
        
        // is this file read only
        bool readOnly;
    };

    DEFINE_TYPED_PTR(File);

    struct ListElemRemover
    {
        ListElemRemover(FilePtr& pFile) 
            : fileToRemove(pFile)
        {
        }

        bool operator() (const FilePtr& pFile) 
        {
            return (pFile->fileHandle == fileToRemove->fileHandle);
        }

        FilePtr& fileToRemove;
    };

public:
    BlockFileSystem();
    ~BlockFileSystem();

public:
    /**
     * Open file system in specified mode
     * @param sFsPath path of file system
     * @param mode file system mode, READ for read-only,
     *  CREATE for creating a new file
     * @throw throw FileIOException if failed.
     */
    void open(const std::string& sFsPath, FileSystem::OpenMode mode);

    /**    
     * Close file system.
     */
    void close();

    /**
     * Create a new file for write
     * @param sFileName relative file path of the file system
     * @return file stream for write
     * @throw throw FileIOException if creation failed.
     */
    OutputStreamPtr createFile(const std::string& sFileName);

    /**
     * Open a existing file for read
     * @param sFileName relative file path of the file system
     * @return file stream for read
     * @throw throw FileIOException if creation failed.
     */
    InputStreamPtr openFile(const std::string& sFileName);

    /**
     * Mmap a existing file for read
     * @param sFileName relative file path of the file system
     * @return mmap file for read
     * @throw FileIOException
     */
    MMapFilePtr mmapFile(const std::string& sFileName);

    /** 
     * Return true iff the file exists. 
     */
    bool fileExists(const std::string& sFileName);

    /**
     * Delete a specified file in the file system
     * @throw throw FileIOException if failed.
     */
    void deleteFile(const std::string& sFileName);

    /**
     * Rename a file
     * @param sForm source file name
     * @param sTo dest file name
     * @throw throw FileIOException if failed.
     */
    void renameFile(const std::string& sFrom,
                    const std::string& sTo);

    /** 
     * Copies the file (or directory) to the given path and 
     * removes the original file. The target path can be a directory.
     */
    void moveTo(const std::string& sFrom,
                const std::string& sTo);

    /**
     * List all files in the file system
     * @return relative file paths in the file system
     */
    FileSet listFiles();

    /**
     * Make directory if not exist
     * 
     * @param sDir directory name
     * @param bRecursive create directory recursively or not 
     * @throw throw FileIOException if creation failed.
     */
    void makeDirectory(const std::string& sDir, bool bRecursive = false);

    /**
     * Delete directory if exists
     *
     * @param sDir directory name
     * @param bRecursive delete directory recursively or not 
     * @throw throw FileIOException if creation failed.
     */
    void deleteDirectory(const std::string& sDir, bool bRecursive = false);

    /**
     * Return modified time of file
     */
    FX_NS(utility)::Timestamp fileModified(const std::string& sFileName);

    /**
     * Return file path of this file system
     */
    std::string getPath() const;

public:

    /**
     * Create a new file
     * @param sFileName relative file path of the file system
     * @return an opened file
     * @throw throw FileIOException if failed.
     */
    FilePtr doCreateFile(const std::string& sFileName);

    /**
     * Open an existing file
     * @param sFileName relative file path of the file system
     * @return an opened file
     * @throw throw FileIOException if failed.
     */
    FilePtr doOpenFile(const std::string& sFileName);

    /**
     * Close an opend file
     * @param pFile file object
     * @throw throw FileIOException if failed.
     */
    virtual void closeFile(FilePtr& pFile);

    /**
     * Read a block from a file
     * @param pFile file handle to read
     * @param blockId block id 
     * @param block read data stored here
     * @throw throw FileIOException if read failed.
     */
    void readBlock(FilePtr& pFile, blockid_t blockId, void* block);

    /**
     * Write a block to a file
     * @param pFile file handle to write
     * @param blockId block id 
     * @param block source data to write
     * @throw throw FileIOException if write failed.
     */
    void writeBlock(FilePtr& pFile, blockid_t blockId, const void* block);

    /**
     * Return block size of this file system
     */
    size_t getBlockSize() const;

    /**
     * Return number of opened files
     */
    size_t openedFiles();

    /**
     * Determin a file exists or not
     * @return true if exists
     */
    bool exists(const std::string& sFileName);

private:
    // void readMetaInfo();
    // void writeMetaInfo();
    void clearFiles();
    
    bool isPermitted(const std::string& sFileName, bool bForWrite) const;

    inline FX_OS_OFFSET blockIdToFileOffset(blockid_t blockId) const;

private:
    typedef std::list<FilePtr> FileList;
    typedef std::map<std::string, FileList> FileMap;

    std::string m_sFsPath;
    FileSystem::OpenMode m_mode;

    FX_NS(thread)::FastMutex m_lock;
    FileMap m_filesOpened;

    /// The size of a physical block on the device this file system resides.
    uint32_t m_nBlockSizeOfOS;

    // Size in bytes of each logical block.
    uint32_t m_nBlockSize;
    
private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(BlockFileSystem);

////////////////////////////////////////////
//
inline FX_OS_OFFSET 
BlockFileSystem::blockIdToFileOffset(blockid_t blockId) const 
{ 
    return m_nBlockSize * blockId; 
}

inline size_t BlockFileSystem::getBlockSize() const
{
    return m_nBlockSize;
}

inline std::string BlockFileSystem::getPath() const
{
    return m_sFsPath;
}

FX_NS_END

#endif //__FX_BLOCKFILESYSTEM_H
