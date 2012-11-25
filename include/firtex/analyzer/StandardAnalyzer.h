//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : ¹ùÈð½Ü(GuoRuijie)
// Email        : ruijieguo@gmail.com
// Created      : 2005/11/20
//

#ifndef _CHINESEANALYZER_H
#define _CHINESEANALYZER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Singleton.h"
#include "firtex/utility/MMapFile.h"
#include "firtex/utility/Path.h"
#include "firtex/analyzer/Analyzer.h"
#include "firtex/analyzer/Segmenter.h"

#define CORE_DICT_PREFIX "core"
#define USER_DICT_PREFIX "user"

#define DICT_EXT_NAME "dict"

FX_NS_DEF(analyzer);

class StandardAnalyzer : public Analyzer
{
public:
    DECLARE_ANALYZER_CREATOR(StandardAnalyzer, _T("standard"));

public:
    static tstring PARAM_ENCODE;
    static tstring PARAM_ALGORITHM;

    enum SegmentAlgorithm
    {
        SA_MAX_FORWARD = 0,
        SA_UNIGRAM
    };

public:
    StandardAnalyzer();
    StandardAnalyzer(const tstring& sDicName);
    StandardAnalyzer(const StandardAnalyzer& src);
    virtual ~StandardAnalyzer(void);

public:
    /**
     * Initialize and load dictionary
     * @throw FileIOException if FAILED.
     */
    void init();

    /**
     * Initialize and load specified dictionary
     * @throw FileIOException if FAILED.
     */
    void init(const tstring& sDictPath);

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
    inline tstring getCoreDictPath(const tstring& sDictPath) const;

private:
    Segmenter* m_pSegmenter;
    SegmentAlgorithm m_eSegAlgorithm;
    tstring m_sEncodeName;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(StandardAnalyzer);

///////////////////////////////////////////////
//
inline tstring StandardAnalyzer::getCoreDictPath(const tstring& sDictPath) const
{
    FX_NS(utility)::Path dictPath(sDictPath);
    dictPath.makeDirectory();
    tstring sFileName = tstring(CORE_DICT_PREFIX) + "." + 
                        m_sEncodeName + "." + DICT_EXT_NAME;
    dictPath.setFileName(sFileName);
    return dictPath.toString();
}

FX_NS_END

#endif
