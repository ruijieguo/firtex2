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
// Created	: 2010-08-14 19:11:15

#ifndef __FX_TYPEDDICT_H
#define __FX_TYPEDDICT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/OutputStream.h"
#include <algorithm>
#include <vector>
#include <map>

FX_NS_DEF(index);

template <typename K, typename V>
class TypedDict
{
public:
    const static size_t INDEX_INTERVAL = 128;
    const static uint64_t DICT_MAGIC_NUMBER = 0x64696374;

public:
    typedef K KeyType;
    typedef V ValueType;

    typedef std::pair<KeyType, ValueType> KeyValueType;

    typedef std::vector<KeyValueType> Table;

public:
    class Iterator
    {
    public:
        Iterator() {}
        Iterator(const typename Table::const_iterator& startIt,
                 const typename Table::const_iterator& endIt)
            : m_iterator(startIt)
            , m_endIt(endIt)
        {
        }

        Iterator(const Table& table)
            : m_iterator(table.begin())
            , m_endIt(table.end())
        {
        }

    public:
        bool hasNext() const
        {
            return (m_iterator != m_endIt);
        }
        
        KeyValueType next()
        {
            return *m_iterator++;
        }

        size_t size() const {return m_endIt - m_iterator;}

    protected:
        typename Table::const_iterator m_iterator;
        typename Table::const_iterator m_endIt;
    };

public:
    TypedDict();
    ~TypedDict();

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
     * Find value by key
     * @param key key to find
     * @param value value related to #key
     * @return true if the value related to #key is exist
     */
    bool find(const KeyType& key, ValueType& value) const;

    /**
     * Return iterator of specified range
     * @param startKey start key of range
     * @param endKey end key of range
     * @return iterator of the range
     */
    Iterator rangeIterator(const KeyType& startKey, const KeyType& endKey) const;

    /**
     * Return iterator of dict
     */
    Iterator iterator() const;

    /**
     * Finalize the dict and generate index if necessary
     */
    void finalize();

    /**
     * Load data from input stream
     * @throw throw FileIOException if failed.
     */
    void load(FX_NS(store)::InputStreamPtr& pInStream);

    /**
     * Svae data to output stream
     * @throw throw FileIOException if failed.
     */
    void save(FX_NS(store)::OutputStreamPtr& pOutStream);
    
protected:
    static bool SORT_CMP(const KeyValueType& v1, const KeyValueType& v2)
    {
        return v1.first < v2.first;
    }

protected:
    Table m_index;
    Table m_table;

private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////////////////
//

SETUP_STREAM_LOGGER_TEMP2(index, TypedDict);

template <typename K, typename V>
TypedDict<K, V>::TypedDict()
{
}

template <typename K, typename V>
TypedDict<K, V>::~TypedDict()
{
}

template<typename K, typename V>
void TypedDict<K, V>::reserve(size_t size)
{
    if (size > 0)
    {
        m_table.reserve(size);
    }
}

template<typename K, typename V>
void TypedDict<K, V>::pushBack(const KeyType& key, const ValueType& value)
{
    m_table.push_back(std::make_pair(key, value));
}

template<typename K, typename V>
bool TypedDict<K, V>::find(const KeyType& key, ValueType& value) const
{
    size_t begin = 0, end = m_table.size();
    if (m_index.size() > 0)
    {
        std::pair<typename Table::const_iterator, typename Table::const_iterator> ret = 
            std::equal_range(m_index.begin(), m_index.end(), std::make_pair(key, value), SORT_CMP);
        if (ret.first == ret.second)
        {
            end = (ret.first - m_index.begin() + 1) * INDEX_INTERVAL;
            begin = end - INDEX_INTERVAL;
            if (end > m_table.size())
            {
                end = m_table.size();
            }
        }
        else 
        {
            value = (*(ret.first)).second;
            return true;
        }
    }
    typename Table::const_iterator it = std::lower_bound(m_table.begin() + begin,
            m_table.begin() + end, std::make_pair(key, value), SORT_CMP);
    if (it != m_table.end() && (*it).first == key)
    {
        value = (*it).second;
        return true;
    }
    return false;
}

template<typename K, typename V>
typename TypedDict<K, V>::Iterator TypedDict<K, V>::rangeIterator(
        const KeyType& startKey, const KeyType& endKey) const
{
    size_t begin = 0, end = m_table.size();
    if (m_index.size() > 0)
    {
        std::pair<typename Table::const_iterator, typename Table::const_iterator> ret = 
            std::equal_range(m_index.begin(), m_index.end(), std::make_pair(startKey, (ValueType)0), SORT_CMP);
        if (ret.first == ret.second)
        {
            end = (ret.first - m_index.begin() + 1) * INDEX_INTERVAL;
            begin = end - INDEX_INTERVAL;
            if (end > m_table.size())
            {
                end = m_table.size();
            }
        }
        else 
        {
            begin = end = (ret.first - m_index.begin() + 1) * INDEX_INTERVAL;
        }
    }
    FX_TRACE("Begin range: [%u, %u]", (unsigned int)begin, (unsigned int)end);

    typename Table::const_iterator startIt = std::lower_bound(m_table.begin() + begin,
            m_table.begin() + end, std::make_pair(startKey, (ValueType)0), SORT_CMP);
    if (startIt == m_table.end())
    {
        return Iterator(m_table.end(), m_table.end());
    }
    else if ((*startIt).first != startKey)
    {
        ++startIt;
    }

    begin = 0, end = m_table.size();
    if (m_index.size() > 0)
    {
        std::pair<typename Table::const_iterator, typename Table::const_iterator> ret = 
            std::equal_range(m_index.begin(), m_index.end(), std::make_pair(endKey, (ValueType)0), SORT_CMP);
        if (ret.first == ret.second)
        {
            end = (ret.first - m_index.begin() + 1) * INDEX_INTERVAL;
            begin = end - INDEX_INTERVAL;
            if (end > m_table.size())
            {
                end = m_table.size();
            }
        }
        else 
        {
            begin = end = (ret.first - m_index.begin() + 1) * INDEX_INTERVAL;
        }
    }
    FX_TRACE("End range: [%u, %u]", begin, end);
    typename Table::const_iterator endIt = std::upper_bound(m_table.begin() + begin,
            m_table.begin() + end, std::make_pair(endKey, (ValueType)0), SORT_CMP);

    return Iterator(startIt, endIt);
}

template<typename K, typename V>
typename TypedDict<K, V>::Iterator TypedDict<K, V>::iterator() const
{
    return Iterator(m_table);
}

template<typename K, typename V>
void TypedDict<K, V>::finalize()
{
    std::sort(m_table.begin(), m_table.end(), SORT_CMP);
    if (m_table.size() >= INDEX_INTERVAL)
    {
        m_index.resize(m_table.size() / INDEX_INTERVAL);
        for (size_t i = INDEX_INTERVAL; i <= m_table.size(); i += INDEX_INTERVAL)
        {
            m_index[i / INDEX_INTERVAL - 1] = m_table[i - 1];
        }
    }
}

template<typename K, typename V>
void TypedDict<K, V>::load(FX_NS(store)::InputStreamPtr& pInStream)
{
    FX_TRACE("Load dict");
    //read header
    pInStream->seek(pInStream->getSize() - 2 * sizeof(int64_t));
    uint64_t magicNumber = (uint64_t)pInStream->readInt64();
    if (magicNumber != DICT_MAGIC_NUMBER)
    {
        FIRTEX_THROW(IndexCollapseException, "Load dict file FAILED: bad magic number.");
    }

    int64_t size = pInStream->readInt64();

    FX_TRACE("Read header: size: %lld", (long long int)size);

    //read dict body
    if (size > 0)
    {
        pInStream->seek(0);    
        m_table.resize((size_t)size);
        pInStream->read((void*)&(m_table[0]), (size_t)size * sizeof(KeyValueType));
    }

    //read index
    size_t nIndexSize = (size_t)(size / INDEX_INTERVAL);
    if (nIndexSize > 0)
    {
        m_index.resize(nIndexSize);
        pInStream->read((void*)&(m_index[0]), nIndexSize * sizeof(KeyValueType));
    }
}

template<typename K, typename V>
void TypedDict<K, V>::save(FX_NS(store)::OutputStreamPtr& pOutStream)
{
    if (m_table.size() >= INDEX_INTERVAL && m_index.size() == 0)
    {
        finalize();
    }
    else if (m_table.size() < INDEX_INTERVAL)
    {
        std::sort(m_table.begin(), m_table.end(), SORT_CMP);
    }

    //write dict body
    if (m_table.size() > 0)
    {
        pOutStream->write((const void*)&(m_table[0]), m_table.size() * sizeof(KeyValueType));
    }

    //write index of dictionary
    if (m_index.size() > 0)
    {
        pOutStream->write((const void*)&(m_index[0]), m_index.size() * sizeof(KeyValueType));
    }

    //write header
    int64_t size = (int64_t)m_table.size();
    FX_TRACE("Write table header: size: %lld, offset: %lld", size, pOutStream->tell());

    pOutStream->writeInt64((int64_t)DICT_MAGIC_NUMBER);
    pOutStream->writeInt64(size);
}

FX_NS_END

#endif //__FX_TYPEDDICT_H
