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
// Created	: 2010-12-21 19:52:05

#ifndef __FX_PARAMETERCLAUSE_H
#define __FX_PARAMETERCLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/LoggingLevel.h"
#include "firtex/search/Clause.h"

FX_NS_DEF(search);

class ParameterClause : public Clause
{
public:
    DECLARE_CLAUSE_CREATOR(ParameterClause);

public:
    typedef FX_NS(common)::LoggingLevel::LevelType LevelType;

    const static size_t DEFAULT_TOPDOCS = 10;

    const static std::string PREFIX;
    const static std::string STARTDOC_PREFIX;
    const static std::string TOPDOCS_PREFIX;
    const static std::string PHASE_PREFIX;
    const static std::string TRACE_PREFIX;
    const static std::string SEPARATOR;
    const static std::string KV_SEPARATOR;

public:
    ParameterClause(const std::string& sName,
                    const std::string& sValue);
    ~ParameterClause();

private:
    ParameterClause() {}

public:
    /**
     * Return start doc in hits to retrieval
     */
    size_t getStartDoc() const { return m_nStartDoc;}

    /**
     * Get topk hits to retrieval
     */
    size_t getTopDocs() const { return m_nTopDocs;}

    /**
     * Return query phase
     */
    uint32_t getQueryPhase() const {return m_nQueryPhase;}

    /**
     * Return query trace level
     */
    LevelType getTraceLevel() const {return m_traceLevel;}

    /**
     * Has set trace level or not
     */
    bool hasTrace() const {return m_traceLevel != FX_NS(common)::LoggingLevel::LEVEL_MAX;}

    /// Set parameter
    void setParameter(const std::string& sValue);

    /// Format parameter to string
    virtual void toString(std::stringstream& ss, const std::string& sEqSymbol) const;

protected:
    void fromString(const std::string& sValue);

protected:
    size_t m_nStartDoc;
    size_t m_nTopDocs;
    uint32_t m_nQueryPhase;
    LevelType m_traceLevel;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(ParameterClause);

//////////////////////////////////
//
inline void ParameterClause::setParameter(const std::string& sValue)
{
    fromString(sValue);
}

FX_NS_END

#endif //__FX_PARAMETERCLAUSE_H
