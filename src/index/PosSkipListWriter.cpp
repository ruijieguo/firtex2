#include "firtex/index/PosSkipListWriter.h"
#include "firtex/index/PostingCodingFactory.h"
#include "firtex/index/PosSkipListReader.h"
#include "firtex/index/ByteSliceInputStream.h"

FX_NS_USE(store);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosSkipListWriter);

PosSkipListWriter::PosSkipListWriter(uint32_t nMaxLevels,
                                     Pool* pPostingPool,
                                     Pool* pBufferPool)
    : TypedSkipListWriter<offset_t>(nMaxLevels, pPostingPool, pBufferPool)
{
}

PosSkipListWriter::~PosSkipListWriter() 
{
}

TypedSkipListReader<offset_t>* PosSkipListWriter::createReader() const
{
    uint32_t nSkipLevels = this->m_nMaxSkipLevels;
    while (nSkipLevels > 0 && (!this->m_ppSkipLevelsStream[nSkipLevels - 1]
                               || this->m_ppSkipLevelsStream[nSkipLevels - 1]->tell() == 0))
    {
        --nSkipLevels;
    }

    if (nSkipLevels > 0)
    {
        PosSkipListReader* pReader = new PosSkipListReader;
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

void PosSkipListWriter::writeRecord(uint32_t nLevel,
                                    ByteSliceOutputStream& sliceWriter,
                                    const offset_t* pPoints,
                                    const offset_t* pChildPointers,
                                    uint32_t nSize, bool bSeal)
{
    offset_t lastValue = this->getLastFlushedSkip(nLevel);

    FX_TRACE("Write record: level %u, offset: %lld, first pos: %lld, "
                    "last value: %lld, length: %u",
                    nLevel, sliceWriter.tell(), pPoints[0], lastValue, nSize);

    int32_t offBuffer[RECORD_SIZE];
    int32_t cpBuffer[RECORD_SIZE];

    offBuffer[0] = (int32_t)(pPoints[0] - lastValue);

    if (nLevel > 0)
    {
        cpBuffer[0] = (int32_t)pChildPointers[0];
    }

    for (uint32_t i = 1; i < nSize; ++i)
    {
        offBuffer[i] = (int32_t)(pPoints[i] - pPoints[i - 1]);
        if (nLevel > 0)
        {
            cpBuffer[i] = (int32_t)(pChildPointers[i] - pChildPointers[i - 1]);
        }
    }

    const PostingCoding* pCoder = PostingCodingFactory::instance()->getSkipListCoding();
    pCoder->encodeInt32(sliceWriter, offBuffer, nSize, bSeal);

    if (nLevel > 0)
    {
        pCoder->encodeInt32(sliceWriter, cpBuffer, nSize, bSeal);
    }
}

FX_NS_END

