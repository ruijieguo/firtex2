#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/index/InMemDocPostingDecoder.h"

FX_NS_USE(store);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocPostingStreammer);

/////////////////////////////////////////////////////
//
void DocPostingWriter::init(PostingPool* pPostingPool)
{
    m_streammer.init(pPostingPool);
    TypedDocPostingWriter<DocPostingStreammer>::init(pPostingPool);
}

PostingDecoderPtr DocPostingWriter::createDecoder() const
{
    InMemDocPostingDecoder* pDecoder = new InMemDocPostingDecoder();
    PostingDecoderPtr pDecoderPtr(pDecoder);
    
    DocSkipListReaderPtr pSkipReader;
    if (m_streammer.m_pDocSkipListWriter)
    {
        DocSkipListReader* p = dynamic_cast<DocSkipListReader*>(
                m_streammer.m_pDocSkipListWriter->createReader());
        pSkipReader.assign(p);
    }

    InputStreamPtr pInStreamPtr;
    if (m_streammer.m_pDocListWriter)
    {
        ByteSliceInputStream* pInStream = new ByteSliceInputStream(
                m_streammer.m_pDocListWriter->getHeadSlice(), false);
        pInStreamPtr.assign(pInStream);
    }

    pDecoder->init(m_termMeta, pInStreamPtr, pSkipReader,
                   this->m_lastDocId);
    return pDecoderPtr;
}

FX_NS_END

