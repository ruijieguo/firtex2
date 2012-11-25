#ifndef __FX_PRIMITIVE_TYPE_FORWARD_INDEX_WRITER_H 
#define __FX_PRIMITIVE_TYPE_FORWARD_INDEX_WRITER_H 

#include <stdlib.h>
#include "firtex/common/StdHeader.h"
#include "firtex/index/ForwardIndexWriter.h"
#include "firtex/index/TypedForwardIndexIterator.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/utility/SlicedVector.h"
#include "firtex/index/BarrelDirectory.h"

FX_NS_DEF(index);

template<typename T>
class PrimitiveTypeForwardIndexWriter : public ForwardIndexWriter
{
    template <typename T1>
    struct IteratorTraits
    {
        typedef PrimitiveTypeForwardIndexWriter<T1> ImplType;
    };

public:
    typedef T ValueType;
    typedef TypedForwardIndexIterator<T, IteratorTraits<T> > IteratorType;

public:
    PrimitiveTypeForwardIndexWriter(fdindex_t type);
    ~PrimitiveTypeForwardIndexWriter();

public:
    /**
     * Add field to build forward index
     */
    void addField(docid_t docId, const FX_NS(document)::AnalyzedField* pField);

    /**
     * Seal the forward index . After sealing, modification is forbidden
     * 
     * @param lastDocId the last docid in the barrel
     */
    void seal(docid_t lastDocId);

    /**
     * Finalize forward index and commit data to file system
     * @param pFileSys file system instance
     * @param sFileType the file type (norm or forward index)
     * @param sSuffix suffix of index barrel
     * @return length of forward index file
     */
    int64_t commit(FX_NS(store)::FileSystemPtr& pFileSys,
                   const std::string& sFileType,
                   const std::string& sSuffix);

    inline void setValue(docid_t docId, const T& value);

    /// Return value by docid
    inline const T& getValue(docid_t docId) const;

    /// Return number of values
    inline size_t size() const {return m_pValues->size();}

    /**
     * Create an iterator of this forward index.
     *
     * @return forward index iterator. it is efficient if you access
     * value sequentially
     */
    virtual ForwardIndexIteratorPtr iterator() const;

    /**
     * Get forward index type
     */
    virtual fdindex_t getType() const;

private:
    void strToNumber(const char* str, T& value);

private:
    typedef FX_NS(utility)::SlicedVector<T> Slice;
    DEFINE_TYPED_PTR(Slice);

    fdindex_t m_fdIndexType;
    SlicePtr m_pValues;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////////////
SETUP_STREAM_LOGGER_TEMP(index, PrimitiveTypeForwardIndexWriter);

template <typename T>
PrimitiveTypeForwardIndexWriter<T>::PrimitiveTypeForwardIndexWriter(fdindex_t type)
    : m_fdIndexType(type)
    , m_pValues(new Slice(1024, 100))
{
}

template <typename T>
PrimitiveTypeForwardIndexWriter<T>::~PrimitiveTypeForwardIndexWriter()
{
}

template <typename T>
void PrimitiveTypeForwardIndexWriter<T>::addField(
        docid_t docId, const FX_NS(document)::AnalyzedField* pField)
{
    FX_LOG(TRACE, "Add field: docId[%d]", docId);

    T value = 0;
    const FX_NS(document)::AnalyzedField::TokenView* pTokenView = 
        pField->getTokenView();
    if (pTokenView && pTokenView->getNumTokens() > 0)
    {
        FX_NS(document)::AnalyzedField::TokenView::Iterator it =
            pTokenView->iterator();
        const FX_NS(analyzer)::Token& token = it.next();
        strToNumber(token.getTextValue(), value);
    }
    else
    {
        const FX_NS(document)::Field* pRawField = pField->getField();
        if (!pRawField)
        {
            FIRTEX_THROW(RuntimeException, "Field for forward index is NULL.");
        }
        strToNumber(pRawField->getValue().c_str(), value);
    }
    setValue(docId, value);
}

template <typename T>
inline void PrimitiveTypeForwardIndexWriter<T>::setValue(
        docid_t docId, const T& value)
{
    m_pValues->set((size_t)docId, value);
}

template <typename T>
inline const T& PrimitiveTypeForwardIndexWriter<T>::getValue(docid_t docId) const
{
    FIRTEX_ASSERT2(docId >= 0 && docId < (docid_t)m_pValues->size());
    return (*m_pValues)[(size_t)docId];
}

template <typename T>
void PrimitiveTypeForwardIndexWriter<T>::seal(docid_t lastDocId)
{
    if (lastDocId >= 0 && (size_t)(lastDocId + 1) > m_pValues->size())
    {
        m_pValues->set((size_t)lastDocId, (const T&)0);
    }
}

template <typename T>
int64_t PrimitiveTypeForwardIndexWriter<T>::commit(
        FX_NS(store)::FileSystemPtr& pFileSys,
        const std::string& sFileType,
        const std::string& sSuffix)
{
    std::string sFileName = BarrelDirectory::getFilePath(getFieldSchema()->getName(),
            sFileType, sSuffix);
    FX_NS(store)::OutputStreamPtr pOutStream = pFileSys->createFile(sFileName);
    offset_t off = pOutStream->tell();
    m_pValues->save(pOutStream);
    return int64_t(pOutStream->tell() - off);
}

template <typename T>
ForwardIndexIteratorPtr PrimitiveTypeForwardIndexWriter<T>::iterator() const
{
    return ForwardIndexIteratorPtr(new IteratorType(this));
}

template <typename T>
fdindex_t PrimitiveTypeForwardIndexWriter<T>::getType() const
{
    return m_fdIndexType;
}

template <typename T>
void PrimitiveTypeForwardIndexWriter<T>::strToNumber(const char* str, T& value)
{
    FIRTEX_ASSERT2(false);
}

template <>
inline void PrimitiveTypeForwardIndexWriter<int32_t>::strToNumber(
        const char* str, int32_t& value)
{
    char* end = NULL;
    value = (int32_t)strtol(str, &end, 10);
}

template <>
inline void PrimitiveTypeForwardIndexWriter<int64_t>::strToNumber(
        const char* str, int64_t& value)
{
    char* end = NULL;
    value = (int64_t)STRTOLL(str, &end, 10);
}

template <>
inline void PrimitiveTypeForwardIndexWriter<uint32_t>::strToNumber(
        const char* str, uint32_t& value)
{
    char* end = NULL;
    value = (uint32_t)strtoul(str, &end, 10);
}

template <>
inline void PrimitiveTypeForwardIndexWriter<uint64_t>::strToNumber(
        const char* str, uint64_t& value)
{
    char* end = NULL;
    value = (uint64_t)STRTOULL(str, &end, 10);
}

template <>
inline void PrimitiveTypeForwardIndexWriter<float>::strToNumber(
        const char* str, float& value)
{
    char* end = NULL;
    value = (float)strtod(str, &end);	
}

template <>
inline void PrimitiveTypeForwardIndexWriter<double>::strToNumber(
        const char* str, double& value)
{
    char* end = NULL;
    value = (double)strtod(str, &end);
}

template <>
inline void PrimitiveTypeForwardIndexWriter<int8_t>::strToNumber(
        const char* str, int8_t& value)
{
    char* end = NULL;
    value = (int8_t)strtol(str, &end, 10);
}

FX_NS_END

#endif
