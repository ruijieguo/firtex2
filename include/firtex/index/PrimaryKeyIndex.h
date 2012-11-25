#ifndef __FX_PRIMARY_KEY_INDEX_H
#define __FX_PRIMARY_KEY_INDEX_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Notifiable.h"
#include "firtex/index/primarykey/PrimaryKeyTermReader.h"
#include "firtex/index/MultiTermReader.h"
#include "firtex/store/FileSystem.h"
#include "firtex/utility/HashMap.h"
#include "firtex/utility/Hash.h"
#include <vector>
#include <string>
#include <map>

FX_NS_DEF(index);

class PrimaryKeyIndex
{
public:
    typedef FX_NS(utility)::HashMap<uint64_t, docid_t> HashTable;
    typedef FX_NS(common)::SharedPtr<HashTable> HashTablePtr;
    
public:
    PrimaryKeyIndex(const tstring& sPrimKeyField);
    PrimaryKeyIndex(const PrimaryKeyIndex& src);
    ~PrimaryKeyIndex();

public:
    /**
     * Load primary key index
     * @param pBarrelsInfo barrels info
     * @throw FileIOException if load failed.
     */
    void open(const FX_NS(store)::FileSystemPtr& pFileSys,
              const BarrelsInfoPtr& pBarrelsInfo,
              const HashTablePtr& pHashTable = HashTablePtr());

    /**
     * Refresh the primary key index
     */
    void reopen(const BarrelsInfoPtr& pBarrelsInfo,
                const HashTablePtr& pHashTable = HashTablePtr());

    /// Clone a new object
    PrimaryKeyIndex* clone() const;

    /// Merge an hash map
    void mergeWith(docid_t baseDocId, HashTablePtr pHashMap);

public:
    /// Add a primary key pair
    inline void addPrimaryKey(const char* szPk, docid_t docId);

    /**
     * Lookup docId by primary key
     */
    inline docid_t lookup(const std::string& sPrimKey) const;
    inline docid_t lookup(const char* szPrimKey) const;

    /// Replace a primary key with new value
    inline void replace(const char* szPrimKey, docid_t docId);

    /// Return primary key field name
    inline const std::string& getPrimKeyField() const;
    
    /// Return internal hash map
    inline const HashTablePtr& getHashMap() const;

private:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    FX_NS(store)::InputStreamPoolPtr m_pStreamPool;
    BarrelsInfoPtr m_pBarrelsInfo;
    tstring m_sPrimKeyField;
    HashTablePtr m_pHashTable;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PrimaryKeyIndex);

//////////////////////////////////////////////////
inline docid_t PrimaryKeyIndex::lookup(const std::string& sPrimKey) const
{
    return lookup(sPrimKey.c_str());
}

inline docid_t PrimaryKeyIndex::lookup(const char* szPrimKey) const
{
    uint64_t hashValue = FX_NS(utility)::Hash::hashString64(szPrimKey);
    return m_pHashTable->find(hashValue);
}

inline const std::string& PrimaryKeyIndex::getPrimKeyField() const 
{
    return m_sPrimKeyField;
}

inline const PrimaryKeyIndex::HashTablePtr& PrimaryKeyIndex::getHashMap() const
{
    return m_pHashTable;
}

inline void PrimaryKeyIndex::addPrimaryKey(const char* szPk, docid_t docId)
{
    uint64_t hashValue = FX_NS(utility)::Hash::hashString64(szPk);
    m_pHashTable->insert(hashValue, docId);
}

inline void PrimaryKeyIndex::replace(const char* szPrimKey, docid_t docId)
{
    uint64_t hashValue = FX_NS(utility)::Hash::hashString64(szPrimKey);
    m_pHashTable->replace(hashValue, docId);
}

FX_NS_END

#endif
