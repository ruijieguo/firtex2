#include "firtex/index/text/TextTermIterator.h"
#include <limits>

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_LOGGER(index, TextTermIterator);

bool TextTermIterator::init(const DictType* pDict,
                            InputStreamPtr& pDocStream,
                            InputStreamPtr& pPosStream,
                            const Term* pLowerTerm,
                            const Term* pUpperTerm, 
                            const InputStreamPool* pStreamPool)
{
    return TypedTermIterator<uint64_t, PosPostingTraits<uint64_t> >::init(
            pDict, pDocStream, pPosStream, pLowerTerm, pUpperTerm, pStreamPool);
}

FX_NS_END

