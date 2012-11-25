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

#ifndef __FX_TEXTTERMITERATOR_H
#define __FX_TEXTTERMITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermIterator.h"
#include "firtex/index/DictLoader.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/index/BarrelTermPositionIterator.h"
#include "firtex/index/common/PostingTraits.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/index/common/TypedTermIterator.h"

FX_NS_DEF(index);

class TextTermIterator : public TypedTermIterator<uint64_t, PosPostingTraits<uint64_t> >
{
public:
    TextTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                     const FX_NS(utility)::BitVector* pDocFilter)
        : TypedTermIterator<uint64_t, PosPostingTraits<uint64_t> >(pFieldSchema, pDocFilter) {}
    ~TextTermIterator() {}

public:
    /**
     * Inialize
     */
    bool init(const DictType* pDict,
              FX_NS(store)::InputStreamPtr& pDocStream,
              FX_NS(store)::InputStreamPtr& pPosStream,
              const Term* pLowerTerm, const Term* pUpperTerm,
              const FX_NS(store)::InputStreamPool* pInStreamPool);

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_TEXTTERMITERATOR_H
