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
// Created : 2012-02-24 23:33:48

#ifndef __FX_QUERYTRACER_H
#define __FX_QUERYTRACER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Logger.h"
#include "firtex/search/QueryTracerAppender.h"
#include <vector>

FX_NS_DEF(search);

class QueryTracer;
DEFINE_TYPED_PTR(QueryTracer);

class QueryTracer : public FX_NS(common)::LoggerBase
{
public:
    typedef FX_NS(common)::LoggingLevel::LevelType LevelType;
    typedef QueryTracerAppender::TraceInfo TraceInfo;
    typedef std::vector<QueryTracerPtr> TracerVector;

    static std::string PATH_SEPARATOR;

public:
    QueryTracer(const std::string& sPath, 
                LevelType lvl = FX_NS(common)::LoggingLevel::LEVEL_INFO);
    ~QueryTracer();

public:
    /**
     * Configure the logger
     */
    void configure(FX_NS(config)::Configurator& conf) {}

    /**
     * Return trace info
     */
    const TraceInfo& getTraceInfo() const 
    {
        return m_pTraceAppender->getTraceInfo();
    }

    /**
     * Merge trace info 
     */
    void merge(const QueryTracerPtr& pTracer)
    {
        if (pTracer)
        {
            m_pTraceAppender->merge(pTracer->m_pTraceAppender->getTraceInfo());
        }
    }

    /**
     * Add trace message
     */
    void addTrace(const std::string& sMsg)
    {
        m_pTraceAppender->addTrace(sMsg); 
    }


    /** 
     * Return path of this tracer
     */
    const std::string& getPath() const
    {
        return m_sPath;
    }

    /** 
     * Set path of this tracer
     */
    void setPath(const std::string& sPath)
    {
        m_sPath = sPath;
    }

    /**
     * Prepend sub path
     */
    void prependPath(const std::string& sPath)
    {
        m_sPath = sPath + PATH_SEPARATOR + m_sPath;
    }


    /**
     * Append sub path
     */
    void appendPath(const std::string& sPath)
    {
        m_sPath += (PATH_SEPARATOR + sPath);
    }

    /**
     * Return the child tracer
     */
    const TracerVector& getChildTracers() const
    {
        return m_childTracers;
    }

    TracerVector& getChildTracers()
    {
        return m_childTracers;
    }

    QueryTracerPtr& addChildTracer(const QueryTracerPtr& pChildTracer)
    {
        m_childTracers.push_back(pChildTracer);
        return *(m_childTracers.rbegin());
    }

private:
    /// Path of this tracer
    std::string m_sPath;
    
    /// The child tracer
    TracerVector m_childTracers;

    /// Appender of tracer
    QueryTracerAppenderPtr m_pTraceAppender;
};


#define FX_QUERY_TRACE(lvl, tracer, format, ...) if (tracer) { \
        tracer->log(FX_NS(common)::LoggingLevel::LEVEL_##lvl, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__); }


FX_NS_END

#endif //__FX_QUERYTRACER_H
