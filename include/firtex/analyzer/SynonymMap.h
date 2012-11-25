//
// Copyright(C) 2005--2009 FirteX Develop Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/17
//

#ifndef __SYNONYMMAP_H
#define __SYNONYMMAP_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include <vector>
#include <map>

FX_NS_DEF(analyzer);

class SynonymMap
{
public:
    typedef std::vector<tstring> Group;

private:
    typedef std::map<tstring, SynonymMap::Group*> SynMap;

public:
    typedef SynMap::const_iterator Iterator;

public:
    SynonymMap(void);
    SynonymMap(const SynonymMap& src);
    virtual ~SynonymMap(void);

public:
    /**
     * Return synonyms by word text
     * @param szWord word text
     * @return synonym group
     */
    SynonymMap::Group* getSynonyms(const tchar* szWord) const;
	
    /**
     * load synonym from file
     * @param szFileName synonym file name
     */
    void load(const tstring& sFileName);

    /**
     * save synonyms to file
     * @param szFileName synonym file name
     */
    void save(const tstring& sFileName);

    /**
     * empty or not
     * @return true iff empty
     */
    bool isEmpty() const {return (m_synMap.size() == 0);}

    /**
     * Return number of synonyms
     * @return number fo synonyms
     */
    size_t getNumSynonyms() const {return m_synMap.size();}

    /**
     * add synonyms
     * @param szKey key of synonyms
     * @param szSynonyms synonym words splited by ','
     * @return true iff add success
     */
    bool addSynonyms(const tchar* szKey, const tchar* szSynonyms);

    /**
     * add one synonym to szKey
     * @param szKey key of synonym
     * @param szSynonyms synonym words splited by ','
     * @return true iff add success
     */
    bool addSynonym(const tchar* szKey, const tchar* szSynonym);

    /**
     * remove synonyms
     * @param szKey key of synonyms
     * @return true iff remove success
     */
    bool removeSynonyms(const tchar* szKey);

    /**
     * remove one synonym from szKey 
     * @param szKey key of synonyms
     * @return true iff remove success
     */
    bool removeSynonym(const tchar* szKey, const tchar* szSynonym);

    /**
     * Return synonym and translate to string
     * @param szKey key of synonyms
     * @return synonyms in string format which was seperated by ","
     */     
    tstring toString(const tchar* szKey);

    /**
     * Return iterator for synonyms
     */
    Iterator begin() const { return m_synMap.begin();}
    Iterator end() const { return m_synMap.end();}
    
    /**
     * close and release resource
     */
    void close();

private:
    bool isExist(Group* pGroup, const tstring& sSynonym) const;

private:
    SynMap m_synMap;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
