#ifndef __FX_PRIMITIVE_TYPE_FORWARD_INDEX_READER_MMAPIMPL_H 
#define __FX_PRIMITIVE_TYPE_FORWARD_INDEX_READER_MMAPIMPL_H 

#include "firtex/common/StdHeader.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexReader.h"
#include "firtex/index/BarrelDirectory.h"
#include "firtex/store/FileSystem.h"

FX_NS_DEF(index);

template <typename T>
class PrimitiveTypeForwardIndexReaderMMapImpl : public PrimitiveTypeForwardIndexReader<T>
{
public:
    typedef T ValueType;
    typedef PrimitiveTypeForwardIndexReaderMMapImpl<T> ThisType;
    
public:
    PrimitiveTypeForwardIndexReaderMMapImpl(fdindex_t type);
    ~PrimitiveTypeForwardIndexReaderMMapImpl();

public:
    /**
     * Open forward index from file system
     * @param pFileSys file system instance
     * @param pFieldSchema field schema
     * @param sFileType the file type (norm or forward index)
     * @param sSuffix prefix of forward index file
     */
    void open(FX_NS(store)::FileSystemPtr& pFileSys,
              const FX_NS(document)::FieldSchema* pFieldSchema,
              const std::string& sFileType,
              const std::string& sSuffix);

    /**
     * Return forward index iterator
     */
    ForwardIndexIteratorPtr iterator() const;

    /**
     * Return size of the forward index
     */
    size_t size() const;

    /**
     * Load one block into
     * 
     * @param docIdFrom the target docid
     * @param pValues, the loaded values buffer
     * @param docIdTo upper bound doc id of loaded valudes
     * @return false if failure
     */
    virtual bool loadRange(docid_t docIdFrom, const T*& pValues,
                           docid_t& docIdTo) const;

private:
    FX_NS(store)::FileSystem::MMapFilePtr m_mmapFile;
    const ValueType* m_pMMapValues;
    size_t m_nSize;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////////////
SETUP_STREAM_LOGGER_TEMP(index, PrimitiveTypeForwardIndexReaderMMapImpl);

#include "firtex/index/TypedForwardIndexIterator.h"

template <typename T>
PrimitiveTypeForwardIndexReaderMMapImpl<T>::PrimitiveTypeForwardIndexReaderMMapImpl(fdindex_t type)
    : PrimitiveTypeForwardIndexReader<T>(type)
    , m_pMMapValues(NULL)
    , m_nSize(0)
{
}

template <typename T>
PrimitiveTypeForwardIndexReaderMMapImpl<T>::~PrimitiveTypeForwardIndexReaderMMapImpl()
{
    m_mmapFile.reset();
}

template <typename T>
void PrimitiveTypeForwardIndexReaderMMapImpl<T>::open(
        FX_NS(store)::FileSystemPtr& pFileSys,
        const FX_NS(document)::FieldSchema* pFieldSchema,
        const std::string& sFileType,
        const std::string& sSuffix)
{
    std::string sFileName = BarrelDirectory::getFilePath(pFieldSchema->getName(),
            sFileType, sSuffix);
    m_mmapFile = pFileSys->mmapFile(sFileName);

    m_pMMapValues = (ValueType*)m_mmapFile->begin();
    m_nSize = size_t(m_mmapFile->length()/sizeof(ValueType));
}

template <typename T>
ForwardIndexIteratorPtr PrimitiveTypeForwardIndexReaderMMapImpl<T>::iterator() const
{
    return ForwardIndexIteratorPtr(new TypedForwardIndexIterator<T>(this));
}

template <typename T>
inline size_t PrimitiveTypeForwardIndexReaderMMapImpl<T>::size() const
{
    return m_nSize;
}

template <typename T>
bool PrimitiveTypeForwardIndexReaderMMapImpl<T>::loadRange(docid_t docIdFrom,
        const T*& pValues, docid_t& docIdTo) const
{
    if (docIdFrom < 0 || docIdFrom >= (docid_t)m_nSize)
    {
        return false;
    }
    pValues = m_pMMapValues;
    docIdTo = (docid_t)(m_nSize - 1);
    return true;
}

FX_NS_END

#endif
