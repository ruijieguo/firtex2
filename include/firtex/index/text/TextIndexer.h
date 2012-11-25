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
// Created	: 2010-08-14 13:51:35

#ifndef __FX_TEXTINDEXER_H
#define __FX_TEXTINDEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/utility/HashMap.h"
#include "firtex/index/TypedDict.h"
#include "firtex/index/common/TypedIndexer.h"
#include "firtex/index/common/PostingTraits.h"
#include "firtex/analyzer/Token.h"
#include <vector>

FX_NS_DEF(index);

class TextIndexer : public TypedIndexer<uint64_t, IndexKeyHasher<uint64_t>, PosPostingTraits<uint64_t> >
{
public:
    typedef uint64_t KeyType;
    typedef IndexKeyHasher<uint64_t> HasherType;
    typedef PosPostingTraits<uint64_t>::PostingTableType PostingTableType;

public:
    TextIndexer(const FX_NS(document)::FieldSchema* pFieldSchema)
        : TypedIndexer<uint64_t, IndexKeyHasher<uint64_t>, PosPostingTraits<uint64_t> >(pFieldSchema) {}
    ~TextIndexer() {}

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

DEFINE_TYPED_PTR(TextIndexer);

FX_NS_END

#endif //__FX_TEXTINDEXER_H
