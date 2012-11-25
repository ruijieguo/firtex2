#include "firtex/index/PosPostingDecoder.h"
#include "firtex/index/PostingCodingFactory.h"
#include "firtex/index/PosPostingWriter.h"

FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosPostingDecoder);

PosPostingDecoder::PosPostingDecoder() 
    : m_lastPos(INVALID_POS)
    , m_nPosStartOff(0)
    , m_nPosLength(0)
{
    m_pPosDecoder = PostingCodingFactory::instance()->getPosCoding();
}

PosPostingDecoder::~PosPostingDecoder() 
{
    if (m_pPosStream.isNotNull() && m_pInStreamPool)
    {
        m_pInStreamPool->releaseInputStream(m_pPosStream);
    }
}

void PosPostingDecoder::init(InputStreamPtr& pDocStream,
                             InputStreamPtr& pPosStream,
                             const FX_NS(store)::InputStreamPool* pInStreamPool)
{
    m_lastPos = INVALID_POS;
    m_nPosLength = 0;
    m_pPosStream = pPosStream;
    DocPostingDecoder::init(pDocStream, pPosStream, pInStreamPool);

    FX_TRACE("Init pos decoder: pos offset: %lld", m_nPosStartOff);

    if (m_termMeta.getCTF() > 1)
    {
        m_pPosStream->seek(m_nPosStartOff);
        m_nPosLength = (uint64_t)m_pPosStream->readVInt64();
    }
    if (m_termMeta.getCTF() >= RECORD_SIZE)
    {
        m_pPosSkipListReader = new PosSkipListReader();
        m_pPosSkipListReader->init(m_pPosStream);
    }
    else
    {
        m_pPosSkipListReader.reset();
    }
    if (m_nPosLength > 0)
    {
        m_nPosStartOff -= (offset_t)m_nPosLength;
        FX_TRACE("Pos start off: %lld, length: %llu",
                        m_nPosStartOff, m_nPosLength);
        m_pPosStream->seek(m_nPosStartOff);
    }
}

void PosPostingDecoder::readTermMeta()
{
    df_t docFreq = (df_t)m_pDocStream->readVInt64();
    m_termMeta.getDocFreq() = docFreq;

    if (docFreq == 1)
    {
        m_lastDocId = m_pDocStream->readVInt32();
        if (m_lastDocId & 0x1)
        {
            m_termMeta.getCTF() = (ctf_t)m_pDocStream->readVInt64();
            m_nPosStartOff = m_pDocStream->readVInt64();
        }
        else 
        {
            m_termMeta.getCTF() = 1;
            m_lastPos = (loc_t)m_pDocStream->readVInt64();
        }
        m_lastDocId >>= 1;
    }
    else 
    {
        m_nDocLength = (uint64_t)m_pDocStream->readVInt64();
        m_lastDocId = m_pDocStream->readVInt32();
        m_termMeta.getCTF() = (ctf_t)m_pDocStream->readVInt64();
        m_nPosStartOff = m_pDocStream->readVInt64();
    }
    FX_TRACE("Read meta: length: %llu, df: %d, ctf: %lld, pos offset: %lld, lastDocId: %d",
                    m_nDocLength, docFreq, m_termMeta.getCTF(), m_nPosStartOff, m_lastDocId);
}

uint32_t PosPostingDecoder::decodePosRecord(loc_t* posBuffer,
        ctf_t nSkipPosCount, uint32_t& nStartOff)
{
    if (nSkipPosCount >= m_termMeta.getCTF())
    {
        return 0;
    }
    uint32_t nNumSkipped = 0;
    if (m_pPosSkipListReader.isNotNull())
    {
        offset_t posOff = m_pPosSkipListReader->skipTo(nSkipPosCount);
        if (posOff != (offset_t)-1)
        {
            FX_TRACE("seek to: %lld", posOff);
            m_pPosStream->seek(posOff + m_nPosStartOff);
            nNumSkipped = m_pPosSkipListReader->getNumSkipped();
        }
        else 
        {
            return 0;
        }
    }
    else if (m_termMeta.getCTF() == 1)
    {
        posBuffer[0] = m_lastPos;
        nStartOff = 0;
        return 1;
    }
    bool bExhausted = false;
    size_t ret = m_pPosDecoder->decodeInt32(posBuffer, RECORD_SIZE,
            *m_pPosStream, bExhausted);
    FX_TRACE("decode pos record: first pos: %d, length: %u", posBuffer[0], (uint32_t)ret);

    nStartOff = (uint32_t)(nSkipPosCount - nNumSkipped);
    FIRTEX_ASSERT2(nStartOff <= ret);

    return (uint32_t)ret;
}

FX_NS_END

