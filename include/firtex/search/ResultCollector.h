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
// Created : 2011-09-28 11:39:52

#ifndef __FX_RESULTCOLLECTOR_H
#define __FX_RESULTCOLLECTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/QueryResult.h"

FX_NS_DEF(search);

class ResultCollector
{
public:
    virtual ~ResultCollector() {}

public:
    /**
     * Collect top query results
     * 
     * @param queryResults query results to collect
     */
    virtual void collect(const QueryResult& queryResults) = 0;

    /**
     * Get collected result 
     *
     * @return reference to QueryResult object
     */
    virtual const QueryResult& getResult() = 0;

    /// Return capacity of the collector
    virtual size_t capacity() const = 0;

    /// Clear results
    virtual void clear() = 0;
};

DEFINE_TYPED_PTR(ResultCollector);

FX_NS_END

#endif //__FX_RESULTCOLLECTOR_H
