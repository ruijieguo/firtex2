#include "firtex/index/compress/Unpack.h"

FX_NS_DEF(index);

Unpack::unpack_func Unpack::UNPACK_FUNCTIONS[] =
{
    Unpack::unpack0, Unpack::unpack1,  Unpack::unpack2,  Unpack::unpack3, 
    Unpack::unpack4, Unpack::unpack5,  Unpack::unpack6,  Unpack::unpack7,
    Unpack::unpack8, Unpack::unpack9,  Unpack::unpack10, Unpack::unpack11,
    Unpack::unpack12, Unpack::unpack13, Unpack::unpack14, Unpack::unpack15,
    Unpack::unpack16, Unpack::unpack17, Unpack::unpack18, Unpack::unpack19,
    Unpack::unpack20, Unpack::unpack21, Unpack::unpack22, Unpack::unpack23,
    Unpack::unpack24, Unpack::unpack25, Unpack::unpack26, Unpack::unpack27,
    Unpack::unpack28, Unpack::unpack29, Unpack::unpack30, Unpack::unpack31,
    Unpack::unpack32
};

void Unpack::unpack0(uint32_t* dest, const uint32_t* /*src*/, size_t n)
{
    size_t i;
    for (i = 32; i <= n; i += 32, dest += 32)
    {
        dest[0]  = 0;
        dest[1]  = 0;
        dest[2]  = 0;
        dest[3]  = 0;
        dest[4]  = 0;
        dest[5]  = 0;
        dest[6]  = 0;
        dest[7]  = 0;
        dest[8]  = 0;
        dest[9]  = 0;
        dest[10] = 0;
        dest[11] = 0;
        dest[12] = 0;
        dest[13] = 0;
        dest[14] = 0;
        dest[15] = 0;
        dest[16] = 0;
        dest[17] = 0;
        dest[18] = 0;
        dest[19] = 0;
        dest[20] = 0;
        dest[21] = 0;
        dest[22] = 0;
        dest[23] = 0;
        dest[24] = 0;
        dest[25] = 0;
        dest[26] = 0;
        dest[27] = 0;
        dest[28] = 0;
        dest[29] = 0;
        dest[30] = 0;
        dest[31] = 0;
    }
    if (n & 31)
    {
        for (i = 0; i < (n & 31); i++)
        {
            dest[i] = 0;
        }
    }
}

void Unpack::unpack1(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, src += 1, dest += 32)
    {
        dest[0]  = src[0] & 0x1;
        dest[1]  = (src[0] >> 1) & 0x1;
        dest[2]  = (src[0] >> 2) & 0x1;
        dest[3]  = (src[0] >> 3) & 0x1;
        dest[4]  = (src[0] >> 4) & 0x1;
        dest[5]  = (src[0] >> 5) & 0x1;
        dest[6]  = (src[0] >> 6) & 0x1;
        dest[7]  = (src[0] >> 7) & 0x1;
        dest[8]  = (src[0] >> 8) & 0x1;
        dest[9]  = (src[0] >> 9) & 0x1;
        dest[10] = (src[0] >> 10) & 0x1;
        dest[11] = (src[0] >> 11) & 0x1;
        dest[12] = (src[0] >> 12) & 0x1;
        dest[13] = (src[0] >> 13) & 0x1;
        dest[14] = (src[0] >> 14) & 0x1;
        dest[15] = (src[0] >> 15) & 0x1;
        dest[16] = (src[0] >> 16) & 0x1;
        dest[17] = (src[0] >> 17) & 0x1;
        dest[18] = (src[0] >> 18) & 0x1;
        dest[19] = (src[0] >> 19) & 0x1;
        dest[20] = (src[0] >> 20) & 0x1;
        dest[21] = (src[0] >> 21) & 0x1;
        dest[22] = (src[0] >> 22) & 0x1;
        dest[23] = (src[0] >> 23) & 0x1;
        dest[24] = (src[0] >> 24) & 0x1;
        dest[25] = (src[0] >> 25) & 0x1;
        dest[26] = (src[0] >> 26) & 0x1;
        dest[27] = (src[0] >> 27) & 0x1;
        dest[28] = (src[0] >> 28) & 0x1;
        dest[29] = (src[0] >> 29) & 0x1;
        dest[30] = (src[0] >> 30) & 0x1;
        dest[31] = (src[0] >> 31) & 0x1;
    }
    if (n & 31)
    {
        unpackTail1(dest, src, (n & 31));
    }
}

void Unpack::unpack2(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 2)
    {
        dest[0] = src[0] & 0x3;
        dest[1] = (src[0] >> 2) & 0x3;
        dest[2] = (src[0] >> 4) & 0x3;
        dest[3] = (src[0] >> 6) & 0x3;
        dest[4] = (src[0] >> 8) & 0x3;
        dest[5] = (src[0] >> 10) & 0x3;
        dest[6] = (src[0] >> 12) & 0x3;
        dest[7] = (src[0] >> 14) & 0x3;
        dest[8] = (src[0] >> 16) & 0x3;
        dest[9] = (src[0] >> 18) & 0x3;
        dest[10] = (src[0] >> 20) & 0x3;
        dest[11] = (src[0] >> 22) & 0x3;
        dest[12] = (src[0] >> 24) & 0x3;
        dest[13] = (src[0] >> 26) & 0x3;
        dest[14] = (src[0] >> 28) & 0x3;
        dest[15] = (src[0] >> 30) & 0x3;
        dest[16] = src[1] & 0x3;
        dest[17] = (src[1] >> 2) & 0x3;
        dest[18] = (src[1] >> 4) & 0x3;
        dest[19] = (src[1] >> 6) & 0x3;
        dest[20] = (src[1] >> 8) & 0x3;
        dest[21] = (src[1] >> 10) & 0x3;
        dest[22] = (src[1] >> 12) & 0x3;
        dest[23] = (src[1] >> 14) & 0x3;
        dest[24] = (src[1] >> 16) & 0x3;
        dest[25] = (src[1] >> 18) & 0x3;
        dest[26] = (src[1] >> 20) & 0x3;
        dest[27] = (src[1] >> 22) & 0x3;
        dest[28] = (src[1] >> 24) & 0x3;
        dest[29] = (src[1] >> 26) & 0x3;
        dest[30] = (src[1] >> 28) & 0x3;
        dest[31] = (src[1] >> 30) & 0x3;
    }

    if (n & 31)
    {
        unpackTail2(dest, src, (n & 31));
    }
}

void Unpack::unpack3(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; dest += 32, i += 32, src += 3)
    {
        dest[0] = src[0] & 0x7;
        dest[1] = (src[0] >> 3) & 0x7;
        dest[2] = (src[0] >> 6) & 0x7;
        dest[3] = (src[0] >> 9) & 0x7;
        dest[4] = (src[0] >> 12) & 0x7;
        dest[5] = (src[0] >> 15) & 0x7;
        dest[6] = (src[0] >> 18) & 0x7;
        dest[7] = (src[0] >> 21) & 0x7;
        dest[8] = (src[0] >> 24) & 0x7;
        dest[9] = (src[0] >> 27) & 0x7;
        dest[10] = ((src[0] >> 30) | (src[1] << 2)) & 0x7;
        dest[11] = (src[1] >> 1) & 0x7;
        dest[12] = (src[1] >> 4) & 0x7;
        dest[13] = (src[1] >> 7) & 0x7;
        dest[14] = (src[1] >> 10) & 0x7;
        dest[15] = (src[1] >> 13) & 0x7;
        dest[16] = (src[1] >> 16) & 0x7;
        dest[17] = (src[1] >> 19) & 0x7;
        dest[18] = (src[1] >> 22) & 0x7;
        dest[19] = (src[1] >> 25) & 0x7;
        dest[20] = (src[1] >> 28) & 0x7;
        dest[21] = ((src[1] >> 31) | (src[2] << 1)) & 0x7;
        dest[22] = (src[2] >> 2) & 0x7;
        dest[23] = (src[2] >> 5) & 0x7;
        dest[24] = (src[2] >> 8) & 0x7;
        dest[25] = (src[2] >> 11) & 0x7;
        dest[26] = (src[2] >> 14) & 0x7;
        dest[27] = (src[2] >> 17) & 0x7;
        dest[28] = (src[2] >> 20) & 0x7;
        dest[29] = (src[2] >> 23) & 0x7;
        dest[30] = (src[2] >> 26) & 0x7;
        dest[31] = (src[2] >> 29) & 0x7;
    }

    if (n & 31)
    {
        unpackTail3(dest, src, (n & 31));
    }
}


void Unpack::unpack4(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 4)
    {
        dest[0] = src[0] & 0xF;
        dest[1] = (src[0] >> 4) & 0xF;
        dest[2] = (src[0] >> 8) & 0xF;
        dest[3] = (src[0] >> 12) & 0xF;
        dest[4] = (src[0] >> 16) & 0xF;
        dest[5] = (src[0] >> 20) & 0xF;
        dest[6] = (src[0] >> 24) & 0xF;
        dest[7] = (src[0] >> 28) & 0xF;
        dest[8] = src[1] & 0xF;
        dest[9] = (src[1] >> 4) & 0xF;
        dest[10] = (src[1] >> 8) & 0xF;
        dest[11] = (src[1] >> 12) & 0xF;
        dest[12] = (src[1] >> 16) & 0xF;
        dest[13] = (src[1] >> 20) & 0xF;
        dest[14] = (src[1] >> 24) & 0xF;
        dest[15] = (src[1] >> 28) & 0xF;
        dest[16] = src[2] & 0xF;
        dest[17] = (src[2] >> 4) & 0xF;
        dest[18] = (src[2] >> 8) & 0xF;
        dest[19] = (src[2] >> 12) & 0xF;
        dest[20] = (src[2] >> 16) & 0xF;
        dest[21] = (src[2] >> 20) & 0xF;
        dest[22] = (src[2] >> 24) & 0xF;
        dest[23] = (src[2] >> 28) & 0xF;
        dest[24] = src[3] & 0xF;
        dest[25] = (src[3] >> 4) & 0xF;
        dest[26] = (src[3] >> 8) & 0xF;
        dest[27] = (src[3] >> 12) & 0xF;
        dest[28] = (src[3] >> 16) & 0xF;
        dest[29] = (src[3] >> 20) & 0xF;
        dest[30] = (src[3] >> 24) & 0xF;
        dest[31] = (src[3] >> 28) & 0xF;
    }

    if (n & 31)
    {
        unpackTail4(dest, src, (n & 31));
    }
}


void Unpack::unpack5(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 5)
    {
        dest[0] = src[0] & 0x1F;
        dest[1] = (src[0] >> 5) & 0x1F;
        dest[2] = (src[0] >> 10) & 0x1F;
        dest[3] = (src[0] >> 15) & 0x1F;
        dest[4] = (src[0] >> 20) & 0x1F;
        dest[5] = (src[0] >> 25) & 0x1F;
        dest[6] = ((src[0] >> 30) | (src[1] << 2)) & 0x1F;
        dest[7] = (src[1] >> 3) & 0x1F;
        dest[8] = (src[1] >> 8) & 0x1F;
        dest[9] = (src[1] >> 13) & 0x1F;
        dest[10] = (src[1] >> 18) & 0x1F;
        dest[11] = (src[1] >> 23) & 0x1F;
        dest[12] = ((src[1] >> 28) | (src[2] << 4)) & 0x1F;
        dest[13] = (src[2] >> 1) & 0x1F;
        dest[14] = (src[2] >> 6) & 0x1F;
        dest[15] = (src[2] >> 11) & 0x1F;
        dest[16] = (src[2] >> 16) & 0x1F;
        dest[17] = (src[2] >> 21) & 0x1F;
        dest[18] = (src[2] >> 26) & 0x1F;
        dest[19] = ((src[2] >> 31) | (src[3] << 1)) & 0x1F;
        dest[20] = (src[3] >> 4) & 0x1F;
        dest[21] = (src[3] >> 9) & 0x1F;
        dest[22] = (src[3] >> 14) & 0x1F;
        dest[23] = (src[3] >> 19) & 0x1F;
        dest[24] = (src[3] >> 24) & 0x1F;
        dest[25] = ((src[3] >> 29) | (src[4] << 3)) & 0x1F;
        dest[26] = (src[4] >> 2) & 0x1F;
        dest[27] = (src[4] >> 7) & 0x1F;
        dest[28] = (src[4] >> 12) & 0x1F;
        dest[29] = (src[4] >> 17) & 0x1F;
        dest[30] = (src[4] >> 22) & 0x1F;
        dest[31] = (src[4] >> 27) & 0x1F;
    }

    if (n & 31)
    {
        unpackTail5(dest, src, (n & 31));
    }
}

void Unpack::unpack6(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, src += 6, dest += 32)
    {
        dest[0] = src[0] & 0x3F;
        dest[1] = (src[0] >> 6) & 0x3F;
        dest[2] = (src[0] >> 12) & 0x3F;
        dest[3] = (src[0] >> 18) & 0x3F;
        dest[4] = (src[0] >> 24) & 0x3F;
        dest[5] = ((src[0] >> 30) | (src[1] << 2)) & 0x3F;
        dest[6] = (src[1] >> 4) & 0x3F;
        dest[7] = (src[1] >> 10) & 0x3F;
        dest[8] = (src[1] >> 16) & 0x3F;
        dest[9] = (src[1] >> 22) & 0x3F;
        dest[10] = ((src[1] >> 28) | (src[2] << 4)) & 0x3F;
        dest[11] = (src[2] >> 2) & 0x3F;
        dest[12] = (src[2] >> 8) & 0x3F;
        dest[13] = (src[2] >> 14) & 0x3F;
        dest[14] = (src[2] >> 20) & 0x3F;
        dest[15] = (src[2] >> 26) & 0x3F;
        dest[16] = src[3] & 0x3F;
        dest[17] = (src[3] >> 6) & 0x3F;
        dest[18] = (src[3] >> 12) & 0x3F;
        dest[19] = (src[3] >> 18) & 0x3F;
        dest[20] = (src[3] >> 24) & 0x3F;
        dest[21] = ((src[3] >> 30) | (src[4] << 2)) & 0x3F;
        dest[22] = (src[4] >> 4) & 0x3F;
        dest[23] = (src[4] >> 10) & 0x3F;
        dest[24] = (src[4] >> 16) & 0x3F;
        dest[25] = (src[4] >> 22) & 0x3F;
        dest[26] = ((src[4] >> 28) | (src[5] << 4)) & 0x3F;
        dest[27] = (src[5] >> 2) & 0x3F;
        dest[28] = (src[5] >> 8) & 0x3F;
        dest[29] = (src[5] >> 14) & 0x3F;
        dest[30] = (src[5] >> 20) & 0x3F;
        dest[31] = (src[5] >> 26) & 0x3F;
    }

    if (n & 31)
    {
        unpackTail6(dest, src, (n & 31));
    }
}

void Unpack::unpack7(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 7) 
    {
        dest[0] = src[0] & 0x7F;
        dest[1] = (src[0] >> 7) & 0x7F;
        dest[2] = (src[0] >> 14) & 0x7F;
        dest[3] = (src[0] >> 21) & 0x7F;
        dest[4] = ((src[0] >> 28) | (src[1] << 4)) & 0x7F;
        dest[5] = (src[1] >> 3) & 0x7F;
        dest[6] = (src[1] >> 10) & 0x7F;
        dest[7] = (src[1] >> 17) & 0x7F;
        dest[8] = (src[1] >> 24) & 0x7F;
        dest[9] = ((src[1] >> 31) | (src[2] << 1)) & 0x7F;
        dest[10] = (src[2] >> 6) & 0x7F;
        dest[11] = (src[2] >> 13) & 0x7F;
        dest[12] = (src[2] >> 20) & 0x7F;
        dest[13] = ((src[2] >> 27) | (src[3] << 5)) & 0x7F;
        dest[14] = (src[3] >> 2) & 0x7F;
        dest[15] = (src[3] >> 9) & 0x7F;
        dest[16] = (src[3] >> 16) & 0x7F;
        dest[17] = (src[3] >> 23) & 0x7F;
        dest[18] = ((src[3] >> 30) | (src[4] << 2)) & 0x7F;
        dest[19] = (src[4] >> 5) & 0x7F;
        dest[20] = (src[4] >> 12) & 0x7F;
        dest[21] = (src[4] >> 19) & 0x7F;
        dest[22] = ((src[4] >> 26) | (src[5] << 6)) & 0x7F;
        dest[23] = (src[5] >> 1) & 0x7F;
        dest[24] = (src[5] >> 8) & 0x7F;
        dest[25] = (src[5] >> 15) & 0x7F;
        dest[26] = (src[5] >> 22) & 0x7F;
        dest[27] = ((src[5] >> 29) | (src[6] << 3)) & 0x7F;
        dest[28] = (src[6] >> 4) & 0x7F;
        dest[29] = (src[6] >> 11) & 0x7F;
        dest[30] = (src[6] >> 18) & 0x7F;
        dest[31] = (src[6] >> 25) & 0x7F;
    }

    if (n & 31)
    {
        unpackTail7(dest, src, (n & 31));
    }
}

void Unpack::unpack8(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 8) 
    {
        dest[0] = src[0] & 0xFF;
        dest[1] = (src[0] >> 8) & 0xFF;
        dest[2] = (src[0] >> 16) & 0xFF;
        dest[3] = (src[0] >> 24) & 0xFF;
        dest[4] = src[1] & 0xFF;
        dest[5] = (src[1] >> 8) & 0xFF;
        dest[6] = (src[1] >> 16) & 0xFF;
        dest[7] = (src[1] >> 24) & 0xFF;
        dest[8] = src[2] & 0xFF;
        dest[9] = (src[2] >> 8) & 0xFF;
        dest[10] = (src[2] >> 16) & 0xFF;
        dest[11] = (src[2] >> 24) & 0xFF;
        dest[12] = src[3] & 0xFF;
        dest[13] = (src[3] >> 8) & 0xFF;
        dest[14] = (src[3] >> 16) & 0xFF;
        dest[15] = (src[3] >> 24) & 0xFF;
        dest[16] = src[4] & 0xFF;
        dest[17] = (src[4] >> 8) & 0xFF;
        dest[18] = (src[4] >> 16) & 0xFF;
        dest[19] = (src[4] >> 24) & 0xFF;
        dest[20] = src[5] & 0xFF;
        dest[21] = (src[5] >> 8) & 0xFF;
        dest[22] = (src[5] >> 16) & 0xFF;
        dest[23] = (src[5] >> 24) & 0xFF;
        dest[24] = src[6] & 0xFF;
        dest[25] = (src[6] >> 8) & 0xFF;
        dest[26] = (src[6] >> 16) & 0xFF;
        dest[27] = (src[6] >> 24) & 0xFF;
        dest[28] = src[7] & 0xFF;
        dest[29] = (src[7] >> 8) & 0xFF;
        dest[30] = (src[7] >> 16) & 0xFF;
        dest[31] = (src[7] >> 24) & 0xFF;
    }

    if (n & 31)
    {
        unpackTail8(dest, src, (n & 31));
    }
}

void Unpack::unpack9(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 9) 
    {
        dest[0] = src[0] & 0x1FF;
        dest[1] = (src[0] >> 9) & 0x1FF;
        dest[2] = (src[0] >> 18) & 0x1FF;
        dest[3] = ((src[0] >> 27) | (src[1] << 5)) & 0x1FF;
        dest[4] = (src[1] >> 4) & 0x1FF;
        dest[5] = (src[1] >> 13) & 0x1FF;
        dest[6] = (src[1] >> 22) & 0x1FF;
        dest[7] = ((src[1] >> 31) | (src[2] << 1)) & 0x1FF;
        dest[8] = (src[2] >> 8) & 0x1FF;
        dest[9] = (src[2] >> 17) & 0x1FF;
        dest[10] = ((src[2] >> 26) | (src[3] << 6)) & 0x1FF;
        dest[11] = (src[3] >> 3) & 0x1FF;
        dest[12] = (src[3] >> 12) & 0x1FF;
        dest[13] = (src[3] >> 21) & 0x1FF;
        dest[14] = ((src[3] >> 30) | (src[4] << 2)) & 0x1FF;
        dest[15] = (src[4] >> 7) & 0x1FF;
        dest[16] = (src[4] >> 16) & 0x1FF;
        dest[17] = ((src[4] >> 25) | (src[5] << 7)) & 0x1FF;
        dest[18] = (src[5] >> 2) & 0x1FF;
        dest[19] = (src[5] >> 11) & 0x1FF;
        dest[20] = (src[5] >> 20) & 0x1FF;
        dest[21] = ((src[5] >> 29) | (src[6] << 3)) & 0x1FF;
        dest[22] = (src[6] >> 6) & 0x1FF;
        dest[23] = (src[6] >> 15) & 0x1FF;
        dest[24] = ((src[6] >> 24) | (src[7] << 8)) & 0x1FF;
        dest[25] = (src[7] >> 1) & 0x1FF;
        dest[26] = (src[7] >> 10) & 0x1FF;
        dest[27] = (src[7] >> 19) & 0x1FF;
        dest[28] = ((src[7] >> 28) | (src[8] << 4)) & 0x1FF;
        dest[29] = (src[8] >> 5) & 0x1FF;
        dest[30] = (src[8] >> 14) & 0x1FF;
        dest[31] = (src[8] >> 23) & 0x1FF;
    }

    if (n & 31)
    {
        unpackTail9(dest, src, (n & 31));
    }
}

void Unpack::unpack10(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 10) 
    {
        dest[0] = src[0] & 0x3FF;
        dest[1] = (src[0] >> 10) & 0x3FF;
        dest[2] = (src[0] >> 20) & 0x3FF;
        dest[3] = ((src[0] >> 30) | (src[1] << 2)) & 0x3FF;
        dest[4] = (src[1] >> 8) & 0x3FF;
        dest[5] = (src[1] >> 18) & 0x3FF;
        dest[6] = ((src[1] >> 28) | (src[2] << 4)) & 0x3FF;
        dest[7] = (src[2] >> 6) & 0x3FF;
        dest[8] = (src[2] >> 16) & 0x3FF;
        dest[9] = ((src[2] >> 26) | (src[3] << 6)) & 0x3FF;
        dest[10] = (src[3] >> 4) & 0x3FF;
        dest[11] = (src[3] >> 14) & 0x3FF;
        dest[12] = ((src[3] >> 24) | (src[4] << 8)) & 0x3FF;
        dest[13] = (src[4] >> 2) & 0x3FF;
        dest[14] = (src[4] >> 12) & 0x3FF;
        dest[15] = (src[4] >> 22) & 0x3FF;
        dest[16] = src[5] & 0x3FF;
        dest[17] = (src[5] >> 10) & 0x3FF;
        dest[18] = (src[5] >> 20) & 0x3FF;
        dest[19] = ((src[5] >> 30) | (src[6] << 2)) & 0x3FF;
        dest[20] = (src[6] >> 8) & 0x3FF;
        dest[21] = (src[6] >> 18) & 0x3FF;
        dest[22] = ((src[6] >> 28) | (src[7] << 4)) & 0x3FF;
        dest[23] = (src[7] >> 6) & 0x3FF;
        dest[24] = (src[7] >> 16) & 0x3FF;
        dest[25] = ((src[7] >> 26) | (src[8] << 6)) & 0x3FF;
        dest[26] = (src[8] >> 4) & 0x3FF;
        dest[27] = (src[8] >> 14) & 0x3FF;
        dest[28] = ((src[8] >> 24) | (src[9] << 8)) & 0x3FF;
        dest[29] = (src[9] >> 2) & 0x3FF;
        dest[30] = (src[9] >> 12) & 0x3FF;
        dest[31] = (src[9] >> 22) & 0x3FF;
    }

    if (n & 31)
    {
        unpackTail10(dest, src, (n & 31));
    }
}

void Unpack::unpack11(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 11) 
    {
        dest[0] = src[0] & 0x7FF;
        dest[1] = (src[0] >> 11) & 0x7FF;
        dest[2] = ((src[0] >> 22) | (src[1] << 10)) & 0x7FF;
        dest[3] = (src[1] >> 1) & 0x7FF;
        dest[4] = (src[1] >> 12) & 0x7FF;
        dest[5] = ((src[1] >> 23) | (src[2] << 9)) & 0x7FF;
        dest[6] = (src[2] >> 2) & 0x7FF;
        dest[7] = (src[2] >> 13) & 0x7FF;
        dest[8] = ((src[2] >> 24) | (src[3] << 8)) & 0x7FF;
        dest[9] = (src[3] >> 3) & 0x7FF;
        dest[10] = (src[3] >> 14) & 0x7FF;
        dest[11] = ((src[3] >> 25) | (src[4] << 7)) & 0x7FF;
        dest[12] = (src[4] >> 4) & 0x7FF;
        dest[13] = (src[4] >> 15) & 0x7FF;
        dest[14] = ((src[4] >> 26) | (src[5] << 6)) & 0x7FF;
        dest[15] = (src[5] >> 5) & 0x7FF;
        dest[16] = (src[5] >> 16) & 0x7FF;
        dest[17] = ((src[5] >> 27) | (src[6] << 5)) & 0x7FF;
        dest[18] = (src[6] >> 6) & 0x7FF;
        dest[19] = (src[6] >> 17) & 0x7FF;
        dest[20] = ((src[6] >> 28) | (src[7] << 4)) & 0x7FF;
        dest[21] = (src[7] >> 7) & 0x7FF;
        dest[22] = (src[7] >> 18) & 0x7FF;
        dest[23] = ((src[7] >> 29) | (src[8] << 3)) & 0x7FF;
        dest[24] = (src[8] >> 8) & 0x7FF;
        dest[25] = (src[8] >> 19) & 0x7FF;
        dest[26] = ((src[8] >> 30) | (src[9] << 2)) & 0x7FF;
        dest[27] = (src[9] >> 9) & 0x7FF;
        dest[28] = (src[9] >> 20) & 0x7FF;
        dest[29] = ((src[9] >> 31) | (src[10] << 1)) & 0x7FF;
        dest[30] = (src[10] >> 10) & 0x7FF;
        dest[31] = (src[10] >> 21) & 0x7FF;
    }

    if (n & 31)
    {
        unpackTail11(dest, src, (n & 31));
    }
}

void Unpack::unpack12(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 12) 
    {
        dest[0] = src[0] & 0xFFF;
        dest[1] = (src[0] >> 12) & 0xFFF;
        dest[2] = ((src[0] >> 24) | (src[1] << 8)) & 0xFFF;
        dest[3] = (src[1] >> 4) & 0xFFF;
        dest[4] = (src[1] >> 16) & 0xFFF;
        dest[5] = ((src[1] >> 28) | (src[2] << 4)) & 0xFFF;
        dest[6] = (src[2] >> 8) & 0xFFF;
        dest[7] = (src[2] >> 20) & 0xFFF;
        dest[8] = src[3] & 0xFFF;
        dest[9] = (src[3] >> 12) & 0xFFF;
        dest[10] = ((src[3] >> 24) | (src[4] << 8)) & 0xFFF;
        dest[11] = (src[4] >> 4) & 0xFFF;
        dest[12] = (src[4] >> 16) & 0xFFF;
        dest[13] = ((src[4] >> 28) | (src[5] << 4)) & 0xFFF;
        dest[14] = (src[5] >> 8) & 0xFFF;
        dest[15] = (src[5] >> 20) & 0xFFF;
        dest[16] = src[6] & 0xFFF;
        dest[17] = (src[6] >> 12) & 0xFFF;
        dest[18] = ((src[6] >> 24) | (src[7] << 8)) & 0xFFF;
        dest[19] = (src[7] >> 4) & 0xFFF;
        dest[20] = (src[7] >> 16) & 0xFFF;
        dest[21] = ((src[7] >> 28) | (src[8] << 4)) & 0xFFF;
        dest[22] = (src[8] >> 8) & 0xFFF;
        dest[23] = (src[8] >> 20) & 0xFFF;
        dest[24] = src[9] & 0xFFF;
        dest[25] = (src[9] >> 12) & 0xFFF;
        dest[26] = ((src[9] >> 24) | (src[10] << 8)) & 0xFFF;
        dest[27] = (src[10] >> 4) & 0xFFF;
        dest[28] = (src[10] >> 16) & 0xFFF;
        dest[29] = ((src[10] >> 28) | (src[11] << 4)) & 0xFFF;
        dest[30] = (src[11] >> 8) & 0xFFF;
        dest[31] = (src[11] >> 20) & 0xFFF;
    }

    if (n & 31)
    {
        unpackTail12(dest, src, (n & 31));
    }
}

void Unpack::unpack13(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 13) 
    {
        dest[0] = src[0] & 0x1FFF;
        dest[1] = (src[0] >> 13) & 0x1FFF;
        dest[2] = ((src[0] >> 26) | (src[1] << 6)) & 0x1FFF;
        dest[3] = (src[1] >> 7) & 0x1FFF;
        dest[4] = ((src[1] >> 20) | (src[2] << 12)) & 0x1FFF;
        dest[5] = (src[2] >> 1) & 0x1FFF;
        dest[6] = (src[2] >> 14) & 0x1FFF;
        dest[7] = ((src[2] >> 27) | (src[3] << 5)) & 0x1FFF;
        dest[8] = (src[3] >> 8) & 0x1FFF;
        dest[9] = ((src[3] >> 21) | (src[4] << 11)) & 0x1FFF;
        dest[10] = (src[4] >> 2) & 0x1FFF;
        dest[11] = (src[4] >> 15) & 0x1FFF;
        dest[12] = ((src[4] >> 28) | (src[5] << 4)) & 0x1FFF;
        dest[13] = (src[5] >> 9) & 0x1FFF;
        dest[14] = ((src[5] >> 22) | (src[6] << 10)) & 0x1FFF;
        dest[15] = (src[6] >> 3) & 0x1FFF;
        dest[16] = (src[6] >> 16) & 0x1FFF;
        dest[17] = ((src[6] >> 29) | (src[7] << 3)) & 0x1FFF;
        dest[18] = (src[7] >> 10) & 0x1FFF;
        dest[19] = ((src[7] >> 23) | (src[8] << 9)) & 0x1FFF;
        dest[20] = (src[8] >> 4) & 0x1FFF;
        dest[21] = (src[8] >> 17) & 0x1FFF;
        dest[22] = ((src[8] >> 30) | (src[9] << 2)) & 0x1FFF;
        dest[23] = (src[9] >> 11) & 0x1FFF;
        dest[24] = ((src[9] >> 24) | (src[10] << 8)) & 0x1FFF;
        dest[25] = (src[10] >> 5) & 0x1FFF;
        dest[26] = (src[10] >> 18) & 0x1FFF;
        dest[27] = ((src[10] >> 31) | (src[11] << 1)) & 0x1FFF;
        dest[28] = (src[11] >> 12) & 0x1FFF;
        dest[29] = ((src[11] >> 25) | (src[12] << 7)) & 0x1FFF;
        dest[30] = (src[12] >> 6) & 0x1FFF;
        dest[31] = (src[12] >> 19) & 0x1FFF;
    }

    if (n & 31)
    {
        unpackTail13(dest, src, (n & 31));
    }
}

void Unpack::unpack14(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 14) 
    {
        dest[0] = src[0] & 0x3FFF;
        dest[1] = (src[0] >> 14) & 0x3FFF;
        dest[2] = ((src[0] >> 28) | (src[1] << 4)) & 0x3FFF;
        dest[3] = (src[1] >> 10) & 0x3FFF;
        dest[4] = ((src[1] >> 24) | (src[2] << 8)) & 0x3FFF;
        dest[5] = (src[2] >> 6) & 0x3FFF;
        dest[6] = ((src[2] >> 20) | (src[3] << 12)) & 0x3FFF;
        dest[7] = (src[3] >> 2) & 0x3FFF;
        dest[8] = (src[3] >> 16) & 0x3FFF;
        dest[9] = ((src[3] >> 30) | (src[4] << 2)) & 0x3FFF;
        dest[10] = (src[4] >> 12) & 0x3FFF;
        dest[11] = ((src[4] >> 26) | (src[5] << 6)) & 0x3FFF;
        dest[12] = (src[5] >> 8) & 0x3FFF;
        dest[13] = ((src[5] >> 22) | (src[6] << 10)) & 0x3FFF;
        dest[14] = (src[6] >> 4) & 0x3FFF;
        dest[15] = (src[6] >> 18) & 0x3FFF;
        dest[16] = src[7] & 0x3FFF;
        dest[17] = (src[7] >> 14) & 0x3FFF;
        dest[18] = ((src[7] >> 28) | (src[8] << 4)) & 0x3FFF;
        dest[19] = (src[8] >> 10) & 0x3FFF;
        dest[20] = ((src[8] >> 24) | (src[9] << 8)) & 0x3FFF;
        dest[21] = (src[9] >> 6) & 0x3FFF;
        dest[22] = ((src[9] >> 20) | (src[10] << 12)) & 0x3FFF;
        dest[23] = (src[10] >> 2) & 0x3FFF;
        dest[24] = (src[10] >> 16) & 0x3FFF;
        dest[25] = ((src[10] >> 30) | (src[11] << 2)) & 0x3FFF;
        dest[26] = (src[11] >> 12) & 0x3FFF;
        dest[27] = ((src[11] >> 26) | (src[12] << 6)) & 0x3FFF;
        dest[28] = (src[12] >> 8) & 0x3FFF;
        dest[29] = ((src[12] >> 22) | (src[13] << 10)) & 0x3FFF;
        dest[30] = (src[13] >> 4) & 0x3FFF;
        dest[31] = (src[13] >> 18) & 0x3FFF;
    }

    if (n & 31)
    {
        unpackTail14(dest, src, (n & 31));
    }
}

void Unpack::unpack15(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 15) 
    {
        dest[0] = src[0] & 0x7FFF;
        dest[1] = (src[0] >> 15) & 0x7FFF;
        dest[2] = ((src[0] >> 30) | (src[1] << 2)) & 0x7FFF;
        dest[3] = (src[1] >> 13) & 0x7FFF;
        dest[4] = ((src[1] >> 28) | (src[2] << 4)) & 0x7FFF;
        dest[5] = (src[2] >> 11) & 0x7FFF;
        dest[6] = ((src[2] >> 26) | (src[3] << 6)) & 0x7FFF;
        dest[7] = (src[3] >> 9) & 0x7FFF;
        dest[8] = ((src[3] >> 24) | (src[4] << 8)) & 0x7FFF;
        dest[9] = (src[4] >> 7) & 0x7FFF;
        dest[10] = ((src[4] >> 22) | (src[5] << 10)) & 0x7FFF;
        dest[11] = (src[5] >> 5) & 0x7FFF;
        dest[12] = ((src[5] >> 20) | (src[6] << 12)) & 0x7FFF;
        dest[13] = (src[6] >> 3) & 0x7FFF;
        dest[14] = ((src[6] >> 18) | (src[7] << 14)) & 0x7FFF;
        dest[15] = (src[7] >> 1) & 0x7FFF;
        dest[16] = (src[7] >> 16) & 0x7FFF;
        dest[17] = ((src[7] >> 31) | (src[8] << 1)) & 0x7FFF;
        dest[18] = (src[8] >> 14) & 0x7FFF;
        dest[19] = ((src[8] >> 29) | (src[9] << 3)) & 0x7FFF;
        dest[20] = (src[9] >> 12) & 0x7FFF;
        dest[21] = ((src[9] >> 27) | (src[10] << 5)) & 0x7FFF;
        dest[22] = (src[10] >> 10) & 0x7FFF;
        dest[23] = ((src[10] >> 25) | (src[11] << 7)) & 0x7FFF;
        dest[24] = (src[11] >> 8) & 0x7FFF;
        dest[25] = ((src[11] >> 23) | (src[12] << 9)) & 0x7FFF;
        dest[26] = (src[12] >> 6) & 0x7FFF;
        dest[27] = ((src[12] >> 21) | (src[13] << 11)) & 0x7FFF;
        dest[28] = (src[13] >> 4) & 0x7FFF;
        dest[29] = ((src[13] >> 19) | (src[14] << 13)) & 0x7FFF;
        dest[30] = (src[14] >> 2) & 0x7FFF;
        dest[31] = (src[14] >> 17) & 0x7FFF;
    }

    if (n & 31)
    {
        unpackTail15(dest, src, (n & 31));
    }
}

void Unpack::unpack16(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 16) 
    {
        dest[0] = src[0] & 0xFFFF;
        dest[1] = (src[0] >> 16) & 0xFFFF;
        dest[2] = src[1] & 0xFFFF;
        dest[3] = (src[1] >> 16) & 0xFFFF;
        dest[4] = src[2] & 0xFFFF;
        dest[5] = (src[2] >> 16) & 0xFFFF;
        dest[6] = src[3] & 0xFFFF;
        dest[7] = (src[3] >> 16) & 0xFFFF;
        dest[8] = src[4] & 0xFFFF;
        dest[9] = (src[4] >> 16) & 0xFFFF;
        dest[10] = src[5] & 0xFFFF;
        dest[11] = (src[5] >> 16) & 0xFFFF;
        dest[12] = src[6] & 0xFFFF;
        dest[13] = (src[6] >> 16) & 0xFFFF;
        dest[14] = src[7] & 0xFFFF;
        dest[15] = (src[7] >> 16) & 0xFFFF;
        dest[16] = src[8] & 0xFFFF;
        dest[17] = (src[8] >> 16) & 0xFFFF;
        dest[18] = src[9] & 0xFFFF;
        dest[19] = (src[9] >> 16) & 0xFFFF;
        dest[20] = src[10] & 0xFFFF;
        dest[21] = (src[10] >> 16) & 0xFFFF;
        dest[22] = src[11] & 0xFFFF;
        dest[23] = (src[11] >> 16) & 0xFFFF;
        dest[24] = src[12] & 0xFFFF;
        dest[25] = (src[12] >> 16) & 0xFFFF;
        dest[26] = src[13] & 0xFFFF;
        dest[27] = (src[13] >> 16) & 0xFFFF;
        dest[28] = src[14] & 0xFFFF;
        dest[29] = (src[14] >> 16) & 0xFFFF;
        dest[30] = src[15] & 0xFFFF;
        dest[31] = (src[15] >> 16) & 0xFFFF;
    }

    if (n & 31)
    {
        unpackTail16(dest, src, (n & 31));
    }
}

void Unpack::unpack17(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 17) 
    {
        dest[0] = src[0] & 0x1FFFF;
        dest[1] = ((src[0] >> 17) | (src[1] << 15)) & 0x1FFFF;
        dest[2] = (src[1] >> 2) & 0x1FFFF;
        dest[3] = ((src[1] >> 19) | (src[2] << 13)) & 0x1FFFF;
        dest[4] = (src[2] >> 4) & 0x1FFFF;
        dest[5] = ((src[2] >> 21) | (src[3] << 11)) & 0x1FFFF;
        dest[6] = (src[3] >> 6) & 0x1FFFF;
        dest[7] = ((src[3] >> 23) | (src[4] << 9)) & 0x1FFFF;
        dest[8] = (src[4] >> 8) & 0x1FFFF;
        dest[9] = ((src[4] >> 25) | (src[5] << 7)) & 0x1FFFF;
        dest[10] = (src[5] >> 10) & 0x1FFFF;
        dest[11] = ((src[5] >> 27) | (src[6] << 5)) & 0x1FFFF;
        dest[12] = (src[6] >> 12) & 0x1FFFF;
        dest[13] = ((src[6] >> 29) | (src[7] << 3)) & 0x1FFFF;
        dest[14] = (src[7] >> 14) & 0x1FFFF;
        dest[15] = ((src[7] >> 31) | (src[8] << 1)) & 0x1FFFF;
        dest[16] = ((src[8] >> 16) | (src[9] << 16)) & 0x1FFFF;
        dest[17] = (src[9] >> 1) & 0x1FFFF;
        dest[18] = ((src[9] >> 18) | (src[10] << 14)) & 0x1FFFF;
        dest[19] = (src[10] >> 3) & 0x1FFFF;
        dest[20] = ((src[10] >> 20) | (src[11] << 12)) & 0x1FFFF;
        dest[21] = (src[11] >> 5) & 0x1FFFF;
        dest[22] = ((src[11] >> 22) | (src[12] << 10)) & 0x1FFFF;
        dest[23] = (src[12] >> 7) & 0x1FFFF;
        dest[24] = ((src[12] >> 24) | (src[13] << 8)) & 0x1FFFF;
        dest[25] = (src[13] >> 9) & 0x1FFFF;
        dest[26] = ((src[13] >> 26) | (src[14] << 6)) & 0x1FFFF;
        dest[27] = (src[14] >> 11) & 0x1FFFF;
        dest[28] = ((src[14] >> 28) | (src[15] << 4)) & 0x1FFFF;
        dest[29] = (src[15] >> 13) & 0x1FFFF;
        dest[30] = ((src[15] >> 30) | (src[16] << 2)) & 0x1FFFF;
        dest[31] = (src[16] >> 15) & 0x1FFFF;
    }

    if (n & 31)
    {
        unpackTail17(dest, src, (n & 31));
    }
}

void Unpack::unpack18(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 18) 
    {
        dest[0] = src[0] & 0x3FFFF;
        dest[1] = ((src[0] >> 18) | (src[1] << 14)) & 0x3FFFF;
        dest[2] = (src[1] >> 4) & 0x3FFFF;
        dest[3] = ((src[1] >> 22) | (src[2] << 10)) & 0x3FFFF;
        dest[4] = (src[2] >> 8) & 0x3FFFF;
        dest[5] = ((src[2] >> 26) | (src[3] << 6)) & 0x3FFFF;
        dest[6] = (src[3] >> 12) & 0x3FFFF;
        dest[7] = ((src[3] >> 30) | (src[4] << 2)) & 0x3FFFF;
        dest[8] = ((src[4] >> 16) | (src[5] << 16)) & 0x3FFFF;
        dest[9] = (src[5] >> 2) & 0x3FFFF;
        dest[10] = ((src[5] >> 20) | (src[6] << 12)) & 0x3FFFF;
        dest[11] = (src[6] >> 6) & 0x3FFFF;
        dest[12] = ((src[6] >> 24) | (src[7] << 8)) & 0x3FFFF;
        dest[13] = (src[7] >> 10) & 0x3FFFF;
        dest[14] = ((src[7] >> 28) | (src[8] << 4)) & 0x3FFFF;
        dest[15] = (src[8] >> 14) & 0x3FFFF;
        dest[16] = src[9] & 0x3FFFF;
        dest[17] = ((src[9] >> 18) | (src[10] << 14)) & 0x3FFFF;
        dest[18] = (src[10] >> 4) & 0x3FFFF;
        dest[19] = ((src[10] >> 22) | (src[11] << 10)) & 0x3FFFF;
        dest[20] = (src[11] >> 8) & 0x3FFFF;
        dest[21] = ((src[11] >> 26) | (src[12] << 6)) & 0x3FFFF;
        dest[22] = (src[12] >> 12) & 0x3FFFF;
        dest[23] = ((src[12] >> 30) | (src[13] << 2)) & 0x3FFFF;
        dest[24] = ((src[13] >> 16) | (src[14] << 16)) & 0x3FFFF;
        dest[25] = (src[14] >> 2) & 0x3FFFF;
        dest[26] = ((src[14] >> 20) | (src[15] << 12)) & 0x3FFFF;
        dest[27] = (src[15] >> 6) & 0x3FFFF;
        dest[28] = ((src[15] >> 24) | (src[16] << 8)) & 0x3FFFF;
        dest[29] = (src[16] >> 10) & 0x3FFFF;
        dest[30] = ((src[16] >> 28) | (src[17] << 4)) & 0x3FFFF;
        dest[31] = (src[17] >> 14) & 0x3FFFF;
    }

    if (n & 31)
    {
        unpackTail18(dest, src, (n & 31));
    }
}

void Unpack::unpack19(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 19) 
    {
        dest[0] = src[0] & 0x7FFFF;
        dest[1] = ((src[0] >> 19) | (src[1] << 13)) & 0x7FFFF;
        dest[2] = (src[1] >> 6) & 0x7FFFF;
        dest[3] = ((src[1] >> 25) | (src[2] << 7)) & 0x7FFFF;
        dest[4] = (src[2] >> 12) & 0x7FFFF;
        dest[5] = ((src[2] >> 31) | (src[3] << 1)) & 0x7FFFF;
        dest[6] = ((src[3] >> 18) | (src[4] << 14)) & 0x7FFFF;
        dest[7] = (src[4] >> 5) & 0x7FFFF;
        dest[8] = ((src[4] >> 24) | (src[5] << 8)) & 0x7FFFF;
        dest[9] = (src[5] >> 11) & 0x7FFFF;
        dest[10] = ((src[5] >> 30) | (src[6] << 2)) & 0x7FFFF;
        dest[11] = ((src[6] >> 17) | (src[7] << 15)) & 0x7FFFF;
        dest[12] = (src[7] >> 4) & 0x7FFFF;
        dest[13] = ((src[7] >> 23) | (src[8] << 9)) & 0x7FFFF;
        dest[14] = (src[8] >> 10) & 0x7FFFF;
        dest[15] = ((src[8] >> 29) | (src[9] << 3)) & 0x7FFFF;
        dest[16] = ((src[9] >> 16) | (src[10] << 16)) & 0x7FFFF;
        dest[17] = (src[10] >> 3) & 0x7FFFF;
        dest[18] = ((src[10] >> 22) | (src[11] << 10)) & 0x7FFFF;
        dest[19] = (src[11] >> 9) & 0x7FFFF;
        dest[20] = ((src[11] >> 28) | (src[12] << 4)) & 0x7FFFF;
        dest[21] = ((src[12] >> 15) | (src[13] << 17)) & 0x7FFFF;
        dest[22] = (src[13] >> 2) & 0x7FFFF;
        dest[23] = ((src[13] >> 21) | (src[14] << 11)) & 0x7FFFF;
        dest[24] = (src[14] >> 8) & 0x7FFFF;
        dest[25] = ((src[14] >> 27) | (src[15] << 5)) & 0x7FFFF;
        dest[26] = ((src[15] >> 14) | (src[16] << 18)) & 0x7FFFF;
        dest[27] = (src[16] >> 1) & 0x7FFFF;
        dest[28] = ((src[16] >> 20) | (src[17] << 12)) & 0x7FFFF;
        dest[29] = (src[17] >> 7) & 0x7FFFF;
        dest[30] = ((src[17] >> 26) | (src[18] << 6)) & 0x7FFFF;
        dest[31] = (src[18] >> 13) & 0x7FFFF;
    }

    if (n & 31)
    {
        unpackTail19(dest, src, (n & 31));
    }
}

void Unpack::unpack20(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 20) 
    {
        dest[0] = src[0] & 0xFFFFF;
        dest[1] = ((src[0] >> 20) | (src[1] << 12)) & 0xFFFFF;
        dest[2] = (src[1] >> 8) & 0xFFFFF;
        dest[3] = ((src[1] >> 28) | (src[2] << 4)) & 0xFFFFF;
        dest[4] = ((src[2] >> 16) | (src[3] << 16)) & 0xFFFFF;
        dest[5] = (src[3] >> 4) & 0xFFFFF;
        dest[6] = ((src[3] >> 24) | (src[4] << 8)) & 0xFFFFF;
        dest[7] = (src[4] >> 12) & 0xFFFFF;
        dest[8] = src[5] & 0xFFFFF;
        dest[9] = ((src[5] >> 20) | (src[6] << 12)) & 0xFFFFF;
        dest[10] = (src[6] >> 8) & 0xFFFFF;
        dest[11] = ((src[6] >> 28) | (src[7] << 4)) & 0xFFFFF;
        dest[12] = ((src[7] >> 16) | (src[8] << 16)) & 0xFFFFF;
        dest[13] = (src[8] >> 4) & 0xFFFFF;
        dest[14] = ((src[8] >> 24) | (src[9] << 8)) & 0xFFFFF;
        dest[15] = (src[9] >> 12) & 0xFFFFF;
        dest[16] = src[10] & 0xFFFFF;
        dest[17] = ((src[10] >> 20) | (src[11] << 12)) & 0xFFFFF;
        dest[18] = (src[11] >> 8) & 0xFFFFF;
        dest[19] = ((src[11] >> 28) | (src[12] << 4)) & 0xFFFFF;
        dest[20] = ((src[12] >> 16) | (src[13] << 16)) & 0xFFFFF;
        dest[21] = (src[13] >> 4) & 0xFFFFF;
        dest[22] = ((src[13] >> 24) | (src[14] << 8)) & 0xFFFFF;
        dest[23] = (src[14] >> 12) & 0xFFFFF;
        dest[24] = src[15] & 0xFFFFF;
        dest[25] = ((src[15] >> 20) | (src[16] << 12)) & 0xFFFFF;
        dest[26] = (src[16] >> 8) & 0xFFFFF;
        dest[27] = ((src[16] >> 28) | (src[17] << 4)) & 0xFFFFF;
        dest[28] = ((src[17] >> 16) | (src[18] << 16)) & 0xFFFFF;
        dest[29] = (src[18] >> 4) & 0xFFFFF;
        dest[30] = ((src[18] >> 24) | (src[19] << 8)) & 0xFFFFF;
        dest[31] = (src[19] >> 12) & 0xFFFFF;
    }

    if (n & 31)
    {
        unpackTail20(dest, src, (n & 31));
    }
}

void Unpack::unpack21(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 21) 
    {
        dest[0] = src[0] & 0x1FFFFF;
        dest[1] = ((src[0] >> 21) | (src[1] << 11)) & 0x1FFFFF;
        dest[2] = (src[1] >> 10) & 0x1FFFFF;
        dest[3] = ((src[1] >> 31) | (src[2] << 1)) & 0x1FFFFF;
        dest[4] = ((src[2] >> 20) | (src[3] << 12)) & 0x1FFFFF;
        dest[5] = (src[3] >> 9) & 0x1FFFFF;
        dest[6] = ((src[3] >> 30) | (src[4] << 2)) & 0x1FFFFF;
        dest[7] = ((src[4] >> 19) | (src[5] << 13)) & 0x1FFFFF;
        dest[8] = (src[5] >> 8) & 0x1FFFFF;
        dest[9] = ((src[5] >> 29) | (src[6] << 3)) & 0x1FFFFF;
        dest[10] = ((src[6] >> 18) | (src[7] << 14)) & 0x1FFFFF;
        dest[11] = (src[7] >> 7) & 0x1FFFFF;
        dest[12] = ((src[7] >> 28) | (src[8] << 4)) & 0x1FFFFF;
        dest[13] = ((src[8] >> 17) | (src[9] << 15)) & 0x1FFFFF;
        dest[14] = (src[9] >> 6) & 0x1FFFFF;
        dest[15] = ((src[9] >> 27) | (src[10] << 5)) & 0x1FFFFF;
        dest[16] = ((src[10] >> 16) | (src[11] << 16)) & 0x1FFFFF;
        dest[17] = (src[11] >> 5) & 0x1FFFFF;
        dest[18] = ((src[11] >> 26) | (src[12] << 6)) & 0x1FFFFF;
        dest[19] = ((src[12] >> 15) | (src[13] << 17)) & 0x1FFFFF;
        dest[20] = (src[13] >> 4) & 0x1FFFFF;
        dest[21] = ((src[13] >> 25) | (src[14] << 7)) & 0x1FFFFF;
        dest[22] = ((src[14] >> 14) | (src[15] << 18)) & 0x1FFFFF;
        dest[23] = (src[15] >> 3) & 0x1FFFFF;
        dest[24] = ((src[15] >> 24) | (src[16] << 8)) & 0x1FFFFF;
        dest[25] = ((src[16] >> 13) | (src[17] << 19)) & 0x1FFFFF;
        dest[26] = (src[17] >> 2) & 0x1FFFFF;
        dest[27] = ((src[17] >> 23) | (src[18] << 9)) & 0x1FFFFF;
        dest[28] = ((src[18] >> 12) | (src[19] << 20)) & 0x1FFFFF;
        dest[29] = (src[19] >> 1) & 0x1FFFFF;
        dest[30] = ((src[19] >> 22) | (src[20] << 10)) & 0x1FFFFF;
        dest[31] = (src[20] >> 11) & 0x1FFFFF;
    }

    if (n & 31)
    {
        unpackTail21(dest, src, (n & 31));
    }
}

void Unpack::unpack22(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 22) 
    {
        dest[0] = src[0] & 0x3FFFFF;
        dest[1] = ((src[0] >> 22) | (src[1] << 10)) & 0x3FFFFF;
        dest[2] = ((src[1] >> 12) | (src[2] << 20)) & 0x3FFFFF;
        dest[3] = (src[2] >> 2) & 0x3FFFFF;
        dest[4] = ((src[2] >> 24) | (src[3] << 8)) & 0x3FFFFF;
        dest[5] = ((src[3] >> 14) | (src[4] << 18)) & 0x3FFFFF;
        dest[6] = (src[4] >> 4) & 0x3FFFFF;
        dest[7] = ((src[4] >> 26) | (src[5] << 6)) & 0x3FFFFF;
        dest[8] = ((src[5] >> 16) | (src[6] << 16)) & 0x3FFFFF;
        dest[9] = (src[6] >> 6) & 0x3FFFFF;
        dest[10] = ((src[6] >> 28) | (src[7] << 4)) & 0x3FFFFF;
        dest[11] = ((src[7] >> 18) | (src[8] << 14)) & 0x3FFFFF;
        dest[12] = (src[8] >> 8) & 0x3FFFFF;
        dest[13] = ((src[8] >> 30) | (src[9] << 2)) & 0x3FFFFF;
        dest[14] = ((src[9] >> 20) | (src[10] << 12)) & 0x3FFFFF;
        dest[15] = (src[10] >> 10) & 0x3FFFFF;
        dest[16] = src[11] & 0x3FFFFF;
        dest[17] = ((src[11] >> 22) | (src[12] << 10)) & 0x3FFFFF;
        dest[18] = ((src[12] >> 12) | (src[13] << 20)) & 0x3FFFFF;
        dest[19] = (src[13] >> 2) & 0x3FFFFF;
        dest[20] = ((src[13] >> 24) | (src[14] << 8)) & 0x3FFFFF;
        dest[21] = ((src[14] >> 14) | (src[15] << 18)) & 0x3FFFFF;
        dest[22] = (src[15] >> 4) & 0x3FFFFF;
        dest[23] = ((src[15] >> 26) | (src[16] << 6)) & 0x3FFFFF;
        dest[24] = ((src[16] >> 16) | (src[17] << 16)) & 0x3FFFFF;
        dest[25] = (src[17] >> 6) & 0x3FFFFF;
        dest[26] = ((src[17] >> 28) | (src[18] << 4)) & 0x3FFFFF;
        dest[27] = ((src[18] >> 18) | (src[19] << 14)) & 0x3FFFFF;
        dest[28] = (src[19] >> 8) & 0x3FFFFF;
        dest[29] = ((src[19] >> 30) | (src[20] << 2)) & 0x3FFFFF;
        dest[30] = ((src[20] >> 20) | (src[21] << 12)) & 0x3FFFFF;
        dest[31] = (src[21] >> 10) & 0x3FFFFF;
    }

    if (n & 31)
    {
        unpackTail22(dest, src, (n & 31));
    }
}

void Unpack::unpack23(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 23) 
    {
        dest[0] = src[0] & 0x7FFFFF;
        dest[1] = ((src[0] >> 23) | (src[1] << 9)) & 0x7FFFFF;
        dest[2] = ((src[1] >> 14) | (src[2] << 18)) & 0x7FFFFF;
        dest[3] = (src[2] >> 5) & 0x7FFFFF;
        dest[4] = ((src[2] >> 28) | (src[3] << 4)) & 0x7FFFFF;
        dest[5] = ((src[3] >> 19) | (src[4] << 13)) & 0x7FFFFF;
        dest[6] = ((src[4] >> 10) | (src[5] << 22)) & 0x7FFFFF;
        dest[7] = (src[5] >> 1) & 0x7FFFFF;
        dest[8] = ((src[5] >> 24) | (src[6] << 8)) & 0x7FFFFF;
        dest[9] = ((src[6] >> 15) | (src[7] << 17)) & 0x7FFFFF;
        dest[10] = (src[7] >> 6) & 0x7FFFFF;
        dest[11] = ((src[7] >> 29) | (src[8] << 3)) & 0x7FFFFF;
        dest[12] = ((src[8] >> 20) | (src[9] << 12)) & 0x7FFFFF;
        dest[13] = ((src[9] >> 11) | (src[10] << 21)) & 0x7FFFFF;
        dest[14] = (src[10] >> 2) & 0x7FFFFF;
        dest[15] = ((src[10] >> 25) | (src[11] << 7)) & 0x7FFFFF;
        dest[16] = ((src[11] >> 16) | (src[12] << 16)) & 0x7FFFFF;
        dest[17] = (src[12] >> 7) & 0x7FFFFF;
        dest[18] = ((src[12] >> 30) | (src[13] << 2)) & 0x7FFFFF;
        dest[19] = ((src[13] >> 21) | (src[14] << 11)) & 0x7FFFFF;
        dest[20] = ((src[14] >> 12) | (src[15] << 20)) & 0x7FFFFF;
        dest[21] = (src[15] >> 3) & 0x7FFFFF;
        dest[22] = ((src[15] >> 26) | (src[16] << 6)) & 0x7FFFFF;
        dest[23] = ((src[16] >> 17) | (src[17] << 15)) & 0x7FFFFF;
        dest[24] = (src[17] >> 8) & 0x7FFFFF;
        dest[25] = ((src[17] >> 31) | (src[18] << 1)) & 0x7FFFFF;
        dest[26] = ((src[18] >> 22) | (src[19] << 10)) & 0x7FFFFF;
        dest[27] = ((src[19] >> 13) | (src[20] << 19)) & 0x7FFFFF;
        dest[28] = (src[20] >> 4) & 0x7FFFFF;
        dest[29] = ((src[20] >> 27) | (src[21] << 5)) & 0x7FFFFF;
        dest[30] = ((src[21] >> 18) | (src[22] << 14)) & 0x7FFFFF;
        dest[31] = (src[22] >> 9) & 0x7FFFFF;
    }

    if (n & 31)
    {
        unpackTail23(dest, src, (n & 31));
    }
}

void Unpack::unpack24(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 24) 
    {
        dest[0] = src[0] & 0xFFFFFF;
        dest[1] = ((src[0] >> 24) | (src[1] << 8)) & 0xFFFFFF;
        dest[2] = ((src[1] >> 16) | (src[2] << 16)) & 0xFFFFFF;
        dest[3] = (src[2] >> 8) & 0xFFFFFF;
        dest[4] = src[3] & 0xFFFFFF;
        dest[5] = ((src[3] >> 24) | (src[4] << 8)) & 0xFFFFFF;
        dest[6] = ((src[4] >> 16) | (src[5] << 16)) & 0xFFFFFF;
        dest[7] = (src[5] >> 8) & 0xFFFFFF;
        dest[8] = src[6] & 0xFFFFFF;
        dest[9] = ((src[6] >> 24) | (src[7] << 8)) & 0xFFFFFF;
        dest[10] = ((src[7] >> 16) | (src[8] << 16)) & 0xFFFFFF;
        dest[11] = (src[8] >> 8) & 0xFFFFFF;
        dest[12] = src[9] & 0xFFFFFF;
        dest[13] = ((src[9] >> 24) | (src[10] << 8)) & 0xFFFFFF;
        dest[14] = ((src[10] >> 16) | (src[11] << 16)) & 0xFFFFFF;
        dest[15] = (src[11] >> 8) & 0xFFFFFF;
        dest[16] = src[12] & 0xFFFFFF;
        dest[17] = ((src[12] >> 24) | (src[13] << 8)) & 0xFFFFFF;
        dest[18] = ((src[13] >> 16) | (src[14] << 16)) & 0xFFFFFF;
        dest[19] = (src[14] >> 8) & 0xFFFFFF;
        dest[20] = src[15] & 0xFFFFFF;
        dest[21] = ((src[15] >> 24) | (src[16] << 8)) & 0xFFFFFF;
        dest[22] = ((src[16] >> 16) | (src[17] << 16)) & 0xFFFFFF;
        dest[23] = (src[17] >> 8) & 0xFFFFFF;
        dest[24] = src[18] & 0xFFFFFF;
        dest[25] = ((src[18] >> 24) | (src[19] << 8)) & 0xFFFFFF;
        dest[26] = ((src[19] >> 16) | (src[20] << 16)) & 0xFFFFFF;
        dest[27] = (src[20] >> 8) & 0xFFFFFF;
        dest[28] = src[21] & 0xFFFFFF;
        dest[29] = ((src[21] >> 24) | (src[22] << 8)) & 0xFFFFFF;
        dest[30] = ((src[22] >> 16) | (src[23] << 16)) & 0xFFFFFF;
        dest[31] = (src[23] >> 8) & 0xFFFFFF;
    }

    if (n & 31)
    {
        unpackTail24(dest, src, (n & 31));
    }
}

void Unpack::unpack25(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 25) 
    {
        dest[0] = src[0] & 0x1FFFFFF;
        dest[1] = ((src[0] >> 25) | (src[1] << 7)) & 0x1FFFFFF;
        dest[2] = ((src[1] >> 18) | (src[2] << 14)) & 0x1FFFFFF;
        dest[3] = ((src[2] >> 11) | (src[3] << 21)) & 0x1FFFFFF;
        dest[4] = (src[3] >> 4) & 0x1FFFFFF;
        dest[5] = ((src[3] >> 29) | (src[4] << 3)) & 0x1FFFFFF;
        dest[6] = ((src[4] >> 22) | (src[5] << 10)) & 0x1FFFFFF;
        dest[7] = ((src[5] >> 15) | (src[6] << 17)) & 0x1FFFFFF;
        dest[8] = ((src[6] >> 8) | (src[7] << 24)) & 0x1FFFFFF;
        dest[9] = (src[7] >> 1) & 0x1FFFFFF;
        dest[10] = ((src[7] >> 26) | (src[8] << 6)) & 0x1FFFFFF;
        dest[11] = ((src[8] >> 19) | (src[9] << 13)) & 0x1FFFFFF;
        dest[12] = ((src[9] >> 12) | (src[10] << 20)) & 0x1FFFFFF;
        dest[13] = (src[10] >> 5) & 0x1FFFFFF;
        dest[14] = ((src[10] >> 30) | (src[11] << 2)) & 0x1FFFFFF;
        dest[15] = ((src[11] >> 23) | (src[12] << 9)) & 0x1FFFFFF;
        dest[16] = ((src[12] >> 16) | (src[13] << 16)) & 0x1FFFFFF;
        dest[17] = ((src[13] >> 9) | (src[14] << 23)) & 0x1FFFFFF;
        dest[18] = (src[14] >> 2) & 0x1FFFFFF;
        dest[19] = ((src[14] >> 27) | (src[15] << 5)) & 0x1FFFFFF;
        dest[20] = ((src[15] >> 20) | (src[16] << 12)) & 0x1FFFFFF;
        dest[21] = ((src[16] >> 13) | (src[17] << 19)) & 0x1FFFFFF;
        dest[22] = (src[17] >> 6) & 0x1FFFFFF;
        dest[23] = ((src[17] >> 31) | (src[18] << 1)) & 0x1FFFFFF;
        dest[24] = ((src[18] >> 24) | (src[19] << 8)) & 0x1FFFFFF;
        dest[25] = ((src[19] >> 17) | (src[20] << 15)) & 0x1FFFFFF;
        dest[26] = ((src[20] >> 10) | (src[21] << 22)) & 0x1FFFFFF;
        dest[27] = (src[21] >> 3) & 0x1FFFFFF;
        dest[28] = ((src[21] >> 28) | (src[22] << 4)) & 0x1FFFFFF;
        dest[29] = ((src[22] >> 21) | (src[23] << 11)) & 0x1FFFFFF;
        dest[30] = ((src[23] >> 14) | (src[24] << 18)) & 0x1FFFFFF;
        dest[31] = (src[24] >> 7) & 0x1FFFFFF;
    }

    if (n & 31)
    {
        unpackTail25(dest, src, (n & 31));
    }
}

void Unpack::unpack26(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 26) 
    {
        dest[0] = src[0] & 0x3FFFFFF;
        dest[1] = ((src[0] >> 26) | (src[1] << 6)) & 0x3FFFFFF;
        dest[2] = ((src[1] >> 20) | (src[2] << 12)) & 0x3FFFFFF;
        dest[3] = ((src[2] >> 14) | (src[3] << 18)) & 0x3FFFFFF;
        dest[4] = ((src[3] >> 8) | (src[4] << 24)) & 0x3FFFFFF;
        dest[5] = (src[4] >> 2) & 0x3FFFFFF;
        dest[6] = ((src[4] >> 28) | (src[5] << 4)) & 0x3FFFFFF;
        dest[7] = ((src[5] >> 22) | (src[6] << 10)) & 0x3FFFFFF;
        dest[8] = ((src[6] >> 16) | (src[7] << 16)) & 0x3FFFFFF;
        dest[9] = ((src[7] >> 10) | (src[8] << 22)) & 0x3FFFFFF;
        dest[10] = (src[8] >> 4) & 0x3FFFFFF;
        dest[11] = ((src[8] >> 30) | (src[9] << 2)) & 0x3FFFFFF;
        dest[12] = ((src[9] >> 24) | (src[10] << 8)) & 0x3FFFFFF;
        dest[13] = ((src[10] >> 18) | (src[11] << 14)) & 0x3FFFFFF;
        dest[14] = ((src[11] >> 12) | (src[12] << 20)) & 0x3FFFFFF;
        dest[15] = (src[12] >> 6) & 0x3FFFFFF;
        dest[16] = src[13] & 0x3FFFFFF;
        dest[17] = ((src[13] >> 26) | (src[14] << 6)) & 0x3FFFFFF;
        dest[18] = ((src[14] >> 20) | (src[15] << 12)) & 0x3FFFFFF;
        dest[19] = ((src[15] >> 14) | (src[16] << 18)) & 0x3FFFFFF;
        dest[20] = ((src[16] >> 8) | (src[17] << 24)) & 0x3FFFFFF;
        dest[21] = (src[17] >> 2) & 0x3FFFFFF;
        dest[22] = ((src[17] >> 28) | (src[18] << 4)) & 0x3FFFFFF;
        dest[23] = ((src[18] >> 22) | (src[19] << 10)) & 0x3FFFFFF;
        dest[24] = ((src[19] >> 16) | (src[20] << 16)) & 0x3FFFFFF;
        dest[25] = ((src[20] >> 10) | (src[21] << 22)) & 0x3FFFFFF;
        dest[26] = (src[21] >> 4) & 0x3FFFFFF;
        dest[27] = ((src[21] >> 30) | (src[22] << 2)) & 0x3FFFFFF;
        dest[28] = ((src[22] >> 24) | (src[23] << 8)) & 0x3FFFFFF;
        dest[29] = ((src[23] >> 18) | (src[24] << 14)) & 0x3FFFFFF;
        dest[30] = ((src[24] >> 12) | (src[25] << 20)) & 0x3FFFFFF;
        dest[31] = (src[25] >> 6) & 0x3FFFFFF;
    }

    if (n & 31)
    {
        unpackTail26(dest, src, (n & 31));
    }
}

void Unpack::unpack27(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 27) 
    {
        dest[0] = src[0] & 0x7FFFFFF;
        dest[1] = ((src[0] >> 27) | (src[1] << 5)) & 0x7FFFFFF;
        dest[2] = ((src[1] >> 22) | (src[2] << 10)) & 0x7FFFFFF;
        dest[3] = ((src[2] >> 17) | (src[3] << 15)) & 0x7FFFFFF;
        dest[4] = ((src[3] >> 12) | (src[4] << 20)) & 0x7FFFFFF;
        dest[5] = ((src[4] >> 7) | (src[5] << 25)) & 0x7FFFFFF;
        dest[6] = (src[5] >> 2) & 0x7FFFFFF;
        dest[7] = ((src[5] >> 29) | (src[6] << 3)) & 0x7FFFFFF;
        dest[8] = ((src[6] >> 24) | (src[7] << 8)) & 0x7FFFFFF;
        dest[9] = ((src[7] >> 19) | (src[8] << 13)) & 0x7FFFFFF;
        dest[10] = ((src[8] >> 14) | (src[9] << 18)) & 0x7FFFFFF;
        dest[11] = ((src[9] >> 9) | (src[10] << 23)) & 0x7FFFFFF;
        dest[12] = (src[10] >> 4) & 0x7FFFFFF;
        dest[13] = ((src[10] >> 31) | (src[11] << 1)) & 0x7FFFFFF;
        dest[14] = ((src[11] >> 26) | (src[12] << 6)) & 0x7FFFFFF;
        dest[15] = ((src[12] >> 21) | (src[13] << 11)) & 0x7FFFFFF;
        dest[16] = ((src[13] >> 16) | (src[14] << 16)) & 0x7FFFFFF;
        dest[17] = ((src[14] >> 11) | (src[15] << 21)) & 0x7FFFFFF;
        dest[18] = ((src[15] >> 6) | (src[16] << 26)) & 0x7FFFFFF;
        dest[19] = (src[16] >> 1) & 0x7FFFFFF;
        dest[20] = ((src[16] >> 28) | (src[17] << 4)) & 0x7FFFFFF;
        dest[21] = ((src[17] >> 23) | (src[18] << 9)) & 0x7FFFFFF;
        dest[22] = ((src[18] >> 18) | (src[19] << 14)) & 0x7FFFFFF;
        dest[23] = ((src[19] >> 13) | (src[20] << 19)) & 0x7FFFFFF;
        dest[24] = ((src[20] >> 8) | (src[21] << 24)) & 0x7FFFFFF;
        dest[25] = (src[21] >> 3) & 0x7FFFFFF;
        dest[26] = ((src[21] >> 30) | (src[22] << 2)) & 0x7FFFFFF;
        dest[27] = ((src[22] >> 25) | (src[23] << 7)) & 0x7FFFFFF;
        dest[28] = ((src[23] >> 20) | (src[24] << 12)) & 0x7FFFFFF;
        dest[29] = ((src[24] >> 15) | (src[25] << 17)) & 0x7FFFFFF;
        dest[30] = ((src[25] >> 10) | (src[26] << 22)) & 0x7FFFFFF;
        dest[31] = (src[26] >> 5) & 0x7FFFFFF;
    }

    if (n & 31)
    {
        unpackTail27(dest, src, (n & 31));
    }
}

void Unpack::unpack28(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 28) 
    {
        dest[0] = src[0] & 0xFFFFFFF;
        dest[1] = ((src[0] >> 28) | (src[1] << 4)) & 0xFFFFFFF;
        dest[2] = ((src[1] >> 24) | (src[2] << 8)) & 0xFFFFFFF;
        dest[3] = ((src[2] >> 20) | (src[3] << 12)) & 0xFFFFFFF;
        dest[4] = ((src[3] >> 16) | (src[4] << 16)) & 0xFFFFFFF;
        dest[5] = ((src[4] >> 12) | (src[5] << 20)) & 0xFFFFFFF;
        dest[6] = ((src[5] >> 8) | (src[6] << 24)) & 0xFFFFFFF;
        dest[7] = (src[6] >> 4) & 0xFFFFFFF;
        dest[8] = src[7] & 0xFFFFFFF;
        dest[9] = ((src[7] >> 28) | (src[8] << 4)) & 0xFFFFFFF;
        dest[10] = ((src[8] >> 24) | (src[9] << 8)) & 0xFFFFFFF;
        dest[11] = ((src[9] >> 20) | (src[10] << 12)) & 0xFFFFFFF;
        dest[12] = ((src[10] >> 16) | (src[11] << 16)) & 0xFFFFFFF;
        dest[13] = ((src[11] >> 12) | (src[12] << 20)) & 0xFFFFFFF;
        dest[14] = ((src[12] >> 8) | (src[13] << 24)) & 0xFFFFFFF;
        dest[15] = (src[13] >> 4) & 0xFFFFFFF;
        dest[16] = src[14] & 0xFFFFFFF;
        dest[17] = ((src[14] >> 28) | (src[15] << 4)) & 0xFFFFFFF;
        dest[18] = ((src[15] >> 24) | (src[16] << 8)) & 0xFFFFFFF;
        dest[19] = ((src[16] >> 20) | (src[17] << 12)) & 0xFFFFFFF;
        dest[20] = ((src[17] >> 16) | (src[18] << 16)) & 0xFFFFFFF;
        dest[21] = ((src[18] >> 12) | (src[19] << 20)) & 0xFFFFFFF;
        dest[22] = ((src[19] >> 8) | (src[20] << 24)) & 0xFFFFFFF;
        dest[23] = (src[20] >> 4) & 0xFFFFFFF;
        dest[24] = src[21] & 0xFFFFFFF;
        dest[25] = ((src[21] >> 28) | (src[22] << 4)) & 0xFFFFFFF;
        dest[26] = ((src[22] >> 24) | (src[23] << 8)) & 0xFFFFFFF;
        dest[27] = ((src[23] >> 20) | (src[24] << 12)) & 0xFFFFFFF;
        dest[28] = ((src[24] >> 16) | (src[25] << 16)) & 0xFFFFFFF;
        dest[29] = ((src[25] >> 12) | (src[26] << 20)) & 0xFFFFFFF;
        dest[30] = ((src[26] >> 8) | (src[27] << 24)) & 0xFFFFFFF;
        dest[31] = (src[27] >> 4) & 0xFFFFFFF;
    }

    if (n & 31)
    {
        unpackTail28(dest, src, (n & 31));
    }
}

void Unpack::unpack29(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 29) 
    {
        dest[0] = src[0] & 0x1FFFFFFF;
        dest[1] = ((src[0] >> 29) | (src[1] << 3)) & 0x1FFFFFFF;
        dest[2] = ((src[1] >> 26) | (src[2] << 6)) & 0x1FFFFFFF;
        dest[3] = ((src[2] >> 23) | (src[3] << 9)) & 0x1FFFFFFF;
        dest[4] = ((src[3] >> 20) | (src[4] << 12)) & 0x1FFFFFFF;
        dest[5] = ((src[4] >> 17) | (src[5] << 15)) & 0x1FFFFFFF;
        dest[6] = ((src[5] >> 14) | (src[6] << 18)) & 0x1FFFFFFF;
        dest[7] = ((src[6] >> 11) | (src[7] << 21)) & 0x1FFFFFFF;
        dest[8] = ((src[7] >> 8) | (src[8] << 24)) & 0x1FFFFFFF;
        dest[9] = ((src[8] >> 5) | (src[9] << 27)) & 0x1FFFFFFF;
        dest[10] = (src[9] >> 2) & 0x1FFFFFFF;
        dest[11] = ((src[9] >> 31) | (src[10] << 1)) & 0x1FFFFFFF;
        dest[12] = ((src[10] >> 28) | (src[11] << 4)) & 0x1FFFFFFF;
        dest[13] = ((src[11] >> 25) | (src[12] << 7)) & 0x1FFFFFFF;
        dest[14] = ((src[12] >> 22) | (src[13] << 10)) & 0x1FFFFFFF;
        dest[15] = ((src[13] >> 19) | (src[14] << 13)) & 0x1FFFFFFF;
        dest[16] = ((src[14] >> 16) | (src[15] << 16)) & 0x1FFFFFFF;
        dest[17] = ((src[15] >> 13) | (src[16] << 19)) & 0x1FFFFFFF;
        dest[18] = ((src[16] >> 10) | (src[17] << 22)) & 0x1FFFFFFF;
        dest[19] = ((src[17] >> 7) | (src[18] << 25)) & 0x1FFFFFFF;
        dest[20] = ((src[18] >> 4) | (src[19] << 28)) & 0x1FFFFFFF;
        dest[21] = (src[19] >> 1) & 0x1FFFFFFF;
        dest[22] = ((src[19] >> 30) | (src[20] << 2)) & 0x1FFFFFFF;
        dest[23] = ((src[20] >> 27) | (src[21] << 5)) & 0x1FFFFFFF;
        dest[24] = ((src[21] >> 24) | (src[22] << 8)) & 0x1FFFFFFF;
        dest[25] = ((src[22] >> 21) | (src[23] << 11)) & 0x1FFFFFFF;
        dest[26] = ((src[23] >> 18) | (src[24] << 14)) & 0x1FFFFFFF;
        dest[27] = ((src[24] >> 15) | (src[25] << 17)) & 0x1FFFFFFF;
        dest[28] = ((src[25] >> 12) | (src[26] << 20)) & 0x1FFFFFFF;
        dest[29] = ((src[26] >> 9) | (src[27] << 23)) & 0x1FFFFFFF;
        dest[30] = ((src[27] >> 6) | (src[28] << 26)) & 0x1FFFFFFF;
        dest[31] = (src[28] >> 3) & 0x1FFFFFFF;
    }

    if (n & 31)
    {
        unpackTail29(dest, src, (n & 31));
    }
}

void Unpack::unpack30(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 30) 
    {
        dest[0] = src[0] & 0x3FFFFFFF;
        dest[1] = ((src[0] >> 30) | (src[1] << 2)) & 0x3FFFFFFF;
        dest[2] = ((src[1] >> 28) | (src[2] << 4)) & 0x3FFFFFFF;
        dest[3] = ((src[2] >> 26) | (src[3] << 6)) & 0x3FFFFFFF;
        dest[4] = ((src[3] >> 24) | (src[4] << 8)) & 0x3FFFFFFF;
        dest[5] = ((src[4] >> 22) | (src[5] << 10)) & 0x3FFFFFFF;
        dest[6] = ((src[5] >> 20) | (src[6] << 12)) & 0x3FFFFFFF;
        dest[7] = ((src[6] >> 18) | (src[7] << 14)) & 0x3FFFFFFF;
        dest[8] = ((src[7] >> 16) | (src[8] << 16)) & 0x3FFFFFFF;
        dest[9] = ((src[8] >> 14) | (src[9] << 18)) & 0x3FFFFFFF;
        dest[10] = ((src[9] >> 12) | (src[10] << 20)) & 0x3FFFFFFF;
        dest[11] = ((src[10] >> 10) | (src[11] << 22)) & 0x3FFFFFFF;
        dest[12] = ((src[11] >> 8) | (src[12] << 24)) & 0x3FFFFFFF;
        dest[13] = ((src[12] >> 6) | (src[13] << 26)) & 0x3FFFFFFF;
        dest[14] = ((src[13] >> 4) | (src[14] << 28)) & 0x3FFFFFFF;
        dest[15] = (src[14] >> 2) & 0x3FFFFFFF;
        dest[16] = src[15] & 0x3FFFFFFF;
        dest[17] = ((src[15] >> 30) | (src[16] << 2)) & 0x3FFFFFFF;
        dest[18] = ((src[16] >> 28) | (src[17] << 4)) & 0x3FFFFFFF;
        dest[19] = ((src[17] >> 26) | (src[18] << 6)) & 0x3FFFFFFF;
        dest[20] = ((src[18] >> 24) | (src[19] << 8)) & 0x3FFFFFFF;
        dest[21] = ((src[19] >> 22) | (src[20] << 10)) & 0x3FFFFFFF;
        dest[22] = ((src[20] >> 20) | (src[21] << 12)) & 0x3FFFFFFF;
        dest[23] = ((src[21] >> 18) | (src[22] << 14)) & 0x3FFFFFFF;
        dest[24] = ((src[22] >> 16) | (src[23] << 16)) & 0x3FFFFFFF;
        dest[25] = ((src[23] >> 14) | (src[24] << 18)) & 0x3FFFFFFF;
        dest[26] = ((src[24] >> 12) | (src[25] << 20)) & 0x3FFFFFFF;
        dest[27] = ((src[25] >> 10) | (src[26] << 22)) & 0x3FFFFFFF;
        dest[28] = ((src[26] >> 8) | (src[27] << 24)) & 0x3FFFFFFF;
        dest[29] = ((src[27] >> 6) | (src[28] << 26)) & 0x3FFFFFFF;
        dest[30] = ((src[28] >> 4) | (src[29] << 28)) & 0x3FFFFFFF;
        dest[31] = (src[29] >> 2) & 0x3FFFFFFF;
    }

    if (n & 31)
    {
        unpackTail30(dest, src, (n & 31));
    }
}

void Unpack::unpack31(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 31) 
    {
        dest[0] = src[0] & 0x7FFFFFFF;
        dest[1] = ((src[0] >> 31) | (src[1] << 1)) & 0x7FFFFFFF;
        dest[2] = ((src[1] >> 30) | (src[2] << 2)) & 0x7FFFFFFF;
        dest[3] = ((src[2] >> 29) | (src[3] << 3)) & 0x7FFFFFFF;
        dest[4] = ((src[3] >> 28) | (src[4] << 4)) & 0x7FFFFFFF;
        dest[5] = ((src[4] >> 27) | (src[5] << 5)) & 0x7FFFFFFF;
        dest[6] = ((src[5] >> 26) | (src[6] << 6)) & 0x7FFFFFFF;
        dest[7] = ((src[6] >> 25) | (src[7] << 7)) & 0x7FFFFFFF;
        dest[8] = ((src[7] >> 24) | (src[8] << 8)) & 0x7FFFFFFF;
        dest[9] = ((src[8] >> 23) | (src[9] << 9)) & 0x7FFFFFFF;
        dest[10] = ((src[9] >> 22) | (src[10] << 10)) & 0x7FFFFFFF;
        dest[11] = ((src[10] >> 21) | (src[11] << 11)) & 0x7FFFFFFF;
        dest[12] = ((src[11] >> 20) | (src[12] << 12)) & 0x7FFFFFFF;
        dest[13] = ((src[12] >> 19) | (src[13] << 13)) & 0x7FFFFFFF;
        dest[14] = ((src[13] >> 18) | (src[14] << 14)) & 0x7FFFFFFF;
        dest[15] = ((src[14] >> 17) | (src[15] << 15)) & 0x7FFFFFFF;
        dest[16] = ((src[15] >> 16) | (src[16] << 16)) & 0x7FFFFFFF;
        dest[17] = ((src[16] >> 15) | (src[17] << 17)) & 0x7FFFFFFF;
        dest[18] = ((src[17] >> 14) | (src[18] << 18)) & 0x7FFFFFFF;
        dest[19] = ((src[18] >> 13) | (src[19] << 19)) & 0x7FFFFFFF;
        dest[20] = ((src[19] >> 12) | (src[20] << 20)) & 0x7FFFFFFF;
        dest[21] = ((src[20] >> 11) | (src[21] << 21)) & 0x7FFFFFFF;
        dest[22] = ((src[21] >> 10) | (src[22] << 22)) & 0x7FFFFFFF;
        dest[23] = ((src[22] >> 9) | (src[23] << 23)) & 0x7FFFFFFF;
        dest[24] = ((src[23] >> 8) | (src[24] << 24)) & 0x7FFFFFFF;
        dest[25] = ((src[24] >> 7) | (src[25] << 25)) & 0x7FFFFFFF;
        dest[26] = ((src[25] >> 6) | (src[26] << 26)) & 0x7FFFFFFF;
        dest[27] = ((src[26] >> 5) | (src[27] << 27)) & 0x7FFFFFFF;
        dest[28] = ((src[27] >> 4) | (src[28] << 28)) & 0x7FFFFFFF;
        dest[29] = ((src[28] >> 3) | (src[29] << 29)) & 0x7FFFFFFF;
        dest[30] = ((src[29] >> 2) | (src[30] << 30)) & 0x7FFFFFFF;
        dest[31] = (src[30] >> 1) & 0x7FFFFFFF;
    }

    if (n & 31)
    {
        unpackTail31(dest, src, (n & 31));
    }
}

void Unpack::unpack32(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (size_t i = 32; i <= n; i += 32, dest += 32, src += 32) 
    {
        dest[0] = src[0] & 0xFFFFFFFF;
        dest[1] = src[1] & 0xFFFFFFFF;
        dest[2] = src[2] & 0xFFFFFFFF;
        dest[3] = src[3] & 0xFFFFFFFF;
        dest[4] = src[4] & 0xFFFFFFFF;
        dest[5] = src[5] & 0xFFFFFFFF;
        dest[6] = src[6] & 0xFFFFFFFF;
        dest[7] = src[7] & 0xFFFFFFFF;
        dest[8] = src[8] & 0xFFFFFFFF;
        dest[9] = src[9] & 0xFFFFFFFF;
        dest[10] = src[10] & 0xFFFFFFFF;
        dest[11] = src[11] & 0xFFFFFFFF;
        dest[12] = src[12] & 0xFFFFFFFF;
        dest[13] = src[13] & 0xFFFFFFFF;
        dest[14] = src[14] & 0xFFFFFFFF;
        dest[15] = src[15] & 0xFFFFFFFF;
        dest[16] = src[16] & 0xFFFFFFFF;
        dest[17] = src[17] & 0xFFFFFFFF;
        dest[18] = src[18] & 0xFFFFFFFF;
        dest[19] = src[19] & 0xFFFFFFFF;
        dest[20] = src[20] & 0xFFFFFFFF;
        dest[21] = src[21] & 0xFFFFFFFF;
        dest[22] = src[22] & 0xFFFFFFFF;
        dest[23] = src[23] & 0xFFFFFFFF;
        dest[24] = src[24] & 0xFFFFFFFF;
        dest[25] = src[25] & 0xFFFFFFFF;
        dest[26] = src[26] & 0xFFFFFFFF;
        dest[27] = src[27] & 0xFFFFFFFF;
        dest[28] = src[28] & 0xFFFFFFFF;
        dest[29] = src[29] & 0xFFFFFFFF;
        dest[30] = src[30] & 0xFFFFFFFF;
        dest[31] = src[31] & 0xFFFFFFFF;
    }

    if (n & 31)
    {
        unpackTail32(dest, src, (n & 31));
    }
}

FX_NS_END

