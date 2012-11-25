#include "firtex/index/PosSkipListReader.h"
#include "firtex/index/PostingCodingFactory.h"

FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosSkipListReader);

PosSkipListReader::~PosSkipListReader() 
{
}

size_t PosSkipListReader::readRecord(uint32_t nLevel,
                                     InputStream* pInStream,
                                     offset_t* pPointers,
                                     offset_t* pChildOffsets,
                                     uint32_t nSize, bool& bExhausted)
{
    offset_t lastValue = this->getLastPoint(nLevel);

//    std::cout << "level: " << nLevel << ", lastValue: " << lastValue << std::endl;
    int32_t posBuffer[RECORD_SIZE];

    const PostingCoding* pCoder = PostingCodingFactory::instance()->getSkipListCoding();
    size_t ret1 = pCoder->decodeInt32((int32_t*)posBuffer, RECORD_SIZE,
            *pInStream, bExhausted);

    pPointers[0] = (offset_t)(posBuffer[0] + lastValue);

    // if (posBuffer[0] < 0)
    // {
    //     std::cout << "========" << std::endl;
    //     for (size_t i = 0; i < ret1; ++i)
    //     {
    //         std::cout << posBuffer[i] << ", ";
    //     }
    //     std::cout << "========" << std::endl;
    // }
    for (size_t i = 1; i < ret1; ++i)
    {
        pPointers[i] = pPointers[i - 1] + posBuffer[i];
    }

    if (nLevel > 0)
    {
        int32_t childOffBuffer[RECORD_SIZE];
        size_t ret2 = pCoder->decodeInt32((int32_t*)childOffBuffer, 
                RECORD_SIZE, *pInStream, bExhausted);

        FIRTEX_ASSERT2(ret2 == ret1);
        pChildOffsets[0] = childOffBuffer[0];
        for (size_t i = 1; i < ret2; ++i)
        {
            pChildOffsets[i] = pChildOffsets[i - 1] + childOffBuffer[i];
        }
    }
    return ret1;
}

FX_NS_END

