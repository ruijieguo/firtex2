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
// Created : 2012-01-23 17:00:50

#ifndef __FX_DOCSELECTCLAUSE_H
#define __FX_DOCSELECTCLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Clause.h"

FX_NS_DEF(search);

class DocSelectClause : public Clause
{
public:
    DECLARE_CLAUSE_CREATOR(DocSelectClause);

    const static std::string PREFIX;

public:
    DocSelectClause(const std::string& sName,
                    const std::string& sValue)
        : Clause(sName, sValue) {}
};

DEFINE_TYPED_PTR(DocSelectClause);

FX_NS_END

#endif //__FX_DOCSELECTCLAUSE_H
