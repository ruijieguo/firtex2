#include "firtex/index/datetime/DateTimeTermReader.h"
#include "firtex/index/datetime/DateTimeTermIterator.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Hash.h"

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_LOGGER(index, DateTimeTermReader);

DateTimeTermReader::DateTimeTermReader() 
{
}

DateTimeTermReader::DateTimeTermReader(const DateTimeTermReader& src)
    : TypedTermReader<int64_t, DateTimeHasher<int64_t> >(src)
{
}

DateTimeTermReader::~DateTimeTermReader() 
{
}

TermIteratorPtr DateTimeTermReader::createTermIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        const Term* pLowerTerm, const Term* pUpperTerm) const
{

    InputStreamPtr pDocStream = pStreamPool->getInputStream(
            getFileName(DOC_POSTING_FILEEXT));
    if (pDocStream.isNull())
    {
        FIRTEX_THROW(FileIOException, "Open posting-list file of [%s] FAILED.",
                     getFieldSchema()->getName().c_str());
    }

    DateTimeTermIterator* pTmpIter = new DateTimeTermIterator(
            getFieldSchema(), getDocFilter());
    TermIteratorPtr pTermIter(pTmpIter);
    if (!pTmpIter->init(getDict(), pDocStream, pLowerTerm,
                        pUpperTerm, pStreamPool.get()))
    {
        return TermIteratorPtr();
    }
    return pTermIter;
}

TermPostingIteratorPtr DateTimeTermReader::createTermPostingIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        offset_t nTermOffset) const
{
    InputStreamPtr pDocStream = pStreamPool->getInputStream(
                getFileName(DOC_POSTING_FILEEXT));
    if (pDocStream.isNull())
    {
        FIRTEX_THROW(FileIOException, _T("Open posting-list file of [%s] FAILED."),
                     getFieldSchema()->getName().c_str());
    }
    
    pDocStream->seek(nTermOffset);
    DocPostingDecoder* pDecoder = new DocPostingDecoder();
    PostingDecoderPtr pDecoderPtr(pDecoder);
    pDecoder->init(pDocStream, NULL, pStreamPool.get());

    BarrelTermPostingIterator* pTmpIter = new BarrelTermPostingIterator();
    TermPostingIteratorPtr pIter(pTmpIter);
    pTmpIter->init(pDecoderPtr, getDocFilter());
    return pIter;
}

TermReader* DateTimeTermReader::clone() const
{
    return new DateTimeTermReader(*this);
}

FX_NS_END

