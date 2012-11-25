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
// Created : 2011-06-19 17:41:33

#ifndef __FX_FILEFILTER_H
#define __FX_FILEFILTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Path.h"

FX_NS_DEF(collection);

class FileFilter
{
public:
    virtual ~FileFilter() {}

public:
    /**
     * Determine one file filtered or not
     *
     * @return true if filtered
     */
    virtual bool isFiltered(const FX_NS(utility)::Path& filePath) = 0;
};

DEFINE_TYPED_PTR(FileFilter);

FX_NS_END

#endif //__FX_FILEFILTER_H
