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
// Created	: 2010-08-04 23:42:09

#ifndef __FX_FILESYSTEM_H
#define __FX_FILESYSTEM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/OutputStream.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/utility/MMapFile.h"
#include <string>
#include <set>

FX_NS_DEF(store);


#define DECLARE_FILESYSTEM_CREATOR(fileSysClass, policyName)            \
    class Creator : public FileSystem::Creator                          \
    {                                                                   \
    public:                                                             \
        Creator()                                                       \
        {                                                               \
            m_identifier += ".";                                        \
            m_identifier += policyName;                                 \
        }                                                               \
        virtual ~Creator(){}                                            \
    public:                                                             \
        virtual FileSystem* create() const {return new fileSysClass;}   \
    };

class FileSystem
{
public:
    class Creator
    {
    public:
        Creator() : m_identifier("filesystem")
        {}
        virtual ~Creator(){}

    public:
        virtual FileSystem* create() const = 0;
        virtual const std::string& getIdentifier() const {return m_identifier;}

    protected:
        std::string m_identifier;
    };
    DEFINE_TYPED_PTR(Creator);

public:
    typedef enum
    {
        READ    = 1,
        CREATE  = 2,
        APPEND  = 3,
    }OpenMode;

    typedef std::set<std::string> FileSet;

    typedef FX_NS(utility)::MMapFile MMapFile;
    typedef FX_NS(utility)::MMapFilePtr MMapFilePtr;
    
public:
    FileSystem() {}
    virtual ~FileSystem() {}

public:
    /**
     * Open file system in specified mode
     * @param sFsPath path of file system
     * @param mode file system mode, READ for read-only,
     *  CREATE for creating a new file
     * @throw throw FileIOException if failed.
     */
    virtual void open(const std::string& sFsPath, OpenMode mode) = 0;

    /**    
     * Close file system.
     */
    virtual void close() = 0;

    /**
     * Create a new file for write
     * @param sFileName relative file path of the file system
     * @return file stream for write
     * @throw throw FileIOException if creation failed.
     */
    virtual OutputStreamPtr createFile(const std::string& sFileName) = 0;

    /**
     * Open a existing file for read
     * @param sFileName relative file path of the file system
     * @return file stream for read
     * @throw throw FileIOException if creation failed.
     */
    virtual InputStreamPtr openFile(const std::string& sFileName) = 0;

    /**
     * Mmap a existing file for read
     * @param sFileName relative file path of the file system
     * @return mmap file for read
     * @throw FileIOException
     */
    virtual MMapFilePtr mmapFile(const std::string& sFileName) = 0;

    /** 
     * Return true iff the file exists. 
     */
    virtual bool fileExists(const std::string& sFileName) = 0;

    /**
     * Delete a specified file in the file system
     * @throw throw FileIOException if failed.
     */
    virtual void deleteFile(const std::string& sFileName) = 0;

    /**
     * Rename a file
     * @param sForm source file name
     * @param sTo dest file name
     * @throw throw FileIOException if failed.
     */
    virtual void renameFile(const std::string& sFrom,
                            const std::string& sTo) = 0;

    /** 
     * Copies the file (or directory) to the given path and 
     * removes the original file. The target path can be a directory.
     */
    virtual void moveTo(const std::string& sFrom,
                        const std::string& sTo) = 0;

    /**
     * List all files in the file system
     * @return relative file paths in the file system
     */
    virtual FileSet listFiles() = 0;

    /**
     * Make directory if not exist
     * 
     * @param sDir directory name
     * @param bRecursive create directory recursively or not 
     * @throw throw FileIOException if creation failed.
     */
    virtual void makeDirectory(const std::string& sDir, bool bRecursive = false) = 0;

    /**
     * Delete directory if exists
     *
     * @param sDir directory name
     * @param bRecursive delete directory recursively or not 
     * @throw throw FileIOException if creation failed.
     */
    virtual void deleteDirectory(const std::string& sDir, bool bRecursive = false) = 0;

    /**
     * Return modified time of file
     */
    virtual FX_NS(utility)::Timestamp fileModified(const std::string& sFileName) = 0;
};

DEFINE_TYPED_PTR(FileSystem);

FX_NS_END

#endif //__FX_FILESYSTEM_H
