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
// Created	: 2010-08-21 19:58:22

#ifndef __FX_FORWARDINDEXMERGER_H
#define __FX_FORWARDINDEXMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/MergeInfo.h"
#include "firtex/document/FieldSchema.h"

FX_NS_DEF(index);

class ForwardIndexMerger
{
public:
    ForwardIndexMerger() {}
    virtual ~ForwardIndexMerger() {}

public:
    /**
     * Initialize 
     * @param pFileSys file system instance
     */
    virtual void init(const FX_NS(store)::FileSystemPtr& pFileSys,
                      const FX_NS(document)::FieldSchema* pFieldSchema)
    {
        m_pFileSys = pFileSys;
        m_pFieldSchema = pFieldSchema;
    }

    /**
     * Merge multi forward index barrel of one field to one barrel
     * @param mergeInfos merge infos
     */
    virtual void merge(const ForwardIndexMergeInfos& mergeInfos) = 0;

public:
    const FX_NS(document)::FieldSchema* getFieldSchema() const
    {
        return m_pFieldSchema;
    }

protected:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    const FX_NS(document)::FieldSchema* m_pFieldSchema;
};

DEFINE_TYPED_PTR(ForwardIndexMerger);

FX_NS_END

#endif //__FX_FORWARDINDEXMERGER_H
