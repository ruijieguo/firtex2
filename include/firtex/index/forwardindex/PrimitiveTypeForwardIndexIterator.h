#ifndef __FX_PRIMITIVE_TYPE_FORWARD_INDEX_ITERATOR_H 
#define __FX_PRIMITIVE_TYPE_FORWARD_INDEX_ITERATOR_H 

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TypedForwardIndexIterator.h"

FX_NS_DEF(index);

typedef TypedForwardIndexIterator<int32_t> Int32ForwardIndexIterator;
DEFINE_TYPED_PTR(Int32ForwardIndexIterator);

typedef TypedForwardIndexIterator<int64_t> Int64ForwardIndexIterator;
DEFINE_TYPED_PTR(Int64ForwardIndexIterator);

typedef TypedForwardIndexIterator<uint32_t> UInt32ForwardIndexIterator;
DEFINE_TYPED_PTR(UInt32ForwardIndexIterator);

typedef TypedForwardIndexIterator<uint64_t> UInt64ForwardIndexIterator;
DEFINE_TYPED_PTR(UInt64ForwardIndexIterator);

typedef TypedForwardIndexIterator<float> FloatForwardIndexIterator;
DEFINE_TYPED_PTR(FloatForwardIndexIterator);

typedef TypedForwardIndexIterator<double> DoubleForwardIndexIterator;
DEFINE_TYPED_PTR(DoubleForwardIndexIterator);

FX_NS_END

#endif
