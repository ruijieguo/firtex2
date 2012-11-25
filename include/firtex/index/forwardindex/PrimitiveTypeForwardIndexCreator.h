#ifndef __FX_PRIMITIVE_TYPE_FORWARD_INDEX_CREATOR_H
#define __FX_PRIMITIVE_TYPE_FORWARD_INDEX_CREATOR_H

#include "firtex/utility/Singleton.h"
#include "firtex/index/ForwardIndexCreator.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexWriter.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexReader.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexReaderMMapImpl.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexMerger.h"

FX_NS_DEF(index);

template <typename T>
class PrimitiveTypeForwardIndexCreator : public ForwardIndexCreator
                                       , public FX_NS(utility)::Singleton<PrimitiveTypeForwardIndexCreator<T> >
{
public:
    PrimitiveTypeForwardIndexCreator()
    {
    }

    ForwardIndexWriter* createWriter() const;
    ForwardIndexReader* createReader() const;
    ForwardIndexMerger* createMerger() const;
};

/////////////////////////////////////////////
////
template <>
PrimitiveTypeForwardIndexCreator<int32_t>::PrimitiveTypeForwardIndexCreator()
{
    FORWARD_INDEX_DEFINE(FdIndexType::INT32, "int32");
}

template <>
PrimitiveTypeForwardIndexCreator<int64_t>::PrimitiveTypeForwardIndexCreator()
{
    FORWARD_INDEX_DEFINE(FdIndexType::INT64, "int64");
}

template <>
PrimitiveTypeForwardIndexCreator<uint32_t>::PrimitiveTypeForwardIndexCreator()
{
    FORWARD_INDEX_DEFINE(FdIndexType::UINT32, "uint32");
}

template <>
PrimitiveTypeForwardIndexCreator<uint64_t>::PrimitiveTypeForwardIndexCreator()
{
    FORWARD_INDEX_DEFINE(FdIndexType::UINT64, "uint64");
}

template <>
PrimitiveTypeForwardIndexCreator<float>::PrimitiveTypeForwardIndexCreator()
{
   FORWARD_INDEX_DEFINE(FdIndexType::FLOAT, "float");
}

template <>
PrimitiveTypeForwardIndexCreator<double>::PrimitiveTypeForwardIndexCreator()
{
   FORWARD_INDEX_DEFINE(FdIndexType::DOUBLE, "double");
}

template <typename T>
ForwardIndexWriter* PrimitiveTypeForwardIndexCreator<T>::createWriter() const 
{
    return new PrimitiveTypeForwardIndexWriter<T>(getType());
} 

template <typename T>
ForwardIndexReader* PrimitiveTypeForwardIndexCreator<T>::createReader() const 
{
    return new PrimitiveTypeForwardIndexReaderMMapImpl<T>(getType());
} 

template <typename T>
ForwardIndexMerger* PrimitiveTypeForwardIndexCreator<T>::createMerger() const 
{
    return new PrimitiveTypeForwardIndexMerger<T>();
} 

FX_NS_END

#endif
