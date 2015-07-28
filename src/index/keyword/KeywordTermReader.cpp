#include "firtex/index/keyword/KeywordTermReader.h"
#include "firtex/index/keyword/KeywordTermIterator.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Hash.h"

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, KeywordTermReader);

KeywordTermReader::KeywordTermReader() 
{
}

KeywordTermReader::KeywordTermReader(const KeywordTermReader& src)
    : TypedTermReader<uint64_t, IndexKeyHasher<uint64_t> >(src)
{
}

KeywordTermReader::~KeywordTermReader() 
{
}

TermIteratorPtr KeywordTermReader::createTermIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        const Term* pLowerTerm, const Term* pUpperTerm) const
{

    InputStreamPtr pDocStream = pStreamPool->getInputStream(
            getFileName(DOC_POSTING_FILEEXT));
    if (!pDocStream)
    {
        FIRTEX_THROW(FileIOException, _T("Open posting-list file of [%s] FAILED."),
                     getFieldSchema()->getName().c_str());
    }

    KeywordTermIterator* pTmpIter = new KeywordTermIterator(
            getFieldSchema(), getDocFilter());
    TermIteratorPtr pTermIter(pTmpIter);
    if (!pTmpIter->init(getDict(), pDocStream, pLowerTerm,
                        pUpperTerm, pStreamPool.get()))
    {
        return TermIteratorPtr();
    }
    return pTermIter;
}

TermPostingIteratorPtr KeywordTermReader::createTermPostingIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        offset_t nTermOffset) const
{
    InputStreamPtr pDocStream = pStreamPool->getInputStream(
                getFileName(DOC_POSTING_FILEEXT));
    if (!pDocStream)
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

TermReader* KeywordTermReader::clone() const
{
    return new KeywordTermReader(*this);
}

FX_NS_END

