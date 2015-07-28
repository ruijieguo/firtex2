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
// Created : 2011-12-25 09:46:15

#ifndef __FX_POSTING_TRAITS_H
#define __FX_POSTING_TRAITS_H

#include "firtex/common/SharedPtr.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/PosPostingDecoder.h"
#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/index/BarrelTermPositionIterator.h"
#include "firtex/utility/HashMap.h"
#include "firtex/index/common/IndexKeyHasher.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/index/TypedDict.h"

FX_NS_DEF(index);

template <typename Key>
struct DocPostingTraits
{
    typedef Key KeyType;
    typedef TypedTerm<KeyType> TermType;
    typedef TypedDict<KeyType, offset_t> DictType;

    typedef DocPostingWriter PostingWriterType;
    typedef DocPostingDecoder PostingDecoderType;
    typedef BarrelTermPostingIterator PostingIteratorType;

    DEFINE_TYPED_PTR(PostingDecoderType);

    typedef PostingPool::PoolType PoolType;
    typedef FX_NS(utility)::HashMap<KeyType, PostingWriterType*, PoolType> PostingTableType;
    DEFINE_TYPED_PTR(PostingTableType);

    typedef TypedDict<KeyType, PostingWriterType*> InMemDictType;

    const static bool WITH_POSITION = false;
};

template <typename Key>
struct PosPostingTraits
{
    typedef Key KeyType;
    typedef TypedTerm<KeyType> TermType;
    typedef TypedDict<KeyType, offset_t> DictType;

    typedef PosPostingWriter PostingWriterType;
    typedef PosPostingDecoder PostingDecoderType;
    typedef BarrelTermPositionIterator PostingIteratorType;

    DEFINE_TYPED_PTR(PostingDecoderType);

    typedef PostingPool::PoolType PoolType;
    typedef FX_NS(utility)::HashMap<KeyType, PostingWriterType*, PoolType> PostingTableType;
    DEFINE_TYPED_PTR(PostingTableType);

    typedef TypedDict<KeyType, PostingWriterType*> InMemDictType;

    const static bool WITH_POSITION = true;
};

FX_NS_END

#endif
