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
// Created : 2011-07-15 23:47:46

#ifndef __FX_FORWARDINDEXITERATORTRAITS_H
#define __FX_FORWARDINDEXITERATORTRAITS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
//#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexReader.h"

FX_NS_DEF(index);

template <typename T>
class PrimitiveTypeForwardIndexReader;

template <typename T>
struct ForwardIndexIteratorTraits
{
};

template <>
struct ForwardIndexIteratorTraits<int32_t>
{
    typedef PrimitiveTypeForwardIndexReader<int32_t> ImplType;
};

template <>
struct ForwardIndexIteratorTraits<int64_t>
{
    typedef PrimitiveTypeForwardIndexReader<int64_t> ImplType;
};

template <>
struct ForwardIndexIteratorTraits<uint32_t>
{
    typedef PrimitiveTypeForwardIndexReader<uint32_t> ImplType;
};

template <>
struct ForwardIndexIteratorTraits<uint64_t>
{
    typedef PrimitiveTypeForwardIndexReader<uint64_t> ImplType;
};

template <>
struct ForwardIndexIteratorTraits<float>
{
    typedef PrimitiveTypeForwardIndexReader<float> ImplType;
};

template <>
struct ForwardIndexIteratorTraits<double>
{
    typedef PrimitiveTypeForwardIndexReader<double> ImplType;
};

FX_NS_END

#endif //__FX_FORWARDINDEXITERATORTRAITS_H
