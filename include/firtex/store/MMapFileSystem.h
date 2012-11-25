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
// Created : 2011-07-09 19:22:11

#ifndef __FX_MMAPFILESYSTEM_H
#define __FX_MMAPFILESYSTEM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/utility/MMapFile.h"
#include <map>

FX_NS_DEF(store);

class MMapFileSystem : public BlockFileSystem
{
public:
    DECLARE_FILESYSTEM_CREATOR(MMapFileSystem, "mmap");

public:
    MMapFileSystem();
    ~MMapFileSystem();

public:
    /**
     * Open a existing file for read
     * @param sFileName relative file path of the file system
     * @return file stream for read
     * @throw throw FileIOException if creation failed.
     */
    virtual InputStreamPtr openFile(const std::string& sFileName);


    /**
     * Close an opend file
     * @param pFile file object
     * @throw throw FileIOException if failed.
     */
    virtual void closeFile(FilePtr& pFile);

private:
    FX_NS(thread)::FastMutex m_mmapLock;
    typedef std::map<std::string, std::pair<FilePtr, FX_NS(utility)::MMapFilePtr> > MMapFileMap;
    MMapFileMap m_mmapfiles;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_MMAPFILESYSTEM_H
