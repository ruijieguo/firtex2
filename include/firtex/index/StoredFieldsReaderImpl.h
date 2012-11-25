//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/4/12
//
#ifndef __STOREDFIELDREADERIMPL_H
#define __STOREDFIELDREADERIMPL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/Document.h"
#include "firtex/index/StoredFieldsReader.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/store/FileSystem.h"
#include <string>

FX_NS_DEF(index);

class StoredFieldsReaderImpl : public StoredFieldsReader
{
public:
    StoredFieldsReaderImpl(const FX_NS(document)::DocumentSchema* pDocSchema);
    StoredFieldsReaderImpl(const StoredFieldsReaderImpl& src);
    virtual ~StoredFieldsReaderImpl();

public:
    /**
     * Open and initialize
     */
    virtual void open(const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
                      const std::string& sSuffix);


    /**
     * Extract document content from stored fields
     * 
     * @param selector field selector
     * @param resultDoc result document
     *
     * @return true if document is extracted
     */
    bool getDocument(const FieldSelector& selector, ResultDoc& resultDoc);

    /// Close reader
    virtual void close();

    /// Clone a new object
    virtual StoredFieldsReader* clone() const;

protected:
    const static size_t DEFAULT_READ_BUFFER_SIZE = 512 * 1024; //512k

    FX_NS(store)::InputStreamPoolPtr m_pStreamPool;
    FX_NS(store)::InputStreamPtr m_pDataReader;
    FX_NS(store)::InputStreamPtr m_pIdxReader;

    const FX_NS(document)::DocumentSchema* m_pDocSchema;
    fieldid_t m_baseField;

    std::string m_sContent;

private:
    DECLARE_STREAM_LOGGER();
};

typedef FX_NS(common)::SharedPtr<StoredFieldsReaderImpl> StoredFieldsReaderImplPtr;

FX_NS_END

#endif
