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
// Created	: 2011-04-03 11:24:52

#ifndef __FX_SINGLEWORDANALYZER_H
#define __FX_SINGLEWORDANALYZER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/Analyzer.h"
#include "firtex/analyzer/Tokenizer.h"

FX_NS_DEF(analyzer);

class SingleWordAnalyzer : public Analyzer
{
public:
    DECLARE_ANALYZER_CREATOR(SingleWordAnalyzer, _T("single"));

public:
    static tstring PARAM_ENCODE;

public:
    SingleWordAnalyzer();
    SingleWordAnalyzer(const tstring& sEncode);
    SingleWordAnalyzer(const SingleWordAnalyzer& src);
    ~SingleWordAnalyzer();

public:
    /**
     * Initialize or load dictionary
     */
    void init();

    /**
     * Set parameter for analyzer
     * @param[in] sParam parameter for analyzer.
     *             format: param_name=param_value,...
     */
    void setParam(const tstring& sParam);

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
    tstring m_sEncodeName;
    TokenizerPtr m_pTokenizer;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_SINGLEWORDANALYZER_H
