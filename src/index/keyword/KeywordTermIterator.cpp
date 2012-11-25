#include "firtex/index/keyword/KeywordTermIterator.h"
#include <limits>

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

bool KeywordTermIterator::init(const DictType* pDict,
                               const InputStreamPtr& pDocStream,
                               const Term* pLowerTerm,
                               const Term* pUpperTerm,
                               const FX_NS(store)::InputStreamPool* pInStreamPool)
{
    return TypedTermIterator<uint64_t, DocPostingTraits<uint64_t> >::init(
            pDict, pDocStream, NULL, pLowerTerm, pUpperTerm, pInStreamPool);
}

FX_NS_END

