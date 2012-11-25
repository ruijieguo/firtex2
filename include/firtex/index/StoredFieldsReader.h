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
// Created	: 2006/4/12
//
#ifndef __STOREDFIELDREADER_H
#define __STOREDFIELDREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/document/Document.h"
#include "firtex/index/FieldSelector.h"
#include "firtex/index/ResultDoc.h"
#include "firtex/store/InputStreamPool.h"
#include "firtex/common/SharedPtr.h"
#include <string>

FX_NS_DEF(store)
class IndexInput;
FX_NS_END

FX_NS_DEF(index);

class FieldsInfo;
class BarrelsInfo;
class StoredFieldsReader
{
public:
    StoredFieldsReader() {}
    virtual ~StoredFieldsReader() {}

public:
    /**
     * Open for read
     *
     * @param pStreamPool input stream pool
     * @param sBarrel barrel name
     */
    virtual void open(const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
                      const std::string& sBarrel) = 0;

    /// Close reader
    virtual void close() = 0;

    /**
     * Extract document content from stored fields
     * 
     * @param selector field selector
     * @param resultDoc result document
     *
     * @return true if document is extracted
     */
    virtual bool getDocument(const FieldSelector& selector, ResultDoc& resultDoc) = 0;

    /// Clone a new object
    virtual StoredFieldsReader* clone() const = 0;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(StoredFieldsReader);

FX_NS_END

#endif
