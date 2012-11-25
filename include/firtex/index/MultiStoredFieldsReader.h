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
#ifndef __MULTI_STORED_FIELD_READER_H
#define __MULTI_STORED_FIELD_READER_H

#include <vector>
#include "firtex/index/StoredFieldsReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class MultiStoredFieldsReader : public StoredFieldsReader
{
public:
    MultiStoredFieldsReader(const BarrelsInfoPtr& pBarrelsInfo);
    MultiStoredFieldsReader(const MultiStoredFieldsReader& src);
    ~MultiStoredFieldsReader();

public:
    /**
     * Extract document content from stored fields
     * 
     * @param selector field selector
     * @param resultDoc result document
     *
     * @return true if document is extracted
     */
    bool getDocument(const FieldSelector& selector, ResultDoc& resultDoc);

    void open(const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
              const std::string& barrel);
    void close();

    StoredFieldsReader* clone() const;

public:
    /**
     * Add a single barrel stored fields reader
     */
    void addRead(const StoredFieldsReaderPtr& reader);

private:
    StoredFieldsReaderPtr& getReaderByDocId(docid_t& nDocId);

protected:
    BarrelsInfoPtr m_pBarrelsInfo;

    typedef std::vector<StoredFieldsReaderPtr> ReaderVector;

    ReaderVector m_readers;

private:
    DECLARE_STREAM_LOGGER();
};

typedef FX_NS(common)::SharedPtr<MultiStoredFieldsReader> MultiStoredFieldsReaderPtr;

////////////////////////////////////////////////////////
///
inline StoredFieldsReaderPtr& MultiStoredFieldsReader::getReaderByDocId(docid_t& nDocId)
{
    BarrelsInfo::Iterator it = m_pBarrelsInfo->iterator();
    size_t nReadIdx = 0;
    while(it.hasNext())
    {
        const BarrelInfo& barrelInfo = it.next();
        if( (nDocId >= barrelInfo.getBaseDocId()) &&
           (nDocId < (barrelInfo.getBaseDocId() + barrelInfo.getDocCount())) )
        {
            nDocId -= barrelInfo.getBaseDocId();
            return m_readers[nReadIdx];
        }
        nReadIdx++;
    }
    FIRTEX_THROW(BadParameterException, _T("DocId is out of range."));
}

FX_NS_END

#endif
