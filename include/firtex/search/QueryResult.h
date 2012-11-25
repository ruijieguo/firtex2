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
// Created : 2011-09-23 22:05:37

#ifndef __FX_QUERYRESULT_H
#define __FX_QUERYRESULT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/ResultDoc.h"
#include "firtex/index/IndexReader.h"
#include "firtex/search/QueryHits.h"
#include "firtex/search/QueryTracer.h"

FX_NS_DEF(search);

class QueryResult
{
    typedef std::vector<FX_NS(index)::ResultDocPtr> DocVector;

public:
    class Iterator
    {
    public:
        Iterator(const DocVector& docs) : m_docs(docs), m_cursor(0) {}
        Iterator(const Iterator& src) : m_docs(src.m_docs), m_cursor(src.m_cursor) {}

    public:
        bool hasNext() const
        {
            return (m_cursor < m_docs.size());
        }

        const FX_NS(index)::ResultDocPtr& next()
        {
            return m_docs[m_cursor++];
        }

        size_t size() const
        {
            return m_docs.size();
        }

    private:
        const DocVector& m_docs;
        size_t m_cursor;
    };

public:
    QueryResult();
    ~QueryResult();

public:
    /**
     * Initialize by query hits
     *
     * @param pIndexReader index reader instance
     * @param hits query hits
     */
    void init(const FX_NS(index)::IndexReaderPtr& pIndexReader,
              const QueryHits& hits);

    /**
     * Initialize by specified fields elector
     *
     * @param selector field selector
     * @param pIndexReader index reader instance
     * @param hits query hits
     */
    void init(const FX_NS(index)::FieldSelector& selector, 
              const FX_NS(index)::IndexReaderPtr& pIndexReader,
              const QueryHits& hits);

    /**
     * Add one document
     *
     * @param pDoc document object
     */
    void addDoc(const FX_NS(index)::ResultDocPtr& pDoc);
    
    /**
     * Return iterator of result doc
     */
    Iterator iterator() const;

    /**
     * Add a field key value pair for all documents in the result
     * 
     * @param sFieldName field name
     * @param sFieldValue field value
     */
    void setShardId(shardid_t shardId);

    /**
     * Add a field key value pair for all documents in the result
     * 
     * @param sFieldName field name
     * @param sFieldValue field value
     */
    void addField(const std::string& sFieldName, 
                  const std::string& sFieldValue);
    
    /**
     * Determine whether a field exists or not 
     *
     * @param sFieldName field name
     * @return true if there is any document containing the specified field
     */
    bool hasField(const std::string& sFieldName) const;

    /// Clear all result docs
    void clear();

    /**
     * Return query tracer
     */
    QueryTracerPtr& getTracer();
    const QueryTracerPtr& getTracer() const;

    /// Set tracer;
    void setTracer(const QueryTracerPtr& pTracer);

    /**
     * Set encoding of the result content
     * @param sEncodeName encode name, "GBK", "UTF-8", etc.
     */
    void setEncoding(const std::string& sEncodeName);

    /// Return the encoding name of the result content
    const std::string& getEncoding() const;

    /// Return total number of hited docs
    uint64_t getTotalHits() const;

    /// Set total hits
    void setTotalHits(uint64_t nTotalHits);

    /// Return time cost
    int64_t getTimeCost() const;
    
    /// Set time cost
    void setTimeCost(int64_t nTimeCost);

    /// Return true if shardid field is valid
    bool hasShardId() const;
    
    /// Set the existence of shardid field
    void hasShardId(bool bHasShardId);

    /// Return true if docid field is valid
    bool hasDocId() const;

    /// Set the existence of docid field
    void hasDocId(bool bHasDocId);

    /// Return true if score field is valid
    bool hasScore() const;

    /// Set the existence of score field
    void hasScore(bool bHasScore);

    /// Return true if containing any fields
    bool hasFields() const;

    /// Set the existence of fields
    void hasFields(bool bHasFields);

    /// Return true if error field is valid
    bool hasError() const;

    /// Clear error message
    void clearError();

    // Return error message
    const std::string& getErrorMsg() const;

    /// Set error message
    void setError(const std::string& sErrorMsg);

    /// Return the number of result documents
    size_t size() const;

public:
    const FX_NS(index)::ResultDocPtr& operator [] (size_t idx) const;
    FX_NS(index)::ResultDocPtr& operator [] (size_t idx);

private:
    /// Result documents
    DocVector m_docs;
    
    /// Query tracer
    QueryTracerPtr m_pQueryTracer;

    /// Encoding of the result content
    std::string m_sEncoding;

    /// Error message
    std::string m_sErrorMsg;

    /// Return total number of hited docs
    uint64_t m_nTotalHits;

    /// The execution time of this result (in milliseconds)
    int64_t m_nTimeCost;

    /// Indicate shardid field is valid
    bool m_bHasShardId;

    /// Indicate docId field is valid
    bool m_bHasDocId;

    /// Indicate score field is valid
    bool m_bHasScore;

    /// Indicate fields is valid
    bool m_bHasFields;

    /// Indicate error field is valid
    bool m_bHasError;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(QueryResult);
/////////////////////////////////////////////////////
//
inline void QueryResult::addDoc(const FX_NS(index)::ResultDocPtr& pDoc)
{
    m_docs.push_back(pDoc);
}

inline void QueryResult::setEncoding(const std::string& sEncodeName)
{
    m_sEncoding = sEncodeName;
}

inline const std::string& QueryResult::getEncoding() const
{
    return m_sEncoding;
}

inline size_t QueryResult::size() const
{
    return m_docs.size();
}

inline uint64_t QueryResult::getTotalHits() const
{
    return m_nTotalHits;
}

inline void QueryResult::setTotalHits(uint64_t nTotalHits)
{
    m_nTotalHits = nTotalHits;
}

inline QueryTracerPtr& QueryResult::getTracer()
{
    return m_pQueryTracer;
}

inline const QueryTracerPtr& QueryResult::getTracer() const
{
    return m_pQueryTracer;
}

inline void QueryResult::setTracer(const QueryTracerPtr& pTracer)
{
    m_pQueryTracer = pTracer;
}

inline int64_t QueryResult::getTimeCost() const
{
    return m_nTimeCost;
}
    
inline void QueryResult::setTimeCost(int64_t nTimeCost)
{
    m_nTimeCost = nTimeCost;
}

inline bool QueryResult::hasShardId() const
{
    return m_bHasShardId;
}
    
inline void QueryResult::hasShardId(bool bHasShardId)
{
    m_bHasShardId = bHasShardId;
}

inline bool QueryResult::hasDocId() const
{
    return m_bHasDocId;
}

inline void QueryResult::hasDocId(bool bHasDocId)
{
    m_bHasDocId = bHasDocId;
}

inline bool QueryResult::hasScore() const
{
    return m_bHasScore;
}

inline void QueryResult::hasScore(bool bHasScore)
{
    m_bHasScore = bHasScore;
}

inline bool QueryResult::hasFields() const
{
    return m_bHasFields;
}

inline void QueryResult::hasFields(bool bHasFields)
{
    m_bHasFields = bHasFields;
}

inline bool QueryResult::hasError() const
{
    return m_bHasError;
}

inline void QueryResult::clearError()
{
    m_bHasError = false;
    m_sEncoding.clear();
}

inline const std::string& QueryResult::getErrorMsg() const
{
    return m_sErrorMsg;
}

inline void QueryResult::setError(const std::string& sErrorMsg)
{
    m_sErrorMsg = sErrorMsg;
    m_bHasError = true;
}

inline QueryResult::Iterator QueryResult::iterator() const
{
    return Iterator(this->m_docs);
}

inline const FX_NS(index)::ResultDocPtr& QueryResult::operator [] (size_t idx) const
{
    FIRTEX_ASSERT2(idx < m_docs.size());
    return m_docs[idx];
}

inline FX_NS(index)::ResultDocPtr& QueryResult::operator [] (size_t idx)
{
    FIRTEX_ASSERT2(idx < m_docs.size());
    return m_docs[idx];
}

FX_NS_END

#endif //__FX_QUERYRESULT_H
