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
// Created	: 2010-12-20 22:44:34

#ifndef __FX_QUERYCLAUSE_H
#define __FX_QUERYCLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Clause.h"
#include "firtex/search/Query.h"

FX_NS_DEF(search);

class QueryClause : public Clause
{
public:
    DECLARE_CLAUSE_CREATOR(QueryClause);

    const static std::string PREFIX;

public:
    QueryClause(const std::string& sName,
                const std::string& sValue);
    ~QueryClause();

public:
    const std::string& getQueryString() const 
    {
        return m_sClauseValue;
    }

};

DEFINE_TYPED_PTR(QueryClause);

FX_NS_END

#endif //__FX_QUERYCLAUSE_H
