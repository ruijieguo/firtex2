//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: uijieguo@gmail.com
// Created	: 2009/3/12
//
#ifndef __KEYWORDANALYZER
#define __KEYWORDANALYZER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Analyzer.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(analyzer)

class KeywordAnalyzer :	public Analyzer
{	
public:
    DECLARE_ANALYZER_CREATOR(KeywordAnalyzer, _T("keyword"));

    const static int32_t MAX_TOKEN_TO_ANALYZE = 100;

public:
    KeywordAnalyzer();
    KeywordAnalyzer(const KeywordAnalyzer& src);
    virtual ~KeywordAnalyzer(void);

public:
    /**
     * get identifier of analyzer
     * @return the identifier
     */
    const tstring getIdentifier() const;

    /**
     * Clone a new object
     */
    Analyzer* clone() const;

protected:
    void doTokenize(TokenSourcePtr& tokenSource) const;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
