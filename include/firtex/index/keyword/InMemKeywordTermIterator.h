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
// Created : 2011-07-15 20:49:18

#ifndef __FX_INMEMKEYWORDTERMITERATOR_H
#define __FX_INMEMKEYWORDTERMITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermIterator.h"
#include "firtex/utility/HashMap.h"
#include "firtex/utility/BitVector.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/index/TypedDict.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/index/keyword/KeywordIndexer.h"
#include "firtex/index/common/PostingTraits.h"
#include "firtex/index/common/TypedInMemTermIterator.h"

FX_NS_DEF(index);

class InMemKeywordTermIterator : public TypedInMemTermIterator<uint64_t, DocPostingTraits<uint64_t> >
{
public:
    InMemKeywordTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                             const FX_NS(utility)::BitVector* pDocFilter)
        : TypedInMemTermIterator<uint64_t, DocPostingTraits<uint64_t> >(pFieldSchema, pDocFilter) {}
    ~InMemKeywordTermIterator() {}
};

FX_NS_END

#endif //__FX_INMEMKEYWORDTERMITERATOR_H
