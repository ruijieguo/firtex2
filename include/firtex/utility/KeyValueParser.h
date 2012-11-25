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
// Created	: 2007/1/17
//
#ifndef __KEYVALUEPARSER_H
#define __KEYVALUEPARSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include <vector>
#include <map>

FX_NS_DEF(utility)
	
class KeyValueParser
{
public:
    typedef std::pair<std::string, std::string> KVPair;
    typedef std::vector<KVPair> KeyValueVector;

public:
    KeyValueParser();
    ~KeyValueParser();

public:
    /**
     * Parse key value pair, format: key1=value1; key2=value2;
     * @param sKVs key value string
     * @param sSep separator of key value pairs
     * @param sEq separator between key and value
     * @return true true for success
     */
    bool parse(const std::string& sKVs, const std::string& sSep = ";",
               const std::string& sEq = "=");

    /**
     * Parse key value pair without key£¬format£ºvalue1;value2;..
     * @param sKVs key value string
     * @param sSep separator of key value pairs
     * @return true true for success
     */
    bool parseValue(const std::string& sKVs, const std::string& sSep = ";");

    /** 
     * Return number of key value pairs
     */
    size_t getKVCount() const {return m_items.size();}

    /**
     * Get value of specified key after parsing success
     * @param sKey name of key
     * @param value value of key
     * @return true for success
     */
    bool getValue(const std::string& sKey, uint32_t& value) const;
    bool getValue(const std::string& sKey, int32_t& value) const;
    bool getValue(const std::string& sKey, int64_t& value) const;
    bool getValue(const std::string& sKey, float& value) const;
    bool getValue(const std::string& sKey, std::string& value) const;

    /**
     * Get key value pair by offset
     * @param _off offset of key value in kv pairs
     * @param sKey key returned
     * @param value key returned
     * @return true for success
     */
    bool getKeyValue(size_t _off, std::string& sKey, std::string& value) const;

    /**
     * Get value by offset
     * @param _off offset of key value in kv pairs
     * @param value key returned
     * @return true for success
     */
    bool getValue(size_t _off, std::string& value) const;

protected:
    bool findKeyValue(const std::string& sKey, KVPair& kv) const;

protected:
    KeyValueVector m_items;
};

FX_NS_END

#endif
