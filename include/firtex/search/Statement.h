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
// Created	: 2010-12-20 22:18:34

#ifndef __FX_STATEMENT_H
#define __FX_STATEMENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/QueryClause.h"
#include "firtex/search/ParameterClause.h"
#include "firtex/search/SortClause.h"
#include "firtex/search/FilterClause.h"
#include "firtex/search/FieldSelectClause.h"
#include "firtex/search/DocSelectClause.h"
#include "firtex/search/ScorerProfileClause.h"
#include <map>
#include <vector>

FX_NS_DEF(search);

class Statement
{
public:
    const static std::string CLAUSE_SEPARATOR;       // ";"
    const static std::string CLAUSE_EQUAL_SYMBOL;    // "="
    const static std::string CLAUSE_KV_SEPARATOR;    // ":"

public:
    Statement();
    virtual ~Statement();

public:
    /**
     * Parse statements from string
     * 
     * @param sStat statement string
     * @return true if accept, otherwise false
     */
    bool fromString(const std::string& sStat);

    /**
     * Format the statement to string
     */
    void toString(std::stringstream& ss) const;

    /**
     * Remove clause by name
     *
     * @param sClauseName clause to remove
     */
    void removeClause(const std::string& sClauseName);

    /**
     * Add clause by name/value pair
     *
     * @param sClauseName clause name to add
     * @param sClauseValue clause value to add
     * @return true if success
     */
    bool addClause(const std::string& sClauseName,
                   const std::string& sClauseValue);

    /// Set key-value pair of parameter
    void setParameter(const std::string& sKey, const std::string& sValue);

    /// Clear all clauses for reuse
    void clearClauses();

    /**
     * Register a clasue creator
     */
    void registerClause(const std::string& sClausePrefix,
                        Clause::Creator* pCreator);

    /**
     * Return error message if parse failed.
     */
    std::string getErrorMessage() const;

public:
    QueryClausePtr getQueryClause() const;
    ParameterClausePtr getParameterClause() const;
    SortClausePtr getSortClause() const;
    FilterClausePtr getFilterClause() const;
    FieldSelectClausePtr getFieldSelectClause() const;
    DocSelectClausePtr getDocSelectClause() const;
    ScorerProfileClausePtr getScorerProfileClause() const;
    
    ClausePtr getClause(const std::string& sClauseName) const;

private:
    void extractBuiltInClause(bool bOverride);
    bool doAddClause(const std::string& sClauseName,
                     const std::string& sClauseValue);

protected:
    typedef std::map<std::string, Clause::Creator*> ClauseCreatorMap;
    typedef std::map<std::string, ClausePtr> ClauseMap;
    typedef std::vector<std::string> StringVector;

    ClauseCreatorMap m_creators;
    ClauseMap m_clauses;

    std::string m_sOrgStat;
    StringVector m_invalidClauses;

    QueryClausePtr m_pQueryClause;
    ParameterClausePtr m_pParamClause;
    SortClausePtr m_pSortClause;
    FilterClausePtr m_pFilterClause;
    FieldSelectClausePtr m_pFieldSelectClause;
    DocSelectClausePtr m_pDocSelectClause;
    ScorerProfileClausePtr m_pScorerProfileClause;

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////////////////////////
inline QueryClausePtr Statement::getQueryClause() const
{
    return m_pQueryClause;
}

inline ParameterClausePtr Statement::getParameterClause() const
{
    return m_pParamClause;
}

inline SortClausePtr Statement::getSortClause() const
{
    return m_pSortClause;
}

inline FilterClausePtr Statement::getFilterClause() const
{
    return m_pFilterClause;
}

inline FieldSelectClausePtr Statement::getFieldSelectClause() const
{
    return m_pFieldSelectClause;
}

inline DocSelectClausePtr Statement::getDocSelectClause() const
{
    return m_pDocSelectClause;
}

inline ScorerProfileClausePtr Statement::getScorerProfileClause() const
{
    return m_pScorerProfileClause;
}

inline ClausePtr Statement::getClause(const std::string& sClauseName) const
{
    ClauseMap::const_iterator it = m_clauses.find(sClauseName);
    if (it != m_clauses.end())
    {
        return it->second;
    }
    return ClausePtr();
}

FX_NS_END

#endif //__FX_STATEMENT_H
