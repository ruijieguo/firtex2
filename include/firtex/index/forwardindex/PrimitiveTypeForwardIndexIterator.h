#ifndef __FX_PRIMITIVE_TYPE_FORWARD_INDEX_ITERATOR_H 
#define __FX_PRIMITIVE_TYPE_FORWARD_INDEX_ITERATOR_H 

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TypedForwardIndexIterator.h"

FX_NS_DEF(index);

typedef TypedForwardIndexIterator<int32_t> Int32ForwardIndexIterator;
typedef FX_NS(common)::SharedPtr<Int32ForwardIndexIterator> Int32ForwardIndexIteratorPtr;

typedef TypedForwardIndexIterator<int64_t> Int64ForwardIndexIterator;
typedef FX_NS(common)::SharedPtr<Int64ForwardIndexIterator> Int64ForwardIndexIteratorPtr;

typedef TypedForwardIndexIterator<uint32_t> UInt32ForwardIndexIterator;
typedef FX_NS(common)::SharedPtr<UInt32ForwardIndexIterator> UInt32ForwardIndexIteratorPtr;

typedef TypedForwardIndexIterator<uint64_t> UInt64ForwardIndexIterator;
typedef FX_NS(common)::SharedPtr<UInt64ForwardIndexIterator> UInt64ForwardIndexIteratorPtr;

typedef TypedForwardIndexIterator<float> FloatForwardIndexIterator;
typedef FX_NS(common)::SharedPtr<FloatForwardIndexIterator> FloatForwardIndexIteratorPtr;

typedef TypedForwardIndexIterator<double> DoubleForwardIndexIterator;
typedef FX_NS(common)::SharedPtr<DoubleForwardIndexIterator> DoubleForwardIndexIteratorPtr;

FX_NS_END

#endif
