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
// Created	: 2010-08-28 23:31:29

#ifndef __FX_KEYWORDTERMITERATOR_H
#define __FX_KEYWORDTERMITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermIterator.h"
#include "firtex/index/DictLoader.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/common/TypedTermIterator.h"
#include "firtex/index/common/PostingTraits.h"
#include "firtex/document/FieldSchema.h"

FX_NS_DEF(index);

class KeywordTermIterator : public TypedTermIterator<uint64_t, DocPostingTraits<uint64_t> >
{
public:
    KeywordTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                        const FX_NS(utility)::BitVector* pDocFilter)
        : TypedTermIterator<uint64_t, DocPostingTraits<uint64_t> >(pFieldSchema, pDocFilter) {}
    ~KeywordTermIterator() {}

public:
    /**
     * Inialize
     */
    bool init(const DictType* pDict,
              const FX_NS(store)::InputStreamPtr& pDocStream,
              const Term* pLowerTerm, const Term* pUpperTerm,
              const FX_NS(store)::InputStreamPool* pInStreamPool);
};

FX_NS_END

#endif //__FX_KEYWORDTERMITERATOR_H
