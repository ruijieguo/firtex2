#include "firtex/index/datetime/InMemDateTimeTermReader.h"
#include "firtex/index/datetime/InMemDateTimeTermIterator.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/index/BarrelTermPositionIterator.h"

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_LOGGER(index, InMemDateTimeTermReader);

InMemDateTimeTermReader::InMemDateTimeTermReader() 
{
}

InMemDateTimeTermReader::InMemDateTimeTermReader(const InMemDateTimeTermReader& src) 
    : TypedInMemTermReader<int64_t, DateTimeHasher<int64_t>, DocPostingTraits<int64_t> >(src)
{
}

InMemDateTimeTermReader::~InMemDateTimeTermReader() 
{
}

TermIteratorPtr InMemDateTimeTermReader::createTermIterator(
        const PostingTableTypePtr& pTable, 
        const Term* pLowerTerm, 
        const Term* pUpperTerm) const
{
    InMemDateTimeTermIterator* pTmpIter = new InMemDateTimeTermIterator(getFieldSchema(), getDocFilter());
    TermIteratorPtr pTermIter(pTmpIter);
    if (!pTmpIter->init(pTable, pLowerTerm, pUpperTerm))
    {
        return TermIteratorPtr();
    }
    return pTermIter;
}

TermPostingIteratorPtr InMemDateTimeTermReader::createTermPostingIterator(
        const PostingDecoderPtr& pDecoder) const
{
    BarrelTermPostingIterator* pIt = new BarrelTermPostingIterator();
    TermPostingIteratorPtr pItPtr(pIt);
    pIt->init(pDecoder, getDocFilter());
    return pItPtr;
}

TermReader* InMemDateTimeTermReader::clone() const
{
    return new InMemDateTimeTermReader(*this);
}

FX_NS_END

