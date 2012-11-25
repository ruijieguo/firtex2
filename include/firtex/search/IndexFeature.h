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
// Created	: 2010-11-13 14:57:14

#ifndef __FX_INDEXFEATURE_H
#define __FX_INDEXFEATURE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexReader.h"
#include "firtex/index/LengthNormIterator.h"
#include <map>

FX_NS_DEF(search);

class IndexFeature
{
public:
    typedef std::map<tstring, uint32_t> AvgLengthMap;

public:
    IndexFeature();
    ~IndexFeature();

public:
    /**
     * Initialize
     */
    void init(const FX_NS(index)::IndexReaderPtr& pIndexReader);

    /**
     * Return field length(number of terms) of specified field
     */
    FX_NS(index)::LengthNormIteratorPtr lengthNorm(const tstring& sFieldName) const;
    
    /**
     * Return average field length of specified field
     */
    uint32_t getAverageFieldLength(const tstring& sFieldName) const;

    /**
     * Return average document length
     */
    uint32_t getAverageDocLength() const;

    /**
     * Return total number of documents
     */
    uint64_t getDocCount() const;

protected:
    FX_NS(index)::IndexReaderPtr m_pIndexReader;
    AvgLengthMap m_fieldsAvgLength;
    uint64_t m_nTotalTermCount;
    uint64_t m_nTotalDocCount;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////////////////
inline FX_NS(index)::LengthNormIteratorPtr IndexFeature::lengthNorm(
        const tstring& sFieldName) const
{
    return m_pIndexReader->lengthNorm(sFieldName);
}

inline uint32_t IndexFeature::getAverageFieldLength(const tstring& sFieldName) const
{
    AvgLengthMap::const_iterator it = m_fieldsAvgLength.find(sFieldName);
    if (it != m_fieldsAvgLength.end())
    {
        return it->second;
    }
    return 0;
}

inline uint32_t IndexFeature::getAverageDocLength() const
{
    return (uint32_t)(m_nTotalTermCount / m_nTotalDocCount);
}

inline uint64_t IndexFeature::getDocCount() const
{
    return m_nTotalDocCount;
}

FX_NS_END

#endif //__FX_INDEXFEATURE_H
