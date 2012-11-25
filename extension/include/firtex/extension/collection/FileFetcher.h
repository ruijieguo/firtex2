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
// Created : 2011-06-19 16:06:28

#ifndef __FX_FILEFETCHER_H
#define __FX_FILEFETCHER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/processor/RawDocument.h"

FX_NS_DEF(collection);

class FileFetcher
{
public:
    virtual ~FileFetcher() {}

public:
    /**
     * Retrieval one raw document and move to next
     *
     * @param pRawDoc store extracted raw document
     *
     * @return true if there is one document, otherwise return false
     */
    virtual bool fetchNext(FX_NS(processor)::RawDocumentPtr& pRawDoc) = 0;
};

DEFINE_TYPED_PTR(FileFetcher);

FX_NS_END

#endif //__FX_FILEFETCHER_H
