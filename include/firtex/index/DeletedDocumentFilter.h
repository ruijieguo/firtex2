#ifndef __FX_DELETED_DOCUMENT_FILTER_H
#define __FX_DELETED_DOCUMENT_FILTER_H

#include <map>
#include <vector>
#include <sstream>
#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/MergeInfo.h"
#include "firtex/utility/BitVector.h"
#include "firtex/store/FileSystem.h"

FX_NS_DEF(index);

class DeletedDocumentFilter
{
public:
    DeletedDocumentFilter(const FX_NS(store)::FileSystemPtr& pFileSys);
    DeletedDocumentFilter(const DeletedDocumentFilter& src);
    ~DeletedDocumentFilter();

public:
    /**
     * Open deleted document filter from deletion files 
     */
    void open(const BarrelsInfoPtr& pBarrelsInfo);

    /**
     * Refresh the deleted document filter if the deletion state 
     * has changed
     */
    void reopen(const BarrelsInfoPtr& pBarrelsInfo);

    /**
     * Append a new barrel to tail
     */
    void appendBarrel(const BarrelInfo& barrelInfo,
                      const FX_NS(utility)::BitVectorPtr& pDocFilter);
    
    /**
     * commit deletion to file system
     */
    void commit(const BarrelInfo& barrelInfo);

    /**
     * Flush dirty data and release resources.
     */
    void close();

    /**
     * Delete document by docId
     */
    bool deleteDocument(docid_t docId);

    /**
     * Undelete all deleted documents
     */
    void undeleteAll();

    /**
     * Return true if deletion has changed since the deletion was committed.
     */
    bool isDirty() const;

    /// Seal deletion
    void seal();

    /// Return true if sealed
    bool hasSealed() const;

public:
    /**
     * Merge multi index barrel to one barrel
     */
    void merge(const MergeInfos& mergeInfos);
    
public:
    /** 
     * Is document deleted or not
     * @param docId document Id
     */
    bool isDeleted(docid_t docId) const;

    /** 
     * Is any deletions
     * @return true if there is
     */
    bool hasDeletions() const;

    /**
     * Get number of deleted documents
     */
    df_t getDeletedDocCount() const;

    /**
     * Return max docid covered
     */
    docid_t getMaxDocId() const;

    /**
     * Return deleted document filter of index barrel by docid.
     */
    const FX_NS(utility)::BitVector* getDocFilter(docid_t docId) const;

    /**
     * Return deleted document filter of index barrel by docid.
     */
    const FX_NS(utility)::BitVector* operator [](size_t idx) const;

    /**
     * Clone a new instance
     */
    DeletedDocumentFilter* clone() const;

private:
    /**
     * Return deletion file name
     */
    std::string getDeletionFileName(const BarrelInfo& barrelInfo,
                                    commitid_t commitId) const;

    void loadLatestFilter(FX_NS(utility)::BitVectorPtr& pDocFilter,
                          const BarrelInfo& barrelInfo,
                          commitid_t beginCommitId,
                          commitid_t endCommitId);

private:
    struct BarrelFilter
    {
        BarrelFilter(const BarrelInfo* pBarrelInfo,
                     const FX_NS(utility)::BitVectorPtr& pFilter,
                     bool bDirty = false) 
            : m_pBarrelInfo(pBarrelInfo)
            , m_pFilter(pFilter)
            , m_bDirty(bDirty)
        {
        }

        BarrelFilter(const BarrelFilter& src)
            : m_pBarrelInfo(src.m_pBarrelInfo)
            , m_pFilter(src.m_pFilter)
            , m_bDirty(false)
        {
        }

        ~BarrelFilter() 
        {
        }

        const BarrelInfo* m_pBarrelInfo;
        FX_NS(utility)::BitVectorPtr m_pFilter;
        bool m_bDirty;
    };

    typedef std::vector<BarrelFilter> FilterVector;

private:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    BarrelsInfoPtr m_pBarrelsInfo;
    FilterVector m_filters;
    docid_t m_maxDocId;
    bool m_bSealed;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(DeletedDocumentFilter);

//////////////////////////////////////////
inline std::string DeletedDocumentFilter::getDeletionFileName(
        const BarrelInfo& barrelInfo, commitid_t commitId) const
{
    FIRTEX_ASSERT2(commitId >= barrelInfo.getCommitId());

    std::stringstream ss;
    std::string sBarrel = BarrelInfo::getSuffix(commitId);
    ss << BarrelDirectory::getFilePath(DELETION_FILENAME, 
            sBarrel);
    if (commitId != barrelInfo.getCommitId())
    {
        ss << "." << barrelInfo.getCommitId();
    }
    return ss.str();
}

inline docid_t DeletedDocumentFilter::getMaxDocId() const
{
    return m_maxDocId;
}

inline bool DeletedDocumentFilter::isDirty() const
{
    for (FilterVector::const_iterator it = m_filters.begin();
         it != m_filters.end(); ++it)
    {
        if ((*it).m_bDirty)
        {
            return true;
        }
    }
    return false;
}

inline const FX_NS(utility)::BitVector*
DeletedDocumentFilter::operator[] (size_t idx) const
{
    return m_filters[idx].m_pFilter.get();
}

inline void DeletedDocumentFilter::seal()
{
    m_bSealed = true;
}

inline bool DeletedDocumentFilter::hasSealed() const
{
    return m_bSealed;
}

FX_NS_END

#endif
