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
// Created	: 2010-08-18 22:53:26

#ifndef __FX_FILESYSTEMFACTORY_H
#define __FX_FILESYSTEMFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include <map>

FX_NS_DEF(store);

class FileSystemFactory : public FX_NS(utility)::Singleton<FileSystemFactory>
{
public:
    typedef FileSystem::CreatorPtr CreatorPtr;

public:
    FileSystemFactory();
    ~FileSystemFactory();

public:
    /**
     * Register processor
     * 
     * @param pCreator creator of processor
     */
    void registerFileSystem(const CreatorPtr& pCreator);

    /**
     * Create file system by identifier
     */
    FileSystem* createFileSystem(const tstring& sIdentifier);

private:
    typedef std::map<std::string, CreatorPtr> CreatorMap;

    CreatorMap m_fileSysMap;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_FILESYSTEMFACTORY_H
