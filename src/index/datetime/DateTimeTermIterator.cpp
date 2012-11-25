#include "firtex/index/datetime/DateTimeTermIterator.h"
#include <limits>

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

DateTimeTermIterator::DateTimeTermIterator(const FieldSchema* pFieldSchema,
        const BitVector* pDocFilter) 
    : TypedTermIterator<int64_t, DocPostingTraits<int64_t> >(pFieldSchema, pDocFilter)
{
}

DateTimeTermIterator::~DateTimeTermIterator() 
{
}

bool DateTimeTermIterator::init(const DictType* pDict,
                                const InputStreamPtr& pDocStream,
                                const Term* pLowerTerm,
                                const Term* pUpperTerm,
                                const FX_NS(store)::InputStreamPool* pInStreamPool)
{
    return TypedTermIterator<int64_t, DocPostingTraits<int64_t> >::init(
            pDict, pDocStream, NULL, pLowerTerm, pUpperTerm, pInStreamPool);
}

FX_NS_END

