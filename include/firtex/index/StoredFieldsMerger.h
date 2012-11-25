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
// Created	: 2010-08-21 11:25:34

#ifndef __FX_STOREDFIELDSMERGER_H
#define __FX_STOREDFIELDSMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/IndexMerger.h"

FX_NS_DEF(index);

class StoredFieldsMerger
{
public:
    StoredFieldsMerger();
    ~StoredFieldsMerger();

public:
    /**
     * Merge multi barrel to one barrel
     * @param pFileSys file system instance
     * @param mergeInfos merge infos of barrels to merge
     */
    void merge(FX_NS(store)::FileSystemPtr& pFileSys, 
               const MergeInfos& mergeInfos);

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(StoredFieldsMerger);

FX_NS_END

#endif //__FX_STOREDFIELDSMERGER_H
