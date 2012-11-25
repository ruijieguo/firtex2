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

#ifndef __FX_FILTERCLAUSE_H
#define __FX_FILTERCLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Clause.h"

FX_NS_DEF(search);

class FilterClause : public Clause
{
public:
    DECLARE_CLAUSE_CREATOR(FilterClause);

    const static std::string PREFIX;

public:
    FilterClause(const std::string& sName,
                 const std::string& sValue);
    ~FilterClause();

public:
    const std::string& getFilterString() const;
};

DEFINE_TYPED_PTR(FilterClause);

//////////////////////////////////////////
//
inline const std::string& FilterClause::getFilterString() const
{
    return getValue();
}

FX_NS_END

#endif //__FX_FILTERCLAUSE_H
