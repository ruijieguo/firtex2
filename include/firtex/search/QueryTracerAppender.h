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
// Created : 2012-02-25 09:48:34

#ifndef __FX_QUERYTRACERAPPENDER_H
#define __FX_QUERYTRACERAPPENDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/LayoutAppender.h"
#include <iostream>
#include <vector>

FX_NS_DEF(search);

class QueryTracerAppender : public FX_NS(common)::LayoutAppender
{
public:
    typedef std::vector<std::string> TraceInfo;

public:
    QueryTracerAppender();
    ~QueryTracerAppender();

    DECLARE_APPENDER_CREATOR(QueryTracerAppender, "query_tracer");

public:
    virtual void append(const FX_NS(common)::LoggingEvent& event);
    virtual void flush() {}

    virtual void configure(FX_NS(config)::Configurator& conf) {}

public:
    /**
     * Return trace info
     */
    const TraceInfo& getTraceInfo() const
    {
        return m_traceInfo;
    }
    
    /**
     * Merge trace info 
     */
    void merge(const TraceInfo& traceInfo)
    {
        m_traceInfo.insert(m_traceInfo.end(), traceInfo.begin(),
                           traceInfo.end());
    }

    /**
     * Add trace message
     */
    void addTrace(const std::string& sMsg)
    {
        m_traceInfo.push_back(sMsg);
    }

private:
    TraceInfo m_traceInfo;
};

DEFINE_TYPED_PTR(QueryTracerAppender);

FX_NS_END

#endif //__FX_QUERYTRACERAPPENDER_H
