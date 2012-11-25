#include "firtex/index/text/TextTermReader.h"
#include "firtex/index/text/TextTermIterator.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Hash.h"

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_LOGGER(index, TextTermReader);

TermIteratorPtr TextTermReader::createTermIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        const Term* pLowerTerm, const Term* pUpperTerm) const
{

    InputStreamPool::InputStreamPair inStreams = m_pStreamPool->getInputStreams(
            std::make_pair(getFileName(DOC_POSTING_FILEEXT), getFileName(POS_POSTING_FILEEXT)));

    TextTermIterator* pTmpIter = new TextTermIterator(
            getFieldSchema(), getDocFilter());
    TermIteratorPtr pTermIter(pTmpIter);
    if (!pTmpIter->init(getDict(), inStreams.first, inStreams.second, 
                        pLowerTerm, pUpperTerm, pStreamPool.get()))
    {
        return TermIteratorPtr();
    }
    return pTermIter;
}

TermPostingIteratorPtr TextTermReader::createTermPostingIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        offset_t nTermOffset) const
{
    InputStreamPool::InputStreamPair inStreams = m_pStreamPool->getInputStreams(
            std::make_pair(getFileName(DOC_POSTING_FILEEXT), getFileName(POS_POSTING_FILEEXT)));
    
    inStreams.first->seek(nTermOffset);
    PosPostingDecoder* pDecoder = new PosPostingDecoder();
    PostingDecoderPtr pDecoderPtr(pDecoder);
    pDecoder->init(inStreams.first, inStreams.second, pStreamPool.get());

    BarrelTermPositionIterator* pTmpIter = new BarrelTermPositionIterator();
    TermPostingIteratorPtr pIter(pTmpIter);
    pTmpIter->init(pDecoderPtr, getDocFilter());
    return pIter;
}

TermReader* TextTermReader::clone() const
{
    return new TextTermReader(*this);
}

FX_NS_END

