//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2005/7/24
//

#include "firtex/analyzer/NumericAnalyzer.h"
#include "firtex/utility/KeyValueParser.h"

FX_NS_USE(utility);
FX_NS_DEF(analyzer);

template<>
NumericAnalyzerCreator<int32_t>::NumericAnalyzerCreator()
{
    m_identifier += _T(".numeric.int32");
}

template<>
NumericAnalyzerCreator<int64_t>::NumericAnalyzerCreator()
{
    m_identifier += _T(".numeric.int64");
}

FX_NS_END

