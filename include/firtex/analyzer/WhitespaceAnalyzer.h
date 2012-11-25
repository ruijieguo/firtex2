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
// Email	: ruijieguo@gmail.com
// Created	: 2011-02-27 10:11:23

#ifndef __FX_WHITESPACEANALYZER_H
#define __FX_WHITESPACEANALYZER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Singleton.h"
#include "firtex/analyzer/Analyzer.h"

FX_NS_DEF(analyzer);

class WhitespaceAnalyzer : public Analyzer
{
public:
    DECLARE_ANALYZER_CREATOR(WhitespaceAnalyzer, _T("whitespace"));

    const static int32_t MAX_TOKEN_TO_ANALYZE = 10000;

public:
    WhitespaceAnalyzer();
    WhitespaceAnalyzer(const WhitespaceAnalyzer& src);
    ~WhitespaceAnalyzer();

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

#endif //__FX_WHITESPACEANALYZER_H
