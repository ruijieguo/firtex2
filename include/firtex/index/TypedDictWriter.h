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
// Created	: 2010-08-22 17:52:24

#ifndef __FX_TYPEDDICTWRITER_H
#define __FX_TYPEDDICTWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <algorithm>
#include <vector>
#include <map>

FX_NS_DEF(index);

template <typename K, typename V>
class TypedDictWriter
{
public:
    const static size_t KEY_INDEX_INTERVAL = 128;
    const static uint64_t DICT_MAGIC_NUMBER = 0x64696374;
    const static size_t BUFFER_SIZE = KEY_INDEX_INTERVAL;

public:
    typedef K KeyType;
    typedef V ValueType;

    typedef std::pair<KeyType, ValueType> KeyValueType;
    typedef std::vector<KeyValueType> Table;

public:
    TypedDictWriter(FX_NS(store)::OutputStreamPtr& pDictStream);
    ~TypedDictWriter();

public:
    /**
     * Reserve space
     */
    void reserve(size_t size);

    /**
     * Push a key-value pair to back
     */
    void pushBack(const KeyType& key, const ValueType& value);

    /**
     * Commit data in buffer and finalize the dict
     */
    void finalize();

    /**
     * Return size of dict
     */
    int64_t size() const {return m_nSize;}

protected:
    /**
     * Svae data to output stream
     * @throw throw FileIOException if failed.
     */
    void commit();

protected:
    FX_NS(store)::OutputStreamPtr m_pDictStream;
    Table m_index;
    KeyValueType m_buffer[BUFFER_SIZE];
    uint32_t m_nUpTo;
    int64_t m_nSize;
};

/////////////////////////////////////////////////////
//
template <typename K, typename V>
TypedDictWriter<K, V>::TypedDictWriter(FX_NS(store)::OutputStreamPtr& pDictStream)
    : m_pDictStream(pDictStream)
    , m_nUpTo(0)
    , m_nSize(0)
{
}

template <typename K, typename V>
TypedDictWriter<K, V>::~TypedDictWriter()
{
}

template<typename K, typename V>
void TypedDictWriter<K, V>::reserve(size_t size)
{
    m_index.reserve(size / KEY_INDEX_INTERVAL);
}

template<typename K, typename V>
void TypedDictWriter<K, V>::pushBack(const KeyType& key, const ValueType& value)
{
    m_buffer[m_nUpTo].first = key;
    m_buffer[m_nUpTo++].second = value;

    if (m_nUpTo == BUFFER_SIZE)
    {
        commit();
    }
}

template<typename K, typename V>
void TypedDictWriter<K, V>::commit()
{
    m_pDictStream->write((const void*)m_buffer, m_nUpTo * sizeof(KeyValueType));

    if (m_nUpTo == KEY_INDEX_INTERVAL)
    {
        m_index.push_back(m_buffer[KEY_INDEX_INTERVAL - 1]);
    }

    m_nSize += m_nUpTo;
    m_nUpTo = 0;
}

template<typename K, typename V>
void TypedDictWriter<K, V>::finalize()
{
    if (m_nUpTo > 0)
    {
        commit();
    }

    //write index of dictionary
    if (m_index.size() > 0)
    {
        m_pDictStream->write((const void*)&(m_index[0]),
                             m_index.size() * sizeof(KeyValueType));
    }

    //write header
    m_pDictStream->writeInt64((int64_t)DICT_MAGIC_NUMBER);
    m_pDictStream->writeInt64(m_nSize);

    m_index.clear();
}

FX_NS_END

#endif //__FX_TYPEDDICTWRITER_H
