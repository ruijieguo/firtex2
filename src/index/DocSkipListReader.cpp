#include "firtex/index/DocSkipListReader.h"
#include "firtex/index/PostingCodingFactory.h"

FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocSkipListReader);

DocSkipListReader::DocSkipListReader()
    : TypedSkipListReader<DocSkipPoint>()
{
}

DocSkipListReader::~DocSkipListReader() 
{
}

size_t DocSkipListReader::readRecord(uint32_t nLevel,
                                     InputStream* pInStream,
                                     SkipPointType* pPointers,
                                     offset_t* pChildOffsets,
                                     uint32_t nSize, bool& bExhausted)
{
    FX_TRACE("Read skip record: off: %lld", pInStream->tell());
    const DocSkipPoint& lastValue = this->getLastPoint(nLevel);
        
    docid_t docBuffer[RECORD_SIZE];
    int32_t tfBuffer[RECORD_SIZE];
    int32_t docOffBuffer[RECORD_SIZE];

    const PostingCoding* pCoder = PostingCodingFactory::instance()->getSkipListCoding();
    size_t ret1 = pCoder->decodeInt32((int32_t*)docBuffer, RECORD_SIZE,
            *pInStream, bExhausted);
    size_t ret2 = pCoder->decodeInt32((int32_t*)tfBuffer, RECORD_SIZE,
            *pInStream, bExhausted);
    size_t ret3 = pCoder->decodeInt32((int32_t*)docOffBuffer, RECORD_SIZE,
            *pInStream, bExhausted);
    if ((ret1 != ret2) || (ret1 != ret3))
    {
        FIRTEX_THROW(IndexCollapseException, "Skip list collapsed.");
    }

    pPointers[0].docId = docBuffer[0] + lastValue.docId;
    pPointers[0].cumSumTf = (ctf_t)(tfBuffer[0] + lastValue.cumSumTf);
    pPointers[0].docOff = (offset_t)(docOffBuffer[0] + lastValue.docOff);

    for (size_t i = 1; i < ret1; ++i)
    {
        pPointers[i].docId = pPointers[i - 1].docId + docBuffer[i];
        pPointers[i].cumSumTf = pPointers[i - 1].cumSumTf + tfBuffer[i];
        pPointers[i].docOff = pPointers[i - 1].docOff + docOffBuffer[i];
    }

    if (nLevel > 0)
    {
        int32_t childOffBuffer[RECORD_SIZE];
        size_t ret4 = pCoder->decodeInt32((int32_t*)childOffBuffer, RECORD_SIZE, 
                *pInStream, bExhausted);
        FIRTEX_ASSERT2(ret4 == ret1);
        pChildOffsets[0] = childOffBuffer[0] + this->getLastChildOffset(nLevel);
        for (size_t i = 1; i < ret4; ++i)
        {
            pChildOffsets[i] = pChildOffsets[i - 1] + childOffBuffer[i];
        }
    }
    FX_TRACE("Read skip record: level: %u, length: %u", nLevel, (uint32_t)ret1);

    return ret1;
}

FX_NS_END

