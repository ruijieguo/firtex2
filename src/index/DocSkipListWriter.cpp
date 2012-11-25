#include "firtex/index/DocSkipListWriter.h"
#include "firtex/index/PostingCodingFactory.h"
#include "firtex/index/DocSkipListReader.h"
#include "firtex/index/ByteSliceInputStream.h"

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocSkipListWriter);

DocSkipListWriter::DocSkipListWriter(uint32_t nMaxLevels,
                                     Pool* pPostingPool,
                                     Pool* pBufferPool)
    : TypedSkipListWriter<DocSkipPoint>(nMaxLevels, pPostingPool, pBufferPool)
{
}

DocSkipListWriter::~DocSkipListWriter() 
{
}

TypedSkipListReader<DocSkipPoint>* DocSkipListWriter::createReader() const
{
    uint32_t nSkipLevels = this->m_nMaxSkipLevels;
    while (nSkipLevels > 0 && (!this->m_ppSkipLevelsStream[nSkipLevels - 1]
                               || this->m_ppSkipLevelsStream[nSkipLevels - 1]->tell() == 0))
    {
        --nSkipLevels;
    }

    if (nSkipLevels > 0)
    {
        DocSkipListReader* pReader = new DocSkipListReader;
        InputStream** ppInStream = new InputStream*[nSkipLevels];
        for (uint32_t i = 0; i < nSkipLevels; ++i)
        {
            ppInStream[i] = new ByteSliceInputStream(this->m_ppSkipLevelsStream[i]->getHeadSlice(), false);
        }
    
        pReader->init(ppInStream, nSkipLevels);
        return pReader;
    }
    return NULL;
}

void DocSkipListWriter::writeRecord(uint32_t nLevel,
                                    ByteSliceOutputStream& sliceWriter,
                                    const DocSkipPoint* pValues,
                                    const offset_t* pChildOffsets,
                                    uint32_t nSize, bool bSeal)
{
    FX_TRACE("Write skip record: level: %u, off: %llu, length: %u, first docid: %d, sumTf: %lld",
                    nLevel, sliceWriter.tell(), nSize, pValues[0].docId, pValues[0].cumSumTf);
    const DocSkipPoint& lastSkip = getLastFlushedSkip(nLevel);

    int32_t docBuffer[RECORD_SIZE];
    int32_t sumTfBuffer[RECORD_SIZE];
    int32_t docOffBuffer[RECORD_SIZE];
    int32_t cpBuffer[RECORD_SIZE];
        
    docBuffer[0] = (int32_t)(pValues[0].docId - lastSkip.docId);
    sumTfBuffer[0] = (int32_t)(pValues[0].cumSumTf - lastSkip.cumSumTf);
    docOffBuffer[0] = (int32_t)(pValues[0].docOff - lastSkip.docOff);
    

    if (nLevel > 0)
    {
        cpBuffer[0] = (int32_t)(pChildOffsets[0] - getLastFlushedChildOffset(nLevel));
    }

    for (uint32_t i = 1; i < nSize; ++i)
    {
        docBuffer[i] = (int32_t)(pValues[i].docId - pValues[i - 1].docId);
        assert(docBuffer[i] >= 0);
        sumTfBuffer[i] = (int32_t)(pValues[i].cumSumTf - pValues[i - 1].cumSumTf);
        docOffBuffer[i] = (int32_t)(pValues[i].docOff - pValues[i - 1].docOff);
        if (nLevel > 0)
        {
            cpBuffer[i] = (int32_t)(pChildOffsets[i] - pChildOffsets[i - 1]);
        }
    }

    const PostingCoding* pCoder = PostingCodingFactory::instance()->getSkipListCoding();
    pCoder->encodeInt32(sliceWriter, docBuffer, nSize, bSeal);
    pCoder->encodeInt32(sliceWriter, sumTfBuffer, nSize, bSeal);
    pCoder->encodeInt32(sliceWriter, docOffBuffer, nSize, bSeal);

    if (nLevel > 0)
    {
        pCoder->encodeInt32(sliceWriter, cpBuffer, nSize, bSeal);
    }
}

FX_NS_END

