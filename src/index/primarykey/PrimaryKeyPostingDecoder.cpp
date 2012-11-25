#include "firtex/index/primarykey/PrimaryKeyPostingDecoder.h"

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PrimaryKeyPostingDecoder);

uint32_t PrimaryKeyPostingDecoder::decodeDocRecord(
        docid_t* docBuffer, docid_t docId)
{
    if (m_docId != INVALID_DOCID && docId <= m_docId)
    {
        docBuffer[0] = m_docId;
        m_docId = INVALID_DOCID;
        return 1;
    }
    return 0;
}

uint32_t PrimaryKeyPostingDecoder::decodeTfRecord(tf_t* tfBuffer)
{
    tfBuffer[0] = 1;
    return 1;
}


FX_NS_END

