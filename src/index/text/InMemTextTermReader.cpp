#include "firtex/index/text/InMemTextTermReader.h"
#include "firtex/index/text/InMemTextTermIterator.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/index/BarrelTermPositionIterator.h"

FX_NS_DEF(index);

SETUP_LOGGER(index, InMemTextTermReader);

TermIteratorPtr InMemTextTermReader::createTermIterator(
        const PostingTableTypePtr& pTable, 
        const Term* pLowerTerm, 
        const Term* pUpperTerm) const
{
    InMemTextTermIterator* pTmpIter = new InMemTextTermIterator(
            getFieldSchema(), getDocFilter());
    TermIteratorPtr pTermIter(pTmpIter);
    if (!pTmpIter->init(pTable, pLowerTerm, pUpperTerm))
    {
        return TermIteratorPtr();
    }
    return pTermIter;
}

TermPostingIteratorPtr InMemTextTermReader::createTermPostingIterator(
        const PostingDecoderPtr& pDecoder) const
{
    BarrelTermPositionIterator* pIt = new BarrelTermPositionIterator();
    TermPostingIteratorPtr pItPtr(pIt);
    pIt->init(pDecoder, getDocFilter());
    return pItPtr;
}

TermReader* InMemTextTermReader::clone() const
{
    return new InMemTextTermReader(*this);
}

FX_NS_END

