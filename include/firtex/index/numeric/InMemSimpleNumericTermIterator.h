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
// Created : 2012-04-21 17:56:33

#ifndef __FX_INMEMSIMPLENUMERICTERMITERATOR_H
#define __FX_INMEMSIMPLENUMERICTERMITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/BitVector.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/index/common/PostingTraits.h"
#include "firtex/index/common/TypedInMemTermIterator.h"

FX_NS_DEF(index);

template <typename T>
class InMemSimpleNumericTermIterator : public TypedInMemTermIterator<T, DocPostingTraits<T> >
{
public:
    typedef T KeyType;

public:
    InMemSimpleNumericTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                                   const FX_NS(utility)::BitVector* pDocFilter)
        : TypedInMemTermIterator<T, DocPostingTraits<T> >(pFieldSchema, pDocFilter) {}
    ~InMemSimpleNumericTermIterator() {}
};

FX_NS_END

#endif //__FX_INMEMSIMPLENUMERICTERMITERATOR_H
