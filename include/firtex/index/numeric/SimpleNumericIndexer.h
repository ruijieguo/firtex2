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
// Created : 2012-04-21 17:27:26

#ifndef __FX_SIMPLENUMERICINDEXER_H
#define __FX_SIMPLENUMERICINDEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/common/TypedIndexer.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/common/PostingTraits.h"
#include "firtex/index/numeric/NumericHasher.h"
#include "firtex/index/numeric/InMemSimpleNumericTermReader.h"

FX_NS_DEF(index);

template <typename T>
class SimpleNumericIndexer : public TypedIndexer<T, NumericHasher<T>, DocPostingTraits<T> >
{
public:
    typedef T KeyType;
    typedef DateTimeHasher<KeyType> HasherType;
    typedef typename DocPostingTraits<KeyType>::PostingTableType PostingTableType;

public:
    SimpleNumericIndexer(const FX_NS(document)::FieldSchema* pFieldSchema)
        : TypedIndexer<KeyType, NumericHasher<KeyType>, DocPostingTraits<KeyType> >(pFieldSchema) {}
    ~SimpleNumericIndexer() {}

public:
    /**
     * Create term reader for searching in-memory index
     * @return term reader object maintained by caller
     */
    TermReaderPtr termReader() const
    {
        InMemSimpleNumericTermReader<KeyType>* pTermReader =
            new InMemSimpleNumericTermReader<KeyType>();
        TermReaderPtr pTermReaderPtr(pTermReader);
        pTermReader->init(this->getFieldSchema(), this->m_pPostingTable, NULL);
        return pTermReaderPtr;
    }

private:
    DECLARE_LOGGER();
};

SETUP_LOGGER_TEMP(index, SimpleNumericIndexer);

typedef SimpleNumericIndexer<int8_t> SimpleInt8Indexer;
typedef SimpleNumericIndexer<uint8_t> SimpleUInt8Indexer;

typedef SimpleNumericIndexer<int16_t> SimpleInt16Indexer;
typedef SimpleNumericIndexer<uint16_t> SimpleUInt16Indexer;

typedef SimpleNumericIndexer<int32_t> SimpleInt32Indexer;
typedef SimpleNumericIndexer<uint32_t> SimpleUInt32Indexer;

typedef SimpleNumericIndexer<int64_t> SimpleInt64Indexer;
typedef SimpleNumericIndexer<uint64_t> SimpleUInt64Indexer;

FX_NS_END

#endif //__FX_SIMPLENUMERICINDEXER_H
