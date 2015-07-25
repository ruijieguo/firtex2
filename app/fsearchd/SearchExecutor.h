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
// Created : 2015-07-14 08:07:49

#ifndef __FX_SEARCHEXECUTOR_H
#define __FX_SEARCHEXECUTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/search/ResultFormatter.h"
#include <sstream>

FX_NS_DEF(app);

class SearchExecutor
{
public:
    SearchExecutor(FX_NS(search)::IndexSearcher& searcher,
                   FX_NS(search)::ResultFormatter& formatter);
    ~SearchExecutor();

public:
    void search(std::stringstream& ss, const std::string& sState);
    void search(std::stringstream& ss, const FX_NS(search)::Statement& state);
               
private:
    FX_NS(search)::IndexSearcher& m_searcher;
    FX_NS(search)::ResultFormatter& m_formatter;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_SEARCHEXECUTOR_H
