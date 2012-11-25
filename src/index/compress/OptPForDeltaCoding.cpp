#include "firtex/index/compress/OptPForDeltaCoding.h"
#include "firtex/index/compress/Unpack.h"
#include <assert.h>

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, OptPForDeltaCoding);

OptPForDeltaCoding::OptPForDeltaCoding() 
{
    MASK[0] = 0x00000000;
    MASK[1] = 0x00000001;
    MASK[2] = 0x00000003;
    MASK[3] = 0x00000007;
    MASK[4] = 0x0000000f;
    MASK[5] = 0x0000001f;
    MASK[6] = 0x0000003f;
    MASK[7] = 0x0000007f;
    MASK[8] = 0x000000ff;
    MASK[9] = 0x000001ff;
    MASK[10] = 0x000003ff;
    MASK[11] = 0x000007ff;
    MASK[12] = 0x00000fff;
    MASK[13] = 0x00001fff;
    MASK[14] = 0x00003fff;
    MASK[15] = 0x00007fff;
    MASK[16] = 0x0000ffff;
    MASK[17] = 0x0001ffff;
    MASK[18] = 0x0003ffff;
    MASK[19] = 0x0007ffff;
    MASK[20] = 0x000fffff;
    MASK[21] = 0x001fffff;
    MASK[22] = 0x003fffff;
    MASK[23] = 0x007fffff;
    MASK[24] = 0x00ffffff;
    MASK[25] = 0x01ffffff;
    MASK[26] = 0x03ffffff;
    MASK[27] = 0x07ffffff;
    MASK[28] = 0x0fffffff;
    MASK[29] = 0x1fffffff;
    MASK[30] = 0x3fffffff;
    MASK[31] = 0x7fffffff;
    MASK[32] = 0xffffffff;

    BITS_PACK_MAP[0] = 0;
    BITS_PACK_MAP[1] = 1;
    BITS_PACK_MAP[2] = 2;
    BITS_PACK_MAP[3] = 3;
    BITS_PACK_MAP[4] = 4;
    BITS_PACK_MAP[5] = 5;
    BITS_PACK_MAP[6] = 6;
    BITS_PACK_MAP[7] = 7;
    BITS_PACK_MAP[8] = 8;
    BITS_PACK_MAP[9] = 9;
    BITS_PACK_MAP[10] = 10;
    BITS_PACK_MAP[11] = 11;
    BITS_PACK_MAP[12] = 12;
    BITS_PACK_MAP[13] = 13;
    BITS_PACK_MAP[14] = 14;
    BITS_PACK_MAP[15] = 15;
    BITS_PACK_MAP[16] = 16;
    BITS_PACK_MAP[17] = 17;
    BITS_PACK_MAP[18] = 18;
    BITS_PACK_MAP[19] = 19;
    BITS_PACK_MAP[20] = 20;
    BITS_PACK_MAP[21] = 21;
    BITS_PACK_MAP[22] = 22;
    BITS_PACK_MAP[23] = 23;
    BITS_PACK_MAP[24] = 24;
    BITS_PACK_MAP[25] = 25;
    BITS_PACK_MAP[26] = 26;
    BITS_PACK_MAP[27] = 27;
    BITS_PACK_MAP[28] = 28;
    BITS_PACK_MAP[29] = 29;
    BITS_PACK_MAP[30] = 30;
    BITS_PACK_MAP[31] = 32;  //31 map to 32
    BITS_PACK_MAP[32] = 32;  //32
}

OptPForDeltaCoding::~OptPForDeltaCoding() 
{
}

size_t OptPForDeltaCoding::encode(uint32_t* dest, size_t nDestLen,
                                  const uint32_t* src, size_t nSrcLen,
                                  bool bSeal) const
{
    uint32_t* p = dest;
    size_t nNumFrames = (nSrcLen + FRAME_SIZE - 1) / FRAME_SIZE;
    for (size_t i = 0; i < nNumFrames; ++i)
    {
        size_t len = (nSrcLen <= FRAME_SIZE) ? nSrcLen : FRAME_SIZE;
        uint32_t e = 0;
        uint32_t b = determinFrameBits(src, (uint32_t)len, e); 
        size_t encodeSize = encodeFrame(p, nDestLen, src, len, b, 
                e, ((i == nNumFrames - 1) && bSeal));
        p += encodeSize;
        nDestLen -= encodeSize;
        src += len;
        nSrcLen -= len;
    }
    return (p - dest);
}

size_t OptPForDeltaCoding::decode(uint32_t* dest, size_t nDestLen,
                                  const uint32_t* src, size_t nSrcLen,
                                  bool& bExhausted) const
{
    uint32_t* p = (uint32_t*)src;
    size_t nTotalSize = 0;
    size_t nDecoded = 0;
    bool bLastFrame = false;
    while (!bLastFrame && nDestLen > 0)
    {
        nDecoded = decodeFrame(dest + nTotalSize, nDestLen, p, 
                               nSrcLen, bLastFrame);
        nTotalSize += nDecoded;
        nDestLen -= nDecoded;
    }
    bExhausted = bLastFrame;
    return nTotalSize;
}

size_t OptPForDeltaCoding::encodeFrame(uint32_t* dest, size_t nDestLen,
                                       const uint32_t* src, size_t nSrcLen,
                                       uint32_t nFrameBits, uint32_t nNumExceps,
                                       bool bLastFrame) const
{
    if (nNumExceps == 0)
    {
        //No exception
        return doEncodeFrame(dest, nDestLen, nFrameBits, 0,
                             src, nSrcLen, NULL, NULL, 0, bLastFrame);
    }

    uint32_t pre =  0;
    uint32_t max = (uint32_t)(1 << nFrameBits);
    uint32_t code[FRAME_SIZE];
    uint32_t miss[FRAME_SIZE];
    uint32_t exception[FRAME_SIZE];

    code[0] = 0; //remove warnings
    miss[0] = 0; 

    // Step 1. find exceptions
    size_t j = 0;
    for (size_t i = 0 ; i < nSrcLen ; i++)
    {
        uint32_t val = src[i];
        code[i] = val;
        miss[j] = (uint32_t)i;
        if (max <= val)
        {
            j++;
        }
    }

    // Step 2. create offset and upper-bit value list for exceptions
    uint32_t excepOff[FRAME_SIZE];
    
    uint32_t cur = miss[0];
    exception[0] = (uint32_t)(code[cur] >> nFrameBits);
    code[cur] = code[cur] & MASK[nFrameBits];
    pre	= cur ;

    for (size_t i = 1 ; i < j ; i++)
    {
        cur = miss[i];
        exception[i] = code[cur] >> nFrameBits;
        code[cur] = code[cur] & MASK[nFrameBits];
        excepOff[i - 1] = cur - pre - 1;
        pre = cur;
    }

    size_t nFirstExcepPos = miss[0] + 1;
    return doEncodeFrame(dest, nDestLen, nFrameBits, nFirstExcepPos,
                         code, nSrcLen, exception, excepOff,
                         nNumExceps, bLastFrame);
}

size_t OptPForDeltaCoding::decodeFrame(uint32_t* dest, size_t nDestLen,
        uint32_t*& src, size_t& nSrcLen, bool& bLastFrame) const
{
    // Decode header value
    uint32_t header = src[0];
    uint32_t nNumData = (header >> 25) + 1;
    uint32_t nFirstExcepPos = (header << 7) >> 24; //miss[0] + 1 or 0
    uint32_t nFrameBits = BITS_PACK_MAP[((header << 15) >> 27)]; // 0 <= numFramebit <= 31
    uint32_t nExcepIntSize = (header << 20) >> 21;
    bLastFrame = ((header << 31) >> 31 == 1);

    /// Decode normal data
    Unpack::unpack(nFrameBits, dest, src + 1, (uint32_t)nNumData);

    size_t nIntExcepOff = HEADER_INT_SIZE + (nNumData * nFrameBits + 31) / 32;

    /// Decode exception data
    if (nFirstExcepPos != 0 )
    {
        decodeException(dest, src + nIntExcepOff, nExcepIntSize,
                        nFirstExcepPos - 1, nFrameBits);
    }

    src += (nIntExcepOff + nExcepIntSize);
    nSrcLen -= (nIntExcepOff + nExcepIntSize);
    return nNumData;
}

size_t OptPForDeltaCoding::getFrameSize(uint32_t header) const
{
    uint32_t nNumData = (header >> 25) + 1;
    uint32_t nFrameBits = BITS_PACK_MAP[((header << 15) >> 27)];
    uint32_t nExcepIntSize = (header << 20) >> 21;

    return (HEADER_INT_SIZE + (nNumData * nFrameBits + 31) / 32 + nExcepIntSize);
}

FX_NS_END

