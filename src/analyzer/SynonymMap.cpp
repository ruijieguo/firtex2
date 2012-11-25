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
#include "firtex/analyzer/SynonymMap.h"
#include "firtex/utility/TextFile.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(utility);

FX_NS_DEF(analyzer)

SETUP_STREAM_LOGGER(analyzer, SynonymMap);

SynonymMap::SynonymMap(void)
{
}

SynonymMap::SynonymMap(const SynonymMap& src)
{
    for (SynMap::const_iterator it = src.m_synMap.begin();
         it != src.m_synMap.end(); ++it)
    {
        m_synMap.insert(make_pair(it->first, new Group(*(it->second))));
    }
}

SynonymMap::~SynonymMap(void)
{
    close();
}

SynonymMap::Group* SynonymMap::getSynonyms(const char* szWord) const
{
    string sLowcaseSyn = toLower(string(szWord));
    SynonymMap::SynMap::const_iterator iter = m_synMap.find(sLowcaseSyn);
    if(iter != m_synMap.end())
        return iter->second;
    return NULL;
}

void SynonymMap::load(const string& sFileName)
{
    close();
    TextFile tf;
    tf.open(sFileName.c_str(), "r");
    char szLine[512];
    if(tf.isFileOpen())
    {
        while(!tf.isEof() && tf.readLine(szLine, 512, true) > 0)
        {
            StringTokenizer st(szLine, ":", StringTokenizer::TOKEN_TRIM
                    | StringTokenizer::TOKEN_IGNORE_EMPTY );
            if(st.getNumTokens() != 2)
                continue;            
            addSynonyms(st[0].c_str(), st[1].c_str());
        }
    }
}

void SynonymMap::save(const string& sFileName)
{
    TextFile tf;
    tf.open(sFileName.c_str(), "w");
    
    for(SynMap::const_iterator it = m_synMap.begin(); it != m_synMap.end();it++)
    {
        tf.write(it->first.c_str());
        Group* pGroup = it->second;
        tf.write(":");
        for(Group::const_iterator git = pGroup->begin(); git != pGroup->end(); git++)
        {
            tf.write((*git).c_str());
            if( (git + 1) != pGroup->end())
            {
                tf.write(",");
            }            
        }
        tf.write("\n");        
    }
    tf.close();    
}

bool SynonymMap::addSynonyms(const char* szKey, const char* szSynonyms)
{
    string sSyn = toLower(string(szSynonyms));
    StringTokenizer st(sSyn, ",", StringTokenizer::TOKEN_TRIM
                       | StringTokenizer::TOKEN_IGNORE_EMPTY );
    if(st.getNumTokens() == 1)
    {
        bool bSuc = addSynonym(szKey, st[0].c_str());
        bSuc |= (addSynonym(st[0].c_str(), szKey));        
        return bSuc;
    }
    else if(st.getNumTokens() > 1)
    {
        string sKey = toLower(string(szKey));
        SynMap::iterator it = m_synMap.find(sKey);
        Group* pGroup = NULL;
        if(it != m_synMap.end())
        {
            pGroup = it->second;
        }
        else
        {
            pGroup = new SynonymMap::Group();
        }
        bool bAdded = false;
        for(StringTokenizer::Iterator iter = st.begin(); iter != st.end(); iter++)
        {
            if(!isExist(pGroup, *iter))
            {
                pGroup->push_back(*iter);
                bAdded = true;
            }
        }
        if(it == m_synMap.end())
        {
            m_synMap.insert(make_pair(sKey,pGroup));
        }
        return bAdded;
    }
    else
    {
        return false;
    }
    return true;
}

bool SynonymMap::addSynonym(const char* szKey, const char* szSynonym)
{
    if( !szKey || !*szKey || !szSynonym || !*szSynonym)
        return false;
        
    string sKey = toLower(string(szKey));
    SynMap::iterator it = m_synMap.find(sKey);
    Group* pGroup = NULL;
    if(it != m_synMap.end())
    {
        pGroup = it->second;
    }
    else
    {
        pGroup = new SynonymMap::Group();
    }
    string sSyn = toLower(string(szSynonym));
    if(!isExist(pGroup, sSyn))
    {
        pGroup->push_back(sSyn);
        if(it == m_synMap.end())
        {
            m_synMap.insert(make_pair(sKey, pGroup));
        }
        return true;            
    }
    return false;
}

bool SynonymMap::removeSynonyms(const char* szKey)
{
    string sLowcaseSyn = toLower(string(szKey));
    SynMap::iterator it = m_synMap.find(sLowcaseSyn);
    if(it != m_synMap.end())
    {
        Group* pGroup = it->second;
        if(pGroup->size() == 1)
        {
            SynMap::iterator it2 = m_synMap.find((*pGroup)[0]);
            if(it2 != m_synMap.end())
            {
                delete it2->second;
                m_synMap.erase(it2);
            }            
        }
        delete pGroup;        
        m_synMap.erase(it);
        return true;
    }
    return false;    
}

bool SynonymMap::removeSynonym(const char* szKey, const char* szSynonym)
{
    string sLowcaseKey = toLower(string(szKey));
    SynMap::iterator it = m_synMap.find(sLowcaseKey);    
    if(it == m_synMap.end())
    {
        return false;
    }
    Group* pGroup = it->second;
    string sLowcaseSyn = toLower(string(szSynonym));
    for(Group::iterator git = pGroup->begin(); git != pGroup->end(); git++)
    {
        if(sLowcaseSyn == *git)
        {
            pGroup->erase(git);
            if(pGroup->size() == 0)
            {
                delete pGroup;                
                m_synMap.erase(it);                
            }            
            return true;
        }
    }
    return false;    
}

void SynonymMap::close()
{
    for(SynonymMap::SynMap::iterator iter = m_synMap.begin();
        iter != m_synMap.end(); iter++)
    {
        delete iter->second;
    }
    m_synMap.clear();
}

bool SynonymMap::isExist(Group* pGroup, const string& sSynonym) const
{
    for (Group::const_iterator it = pGroup->begin(); it != pGroup->end(); it++)
    {
        if(*it == sSynonym)
            return true;
    }
    return false;    
}

string SynonymMap::toString(const char* szKey)
{
    Iterator it = m_synMap.find(szKey);
    if(it == m_synMap.end())
        return "";
    string sSyns;
    Group* pGroup = it->second;    
    for(Group::const_iterator it2 = pGroup->begin(); it2 != pGroup->end(); it2++)
    {
        sSyns += *it2;
        if(it2 + 1!= pGroup->end())
            sSyns += ",";
    }
    return sSyns;
}

FX_NS_END
