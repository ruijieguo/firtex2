#include "firtex/index/PosPostingWriter.h"
#include "firtex/utility/Exception.h"
#include "firtex/index/InMemPosPostingDecoder.h"
#include "firtex/index/ByteSliceInputStream.h"

FX_NS_USE(store);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosPostingStreammer);

////////////////////////////////////////////

void PosPostingWriter::init(PostingPool* pPostingPool)
{
    m_streammer.init(pPostingPool);
    TypedPosPostingWriter<PosPostingStreammer>::init(pPostingPool);
}

PostingDecoderPtr PosPostingWriter::createDecoder() const
{
    InMemPosPostingDecoder* pDecoder = new InMemPosPostingDecoder();
    PostingDecoderPtr pDecoderPtr(pDecoder);
    
    DocSkipListReaderPtr pDocSkipReader;
    if (m_streammer.m_pDocSkipListWriter)
    {
        DocSkipListReader* p = dynamic_cast<DocSkipListReader*>(
                m_streammer.m_pDocSkipListWriter->createReader());
        pDocSkipReader.assign(p);
    }

    PosSkipListReaderPtr pPosSkipReader;
    if (m_streammer.m_pPosSkipListWriter)
    {
        PosSkipListReader* p = dynamic_cast<PosSkipListReader*>(
                m_streammer.m_pPosSkipListWriter->createReader());
        pPosSkipReader.assign(p);
    }

    InputStreamPtr pDocInStreamPtr;
    InputStreamPtr pPosInStreamPtr;
    if (m_streammer.m_pDocListWriter)
    {
        ByteSliceInputStream* pDocInStream = new ByteSliceInputStream(
                m_streammer.m_pDocListWriter->getHeadSlice(), false);
        pDocInStreamPtr.assign(pDocInStream);
    }

    if (m_streammer.m_pPosListWriter)
    {
        ByteSliceInputStream* pPosInStream = new ByteSliceInputStream(
                m_streammer.m_pPosListWriter->getHeadSlice(), false);
        pPosInStreamPtr.assign(pPosInStream);
    }

    pDecoder->init(m_termMeta, pDocInStreamPtr, pPosInStreamPtr,
                   pDocSkipReader, pPosSkipReader, this->m_lastDocId,
                   this->m_lastPosInDoc);
    return pDecoderPtr;
}

FX_NS_END

