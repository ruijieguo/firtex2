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
// Created : 2011-12-25 18:34:23

#ifndef __FX_DATETIMEINDEXER_H
#define __FX_DATETIMEINDEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/index/common/TypedIndexer.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/common/PostingTraits.h"
#include <map>

FX_NS_DEF(index);

class DateTimeIndexer : public TypedIndexer<int64_t, DateTimeHasher<int64_t>, DocPostingTraits<int64_t> >
{
public:
    typedef int64_t KeyType;
    typedef DateTimeHasher<int64_t> HasherType;
    typedef DocPostingTraits<int64_t>::PostingTableType PostingTableType;

public:
    DateTimeIndexer(const FX_NS(document)::FieldSchema* pFieldSchema)
        : TypedIndexer<int64_t, DateTimeHasher<int64_t>, DocPostingTraits<int64_t> >(pFieldSchema) {}
    ~DateTimeIndexer() {}

public:
    /**
     * Create term reader for searching in-memory index
     * @return term reader object maintained by caller
     */
    TermReaderPtr termReader() const;

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////////////////////////////////
//

DEFINE_TYPED_PTR(DateTimeIndexer);

FX_NS_END

#endif
