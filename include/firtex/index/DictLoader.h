//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-08-14 21:38:12

#ifndef __FX_DICTLOADER_H
#define __FX_DICTLOADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TypedDict.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/store/FileSystem.h"
#include <map>

FX_NS_DEF(index);

template <typename Key>
class DictLoader : public FX_NS(utility)::Singleton<DictLoader<Key> >
{
public:
    typedef Key KeyType;
    typedef TypedDict<KeyType, offset_t> DictType;
    typedef std::pair<int32_t, DictType*>  DictTypeRef;
    typedef std::map<std::string, DictTypeRef> DictTypeMap;

    DECLARE_LAZY_SINGLETON(DictLoader);
protected:
    DictLoader() {}
    ~DictLoader();

public:
    const DictType* loadDict(const std::string& sDictName,
                             const FX_NS(store)::FileSystemPtr& pFileSystem);
    bool incRefDict(const std::string& sDictName);

    void releaseDict(const std::string& sDictName);

protected:
    FX_NS(thread)::FastMutex m_lock;
    DictTypeMap m_name2Dict;

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////////////////////////////
//
SETUP_LOGGER_TEMP(index, DictLoader);

template <typename Key>
DictLoader<Key>::~DictLoader() 
{
    for (typename DictTypeMap::iterator it = m_name2Dict.begin();
         it != m_name2Dict.end(); ++it)
    {
        delete it->second.second;
    }
    m_name2Dict.clear();
}

template <typename Key>
const typename DictLoader<Key>::DictType* 
DictLoader<Key>::loadDict(const std::string& sDictName, 
                          const FX_NS(store)::FileSystemPtr& pFileSystem)
{
    FX_NS(thread)::FastMutex::Guard lock(m_lock);

    typename DictTypeMap::iterator it = m_name2Dict.find(sDictName);
    if (it == m_name2Dict.end())
    {
        FX_NS(store)::FileSystemPtr pTmpFileSystem = pFileSystem;
        FX_NS(store)::InputStreamPtr pInStream = pTmpFileSystem->openFile(sDictName);
        DictType* pDict = new DictType();
        pDict->load(pInStream);
        m_name2Dict.insert(make_pair(sDictName, std::make_pair(1, pDict)));
        FX_TRACE("Load dict: %s, ref: 1", sDictName.c_str());
        return pDict;
    }
    else
    {
        it->second.first++;
        FX_TRACE("Load dict: %s, ref: %d",
                 sDictName.c_str(), it->second.first);
    }
    return it->second.second;
}

template <typename Key>
bool DictLoader<Key>::incRefDict(const std::string& sDictName)
{
    FX_NS(thread)::FastMutex::Guard lock(m_lock);

    typename DictTypeMap::iterator it = m_name2Dict.find(sDictName);
    if (it != m_name2Dict.end())
    {
        it->second.first++;
        FX_TRACE("Load dict: %s, ref: %d",
                        sDictName.c_str(), it->second.first);
        return true;
    }
    return false;
}

template <typename Key>
void DictLoader<Key>::releaseDict(const std::string& sDictName)
{
    FX_NS(thread)::FastMutex::Guard lock(m_lock);
    typename DictTypeMap::iterator it = m_name2Dict.find(sDictName);
    if (it != m_name2Dict.end())
    {
        it->second.first--;
        if (it->second.first == 0)
        {
            delete it->second.second;
            m_name2Dict.erase(it);
            FX_TRACE("Dict: %s released", sDictName.c_str());
        }
    }
}

FX_NS_END

#endif //__FX_DICTLOADER_H
