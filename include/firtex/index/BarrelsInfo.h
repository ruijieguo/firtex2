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
// Created	: 2005/12/27
//

#ifndef __FX_BARRELSINFO_H
#define __FX_BARRELSINFO_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include "firtex/utility/String.h"
#include "firtex/thread/FastMutex.h"
#include <vector>

FX_NS_DEF(index);

/**
 * @brief description of index meta
 */
struct FieldMeta
{
    FieldMeta()
        : totalTermCount(0)
        , distinctTermCount(0)
    {
    }

    FieldMeta(const std::string& sName, uint64_t tc, uint64_t dtc)
        : fieldName(sName)
        , totalTermCount(tc)
        , distinctTermCount(dtc)
    {
    }

    FieldMeta(const FieldMeta& src)
        : fieldName(src.fieldName)
        , totalTermCount(src.totalTermCount)
        , distinctTermCount(src.distinctTermCount)
    {
    }

    FieldMeta& operator = (const FieldMeta& src)
    {
        fieldName = src.fieldName;
        totalTermCount = src.totalTermCount;
        distinctTermCount = src.distinctTermCount;
        return *this;
    }

    std::string fieldName;          //field name
    uint64_t totalTermCount;    ///total term count
    uint64_t distinctTermCount; ///total distinct terms
};

class IndexMeta
{
public:
    IndexMeta() {}
    IndexMeta(const IndexMeta& src) 
    {
        for (size_t i = 0; i < src.m_fieldMetas.size(); ++i)
        {
            fieldMeta(src.m_fieldMetas[i]->fieldName) = *(src.m_fieldMetas[i]);
        }
    }

    ~IndexMeta() 
    {
        clear();
    }

public:
    IndexMeta& operator = (const IndexMeta& src)
    {
        for (size_t i = 0; i < src.m_fieldMetas.size(); ++i)
        {
            fieldMeta(src.m_fieldMetas[i]->fieldName) = *(src.m_fieldMetas[i]);
        }
        return *this;
    }

    size_t size() const
    {
        return m_fieldMetas.size();
    }

    void clear()
    {
        for (MetaVector::iterator it = m_fieldMetas.begin();
             it != m_fieldMetas.end(); ++it)
        {
            delete *it;
        }
        m_fieldMetas.clear();
        m_field2Meta.clear();
    }

    bool hasFieldMeta(const std::string& sFieldName) const
    {
        return m_field2Meta.find(sFieldName) != m_field2Meta.end();
    }

    FieldMeta& fieldMeta(const std::string& sFieldName)
    {
        MetaMap::const_iterator it = m_field2Meta.find(sFieldName);
        if (it != m_field2Meta.end())
        {
            return *(it->second);
        }

        FieldMeta* pMeta = new FieldMeta(sFieldName, 0, 0);
        m_fieldMetas.push_back(pMeta);
        m_field2Meta.insert(std::make_pair(sFieldName, pMeta));
        return *pMeta;
    }

    const FieldMeta& fieldMeta(const std::string& sFieldName) const
    {
        MetaMap::const_iterator it = m_field2Meta.find(sFieldName);
        if (it != m_field2Meta.end())
        {
            return *(it->second);
        }

        FIRTEX_ASSERT2(false);
        //avoid compilation warning
        FieldMeta* pNullMeta = NULL;
        return *(pNullMeta);
    }

    const FieldMeta& operator[] (size_t idx) const
    {
        return *(m_fieldMetas[idx]);
    }

    FieldMeta& operator[] (size_t idx)
    {
        return *(m_fieldMetas[idx]);
    }

private:
    typedef std::vector<FieldMeta*> MetaVector;
    typedef std::map<std::string, FieldMeta*> MetaMap;

private:
    MetaVector m_fieldMetas;
    MetaMap m_field2Meta;
};

/**
 * @brief description of one index barrel
 */
class BarrelInfo
{
public:
    enum State
    {
        BUILDING,
        MERGING,
        ONDISK,
    };

public:
    BarrelInfo()
        : m_commitId(0)
        , m_baseDocId(0)
        , m_nDocCount(0)
        , m_nDeletedDocCount(0)
        , m_state(ONDISK)
    {
    }

    BarrelInfo(commitid_t commitId, docid_t nBaseDocId,
               df_t nCount, df_t nNumDelDocs)
        : m_commitId(commitId)
        , m_baseDocId(nBaseDocId)
        , m_nDocCount(nCount)
        , m_nDeletedDocCount(nNumDelDocs)
        , m_state(ONDISK)
    {
    }

    BarrelInfo(const BarrelInfo& src)
        : m_indexMeta(src.m_indexMeta)
        , m_commitId(src.m_commitId)
        , m_baseDocId(src.m_baseDocId)
        , m_nDocCount(src.m_nDocCount)
        , m_nDeletedDocCount(src.m_nDeletedDocCount)
        , m_state(src.m_state)
    {
    }

    ~BarrelInfo()
    {
    }

public:
    commitid_t getCommitId() const {return m_commitId;}
    void setCommitId(commitid_t rev) { m_commitId = rev;}

    std::string getSuffix() const;

    docid_t getBaseDocId() const {return m_baseDocId;}
    void setBaseDocId(docid_t did){m_baseDocId = did;}

    df_t getDocCount() const {return m_nDocCount;}
    void setDocCount(df_t count){m_nDocCount = count;}

    df_t getDeletedDocCount() const {return m_nDeletedDocCount;}
    void setDeletedDocs(df_t count);

    /// Get/Set true if in memory
    State getState() const {return m_state;}
    void setState(State s) { m_state = s;}

    /**
     * Return index meta for modify
     */
    IndexMeta& getIndexMeta();

    /**
     * Return index meta for read-only access
     */
    const IndexMeta& getIndexMeta() const;

    /// Assign operation
    BarrelInfo& operator = (const BarrelInfo& other);

    /**
     * Remove the index barrel
     */
    void remove(FX_NS(store)::FileSystemPtr& pFileSys);

public:
    static std::string getSuffix(commitid_t commId, State s = ONDISK);

private:
    IndexMeta m_indexMeta;

    commitid_t m_commitId;
    docid_t m_baseDocId;
    df_t m_nDocCount;
    df_t m_nDeletedDocCount;
    State m_state;

    friend class BarrelsInfo;
};

DEFINE_TYPED_PTR(BarrelInfo);

/**
 * @brief description of all barrels in the index database
 */
class BarrelsInfo
{
public:
    typedef std::vector<BarrelInfoPtr> BarrelVector;

    struct BarrelComparator
    {
        bool operator ()(const BarrelInfoPtr& b1, const BarrelInfoPtr& b2)
        {
            return b1->getCommitId() < b2->getCommitId();
        }
    };

public:
    class Iterator
    {
    public:
        Iterator(const BarrelsInfo* pBarrelsInfo)
            : m_pBarrelsInfo(pBarrelsInfo)
            , m_iterator(pBarrelsInfo->m_barrelInfos.begin())
        {
        }

        Iterator(const Iterator& _iter)
            : m_pBarrelsInfo(_iter.m_pBarrelsInfo)
            , m_iterator(_iter.m_pBarrelsInfo->m_barrelInfos.begin())
        {
        }

        ~Iterator(void) 
        {
            m_pBarrelsInfo = NULL;
        }
    public:
        void reset() {m_iterator = m_pBarrelsInfo->m_barrelInfos.begin();}
        bool hasNext() const 
        {
            return (m_iterator != m_pBarrelsInfo->m_barrelInfos.end());
        }
        const BarrelInfo& next() 
        {
            const BarrelInfo& pBI = *(*m_iterator);
            m_iterator++;
            return pBI;
        }

        size_t size() const
        {
            return m_pBarrelsInfo->m_barrelInfos.size();
        }

    private:
        const BarrelsInfo* m_pBarrelsInfo;
        BarrelVector::const_iterator m_iterator;
    };

public:
    BarrelsInfo();
    BarrelsInfo(const BarrelsInfo& src);
    ~BarrelsInfo(void);

public:
    /**
     * Return last commit id
     */
    commitid_t getCommitId() const { return m_commitId;}

    /**
     * Set last commit id.
     */
    void setCommitId(commitid_t commitId) { m_commitId = commitId;}


    /**
     * get number of barrels
     *
     */
    size_t getBarrelCount() const;

    /**
     * get number of documents in all barrels
     *
     */
    df_t getDocCount() const;

    /**
     * get number of deleted documents
     * @return number of deleted documents
     *
     */
    df_t getDeletedDocCount() const;

    /**
     * Return the version of index database.
     * it will be "firtex.version.", where "version" is the 
     * version number of FirteX
     * @return version string
     *
     */
    const std::string& getIndexVersion() const {return m_sIndexVersion;}

    /**
     * Create a new name of barrel
     */
    BarrelInfo& newBarrel();

    /// Add a new barrel
    void addBarrel(const BarrelInfo& barrelInfo);

    /**
     * Return the last barrel
     */
    const BarrelInfo& getLastBarrel() const;
    BarrelInfo& getLastBarrel();

    /**
     * Return barrel by commit id
     */
    const BarrelInfo* getBarrel(commitid_t comId) const;

    /**
     * Determine whether a barrel with commId exists
     */
    bool hasBarrel(commitid_t comId) const;

    /**
     * Set encoding of documents in the index database
     * @param sEncodeName encode name, "GBK", "UTF-8", etc.
     */
    void setEncoding(const std::string& sEncodeName);

    /**
     * Return encoding of documents in the index database
     * @return encode name of documents in the index database
     */
    std::string getEncoding() const;

public:
    /**
     * Read the last committed barrels from file system
     *
     * @param pFileSys file system instance
     */
    void read(FX_NS(store)::FileSystemPtr& pFileSys);

    /**
     * Read specified committed barrels from file system
     *
     * @param pFileSys file system instance
     * @param the commit id of barrels 
     */
    void read(FX_NS(store)::FileSystemPtr& pFileSys, commitid_t commitId);

    /**
     * Write barrels information to file system
     * @param pFileSys file system instance
     */
    void write(FX_NS(store)::FileSystemPtr& pFileSys);

    /**
     * Remove all barrels from file system
     * @param pFileSys file system instance
     */
    void remove(FX_NS(store)::FileSystemPtr& pFileSys);

    /**
     * Clear barrels and release resources
     */
    void clear();

    /**
     * Clone a new instance
     */
    BarrelsInfo* clone() const;

public:
    const BarrelInfo& operator[] (size_t idx) const;

public:
    /**
     * Return iterator of document fields
     * @return iterator 
     * @see BarrelsInfo::Iterator
     */
    BarrelsInfo::Iterator iterator() const;

private:
    static FX_NS(thread)::FastMutex COMMIT_FILE_LOCK;

private:
    commitid_t m_commitId;
    std::string m_sIndexVersion;
    std::string m_sEncoding;

    BarrelVector m_barrelInfos;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(BarrelsInfo);

////////////////////////////////////////////////////
//BarrelInfo
inline void BarrelInfo::setDeletedDocs(df_t nDelDocCount)
{
    m_nDeletedDocCount = nDelDocCount;
}

inline BarrelInfo& BarrelInfo::operator = (const BarrelInfo& other)
{
    m_indexMeta = other.m_indexMeta;
    m_commitId = other.m_commitId;
    m_baseDocId = other.m_baseDocId;
    m_nDocCount = other.m_nDocCount;
    m_nDeletedDocCount = other.m_nDeletedDocCount;
    m_state = other.m_state;
    return *this;
}

inline IndexMeta& BarrelInfo::getIndexMeta()
{
    return m_indexMeta;
}

inline const IndexMeta& BarrelInfo::getIndexMeta() const
{
    return m_indexMeta;
}

////////////////////////////////////////////////////
//BarrelsInfo
inline size_t BarrelsInfo::getBarrelCount()const
{
    return m_barrelInfos.size();
}

inline BarrelInfo& BarrelsInfo::getLastBarrel()
{
    FIRTEX_ASSERT2 (m_barrelInfos.size() > 0);
    return *(m_barrelInfos[m_barrelInfos.size() - 1]);
}

inline const BarrelInfo& BarrelsInfo::getLastBarrel() const
{
    FIRTEX_ASSERT2 (m_barrelInfos.size() > 0);
    return *(m_barrelInfos[m_barrelInfos.size() - 1]);
}

inline const BarrelInfo& BarrelsInfo::operator[] (size_t idx) const 
{
    return *(m_barrelInfos[idx]);
}

inline void BarrelsInfo::setEncoding(const std::string& sEncodeName)
{
    m_sEncoding = sEncodeName;
}

inline std::string BarrelsInfo::getEncoding() const
{
    return m_sEncoding;
}

FX_NS_END

#endif
