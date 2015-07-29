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

#include <time.h>
#include "firtex/analyzer/DateTimeAnalyzer.h"
#include "firtex/analyzer/DateTimeLexer.h"
#include "firtex/utility/NumberFormatter.h"
#include "DateTimeBisonParser.hh"

using namespace std;
using namespace fx_bison;

FX_NS_USE(utility);
FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, DateTimeAnalyzer);

DateTimeAnalyzer::DateTimeAnalyzer(void)
{
    setMaxTokensToAnalyze(1);
}

DateTimeAnalyzer::DateTimeAnalyzer(const DateTimeAnalyzer& src)
    : Analyzer(src)
{
}

DateTimeAnalyzer::~DateTimeAnalyzer(void)
{
}

const tstring DateTimeAnalyzer::getIdentifier() const 
{
    return DateTimeAnalyzer::Creator::instance()->getIdentifier();
};

void DateTimeAnalyzer::doTokenize(TokenSourcePtr& tokenSource) const
{
    TokenViewPtr pInTokenView = tokenSource->getLastView();
    TokenViewPtr pOutTokenView = tokenSource->acquireCustomView(_T("datetime_view"));
    pOutTokenView->setTokenType(Token::TT_DATE);
     
    TokenView::Iterator iter = pInTokenView->iterator();
    while(iter.hasNext())
    {
        const Token& token = iter.next();
        string strRaw(token.getTextValue(), token.getTextLength());
        int64_t t = DateTimeAnalyzer::parse(strRaw);
        string strTime;
        NumberFormatter::append(strTime, t);
        pOutTokenView->addToken(strTime.c_str());
    }
}

int64_t DateTimeAnalyzer::parse(const std::string& sDateTime)
{
    istringstream iss(sDateTime);
    int64_t ret = DateTimeAnalyzer::parse(iss);
    if (ret == 0)
    {
        FX_LOG(WARN, "Invalid date time format: [%s]", sDateTime.c_str());
        return 0;
    }
    return ret;
}

int64_t DateTimeAnalyzer::parse(std::istream& input)
{
    ostringstream oss;
    DateTimeLexer lexer(&input, &oss);
    int64_t dt = 0;
    DateTimeBisonParser parser(lexer, dt);
    if (parser.parse() != 0)
    {
        return 0;
    }
    return dt;
}

int64_t DateTimeAnalyzer::makeTime(int32_t y, int32_t m, int32_t d, 
                                   int32_t h, int32_t min, int32_t s)
{
    if (y < 100)
    {
        if (y >= 69)
        {
            y += 1900;
        }
        else y += 2000;
    }

    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(timeinfo));
    timeinfo.tm_year = y - 1900;
    timeinfo.tm_mon = m - 1;
    timeinfo.tm_mday = d;
    timeinfo.tm_hour = h;
    timeinfo.tm_min = min;
    timeinfo.tm_sec = s;
    return mktime(&timeinfo);
}

Analyzer* DateTimeAnalyzer::clone() const
{
    return new DateTimeAnalyzer(*this);
}

FX_NS_END

