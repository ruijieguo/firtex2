#ifndef __FX_SLICED_VECTOR_H
#define __FX_SLICED_VECTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/MiscUtil.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/SimpleAllocator.h"
#include "firtex/store/OutputStream.h"
#include "firtex/store/InputStream.h"
#include <vector>

FX_NS_DEF(utility);

template <typename T, typename Allocator = SimpleAllocator>
class SlicedVector
{
public:
    typedef T ValueType;
    typedef Allocator AllocatorType;

    const static size_t DEFAULT_SLICE_SIZE = 1024;

public:
    SlicedVector(AllocatorType* pAllocator = NULL);
    SlicedVector(size_t nSliceSize, size_t nNumSlice, AllocatorType* pAllocator = NULL);
    ~SlicedVector();

public:
    /**
     * Save vector to output stream
     */
    void save(FX_NS(store)::OutputStreamPtr& pOutStream);

    /**
     * load vector from input stream
     */
    void load(FX_NS(store)::InputStreamPtr& pInStream);

    /**
     * Push one element at back
     */
    void pushBack(const T& value);
    
    /**
     * Set element at specified position
     */
    void set(size_t off, const T& value);

    /**
     * Size of the vector
     */
    size_t size() const {return m_nSize;}

    /**
     * Return one slice by index
     * 
     * @param off the element position
     * @param nSliceSize the slice size
     */
    const T* getSlice(size_t off, size_t& nSliceSlize);

    /**
     * Slice size of the vector
     */
    size_t getSliceSize() const {return m_nSliceSize;}

    /**
     * Get element at specified position
     */
    const T& operator[] (size_t off) const;
    T& operator[] (size_t off);

private:
    T* getOrAllocateSlice(size_t nSliceOff);

    inline size_t getNextSize(size_t size) const;

private:
    typedef std::vector<ValueType*> SliceVector;

    AllocatorType* m_pAllocator;
    bool m_bOwnPool;

    SliceVector m_slices;
    size_t m_nSize;
    size_t m_nSliceSize;
    size_t m_nLog2OfSliceSize;

private:
    DECLARE_STREAM_LOGGER();
};

SETUP_STREAM_LOGGER_TEMP2(utility, SlicedVector);

///////////////////////////////////////////////////////////
//
template <typename T, typename Allocator>
SlicedVector<T, Allocator>::SlicedVector(AllocatorType* pAllocator)
    : m_bOwnPool(false)
    , m_nSize(0)
    , m_nSliceSize(DEFAULT_SLICE_SIZE)
    , m_nLog2OfSliceSize(0)
{
    m_pAllocator = pAllocator;

    if (!m_pAllocator)
    {
        m_pAllocator = new SimpleAllocator();//nSliceSize * sizeof(ValueType) * nNumSlice);
        m_bOwnPool = true;
    }
}

template <typename T, typename Allocator>
SlicedVector<T, Allocator>::SlicedVector(size_t nSliceSize,
        size_t nNumSlice, AllocatorType* pAllocator)
    : m_bOwnPool(false)
    , m_nSize(0)
{
    m_pAllocator = pAllocator;

    if (!m_pAllocator)
    {
        m_pAllocator = new SimpleAllocator();//nSliceSize * sizeof(ValueType) * nNumSlice);
        m_bOwnPool = true;
    }
 
    m_nLog2OfSliceSize = (size_t)MiscUtil::log2UpperBound((uint32_t)nSliceSize);
    m_nSliceSize = (size_t)MiscUtil::power2((uint32_t)m_nLog2OfSliceSize);

    m_slices.resize(nNumSlice, NULL);
    for (size_t i = 0; i < m_slices.size(); ++i)
    {
        void* p = m_pAllocator->allocate(m_nSliceSize * sizeof(ValueType));
        m_slices[i] = (ValueType*)p;
    }
}

template <typename T, typename Allocator>
SlicedVector<T, Allocator>::~SlicedVector()
{
    if (m_pAllocator && m_bOwnPool)
    {
        delete m_pAllocator;
    }
    m_pAllocator = NULL;
    m_slices.clear();
}

template <typename T, typename Allocator>
void SlicedVector<T, Allocator>::save(FX_NS(store)::OutputStreamPtr& pOutStream)
{
    FX_LOG(TRACE, "totlal slice size: [%lld]", (int64_t)m_nSize);

    size_t i = 0;
    for (i = 0; i < m_nSize/m_nSliceSize; ++i)
    {
        pOutStream->write((const void*)(m_slices[i]), 
                          m_nSliceSize * sizeof(ValueType));
    }
    size_t left = (m_nSize % m_nSliceSize);
    if (left != 0)
    {
        pOutStream->write((const void*)(m_slices[i]),
                          left * sizeof(ValueType));
    }
}

template <typename T, typename Allocator>
void SlicedVector<T, Allocator>::load(FX_NS(store)::InputStreamPtr& pInStream)
{
    m_nSize = (size_t)(pInStream->getSize()/sizeof(ValueType));

    m_nLog2OfSliceSize = (size_t)MiscUtil::log2UpperBound((uint32_t)m_nSliceSize);

    m_slices.resize((m_nSize + m_nSliceSize - 1) / m_nSliceSize, NULL);
    int32_t i;
    for (i = 0; i < (int32_t)m_slices.size() - 1; ++i)
    {
        void* p = m_pAllocator->allocate(m_nSliceSize * sizeof(ValueType));
        pInStream->read(p, m_nSliceSize * sizeof(ValueType));
        m_slices[i] = (ValueType*)p;
    }
    size_t left = (m_nSize % m_nSliceSize);
    if (left == 0)
    {
        left = m_nSliceSize;
    }
    void* p = m_pAllocator->allocate(left * sizeof(ValueType));
    pInStream->read(p, left * sizeof(ValueType));
    m_slices[i] = (ValueType*)p;
}

template <typename T, typename Allocator>
void SlicedVector<T, Allocator>::pushBack(const T& value)
{
    size_t off = m_nSize;
    size_t sliceOff = off >> m_nLog2OfSliceSize;
    size_t offset = off & (m_nSliceSize - 1);

    T* pSlice = getOrAllocateSlice(sliceOff);
    pSlice[offset] = value;
    m_nSize++;
}

template <typename T, typename Allocator>
void SlicedVector<T, Allocator>::set(size_t off, const T& value)
{
    FX_LOG(TRACE, "set: offset: [%u]", (int32_t)off);
    size_t sliceOff = off >> m_nLog2OfSliceSize;
    size_t offset = off & (m_nSliceSize - 1);

    T* pSlice = getOrAllocateSlice(sliceOff);
    pSlice[offset] = value;

    if (off >= m_nSize)
    {
        m_nSize = off + 1;
    }
}

template <typename T, typename Allocator>
inline const T& SlicedVector<T, Allocator>::operator[] (size_t off) const
{
    FIRTEX_ASSERT2(off < m_nSize);

    size_t sliceOff = (off >> m_nLog2OfSliceSize);
    size_t offset = (off & (m_nSliceSize - 1));

    return m_slices[sliceOff][offset];
}

template <typename T, typename Allocator>
inline T& SlicedVector<T, Allocator>::operator[] (size_t off)
{
    FIRTEX_ASSERT2(off < m_nSize);
    size_t sliceOff = (off >> m_nLog2OfSliceSize);
    size_t offset = (off & (m_nSliceSize - 1));

    return m_slices[sliceOff][offset];
}

template <typename T, typename Allocator>
const T* SlicedVector<T, Allocator>::getSlice(size_t off, size_t& nSliceSlize)
{
    if (off >= m_nSize)
    {
        return NULL;
    }

    size_t sliceOff = (off >> m_nLog2OfSliceSize);
    size_t offset = (off & (m_nSliceSize - 1));
    nSliceSlize = m_nSliceSize - offset;
    return m_slices[sliceOff] + offset;
}

template <typename T, typename Allocator>
T* SlicedVector<T, Allocator>::getOrAllocateSlice(size_t nSliceOff)
{
    if (nSliceOff < m_slices.size())
    {
        return m_slices[nSliceOff];
    }

    size_t i = m_slices.size();
    m_slices.resize(getNextSize(nSliceOff), NULL);
    for (; i < m_slices.size(); ++i)
    {
        void* p = m_pAllocator->allocate(m_nSliceSize * sizeof(ValueType));
        m_slices[i] = (ValueType*)p;
    }
    return m_slices[nSliceOff];
}

template <typename T, typename Allocator>
inline size_t SlicedVector<T, Allocator>::getNextSize(size_t size) const
{
    size_t newSize = (size << 1);
    return newSize;
}

FX_NS_END

#endif
