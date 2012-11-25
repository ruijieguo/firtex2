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
// Created	: 2010-08-21 14:53:57

#ifndef __FX_MERGEINFO_H
#define __FX_MERGEINFO_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/DocIdRecycling.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/DocIdRecycling.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/ForwardIndexIterator.h"
#include <vector>

FX_NS_DEF(index);

class MergeInfo
{
public:
    MergeInfo(const BarrelInfo& barrelInfo,
              const DocIdRecyclingPtr& pDocIdRecy,
              docid_t newBaseDocId)
        : m_barrelInfo(barrelInfo)
        , m_pDocIdRemapper(pDocIdRecy)
        , m_newBaseDocId(newBaseDocId)
    {
    }

    MergeInfo(const MergeInfo& src)
        : m_barrelInfo(src.m_barrelInfo)
        , m_pDocIdRemapper(src.m_pDocIdRemapper)
        , m_newBaseDocId(src.m_newBaseDocId)
    {
    }

    virtual ~MergeInfo()
    {
    }

public:
    void setBarrelInfo(const BarrelInfo& barrelInfo) 
    {
        m_barrelInfo = barrelInfo;
    }
    
    const BarrelInfo& getBarrelInfo() const 
    {
        return m_barrelInfo;
    }
    
    BarrelInfo& getBarrelInfo() 
    {
        return m_barrelInfo;
    }

    void setDocIdRecycling(const DocIdRecyclingPtr& pDocIdRecy)
    {
        m_pDocIdRemapper = pDocIdRecy;
    }

    const DocIdRecycling* getDocIdRecycling() const
    {
        return m_pDocIdRemapper.get();
    }

    docid_t getNewBaseDocId() const 
    {
        return m_newBaseDocId;
    }

    void setNewBaseDocId(docid_t newBaseDocId)
    {
        m_newBaseDocId = newBaseDocId;
    }

protected:
    BarrelInfo m_barrelInfo;
    DocIdRecyclingPtr m_pDocIdRemapper;
    docid_t m_newBaseDocId;
};

class IndexMergeInfo : public MergeInfo
{
public:
    IndexMergeInfo(const MergeInfo& mergeInfo,
                   const TermReaderPtr& pTermReader = TermReaderPtr())
        : MergeInfo(mergeInfo)
        , m_pTermReader(pTermReader)
    {
    }

    IndexMergeInfo(const BarrelInfo& barrelInfo,
                   const DocIdRecyclingPtr& pDocIdRecy,
                   docid_t newBaseDocId,
                   const TermReaderPtr& pTermReader = TermReaderPtr())
        : MergeInfo(barrelInfo, pDocIdRecy, newBaseDocId)
        , m_pTermReader(pTermReader)
    {
    }

    IndexMergeInfo(const IndexMergeInfo& src)
        : MergeInfo((const MergeInfo&)src)
        , m_pTermReader(src.m_pTermReader)
    {
    }

    ~IndexMergeInfo()
    {
    }

public:
    void setTermReader(const TermReaderPtr& pTermReader)
    {
        m_pTermReader = pTermReader;
    }

    TermReaderPtr getTermReader() const { return m_pTermReader;}

protected:
    TermReaderPtr m_pTermReader;
};

class ForwardIndexMergeInfo : public MergeInfo
{
public:
    ForwardIndexMergeInfo(const MergeInfo& mergeInfo,
                          const ForwardIndexIteratorPtr& pIterator = ForwardIndexIteratorPtr())
        : MergeInfo(mergeInfo)
        , m_pIterator(pIterator)
    {
    }

    ForwardIndexMergeInfo(const BarrelInfo& barrelInfo,
                          const DocIdRecyclingPtr& pDocIdRecy,
                          docid_t newBaseDocId,
                          const ForwardIndexIteratorPtr& pIterator = ForwardIndexIteratorPtr())
        : MergeInfo(barrelInfo, pDocIdRecy, newBaseDocId)
        , m_pIterator(pIterator)
    {
    }

    ForwardIndexMergeInfo(const ForwardIndexMergeInfo& src)
        : MergeInfo((const MergeInfo&)src)
        , m_pIterator(src.m_pIterator)
    {
    }

    ~ForwardIndexMergeInfo()
    {
    }

public:
    void setIterator(const ForwardIndexIteratorPtr& pIterator)
    {
        m_pIterator = pIterator;
    }

    ForwardIndexIteratorPtr getIterator() const { return m_pIterator;}

protected:
    ForwardIndexIteratorPtr m_pIterator;
};

template <typename T>
class TypedMergeInfos
{
public:
    typedef T InfoType;

public:
    TypedMergeInfos(const std::string& sSuffix)
        : m_sSuffix(sSuffix)
    {
    }

    ~TypedMergeInfos() {}

public:
    const std::string& getSuffix() const
    {
        return m_sSuffix;
    }

    void reserve(size_t size) {m_fields.reserve(size);}
    void pushBack(const InfoType& mergeInfo)
    {
        m_fields.push_back(mergeInfo);
    }
    size_t size() const {return m_fields.size();}

    const InfoType& operator[] (size_t idx) const
    {
        return m_fields[idx];
    }
    InfoType& operator[] (size_t idx)
    {
        return m_fields[idx];
    }

private:
    typedef std::vector<InfoType> InfoVector;

private:
    std::string m_sSuffix;
    InfoVector m_fields;
};

typedef TypedMergeInfos<MergeInfo> MergeInfos;

DEFINE_TYPED_PTR(MergeInfos);

typedef TypedMergeInfos<IndexMergeInfo> IndexMergeInfos;
DEFINE_TYPED_PTR(IndexMergeInfos);

typedef TypedMergeInfos<ForwardIndexMergeInfo> ForwardIndexMergeInfos;
DEFINE_TYPED_PTR(ForwardIndexMergeInfos);

FX_NS_END

#endif //__FX_MERGEFIELDINFO_H
