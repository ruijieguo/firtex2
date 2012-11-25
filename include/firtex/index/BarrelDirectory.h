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
// Created : 2011-07-10 17:56:17

#ifndef __FX_BARRELDIRECTORY_H
#define __FX_BARRELDIRECTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Path.h"
#include "firtex/store/FileSystem.h"
#include <string>

FX_NS_DEF(index);

class BarrelDirectory
{
public:
    /// Create barrel directory
    static void createBarrel(FX_NS(store)::FileSystemPtr& pFileSys,
                             const std::string& sSuffix);
    static std::string getFilePath(const std::string& sFieldName,
                                   const std::string& sFileType,
                                   const std::string& sBarrelSuffix);
    static std::string getFilePath(const std::string& sFileType,
                                   const std::string& sBarrelSuffix);
};

///////////////////////////////////////////////////
///
//static
inline std::string BarrelDirectory::getFilePath(const std::string& sFieldName,
        const std::string& sFileType, const std::string& sBarrelSuffix)
{
    std::string sSep;
    sSep.push_back(FX_NS(utility)::Path::separator());
    return sBarrelSuffix + sSep + sFieldName + "." + sFileType;
}

inline std::string BarrelDirectory::getFilePath(const std::string& sFileType,
        const std::string& sBarrelSuffix)
{
    std::string sSep;
    sSep.push_back(FX_NS(utility)::Path::separator());
    return sBarrelSuffix + sSep + sFileType;
}

FX_NS_END

#endif //__FX_BARRELDIRECTORY_H
