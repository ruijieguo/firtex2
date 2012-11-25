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
// Created	: 2007/1/7
//
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/NumberParser.h"

using namespace std;

FX_NS_DEF(utility);

//////////////////////////////////////////////////////////////////////////
///KeyValueParser
KeyValueParser::KeyValueParser()
{

}
KeyValueParser::~KeyValueParser()
{
}

bool KeyValueParser::parse(const std::string& sKVs,
                           const std::string& sSep,
                           const std::string& sEq)
{
    StringTokenizer st;
    st.tokenize(sKVs, sSep, StringTokenizer::TOKEN_TRIM 
                | StringTokenizer::TOKEN_IGNORE_EMPTY);
    if(st.getNumTokens() <= 0)
        return false;

    bool bSuc = true;
    StringTokenizer::Iterator iter = st.begin();
    while (iter != st.end())
    {
        StringTokenizer itemTokenizer;
        itemTokenizer.tokenize(*iter, sEq, StringTokenizer::TOKEN_TRIM | StringTokenizer::TOKEN_IGNORE_EMPTY);
        if (itemTokenizer.getNumTokens() == 2)
        {
            m_items.push_back(make_pair(itemTokenizer[0], itemTokenizer[1]));
        }
        else if (itemTokenizer.getNumTokens() == 1)
        {
            m_items.push_back(make_pair(itemTokenizer[0], ""));
        }
        else 
        {
            bSuc = false;
        }

        iter++;
    }
    return bSuc;
}

bool KeyValueParser::parseValue(const std::string& sKVs, const std::string& sSep)
{
    StringTokenizer st;
    st.tokenize(sKVs, sSep, StringTokenizer::TOKEN_TRIM | StringTokenizer::TOKEN_IGNORE_EMPTY);

    if(st.getNumTokens() <= 0)
        return false;			
    StringTokenizer::Iterator iter = st.begin();
    while(iter != st.end())
    {							
        m_items.push_back(make_pair(*iter, *iter));
        iter++;
    }
    return true;
}

bool KeyValueParser::getValue(const std::string& sKey, uint32_t& value) const
{
    KVPair kv;
    bool ret = findKeyValue(sKey, kv);
    if(!ret)
    {
        return false;
    }
    return NumberParser::tryParseUInt32(kv.second, value);
}

bool KeyValueParser::getValue(const std::string& sKey, int32_t& value) const
{
    KVPair kv;
    bool ret = findKeyValue(sKey, kv);
    if(!ret)
    {
        return false;
    }
    return NumberParser::tryParseInt32(kv.second, value);
}

bool KeyValueParser::getValue(const std::string& sKey, int64_t& value) const
{
    KVPair kv;
    bool ret = findKeyValue(sKey, kv);
    if(!ret)
    {
        return false;
    }
    return NumberParser::tryParseInt64(kv.second, value);
}

bool KeyValueParser::getValue(const std::string& sKey, float& value) const
{
    KVPair kv;
    bool ret = findKeyValue(sKey, kv);
    if(!ret)
    {
        return false;
    }
    return NumberParser::tryParseFloat(kv.second, value);
}

bool KeyValueParser::getValue(const std::string& sKey, std::string& value) const
{
    KVPair kv;
    bool ret = findKeyValue(sKey, kv);
    if(!ret)
    {
        return false;
    }
    value = kv.second;
    return true;
}

bool KeyValueParser::getKeyValue(size_t _off, std::string& sKey, std::string& value) const
{
    if (m_items.size() <= _off)
        return false;
    sKey = m_items[_off].first;
    value = m_items[_off].second;
    return true;
}

bool KeyValueParser::getValue(size_t _off, std::string& value) const
{
    if(m_items.size() <= _off)
        return false;
    value = m_items[_off].second;
    return true;
}

bool KeyValueParser::findKeyValue(const std::string& sKey, KVPair& kv) const
{
    KeyValueVector::const_iterator iter = m_items.begin();
    while(iter != m_items.end())
    {
        if(!strCompareNoCase(sKey, (*iter).first))
        {
            kv = *iter;
            return true;
        }
        iter++;
    }
    return false;
}

FX_NS_END

