#ifndef __FX_PRIMITIVE_TYPE_FORWARD_INDEX_READER_IMPL_H 
#define __FX_PRIMITIVE_TYPE_FORWARD_INDEX_READER_IMPL_H 

#include "firtex/common/StdHeader.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/utility/SlicedVector.h"
#include "firtex/index/PrimitiveTypeForwardIndexReader.h"
#include "firtex/index/TypedForwardIndexIterator.h"
#include "firtex/index/BarrelDirectory.h"
#include "firtex/store/InputStream.h"

FX_NS_DEF(index);

template<typename T>
class PrimitiveTypeForwardIndexReaderImpl : public PrimitiveTypeForwardIndexReader<T>
{
public:
    typedef T ValueType;

public:
    PrimitiveTypeForwardIndexReaderImpl(fdindex_t type);
    ~PrimitiveTypeForwardIndexReaderImpl();

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

public:
    /**
     * Return forward index iterator
     */
    virtual ForwardIndexIteratorPtr iterator() const;

    /// Return the total size of the forward index
    virtual size_t size() const;

    /**
     * Get forward index type
     */
    virtual fdindex_t getType() const {return m_fdIndexType;}

protected:
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
    typedef FX_NS(utility)::SlicedVector<T> Slice;
    DEFINE_TYPED_PTR(Slice);

    SlicePtr m_pSlices;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////////////
SETUP_STREAM_LOGGER_TEMP(index, PrimitiveTypeForwardIndexReaderImpl);

template <typename T>
PrimitiveTypeForwardIndexReaderImpl<T>::PrimitiveTypeForwardIndexReaderImpl(fdindex_t type)
    : PrimitiveTypeForwardIndexReader<T>(type)
{
}

template <typename T>
PrimitiveTypeForwardIndexReaderImpl<T>::~PrimitiveTypeForwardIndexReaderImpl()
{
}

template <typename T>
void PrimitiveTypeForwardIndexReaderImpl<T>::open(
        FX_NS(store)::FileSystemPtr& pFileSys,
        const FX_NS(document)::FieldSchema* pFieldSchema,
        const std::string& sFileType,
        const std::string& sSuffix)
{
    std::string sFileName = BarrelDirectory::getFilePath(pFieldSchema->getName(),
            sFileType, sSuffix);
    FX_NS(store)::InputStreamPtr pInStream = pFileSys->openFile(sFileName);
    m_pSlices = new Slice();
    m_pSlices->load(pInStream);
}

template <typename T>
ForwardIndexIteratorPtr PrimitiveTypeForwardIndexReaderImpl<T>::iterator() const
{
    return ForwardIndexIteratorPtr(new TypedForwardIndexIterator<T>(this));
}

template <typename T>
inline size_t PrimitiveTypeForwardIndexReaderImpl<T>::size() const
{
    return m_pSlices->size();
}

template <typename T>
bool PrimitiveTypeForwardIndexReaderImpl<T>::loadRange(docid_t docIdFrom,
        const T*& pValues, docid_t& docIdTo) const
{
    size_t nSize = 0;
    const T* pSlice = m_pSlices->getSlice((size_t)docIdFrom, nSize);
    if (!pSlice)
    {
        return false;
    }

    pValues = pSlice;
    docIdTo = docIdFrom + (docid_t)nSize;
    return true;
}

FX_NS_END

#endif
