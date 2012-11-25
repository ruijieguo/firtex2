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
// Created : 2011-09-23 23:34:08

#ifndef __FX_RESULTDOC_H
#define __FX_RESULTDOC_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/LinkedHashMap.h"

FX_NS_DEF(index);

class ResultDoc
{
public:
    typedef FX_NS(utility)::LinkedHashMap<std::string, std::string> FieldMap;
    typedef FieldMap::Entry::KeyValuePair Field;

public:
    class Iterator
    {
    public:
        Iterator(const FieldMap& fields) : m_iterator(fields.iterator()) {}

    public:
        bool hasNext() const
        {
            return m_iterator.hasNext();
        }

        const Field& next()
        {
            return m_iterator.nextKeyValue();
        }

        size_t size() const
        {
            return m_iterator.size();
        }

    private:
        FieldMap::ConstIterator m_iterator;
    };

public:
    ResultDoc(docid_t docId = INVALID_DOCID)
        : m_shardId(INVALID_SHARDID)
        , m_docId(docId)
        , m_fScore(0.0f)
        , m_fields(std::string())
    {}

    ResultDoc(docid_t docId, score_t s, size_t nFieldCount)
        : m_shardId(INVALID_SHARDID)
        , m_docId(docId)
        , m_fScore(s)
        , m_fields((nFieldCount == 0) ? 1 : nFieldCount, std::string())
    {}
    
public:
    /// return shard id
    shardid_t getShardId() const;

    /// Set shard id 
    void setShardId(shardid_t shardId);

    /// Return doc id
    docid_t getDocId() const;

    /// Set doc id
    void setDocId(docid_t docId);
    
    /// Return score
    score_t getScore() const;

    /// Set score
    void setScore(score_t score);

    /**
     * Add a field key value pair
     * 
     * @param sFieldName field name
     * @param sFieldValue field value
     */
    void addField(const std::string& sFieldName, 
                  const std::string& sFieldValue);
    
    /**
     * Return field value by field name
     *
     * @param sFieldName field name
     * @return field value if the field exists, otherwise return empty string
     */
    const std::string& getField(const std::string& sFieldName) const;

    /**
     * Determine whether a field exists or not 
     *
     * @param sFieldName field name
     * @return true if there is the document containing the specified field
     */
    bool hasField(const std::string& sFieldName) const;

    /// Return iterator for fields iteration
    Iterator iterator() const;

    /// Clear all fields
    void clear();

    /// Return the number of fields
    size_t size() const;

private:
    shardid_t m_shardId;
    docid_t m_docId;
    score_t m_fScore;

    FieldMap m_fields;
};

DEFINE_TYPED_PTR(ResultDoc);

////////////////////////////////////////////////////////////
///
inline shardid_t ResultDoc::getShardId() const
{
    return m_shardId;
}

inline void ResultDoc::setShardId(shardid_t shardId)
{
    m_shardId = shardId;
}

inline docid_t ResultDoc::getDocId() const
{
    return m_docId;
}

inline void ResultDoc::setDocId(docid_t docId)
{
    m_docId = docId;
}
    
inline score_t ResultDoc::getScore() const
{
    return m_fScore;
}

inline void ResultDoc::setScore(score_t score)
{
    m_fScore = score;
}

inline void ResultDoc::addField(const std::string& sFieldName, 
                                const std::string& sFieldValue)
{
    m_fields.put(sFieldName, sFieldValue);
}

inline const std::string& ResultDoc::getField(const std::string& sFieldName) const 
{
    return m_fields.get(sFieldName);
}

inline bool ResultDoc::hasField(const std::string& sFieldName) const
{
    return m_fields.containsKey(sFieldName);
}

inline ResultDoc::Iterator ResultDoc::iterator() const
{
    return Iterator(m_fields);
}

inline void ResultDoc::clear()
{
    m_fields.clear();
}

inline size_t ResultDoc::size() const
{
    return m_fields.size();
}

FX_NS_END

#endif //__FX_RESULTDOC_H
