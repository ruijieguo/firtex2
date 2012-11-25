#include "firtex/index/keyword/InMemKeywordTermReader.h"
#include "firtex/index/keyword/InMemKeywordTermIterator.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/index/BarrelTermPositionIterator.h"

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_LOGGER(index, InMemKeywordTermReader);

TermIteratorPtr InMemKeywordTermReader::createTermIterator(
        const PostingTableTypePtr& pTable, 
        const Term* pLowerTerm, 
        const Term* pUpperTerm) const
{
    InMemKeywordTermIterator* pTmpIter = new InMemKeywordTermIterator(
            getFieldSchema(), getDocFilter());
    TermIteratorPtr pTermIter(pTmpIter);
    if (!pTmpIter->init(pTable, pLowerTerm, pUpperTerm))
    {
        return TermIteratorPtr();
    }
    return pTermIter;
}

TermPostingIteratorPtr InMemKeywordTermReader::createTermPostingIterator(
        const PostingDecoderPtr& pDecoder) const
{
    BarrelTermPostingIterator* pIt = new BarrelTermPostingIterator();
    TermPostingIteratorPtr pItPtr(pIt);
    pIt->init(pDecoder, getDocFilter());
    return pItPtr;
}

TermReader* InMemKeywordTermReader::clone() const
{
    return new InMemKeywordTermReader(*this);
}

FX_NS_END

