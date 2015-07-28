#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/PostingCodingFactory.h"
#include "firtex/index/DocPostingWriter.h"

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocPostingDecoder);

DocPostingDecoder::DocPostingDecoder()
    : m_pInStreamPool(NULL)
    , m_lastDocId(INVALID_DOCID)
    , m_nNumDocsDecoded(0)
    , m_nStartOff(0)
    , m_nDocLength(0)
    , m_nLastSeekOff(-1)
{
    m_pDocDecoder = PostingCodingFactory::instance()->getDocCoding();
    m_pTfDecoder = PostingCodingFactory::instance()->getTfCoding();
}

DocPostingDecoder::~DocPostingDecoder() 
{
    if (m_pDocStream && m_pInStreamPool)
    {
        m_pInStreamPool->releaseInputStream(m_pDocStream);
    }
}

void DocPostingDecoder::init(const InputStreamPtr& pDocStream,
                             const InputStreamPtr& pPosStream,
                             const InputStreamPool* pInStreamPool)
{
    m_nNumDocsDecoded = 0;
    m_nStartOff = 0;
    m_nDocLength = 0;
    m_nLastSeekOff = -1;
    m_pDocStream = pDocStream;
    m_pInStreamPool = pInStreamPool;

    offset_t startOff = m_pDocStream->tell();

    // read term meta
    readTermMeta(); 
    
    if (m_termMeta.getDocFreq() >= (df_t)RECORD_SIZE)
    {
        //Init skip list;
        m_pDocSkipListReader.reset(new DocSkipListReader());
        m_pDocSkipListReader->init(m_pDocStream);
    }
    else
    {
        m_pDocSkipListReader.reset();
    }
    if (m_nDocLength > 0)
    {
        m_nStartOff = startOff - (offset_t)m_nDocLength;
        m_pDocStream->seek(m_nStartOff);
    }
    FX_TRACE("Init doc decoder: meta start: %lld, doc start: %lld, length: %llu",
             startOff, m_nStartOff, m_nDocLength);
}

void DocPostingDecoder::readTermMeta()
{
    df_t docFreq = (df_t)m_pDocStream->readVInt64();
    m_termMeta.setDocFreq(docFreq);
    if (docFreq == 1)
    {
        m_lastDocId = m_pDocStream->readVInt32();
        if (m_lastDocId & 0x1)
        {
            m_termMeta.setCTF((ctf_t)m_pDocStream->readVInt64());
        }
        else 
        {
            m_termMeta.setCTF(1);
        }
        m_lastDocId >>= 1;
    }
    else 
    {
        m_nDocLength = (uint64_t)m_pDocStream->readVInt64();
        m_lastDocId = m_pDocStream->readVInt32();
        m_termMeta.setCTF((ctf_t)m_pDocStream->readVInt64());
    }
    FX_TRACE("Read meta: length: %llu, df: %d, ctf: %lld, lastDocId: %d", 
             m_nDocLength, m_termMeta.getDocFreq(), m_termMeta.getCTF(), m_lastDocId);
}

uint32_t DocPostingDecoder::decodeDocRecord(docid_t* docBuffer, docid_t docId)
{
    if (m_nNumDocsDecoded >= m_termMeta.getDocFreq())
    {
        return 0;
    }
    
    docid_t startDocId = 0;
    if (m_pDocSkipListReader != NULL)
    {
        startDocId = m_pDocSkipListReader->skipTo(docId);
        if (startDocId != INVALID_DOCID)
        {
            const DocSkipPoint& skipPoint = m_pDocSkipListReader->getCurPoint();
            if ((m_nLastSeekOff == skipPoint.docOff)
                && !m_pDocSkipListReader->moveToNextSkip())
            {
                return 0;
            }
            startDocId = skipPoint.docId;
            m_pDocStream->seek(skipPoint.docOff + m_nStartOff);
            m_nNumDocsDecoded = m_pDocSkipListReader->getNumSkipped();
            m_nLastSeekOff = skipPoint.docOff;

            if (m_nNumDocsDecoded >= m_termMeta.getDocFreq())
            {
                return 0;
            }
            
            FX_TRACE("Skip to: %d, off: %lld, skipped: %d", skipPoint.docId, 
                     skipPoint.docOff + m_nStartOff, m_nNumDocsDecoded);
        }
        else
        {
            return 0;
        }
    }
    else if(m_termMeta.getDocFreq() == 1)
    {
        docBuffer[0] = m_lastDocId;
        ++m_nNumDocsDecoded;
        return 1;
    }
    bool bExhausted = false;
    size_t ret =  m_pDocDecoder->decodeInt32((int32_t*)docBuffer,
            RECORD_SIZE,  *(m_pDocStream.get()), bExhausted);
    docBuffer[0] += startDocId;

    FX_TRACE("Decode doc record: target doc: %d, first doc: %d, length: %d", 
             docId, docBuffer[0], (int32_t)ret);

    for (size_t i = 1; i < ret; ++i)
    {
        docBuffer[i] += docBuffer[i - 1];
    }
    m_nNumDocsDecoded += (df_t)ret;

    return (uint32_t)ret;
}

uint32_t DocPostingDecoder::decodeTfRecord(tf_t* tfBuffer)
{
    if (m_termMeta.getDocFreq() == 1)
    {
        tfBuffer[0] = (tf_t)m_termMeta.getCTF();
        return 1;
    }
    
    bool bExhausted = false;
    size_t ret = m_pTfDecoder->decodeInt32((int32_t*)tfBuffer,
            RECORD_SIZE, *(m_pDocStream.get()), bExhausted);
    FX_TRACE("Decode tf record: first tf: %d, length: %d", tfBuffer[0], ret);
    return (uint32_t)ret;
}

FX_NS_END

