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
// Created : 2011-09-28 12:21:55

#ifndef __FX_RESULTCOLLECTORIMPL_H
#define __FX_RESULTCOLLECTORIMPL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ResultCollector.h"
#include "firtex/utility/PriorityQueue.h"
#include "firtex/utility/NumberParser.h"

FX_NS_DEF(search);

class ResultCollectorImpl : public ResultCollector
{
public:
    struct Comparator
    {
        virtual ~Comparator() {}

        bool operator () (const FX_NS(index)::ResultDocPtr& pDoc1,
                          const FX_NS(index)::ResultDocPtr& pDoc2)
        {
            return lessThan(pDoc1, pDoc2);
        }
        
        virtual bool lessThan(const FX_NS(index)::ResultDocPtr& pDoc1,
                              const FX_NS(index)::ResultDocPtr& pDoc2) = 0;
    };

    typedef FX_NS(utility)::PriorityQueue<FX_NS(index)::ResultDocPtr, Comparator, 
                                                       FX_NS(utility)::DummyDestroyer<FX_NS(index)::ResultDocPtr> > ResultDocQueue;

    struct ScoreComparator : public Comparator
    {
        bool lessThan(const FX_NS(index)::ResultDocPtr& pDoc1,
                      const FX_NS(index)::ResultDocPtr& pDoc2)
        {
            if (pDoc1->getScore() == pDoc2->getScore())
            {
                return pDoc1->getDocId() > pDoc2->getDocId();
            }
            else
            {
                return pDoc1->getScore() < pDoc2->getScore();
            }
        }
    };

    struct DocIdComparator : public Comparator
    {
        bool lessThan(const FX_NS(index)::ResultDocPtr& pDoc1,
                        const FX_NS(index)::ResultDocPtr& pDoc2)
        {
            return pDoc1->getDocId() > pDoc2->getDocId();
        }
    };

    struct FieldComparator  : public Comparator
    {
        FieldComparator(const std::string& sField) : m_sField(sField) {}

    protected:
        std::string m_sField;
    };
    
#define DECLARE_COMPARATOR(typeName, type)                              \
    struct typeName##FieldComparator : public FieldComparator           \
    {                                                                   \
        typeName##FieldComparator(const std::string& sField)            \
            : FieldComparator(sField) {}                                \
        bool lessThan(const FX_NS(index)::ResultDocPtr& pDoc1,          \
                        const FX_NS(index)::ResultDocPtr& pDoc2)        \
        {                                                               \
            const std::string& sValue1 = pDoc1->getField(m_sField);     \
            const std::string& sValue2 = pDoc2->getField(m_sField);     \
            type v1 = FX_NS(utility)::NumberParser::parse##typeName(sValue1); \
            type v2 = FX_NS(utility)::NumberParser::parse##typeName(sValue2); \
            return v1 > v2;                                             \
        }                                                               \
    }

    DECLARE_COMPARATOR(Int32, int32_t);
    DECLARE_COMPARATOR(UInt32, uint32_t);
    DECLARE_COMPARATOR(Int64, int64_t);
    DECLARE_COMPARATOR(UInt64, uint64_t);
    DECLARE_COMPARATOR(Float, float);
    DECLARE_COMPARATOR(Double, double);

#undef DECLARE_COMPARATOR

    enum CompareField
    {
        COMP_SCORE = 0x1,
        COMP_DOCID,
        COMP_FIELD_INT32,
        COMP_FIELD_UINT32,
        COMP_FIELD_INT64,
        COMP_FIELD_UINT64,
        COMP_FIELD_FLOAT,
        COMP_FIELD_DOUBLE,
    };

public:
    ResultCollectorImpl(size_t nCapacity, CompareField comp = COMP_SCORE,
                        const std::string& sField = "");
    ~ResultCollectorImpl();

public:
    /**
     * Collect top query results
     * 
     * @param queryResults query results to collect
     */
    virtual void collect(const QueryResult& queryResults);

    /**
     * Get collected result 
     *
     * @return reference to QueryResult object
     */
    virtual const QueryResult& getResult();

    /// Return capacity of the collector
    virtual size_t capacity() const;

    /// Clear results
    virtual void clear();
    
private:
    ResultDocQueue* m_pResultQueue;
    QueryResult m_result;
    size_t m_nCapacity;
    
private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_RESULTCOLLECTORIMPL_H
