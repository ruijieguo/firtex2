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
// Created : 2012-04-21 17:51:52

#ifndef __FX_SIMPLENUMERICTERMITERATOR_H
#define __FX_SIMPLENUMERICTERMITERATOR_H

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

template <typename T>
class SimpleNumericTermIterator : public TypedTermIterator<T, DocPostingTraits<T> >
{
public:
    typedef T KeyType;
    typedef typename TypedTermIterator<T, DocPostingTraits<T> >::DictType DictType;

public:
    SimpleNumericTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                              const FX_NS(utility)::BitVector* pDocFilter)
        : TypedTermIterator<KeyType, DocPostingTraits<KeyType> >(pFieldSchema, pDocFilter)
    {}
    ~SimpleNumericTermIterator() {}

public:
    /**
     * Inialize
     */
    bool init(const DictType* pDict,
              const FX_NS(store)::InputStreamPtr& pDocStream,
              const Term* pLowerTerm, const Term* pUpperTerm,
              const FX_NS(store)::InputStreamPool* pInStreamPool)
    {
        return TypedTermIterator<KeyType, DocPostingTraits<KeyType> >::init(
                pDict, pDocStream, NULL, pLowerTerm, pUpperTerm, pInStreamPool);
    }
};

FX_NS_END

#endif //__FX_SIMPLENUMERICTERMITERATOR_H
