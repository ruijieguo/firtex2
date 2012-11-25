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
// Created	: 2005/7/24
//
#ifndef __DATETIMEANALYZER_H
#define __DATETIMEANALYZER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Analyzer.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(analyzer)

class DateTimeAnalyzer : public Analyzer
{		
public:
    DECLARE_ANALYZER_CREATOR(DateTimeAnalyzer, _T("datetime"));

public:
    DateTimeAnalyzer();
    DateTimeAnalyzer(const DateTimeAnalyzer& src);
    virtual ~DateTimeAnalyzer(void);

public:
    /**
     * get identifier of analyzer
     * @return the identifier
     */
    const std::string getIdentifier() const;

    /**
     * Clone a new object
     */
    Analyzer* clone() const;

protected:
    void doTokenize(TokenSourcePtr& tokenSource) const;

public:
    /**
     * @param sDateTime date time string
     * 
     * Format:
     * 2008-01-01T12:00:00Z
     * Sat, 1 Jan 2005 12:00:00 GMT
     * Sat, 01 Jan 2005 11:00:00 GMT
     * Saturday, 1-Jan-05 11:00:00 GMT
     * 2005/1/1 12:00:00 
     * 2005/01/01 12:00:00
     * Sat Jan 01 12:00:00 2005
     * 2005年1月1日 12点00分00秒
     *
     * @return the timestamp expressed in time_t.
     * time_t base time is midnight, January 1, 1970.
     * Resolution is one second.
     */
    static int64_t parse(const std::string& sDateTime);

    static int64_t parse(std::istream& input);

    static int64_t makeTime(int32_t y, int32_t m, int32_t d, 
                            int32_t h, int32_t min, int32_t s);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
