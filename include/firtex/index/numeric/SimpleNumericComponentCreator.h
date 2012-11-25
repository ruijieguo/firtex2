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
// Created : 2012-04-21 18:14:41

#ifndef __FX_SIMPLENUMERICCOMPONENTCREATOR_H
#define __FX_SIMPLENUMERICCOMPONENTCREATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/utility/Singleton.h"
#include "firtex/index/numeric/SimpleNumericIndexer.h"
#include "firtex/index/numeric/SimpleNumericIndexMerger.h"
#include "firtex/index/numeric/SimpleNumericTermReader.h"

FX_NS_DEF(index);

template <typename T>
class SimpleNumericComponentCreator : public FieldComponentCreator,
                                      public FX_NS(utility)::Singleton<SimpleNumericComponentCreator<T> >
{
public:
    typedef T KeyType;

public:
    SimpleNumericComponentCreator()
    {
        m_identifier += ".numeric";
    }
    ~SimpleNumericComponentCreator() {}

public:
    /** 
     * interface for creating indexer component
     *
     * @param pFieldSchema field schema for indexer
     * @return field indexer component
     */
    FieldIndexer* createIndexer(
            const FX_NS(document)::FieldSchema* pFieldSchema) const
    {
        return new SimpleNumericIndexer<KeyType>(pFieldSchema);
    }

    /** 
     * interface for creating merger component	 
     * @return field merger component
     */
    FieldMerger* createMerger() const
    {
        return new SimpleNumericIndexMerger<KeyType>();
    }

    /** 
     * interface for creating term reader component	 
     * @return term reader component
     */
    TermReader* createTermReader() const
    {
        return new SimpleNumericTermReader<KeyType>();
    }

protected:
    DECLARE_LAZY_SINGLETON(SimpleNumericComponentCreator);
};

/////////////////////////////////////////////////////
//
template <>
SimpleNumericComponentCreator<int8_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".int8";
}

template <>
SimpleNumericComponentCreator<uint8_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".uint8";
}

template <>
SimpleNumericComponentCreator<int16_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".int16";
}

template <>
SimpleNumericComponentCreator<uint16_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".uint16";
}

template <>
SimpleNumericComponentCreator<int32_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".int32";
}

template <>
SimpleNumericComponentCreator<uint32_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".uint32";
}

template <>
SimpleNumericComponentCreator<int64_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".int64";
}

template <>
SimpleNumericComponentCreator<uint64_t>::SimpleNumericComponentCreator()
{
    m_identifier += ".uint64";
}


typedef SimpleNumericComponentCreator<int8_t> SimpleInt8ComponentCreator;
typedef SimpleNumericComponentCreator<uint8_t> SimpleUInt8ComponentCreator;

typedef SimpleNumericComponentCreator<int16_t> SimpleInt16ComponentCreator;
typedef SimpleNumericComponentCreator<uint16_t> SimpleUInt16ComponentCreator;

typedef SimpleNumericComponentCreator<int32_t> SimpleInt32ComponentCreator;
typedef SimpleNumericComponentCreator<uint32_t> SimpleUInt32ComponentCreator;

typedef SimpleNumericComponentCreator<int64_t> SimpleInt64ComponentCreator;
typedef SimpleNumericComponentCreator<uint64_t> SimpleUInt64ComponentCreator;

typedef SimpleNumericComponentCreator<uint128_t> SimpleUInt128ComponentCreator;

FX_NS_END

#endif //__FX_SIMPLENUMERICCOMPONENTCREATOR_H
