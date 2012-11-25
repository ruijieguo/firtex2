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
// Created	: 2010-11-23 20:44:31

#ifndef __FX_QUERYHITS_H
#define __FX_QUERYHITS_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/HitQueue.h"
#include "firtex/search/QueryTracer.h"
#include "firtex/index/StoredFieldsReader.h"

FX_NS_DEF(search);

class IndexSearcher;

class QueryHits
{
public:
    QueryHits();
    ~QueryHits();

public:
    class HitDoc
    {
    public:
        HitDoc(docid_t docId, score_t score)
            : m_fScore(score)
            , m_docId(docId)
        {
        }
        ~HitDoc()
        {
        }

    public:
        docid_t	getDocId() const {return m_docId;}
        score_t	getScore() const {return m_fScore;}

    protected:
        score_t	m_fScore;
        docid_t	m_docId;

        friend class QueryHits;
    };

    class Iterator
    {
    public:
        Iterator(const QueryHits* pHits);
        Iterator(const Iterator& _iter);
        virtual ~Iterator(void);

    public:
        void reset();
        bool hasNext() const;
        const HitDoc& next();
        size_t size()const;

    private:
        const QueryHits* m_pQueryHits;
        size_t m_nIterator;
    };

public:
    /**
     * Initialize by hit queue
     */
    void init(HitQueuePtr& hq, size_t nStart);

    /**
     * Initialize by hit doc string, format: doc1,doc2,...
     */
    void init(const std::string& sHits);

    /**
     * Set encoding of documents in the index database
     * @param sEncodeName encode name, "GBK", "UTF-8", etc.
     */
    void setEncoding(const std::string& sEncodeName);

    /**
     * Return encoding of documents in the index database
     * @return encode name of documents in the index database
     */
    std::string getEncoding() const;

    /**
     * Return iterator of hit doc
     */
    Iterator iterator() const;

    /**
     * Return the number of retrieved hit docs 
     */
    size_t size() const;

    /**
     * Return total number of hited docs
     */
    uint64_t getTotalHits() const;

    /**
     * Set total hits
     */
    void setTotalHits(uint64_t nTotalHits);

    /**
     * Return query tracer
     */
    QueryTracerPtr& getTracer();
    const QueryTracerPtr& getTracer() const;

    /**
     * Set query tracer
     */
    void setTracer(const QueryTracerPtr& pTracer);

    /**
     * Clear all hit docs
     */
    void clear();

private:
    typedef std::vector<HitDoc*> Hits;
    Hits m_hitDocs;

    QueryTracerPtr m_pQueryTracer;
    std::string m_sEncoding;
    uint64_t m_nTotalHits;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(QueryHits);

/////////////////////////////////////////////////

inline void QueryHits::setTotalHits(uint64_t nTotalHits)
{
    m_nTotalHits = nTotalHits;
}

inline  size_t QueryHits::size() const
{
    return m_hitDocs.size();
}

inline uint64_t QueryHits::getTotalHits() const
{
    return m_nTotalHits;
}

inline QueryTracerPtr& QueryHits::getTracer()
{
    return m_pQueryTracer;
}

inline const QueryTracerPtr& QueryHits::getTracer() const
{
    return m_pQueryTracer;
}

inline void QueryHits::setTracer(const QueryTracerPtr& pTracer)
{
    m_pQueryTracer = pTracer;
}

inline void QueryHits::setEncoding(const std::string& sEncodeName)
{
    m_sEncoding = sEncodeName;
}

inline std::string QueryHits::getEncoding() const
{
    return m_sEncoding;
}

FX_NS_END

#endif //__FX_QUERYHITS_H
