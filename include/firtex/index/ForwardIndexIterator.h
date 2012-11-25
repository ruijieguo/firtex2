#ifndef __FX_FORWARDINDEXITERATOR_H
#define __FX_FORWARDINDEXITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class ForwardIndexReader;
class ForwardIndexIterator
{
public:
    virtual ~ForwardIndexIterator() {}
    
public:
	/**
     * Get forward index type
     */
    virtual fdindex_t getType() const = 0;

    /**
     * Size of the iterator
     */
    virtual size_t size() const = 0;

    /**
     * Add forward index reader of index barrel
     * @param pIter the const reference of iterator
     * @param baseDocId base document id of the index barrel
     */
    virtual void addReader(const ForwardIndexReader* pReader, docid_t baseDocId) = 0;

    /// Clone a new object
    virtual ForwardIndexIterator* clone() const = 0;
};

DEFINE_TYPED_PTR(ForwardIndexIterator);

FX_NS_END

#endif
