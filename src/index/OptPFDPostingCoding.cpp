#include "firtex/index/OptPFDPostingCoding.h"
#include <assert.h>

FX_NS_USE(store);

FX_NS_DEF(index);

OptPFDPostingCoding::OptPFDPostingCoding() 
{
}

OptPFDPostingCoding::~OptPFDPostingCoding() 
{
}

void OptPFDPostingCoding::encodeInt32(OutputStream& outStream,
                                      const int32_t* pBuffer, size_t n,
                                      bool bSeal) const
{
    uint32_t buffer[CODING_BUFFER_SIZE];
    size_t ret = m_coding.encode(buffer, CODING_BUFFER_SIZE,
                                 (const uint32_t*)pBuffer, n, bSeal);
    outStream.write(buffer, ret << 2);
    // if (pBuffer[0] == 100)// && pBuffer[108] == 0)
    // {
    //     std::cout << "encodeInt32: " << std::endl;
    //     for (size_t i = 0; i < ret; ++i)
    //     {
    //         std::cout << buffer[i] << ", ";
    //     }
    //     std::cout << "end encodeInt32: " << std::endl;

    //     std::cout << "org data: " << std::endl;
    //     for (size_t i = 0; i < n; ++i)
    //     {
    //         std::cout << pBuffer[i] << ", ";
    //     }
    //     std::cout << "end org data: " << std::endl;
    // }
}

size_t OptPFDPostingCoding::decodeInt32(int32_t* pBuffer, size_t nBufSize,
                                      InputStream& inStream,
                                      bool& bExhausted) const
{
    uint32_t buffer[CODING_BUFFER_SIZE];
    inStream.read((void*)buffer, sizeof(uint32_t));
    
    size_t nFrameSize = m_coding.getFrameSize(buffer[0]);
//    FIRTEX_ASSERT2(nFrameSize <= CODING_BUFFER_SIZE);
    assert(nFrameSize <= CODING_BUFFER_SIZE);

    inStream.read((void*)(buffer + 1), (nFrameSize - 1) * sizeof(uint32_t));
    return m_coding.decode((uint32_t*)pBuffer, nBufSize, buffer, nFrameSize, bExhausted);
}

FX_NS_END

