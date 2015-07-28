#ifndef __FX_TYPEDFORWARDINDEXITERATOR_H
#define __FX_TYPEDFORWARDINDEXITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/ForwardIndexCreator.h"
#include "firtex/index/ForwardIndexIterator.h"
#include "firtex/index/ForwardIndexIteratorTraits.h"
#include "firtex/index/ForwardIndexReader.h"
#include <map>
#include <vector>

FX_NS_DEF(index);

template <typename T, typename Traits = ForwardIndexIteratorTraits<T> >
class TypedForwardIndexIterator : public ForwardIndexIterator
{
public:
    typedef typename Traits::ImplType ImplType;
    typedef T ValueType;
    typedef TypedForwardIndexIterator<T, Traits> ThisType;
    DEFINE_TYPED_PTR(ThisType);

public:
    TypedForwardIndexIterator(const ImplType* pImpl);
    TypedForwardIndexIterator(const TypedForwardIndexIterator<T, Traits>& src);
    virtual ~TypedForwardIndexIterator();

public:
    /**
     * Get forward index type
     */
    fdindex_t getType() const;

    /**
     * Size of the iterator
     */
    size_t size() const;

    /**
     * Add forward index reader of index barrel
     * @param pIter the const reference of iterator
     * @param baseDocId base document id of the index barrel
     */
    void addReader(const ForwardIndexReader* pImpl, docid_t baseDocId);

    /**
     * Reset self to initial state
     */
    void reset();

    /**
     * Move to next iterator
     */
    bool next();

    /**
     * Navigate the forward index value. Sequential navigation is efficient
     * @param docId global document Id
     * @param value forward index value returned
     */
    bool seek(docid_t docId, ValueType& value);

    /// Clone a new object
    virtual ForwardIndexIterator* clone() const;

private:
    struct ReaderEntry
    {
        ReaderEntry(const ImplType* pImpl, docid_t base, docid_t max)
            : reader(pImpl), baseDocId(base), maxDocId(max)
        {}
        ReaderEntry(const ReaderEntry& src)
            : reader(src.reader)
            , baseDocId(src.baseDocId)
            , maxDocId(src.maxDocId)
        {}

        const ImplType* reader;
        docid_t baseDocId;
        docid_t maxDocId;
    };

    typedef std::vector<ReaderEntry> ReaderVector;

    fdindex_t m_fdIndexType;

    ReaderVector m_readers;
    size_t m_nCurReader;

    const ReaderEntry* m_pCurReader;
};

/////////////////////////////////////////////////
//
template <typename T, typename Traits>
TypedForwardIndexIterator<T, Traits>::TypedForwardIndexIterator(const ImplType* pImpl)
    : m_fdIndexType(FdIndexType::UNKNOWN)
    , m_nCurReader((size_t)-1)
    , m_pCurReader(NULL)
{
    addReader(pImpl, 0);
}

template <typename T, typename Traits>
TypedForwardIndexIterator<T, Traits>::TypedForwardIndexIterator(
        const TypedForwardIndexIterator<T, Traits>& src)
    : m_fdIndexType(src.m_fdIndexType)
    , m_readers(src.m_readers)
    , m_nCurReader((size_t)-1)
    , m_pCurReader(NULL)
{
}

template <typename T, typename Traits>
TypedForwardIndexIterator<T, Traits>::~TypedForwardIndexIterator()
{
}

template <typename T, typename Traits>
void TypedForwardIndexIterator<T, Traits>::addReader(
        const ForwardIndexReader* pImpl, docid_t baseDocId)
{
    FIRTEX_ASSERT2(pImpl != NULL);

    const ImplType* pTypedReader = dynamic_cast<const ImplType*>(pImpl);
    if (!pTypedReader)
    {
        FIRTEX_THROW(CastException, "Cast ForwardIndexIterator FAILED.");
    }

    //FIRTEX_ASSERT2(pTypedReader->size() > 0);
    if (pTypedReader->size() == 0)
    {
        return;
    }

    docid_t curBaseDocId = 0;
    docid_t curMaxDocId = 0;
    if (m_readers.size() > 0)
    {
        if (m_fdIndexType != pImpl->getType())
        {
            FIRTEX_THROW(InconsisitentException,
                         _T("Forward index type is inconsisitent."));
        }
        const ReaderEntry& entry = (*m_readers.rbegin());
        curBaseDocId = entry.maxDocId + 1; 
        if (curBaseDocId != baseDocId)
        {
            FIRTEX_THROW(InconsisitentException,
                         _T("Base docId of forward index is inconsisitent."));
        }
        curMaxDocId =  curBaseDocId + (docid_t)pTypedReader->size() - 1;
    }
    else 
    {
        m_fdIndexType = pImpl->getType();
        curMaxDocId = (docid_t)pTypedReader->size() - 1;
    }
    m_readers.push_back(ReaderEntry(pTypedReader, curBaseDocId, curMaxDocId));
    m_pCurReader = &(*(m_readers.begin()));
    m_nCurReader = 0;
}

template <typename T, typename Traits>
fdindex_t TypedForwardIndexIterator<T, Traits>::getType() const
{
    return m_fdIndexType;
}

template <typename T, typename Traits>
size_t TypedForwardIndexIterator<T, Traits>::size() const
{
    return (size_t)(m_readers.size() > 0) ? ((*m_readers.rbegin()).maxDocId + 1) : 0;
}

template <typename T, typename Traits>
inline void TypedForwardIndexIterator<T, Traits>::reset()
{
    m_nCurReader = (size_t)-1;
}

template <typename T, typename Traits>
inline bool TypedForwardIndexIterator<T, Traits>::next()
{
    if (m_nCurReader + 1 >= m_readers.size())
    {
        return false;
    }
    ++m_nCurReader;
    m_pCurReader = &(m_readers[m_nCurReader]);
    return true;
}

template <typename T, typename Traits>
bool TypedForwardIndexIterator<T, Traits>::seek(docid_t docId, ValueType& value)
{
    FIRTEX_ASSERT2(m_pCurReader != NULL);

    if (docId < m_pCurReader->baseDocId)
    {
        reset();
    }

    while (docId > m_pCurReader->maxDocId)
    {
        if (!next())
        {
            return false;
        }
    }
    value = m_pCurReader->reader->getValue(docId - m_pCurReader->baseDocId);
    return true;
}

template <typename T, typename Traits>
ForwardIndexIterator* TypedForwardIndexIterator<T, Traits>::clone() const
{
    return new TypedForwardIndexIterator<T, Traits>(*this);
}

FX_NS_END

#endif
