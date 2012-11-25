//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/7/3
//
#ifndef __FORWARDINDEXREADER_H
#define __FORWARDINDEXREADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/index/ForwardIndexIterator.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/FieldSchema.h"

FX_NS_DEF(index)

class ForwardIndexReader
{
public:
    ForwardIndexReader() {}
    virtual ~ForwardIndexReader(void) {}

public:
    /**
     * Open forward index from file system
     * @param pFileSys file system instance
     * @param pFieldSchema field schema
     * @param sFileType the file type (norm or forward index)
     * @param sSuffix prefix of forward index file
     */
    virtual void open(FX_NS(store)::FileSystemPtr& pFileSys,
                      const FX_NS(document)::FieldSchema* pFieldSchema,
                      const std::string& sFileType,
                      const std::string& sSuffix) {};

    /**
     * Create an iterator of this forward index.
     *
     * @return forward index iterator. it is efficient if you access
     * value sequentially
     */
    virtual ForwardIndexIteratorPtr iterator() const = 0;

    /**
     * Get forward index type
     */
    virtual fdindex_t getType() const = 0;
};

DEFINE_TYPED_PTR(ForwardIndexReader);

FX_NS_END

#endif
