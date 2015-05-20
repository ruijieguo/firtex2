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
// Created	: 2011-01-09 16:09:30

#ifndef __FX_SCORERPROFILECLAUSE_H
#define __FX_SCORERPROFILECLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Clause.h"
#include <map>

FX_NS_DEF(search);

class ScorerProfileClause : public Clause
{
public:
    typedef std::map<std::string, std::string> KeyValueMap;

public:
    DECLARE_CLAUSE_CREATOR(ScorerProfileClause);

    const static std::string PREFIX;

public:
    ScorerProfileClause(const std::string& sName,
                        const std::string& sValue);
    ~ScorerProfileClause();

private:
    void fromString(const std::string& sClauseValue);

public:
    const std::string& getProfileName() const;
    const KeyValueMap& getParameters() const;

private:
    std::string m_sProfileName;
    KeyValueMap m_parameters; 

    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(ScorerProfileClause);

//////////////////////////////////////////
//
inline const std::string& ScorerProfileClause::getProfileName() const
{
    return m_sProfileName;
}

inline const ScorerProfileClause::KeyValueMap& ScorerProfileClause::getParameters() const
{
    return m_parameters;
}

FX_NS_END

#endif //__FX_SCORERPROFILECLAUSE_H
