#include "firtex/index/compress/Pack.h"

FX_NS_DEF(index);

Pack::pack_func Pack::PACK_FUNCTIONS[] =
{
    Pack::pack0, Pack::pack1,  Pack::pack2,  Pack::pack3,
    Pack::pack4, Pack::pack5,  Pack::pack6,  Pack::pack7,
    Pack::pack8, Pack::pack9,  Pack::pack10, Pack::pack11,
    Pack::pack12, Pack::pack13, Pack::pack14, Pack::pack15,
    Pack::pack16, Pack::pack17, Pack::pack18, Pack::pack19,
    Pack::pack20, Pack::pack21, Pack::pack22, Pack::pack23,
    Pack::pack24, Pack::pack25, Pack::pack26, Pack::pack27,
    Pack::pack28, Pack::pack29, Pack::pack30, Pack::pack31,
    Pack::pack32
};

void Pack::pack0(uint32_t* dest, const uint32_t* src, size_t n)
{
    return;//nothing to do
}

void Pack::pack1(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, ++dest, src += 32)
    {
        uint32_t v = src[0] & 0x1;
        v |= ((src[1] & 0x1) << 1);
        v |= ((src[2] & 0x1) << 2);
        v |= ((src[3] & 0x1) << 3);
        v |= ((src[4] & 0x1) << 4);
        v |= ((src[5] & 0x1) << 5);
        v |= ((src[6] & 0x1) << 6);
        v |= ((src[7] & 0x1) << 7);
        v |= ((src[8] & 0x1) << 8);
        v |= ((src[9] & 0x1) << 9);
        v |= ((src[10] & 0x1) << 10);
        v |= ((src[11] & 0x1) << 11);
        v |= ((src[12] & 0x1) << 12);
        v |= ((src[13] & 0x1) << 13);
        v |= ((src[14] & 0x1) << 14);
        v |= ((src[15] & 0x1) << 15);
        v |= ((src[16] & 0x1) << 16);
        v |= ((src[17] & 0x1) << 17);
        v |= ((src[18] & 0x1) << 18);
        v |= ((src[19] & 0x1) << 19);
        v |= ((src[20] & 0x1) << 20);
        v |= ((src[21] & 0x1) << 21);
        v |= ((src[22] & 0x1) << 22);
        v |= ((src[23] & 0x1) << 23);
        v |= ((src[24] & 0x1) << 24);
        v |= ((src[25] & 0x1) << 25);
        v |= ((src[26] & 0x1) << 26);
        v |= ((src[27] & 0x1) << 27);
        v |= ((src[28] & 0x1) << 28);
        v |= ((src[29] & 0x1) << 29);
        v |= ((src[30] & 0x1) << 30);
        v |= ((src[31] & 0x1) << 31);
        dest[0] = v;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 1);
    }
}

void Pack::pack2(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 2)
    {
        uint32_t v0;
        uint32_t v1;
        v0 = src[0] & 0x3;
        v0 |= (src[1] & 0x3) << 2;
        v0 |= (src[2] & 0x3) << 4;
        v0 |= (src[3] & 0x3) << 6;
        v0 |= (src[4] & 0x3) << 8;
        v0 |= (src[5] & 0x3) << 10;
        v0 |= (src[6] & 0x3) << 12;
        v0 |= (src[7] & 0x3) << 14;
        v0 |= (src[8] & 0x3) << 16;
        v0 |= (src[9] & 0x3) << 18;
        v0 |= (src[10] & 0x3) << 20;
        v0 |= (src[11] & 0x3) << 22;
        v0 |= (src[12] & 0x3) << 24;
        v0 |= (src[13] & 0x3) << 26;
        v0 |= (src[14] & 0x3) << 28;
        v0 |= (src[15] & 0x3) << 30;

        v1 = src[16] & 0x3;
        v1 |= (src[17] & 0x3) << 2;
        v1 |= (src[18] & 0x3) << 4;
        v1 |= (src[19] & 0x3) << 6;
        v1 |= (src[20] & 0x3) << 8;
        v1 |= (src[21] & 0x3) << 10;
        v1 |= (src[22] & 0x3) << 12;
        v1 |= (src[23] & 0x3) << 14;
        v1 |= (src[24] & 0x3) << 16;
        v1 |= (src[25] & 0x3) << 18;
        v1 |= (src[26] & 0x3) << 20;
        v1 |= (src[27] & 0x3) << 22;
        v1 |= (src[28] & 0x3) << 24;
        v1 |= (src[29] & 0x3) << 26;
        v1 |= (src[30] & 0x3) << 28;
        v1 |= (src[31] & 0x3) << 30;

        dest[0] = v0;
        dest[1] = v1;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 2);
    }
}



void Pack::pack3(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; src += 32, i += 32, dest += 3)
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        v0 = (src[0] & 0x7);
        v0 |= ((src[1] & 0x7) << 3);
        v0 |= ((src[2] & 0x7) << 6);
        v0 |= ((src[3] & 0x7) << 9);
        v0 |= ((src[4] & 0x7) << 12);
        v0 |= ((src[5] & 0x7) << 15);
        v0 |= ((src[6] & 0x7) << 18);
        v0 |= ((src[7] & 0x7) << 21);
        v0 |= ((src[8] & 0x7) << 24);
        v0 |= ((src[9] & 0x7) << 27);
        v0 |= ((src[10] & 0x7) << 30);

        v1 = ((src[10] & 0x7) >> 2);
        v1 |= ((src[11] & 0x7) << 1);
        v1 |= ((src[12] & 0x7) << 4);
        v1 |= ((src[13] & 0x7) << 7);
        v1 |= ((src[14] & 0x7) << 10);
        v1 |= ((src[15] & 0x7) << 13);
        v1 |= ((src[16] & 0x7) << 16);
        v1 |= ((src[17] & 0x7) << 19);
        v1 |= ((src[18] & 0x7) << 22);
        v1 |= ((src[19] & 0x7) << 25);
        v1 |= ((src[20] & 0x7) << 28);
        v1 |= ((src[21] & 0x7) << 31);

        v2 = ((src[21] & 0x7) >> 1);
        v2 |= ((src[22] & 0x7) << 2);
        v2 |= ((src[23] & 0x7) << 5);
        v2 |= ((src[24] & 0x7) << 8);
        v2 |= ((src[25] & 0x7) << 11);
        v2 |= ((src[26] & 0x7) << 14);
        v2 |= ((src[27] & 0x7) << 17);
        v2 |= ((src[28] & 0x7) << 20);
        v2 |= ((src[29] & 0x7) << 23);
        v2 |= ((src[30] & 0x7) << 26);
        v2 |= ((src[31] & 0x7) << 29);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 3);
    }
}


void Pack::pack4(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, dest += 4, src += 32)
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;

        v0 = (src[0] & 0xF);
        v0 |= ((src[1] & 0xF) << 4);
        v0 |= ((src[2] & 0xF) << 8);
        v0 |= ((src[3] & 0xF) << 12);
        v0 |= ((src[4] & 0xF) << 16);
        v0 |= ((src[5] & 0xF) << 20);
        v0 |= ((src[6] & 0xF) << 24);
        v0 |= ((src[7] & 0xF) << 28);
        
        v1 = (src[8] & 0xF);
        v1 |= ((src[9] & 0xF) << 4);
        v1 |= ((src[10] & 0xF) << 8);
        v1 |= ((src[11] & 0xF) << 12);
        v1 |= ((src[12] & 0xF) << 16);
        v1 |= ((src[13] & 0xF) << 20);
        v1 |= ((src[14] & 0xF) << 24);
        v1 |= ((src[15] & 0xF) << 28);

        v2 = (src[16] & 0xF);
        v2 |= ((src[17] & 0xF) << 4);
        v2 |= ((src[18] & 0xF) << 8);
        v2 |= ((src[19] & 0xF) << 12);
        v2 |= ((src[20] & 0xF) << 16);
        v2 |= ((src[21] & 0xF) << 20);
        v2 |= ((src[22] & 0xF) << 24);
        v2 |= ((src[23] & 0xF) << 28);
        
        v3 = (src[24] & 0xF);
        v3 |= ((src[25] & 0xF) << 4);
        v3 |= ((src[26] & 0xF) << 8);
        v3 |= ((src[27] & 0xF) << 12);
        v3 |= ((src[28] & 0xF) << 16);
        v3 |= ((src[29] & 0xF) << 20);
        v3 |= ((src[30] & 0xF) << 24);
        v3 |= ((src[31] & 0xF) << 28);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 4);
    }
}


void Pack::pack5(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 5)
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;

        v0 = (src[0] & 0x1F);
        v0 |= ((src[1] & 0x1F) << 5);
        v0 |= ((src[2] & 0x1F) << 10);
        v0 |= ((src[3] & 0x1F) << 15);
        v0 |= ((src[4] & 0x1F) << 20);
        v0 |= ((src[5] & 0x1F) << 25);
        v0 |= ((src[6] & 0x1F) << 30);

        v1 = ((src[6] & 0x1F) >> 2);
        v1 |= ((src[7] & 0x1F) << 3);
        v1 |= ((src[8] & 0x1F) << 8);
        v1 |= ((src[9] & 0x1F) << 13);
        v1 |= ((src[10] & 0x1F) << 18);
        v1 |= ((src[11] & 0x1F) << 23);
        v1 |= ((src[12] & 0x1F) << 28);

        v2 = ((src[12] & 0x1F) >> 4);
        v2 |= ((src[13] & 0x1F) << 1);
        v2 |= ((src[14] & 0x1F) << 6);
        v2 |= ((src[15] & 0x1F) << 11);
        v2 |= ((src[16] & 0x1F) << 16);
        v2 |= ((src[17] & 0x1F) << 21);
        v2 |= ((src[18] & 0x1F) << 26);
        v2 |= ((src[19] & 0x1F) << 31);

        v3 = ((src[19] & 0x1F) >> 1);
        v3 |= ((src[20] & 0x1F) << 4);
        v3 |= ((src[21] & 0x1F) << 9);
        v3 |= ((src[22] & 0x1F) << 14);
        v3 |= ((src[23] & 0x1F) << 19);
        v3 |= ((src[24] & 0x1F) << 24);
        v3 |= ((src[25] & 0x1F) << 29);
        
        v4 = ((src[25] & 0x1F) >> 3);
        v4 |= ((src[26] & 0x1F) << 2);
        v4 |= ((src[27] & 0x1F) << 7);
        v4 |= ((src[28] & 0x1F) << 12);
        v4 |= ((src[29] & 0x1F) << 17);
        v4 |= ((src[30] & 0x1F) << 22);
        v4 |= ((src[31] & 0x1F) << 27);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 5);
    }
}

void Pack::pack6(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, dest += 6, src += 32)
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;

        v0 = (src[0] & 0x3F);
        v0 |= ((src[1] & 0x3F) << 6);
        v0 |= ((src[2] & 0x3F) << 12);
        v0 |= ((src[3] & 0x3F) << 18);
        v0 |= ((src[4] & 0x3F) << 24);
        v0 |= ((src[5] & 0x3F) << 30);

        v1 = ((src[5] & 0x3F) >> 2);
        v1 |= ((src[6] & 0x3F) << 4);
        v1 |= ((src[7] & 0x3F) << 10);
        v1 |= ((src[8] & 0x3F) << 16);
        v1 |= ((src[9] & 0x3F) << 22);
        v1 |= ((src[10] & 0x3F) << 28);

        v2 = ((src[10] & 0x3F) >> 4);
        v2 |= ((src[11] & 0x3F) << 2);
        v2 |= ((src[12] & 0x3F) << 8);
        v2 |= ((src[13] & 0x3F) << 14);
        v2 |= ((src[14] & 0x3F) << 20);
        v2 |= ((src[15] & 0x3F) << 26);

        v3 = (src[16] & 0x3F);
        v3 |= ((src[17] & 0x3F) << 6);
        v3 |= ((src[18] & 0x3F) << 12);
        v3 |= ((src[19] & 0x3F) << 18);
        v3 |= ((src[20] & 0x3F) << 24);
        v3 |= ((src[21] & 0x3F) << 30);
 
        v4 = ((src[21] & 0x3F) >> 2);
        v4 |= ((src[22] & 0x3F) << 4);
        v4 |= ((src[23] & 0x3F) << 10);
        v4 |= ((src[24] & 0x3F) << 16);
        v4 |= ((src[25] & 0x3F) << 22);
        v4 |= ((src[26] & 0x3F) << 28);

        v5 = ((src[26] & 0x3F) >> 4);
        v5 |= ((src[27] & 0x3F) << 2);
        v5 |= ((src[28] & 0x3F) << 8);
        v5 |= ((src[29] & 0x3F) << 14);
        v5 |= ((src[30] & 0x3F) << 20);
        v5 |= ((src[31] & 0x3F) << 26);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 6);
    }
}

void Pack::pack7(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 7) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;

        v0 = (src[0] & 0x7F);
        v0 |= ((src[1] & 0x7F) << 7);
        v0 |= ((src[2] & 0x7F) << 14);
        v0 |= ((src[3] & 0x7F) << 21);
        v0 |= ((src[4] & 0x7F) << 28);

        v1 = ((src[4] & 0x7F) >> 4);
        v1 |= ((src[5] & 0x7F) << 3);
        v1 |= ((src[6] & 0x7F) << 10);
        v1 |= ((src[7] & 0x7F) << 17);
        v1 |= ((src[8] & 0x7F) << 24);
        v1 |= ((src[9] & 0x7F) << 31);

        v2 = ((src[9] & 0x7F) >> 1);
        v2 |= ((src[10] & 0x7F) << 6);
        v2 |= ((src[11] & 0x7F) << 13);
        v2 |= ((src[12] & 0x7F) << 20);
        v2 |= ((src[13] & 0x7F) << 27);

        v3 = ((src[13] & 0x7F) >> 5);
        v3 |= ((src[14] & 0x7F) << 2);
        v3 |= ((src[15] & 0x7F) << 9);
        v3 |= ((src[16] & 0x7F) << 16);
        v3 |= ((src[17] & 0x7F) << 23);
        v3 |= ((src[18] & 0x7F) << 30);

        v4 = ((src[18] & 0x7F) >> 2);
        v4 |= ((src[19] & 0x7F) << 5);
        v4 |= ((src[20] & 0x7F) << 12);
        v4 |= ((src[21] & 0x7F) << 19);
        v4 |= ((src[22] & 0x7F) << 26);

        v5 = ((src[22] & 0x7F) >> 6);
        v5 |= ((src[23] & 0x7F) << 1);
        v5 |= ((src[24] & 0x7F) << 8);
        v5 |= ((src[25] & 0x7F) << 15);
        v5 |= ((src[26] & 0x7F) << 22);
        v5 |= ((src[27] & 0x7F) << 29);

        v6 = ((src[27] & 0x7F) >> 3);
        v6 |= ((src[28] & 0x7F) << 4);
        v6 |= ((src[29] & 0x7F) << 11);
        v6 |= ((src[30] & 0x7F) << 18);
        v6 |= ((src[31] & 0x7F) << 25);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 7);
    }

}

void Pack::pack8(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 8)
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;

        v0 = (src[0] & 0xFF);
        v0 |= ((src[1] & 0xFF) << 8);
        v0 |= ((src[2] & 0xFF) << 16);
        v0 |= ((src[3] & 0xFF) << 24);

        v1 = (src[4] & 0xFF);
        v1 |= ((src[5] & 0xFF) << 8);
        v1 |= ((src[6] & 0xFF) << 16);
        v1 |= ((src[7] & 0xFF) << 24);

        v2 = (src[8] & 0xFF);
        v2 |= ((src[9] & 0xFF) << 8);
        v2 |= ((src[10] & 0xFF) << 16);
        v2 |= ((src[11] & 0xFF) << 24);

        v3 = (src[12] & 0xFF);
        v3 |= ((src[13] & 0xFF) << 8);
        v3 |= ((src[14] & 0xFF) << 16);
        v3 |= ((src[15] & 0xFF) << 24);

        v4 = (src[16] & 0xFF);
        v4 |= ((src[17] & 0xFF) << 8);
        v4 |= ((src[18] & 0xFF) << 16);
        v4 |= ((src[19] & 0xFF) << 24);

        v5 = (src[20] & 0xFF);
        v5 |= ((src[21] & 0xFF) << 8);
        v5 |= ((src[22] & 0xFF) << 16);
        v5 |= ((src[23] & 0xFF) << 24);

        v6 = (src[24] & 0xFF);
        v6 |= ((src[25] & 0xFF) << 8);
        v6 |= ((src[26] & 0xFF) << 16);
        v6 |= ((src[27] & 0xFF) << 24);

        v7 = (src[28] & 0xFF);
        v7 |= ((src[29] & 0xFF) << 8);
        v7 |= ((src[30] & 0xFF) << 16);
        v7 |= ((src[31] & 0xFF) << 24);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 8);
    }
}

void Pack::pack9(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 9) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;

        v0 = (src[0] & 0x1FF);
        v0 |= ((src[1] & 0x1FF) << 9);
        v0 |= ((src[2] & 0x1FF) << 18);
        v0 |= ((src[3] & 0x1FF) << 27);

        v1 = ((src[3] & 0x1FF) >> 5);
        v1 |= ((src[4] & 0x1FF) << 4);
        v1 |= ((src[5] & 0x1FF) << 13);
        v1 |= ((src[6] & 0x1FF) << 22);
        v1 |= ((src[7] & 0x1FF) << 31);

        v2 = ((src[7] & 0x1FF) >> 1);
        v2 |= ((src[8] & 0x1FF) << 8);
        v2 |= ((src[9] & 0x1FF) << 17);
        v2 |= ((src[10] & 0x1FF) << 26);

        v3 = ((src[10] & 0x1FF) >> 6);
        v3 |= ((src[11] & 0x1FF) << 3);
        v3 |= ((src[12] & 0x1FF) << 12);
        v3 |= ((src[13] & 0x1FF) << 21);
        v3 |= ((src[14] & 0x1FF) << 30);

        v4 = ((src[14] & 0x1FF) >> 2);
        v4 |= ((src[15] & 0x1FF) << 7);
        v4 |= ((src[16] & 0x1FF) << 16);
        v4 |= ((src[17] & 0x1FF) << 25);

        v5 = ((src[17] & 0x1FF) >> 7);
        v5 |= ((src[18] & 0x1FF) << 2);
        v5 |= ((src[19] & 0x1FF) << 11);
        v5 |= ((src[20] & 0x1FF) << 20);
        v5 |= ((src[21] & 0x1FF) << 29);

        v6 = ((src[21] & 0x1FF) >> 3);
        v6 |= ((src[22] & 0x1FF) << 6);
        v6 |= ((src[23] & 0x1FF) << 15);
        v6 |= ((src[24] & 0x1FF) << 24);

        v7 = ((src[24] & 0x1FF) >> 8);
        v7 |= ((src[25] & 0x1FF) << 1);
        v7 |= ((src[26] & 0x1FF) << 10);
        v7 |= ((src[27] & 0x1FF) << 19);
        v7 |= ((src[28] & 0x1FF) << 28);

        v8 = ((src[28] & 0x1FF) >> 4);
        v8 |= ((src[29] & 0x1FF) << 5);
        v8 |= ((src[30] & 0x1FF) << 14);
        v8 |= ((src[31] & 0x1FF) << 23);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 9);
    }

}

void Pack::pack10(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 10) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;

        v0 = (src[0] & 0x3FF);
        v0 |= ((src[1] & 0x3FF) << 10);
        v0 |= ((src[2] & 0x3FF) << 20);
        v0 |= ((src[3] & 0x3FF) << 30);

        v1 = ((src[3] & 0x3FF) >> 2);
        v1 |= ((src[4] & 0x3FF) << 8);
        v1 |= ((src[5] & 0x3FF) << 18);
        v1 |= ((src[6] & 0x3FF) << 28);

        v2 = ((src[6] & 0x3FF) >> 4);
        v2 |= ((src[7] & 0x3FF) << 6);
        v2 |= ((src[8] & 0x3FF) << 16);
        v2 |= ((src[9] & 0x3FF) << 26);
        
        v3 = ((src[9] & 0x3FF) >> 6);
        v3 |= ((src[10] & 0x3FF) << 4);
        v3 |= ((src[11] & 0x3FF) << 14);
        v3 |= ((src[12] & 0x3FF) << 24);

        v4 = ((src[12] & 0x3FF) >> 8);
        v4 |= ((src[13] & 0x3FF) << 2);
        v4 |= ((src[14] & 0x3FF) << 12);
        v4 |= ((src[15] & 0x3FF) << 22);

        v5 = (src[16] & 0x3FF);
        v5 |= ((src[17] & 0x3FF) << 10);
        v5 |= ((src[18] & 0x3FF) << 20);
        v5 |= ((src[19] & 0x3FF) << 30);

        v6 = ((src[19] & 0x3FF) >> 2);
        v6 |= ((src[20] & 0x3FF) << 8);
        v6 |= ((src[21] & 0x3FF) << 18);
        v6 |= ((src[22] & 0x3FF) << 28);

        v7 = ((src[22] & 0x3FF) >> 4);
        v7 |= ((src[23] & 0x3FF) << 6);
        v7 |= ((src[24] & 0x3FF) << 16);
        v7 |= ((src[25] & 0x3FF) << 26);

        v8 = ((src[25] & 0x3FF) >> 6);
        v8 |= ((src[26] & 0x3FF) << 4);
        v8 |= ((src[27] & 0x3FF) << 14);
        v8 |= ((src[28] & 0x3FF) << 24);

        v9 = ((src[28] & 0x3FF) >> 8);
        v9 |= ((src[29] & 0x3FF) << 2);
        v9 |= ((src[30] & 0x3FF) << 12);
        v9 |= ((src[31] & 0x3FF) << 22);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 10);
    }

}

void Pack::pack11(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 11) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;

        v0 = (src[0] & 0x7FF);
        v0 |= ((src[1] & 0x7FF) << 11);
        v0 |= ((src[2] & 0x7FF) << 22);

        v1 = ((src[2] & 0x7FF) >> 10);
        v1 |= ((src[3] & 0x7FF) << 1);
        v1 |= ((src[4] & 0x7FF) << 12);
        v1 |= ((src[5] & 0x7FF) << 23);

        v2 = ((src[5] & 0x7FF) >> 9);
        v2 |= ((src[6] & 0x7FF) << 2);
        v2 |= ((src[7] & 0x7FF) << 13);
        v2 |= ((src[8] & 0x7FF) << 24);

        v3 = ((src[8] & 0x7FF) >> 8);
        v3 |= ((src[9] & 0x7FF) << 3);
        v3 |= ((src[10] & 0x7FF) << 14);
        v3 |= ((src[11] & 0x7FF) << 25);

        v4 = ((src[11] & 0x7FF) >> 7);
        v4 |= ((src[12] & 0x7FF) << 4);
        v4 |= ((src[13] & 0x7FF) << 15);
        v4 |= ((src[14] & 0x7FF) << 26);

        v5 = ((src[14] & 0x7FF) >> 6);
        v5 |= ((src[15] & 0x7FF) << 5);
        v5 |= ((src[16] & 0x7FF) << 16);
        v5 |= ((src[17] & 0x7FF) << 27);

        v6 = ((src[17] & 0x7FF) >> 5);
        v6 |= ((src[18] & 0x7FF) << 6);
        v6 |= ((src[19] & 0x7FF) << 17);
        v6 |= ((src[20] & 0x7FF) << 28);

        v7 = ((src[20] & 0x7FF) >> 4);
        v7 |= ((src[21] & 0x7FF) << 7);
        v7 |= ((src[22] & 0x7FF) << 18);
        v7 |= ((src[23] & 0x7FF) << 29);

        v8 = ((src[23] & 0x7FF) >> 3);
        v8 |= ((src[24] & 0x7FF) << 8);
        v8 |= ((src[25] & 0x7FF) << 19);
        v8 |= ((src[26] & 0x7FF) << 30);

        v9 = ((src[26] & 0x7FF) >> 2);
        v9 |= ((src[27] & 0x7FF) << 9);
        v9 |= ((src[28] & 0x7FF) << 20);
        v9 |= ((src[29] & 0x7FF) << 31);

        v10 = ((src[29] & 0x7FF) >> 1);
        v10 |= ((src[30] & 0x7FF) << 10);
        v10 |= ((src[31] & 0x7FF) << 21);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 11);
    }

}

void Pack::pack12(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 12) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;

        v0 = (src[0] & 0xFFF);
        v0 |= ((src[1] & 0xFFF) << 12);
        v0 |= ((src[2] & 0xFFF) << 24);

        v1 = ((src[2] & 0xFFF) >> 8);
        v1 |= ((src[3] & 0xFFF) << 4);
        v1 |= ((src[4] & 0xFFF) << 16);
        v1 |= ((src[5] & 0xFFF) << 28);

        v2 = ((src[5] & 0xFFF) >> 4);
        v2 |= ((src[6] & 0xFFF) << 8);
        v2 |= ((src[7] & 0xFFF) << 20);

        v3 = (src[8] & 0xFFF);
        v3 |= ((src[9] & 0xFFF) << 12);
        v3 |= ((src[10] & 0xFFF) << 24);

        v4 = ((src[10] & 0xFFF) >> 8);
        v4 |= ((src[11] & 0xFFF) << 4);
        v4 |= ((src[12] & 0xFFF) << 16);
        v4 |= ((src[13] & 0xFFF) << 28);

        v5 = ((src[13] & 0xFFF) >> 4);
        v5 |= ((src[14] & 0xFFF) << 8);
        v5 |= ((src[15] & 0xFFF) << 20);

        v6 = (src[16] & 0xFFF);
        v6 |= ((src[17] & 0xFFF) << 12);
        v6 |= ((src[18] & 0xFFF) << 24);

        v7 = ((src[18] & 0xFFF) >> 8);
        v7 |= ((src[19] & 0xFFF) << 4);
        v7 |= ((src[20] & 0xFFF) << 16);
        v7 |= ((src[21] & 0xFFF) << 28);

        v8 = ((src[21] & 0xFFF) >> 4);
        v8 |= ((src[22] & 0xFFF) << 8);
        v8 |= ((src[23] & 0xFFF) << 20);

        v9 = (src[24] & 0xFFF);
        v9 |= ((src[25] & 0xFFF) << 12);
        v9 |= ((src[26] & 0xFFF) << 24);

        v10 = ((src[26] & 0xFFF) >> 8);
        v10 |= ((src[27] & 0xFFF) << 4);
        v10 |= ((src[28] & 0xFFF) << 16);
        v10 |= ((src[29] & 0xFFF) << 28);

        v11 = ((src[29] & 0xFFF) >> 4);
        v11 |= ((src[30] & 0xFFF) << 8);
        v11 |= ((src[31] & 0xFFF) << 20);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 12);
    }

}

void Pack::pack13(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 13) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        v0 = (src[0] & 0x1FFF);
        v0 |= ((src[1] & 0x1FFF) << 13);
        v0 |= ((src[2] & 0x1FFF) << 26);

        v1 = ((src[2] & 0x1FFF) >> 6);
        v1 |= ((src[3] & 0x1FFF) << 7);
        v1 |= ((src[4] & 0x1FFF) << 20);

        v2 = ((src[4] & 0x1FFF) >> 12);
        v2 |= ((src[5] & 0x1FFF) << 1);
        v2 |= ((src[6] & 0x1FFF) << 14);
        v2 |= ((src[7] & 0x1FFF) << 27);

        v3 = ((src[7] & 0x1FFF) >> 5);
        v3 |= ((src[8] & 0x1FFF) << 8);
        v3 |= ((src[9] & 0x1FFF) << 21);

        v4 = ((src[9] & 0x1FFF) >> 11);
        v4 |= ((src[10] & 0x1FFF) << 2);
        v4 |= ((src[11] & 0x1FFF) << 15);
        v4 |= ((src[12] & 0x1FFF) << 28);

        v5 = ((src[12] & 0x1FFF) >> 4);
        v5 |= ((src[13] & 0x1FFF) << 9);
        v5 |= ((src[14] & 0x1FFF) << 22);

        v6 = ((src[14] & 0x1FFF) >> 10);
        v6 |= ((src[15] & 0x1FFF) << 3);
        v6 |= ((src[16] & 0x1FFF) << 16);
        v6 |= ((src[17] & 0x1FFF) << 29);

        v7 = ((src[17] & 0x1FFF) >> 3);
        v7 |= ((src[18] & 0x1FFF) << 10);
        v7 |= ((src[19] & 0x1FFF) << 23);

        v8 = ((src[19] & 0x1FFF) >> 9);
        v8 |= ((src[20] & 0x1FFF) << 4);
        v8 |= ((src[21] & 0x1FFF) << 17);
        v8 |= ((src[22] & 0x1FFF) << 30);

        v9 = ((src[22] & 0x1FFF) >> 2);
        v9 |= ((src[23] & 0x1FFF) << 11);
        v9 |= ((src[24] & 0x1FFF) << 24);

        v10 = ((src[24] & 0x1FFF) >> 8);
        v10 |= ((src[25] & 0x1FFF) << 5);
        v10 |= ((src[26] & 0x1FFF) << 18);
        v10 |= ((src[27] & 0x1FFF) << 31);

        v11 = ((src[27] & 0x1FFF) >> 1);
        v11 |= ((src[28] & 0x1FFF) << 12);
        v11 |= ((src[29] & 0x1FFF) << 25);

        v12 = ((src[29] & 0x1FFF) >> 7);
        v12 |= ((src[30] & 0x1FFF) << 6);
        v12 |= ((src[31] & 0x1FFF) << 19);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
     
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 13);
    }

}

void Pack::pack14(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 14) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;

        v0 = (src[0] & 0x3FFF);
        v0 |= ((src[1] & 0x3FFF) << 14);
        v0 |= ((src[2] & 0x3FFF) << 28);

        v1 = ((src[2] & 0x3FFF) >> 4);
        v1 |= ((src[3] & 0x3FFF) << 10);
        v1 |= ((src[4] & 0x3FFF) << 24);

        v2 = ((src[4] & 0x3FFF) >> 8);
        v2 |= ((src[5] & 0x3FFF) << 6);
        v2 |= ((src[6] & 0x3FFF) << 20);

        v3 = ((src[6] & 0x3FFF) >> 12);
        v3 |= ((src[7] & 0x3FFF) << 2);
        v3 |= ((src[8] & 0x3FFF) << 16);
        v3 |= ((src[9] & 0x3FFF) << 30);

        v4 = ((src[9] & 0x3FFF) >> 2);
        v4 |= ((src[10] & 0x3FFF) << 12);
        v4 |= ((src[11] & 0x3FFF) << 26);

        v5 = ((src[11] & 0x3FFF) >> 6);
        v5 |= ((src[12] & 0x3FFF) << 8);
        v5 |= ((src[13] & 0x3FFF) << 22);

        v6 = ((src[13] & 0x3FFF) >> 10);
        v6 |= ((src[14] & 0x3FFF) << 4);
        v6 |= ((src[15] & 0x3FFF) << 18);

        v7 = (src[16] & 0x3FFF);
        v7 |= ((src[17] & 0x3FFF) << 14);
        v7 |= ((src[18] & 0x3FFF) << 28);

        v8 = ((src[18] & 0x3FFF) >> 4);
        v8 |= ((src[19] & 0x3FFF) << 10);
        v8 |= ((src[20] & 0x3FFF) << 24);

        v9 = ((src[20] & 0x3FFF) >> 8);
        v9 |= ((src[21] & 0x3FFF) << 6);
        v9 |= ((src[22] & 0x3FFF) << 20);

        v10 = ((src[22] & 0x3FFF) >> 12);
        v10 |= ((src[23] & 0x3FFF) << 2);
        v10 |= ((src[24] & 0x3FFF) << 16);
        v10 |= ((src[25] & 0x3FFF) << 30);

        v11 = ((src[25] & 0x3FFF) >> 2);
        v11 |= ((src[26] & 0x3FFF) << 12);
        v11 |= ((src[27] & 0x3FFF) << 26);

        v12 = ((src[27] & 0x3FFF) >> 6);
        v12 |= ((src[28] & 0x3FFF) << 8);
        v12 |= ((src[29] & 0x3FFF) << 22);

        v13 = ((src[29] & 0x3FFF) >> 10);
        v13 |= ((src[30] & 0x3FFF) << 4);
        v13 |= ((src[31] & 0x3FFF) << 18);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 14);
    }

}

void Pack::pack15(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 15) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;

        v0 = (src[0] & 0x7FFF);
        v0 |= ((src[1] & 0x7FFF) << 15);
        v0 |= ((src[2] & 0x7FFF) << 30);

        v1 = ((src[2] & 0x7FFF) >> 2);
        v1 |= ((src[3] & 0x7FFF) << 13);
        v1 |= ((src[4] & 0x7FFF) << 28);

        v2 = ((src[4] & 0x7FFF) >> 4);
        v2 |= ((src[5] & 0x7FFF) << 11);
        v2 |= ((src[6] & 0x7FFF) << 26);

        v3 = ((src[6] & 0x7FFF) >> 6);
        v3 |= ((src[7] & 0x7FFF) << 9);
        v3 |= ((src[8] & 0x7FFF) << 24);

        v4 = ((src[8] & 0x7FFF) >> 8);
        v4 |= ((src[9] & 0x7FFF) << 7);
        v4 |= ((src[10] & 0x7FFF) << 22);

        v5 = ((src[10] & 0x7FFF) >> 10);
        v5 |= ((src[11] & 0x7FFF) << 5);
        v5 |= ((src[12] & 0x7FFF) << 20);

        v6 = ((src[12] & 0x7FFF) >> 12);
        v6 |= ((src[13] & 0x7FFF) << 3);
        v6 |= ((src[14] & 0x7FFF) << 18);

        v7 = ((src[14] & 0x7FFF) >> 14);
        v7 |= ((src[15] & 0x7FFF) << 1);
        v7 |= ((src[16] & 0x7FFF) << 16);
        v7 |= ((src[17] & 0x7FFF) << 31);

        v8 = ((src[17] & 0x7FFF) >> 1);
        v8 |= ((src[18] & 0x7FFF) << 14);
        v8 |= ((src[19] & 0x7FFF) << 29);

        v9 = ((src[19] & 0x7FFF) >> 3);
        v9 |= ((src[20] & 0x7FFF) << 12);
        v9 |= ((src[21] & 0x7FFF) << 27);

        v10 = ((src[21] & 0x7FFF) >> 5);
        v10 |= ((src[22] & 0x7FFF) << 10);
        v10 |= ((src[23] & 0x7FFF) << 25);

        v11 = ((src[23] & 0x7FFF) >> 7);
        v11 |= ((src[24] & 0x7FFF) << 8);
        v11 |= ((src[25] & 0x7FFF) << 23);

        v12 = ((src[25] & 0x7FFF) >> 9);
        v12 |= ((src[26] & 0x7FFF) << 6);
        v12 |= ((src[27] & 0x7FFF) << 21);

        v13 = ((src[27] & 0x7FFF) >> 11);
        v13 |= ((src[28] & 0x7FFF) << 4);
        v13 |= ((src[29] & 0x7FFF) << 19);

        v14 = ((src[29] & 0x7FFF) >> 13);
        v14 |= ((src[30] & 0x7FFF) << 2);
        v14 |= ((src[31] & 0x7FFF) << 17);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 15);
    }
}

void Pack::pack16(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 16) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;

        v0 = (src[0] & 0xFFFF);
        v0 |= ((src[1] & 0xFFFF) << 16);

        v1 = (src[2] & 0xFFFF);
        v1 |= ((src[3] & 0xFFFF) << 16);

        v2 = (src[4] & 0xFFFF);
        v2 |= ((src[5] & 0xFFFF) << 16);

        v3 = (src[6] & 0xFFFF);
        v3 |= ((src[7] & 0xFFFF) << 16);

        v4 = (src[8] & 0xFFFF);
        v4 |= ((src[9] & 0xFFFF) << 16);

        v5 = (src[10] & 0xFFFF);
        v5 |= ((src[11] & 0xFFFF) << 16);

        v6 = (src[12] & 0xFFFF);
        v6 |= ((src[13] & 0xFFFF) << 16);

        v7 = (src[14] & 0xFFFF);
        v7 |= ((src[15] & 0xFFFF) << 16);

        v8 = (src[16] & 0xFFFF);
        v8 |= ((src[17] & 0xFFFF) << 16);

        v9 = (src[18] & 0xFFFF);
        v9 |= ((src[19] & 0xFFFF) << 16);

        v10 = (src[20] & 0xFFFF);
        v10 |= ((src[21] & 0xFFFF) << 16);

        v11 = (src[22] & 0xFFFF);
        v11 |= ((src[23] & 0xFFFF) << 16);

        v12 = (src[24] & 0xFFFF);
        v12 |= ((src[25] & 0xFFFF) << 16);

        v13 = (src[26] & 0xFFFF);
        v13 |= ((src[27] & 0xFFFF) << 16);

        v14 = (src[28] & 0xFFFF);
        v14 |= ((src[29] & 0xFFFF) << 16);

        v15 = (src[30] & 0xFFFF);
        v15 |= ((src[31] & 0xFFFF) << 16);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 16);
    }

}

void Pack::pack17(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 17) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;

        v0 = (src[0] & 0x1FFFF);
        v0 |= ((src[1] & 0x1FFFF) << 17);

        v1 = ((src[1] & 0x1FFFF) >> 15);
        v1 |= ((src[2] & 0x1FFFF) << 2);
        v1 |= ((src[3] & 0x1FFFF) << 19);

        v2 = ((src[3] & 0x1FFFF) >> 13);
        v2 |= ((src[4] & 0x1FFFF) << 4);
        v2 |= ((src[5] & 0x1FFFF) << 21);

        v3 = ((src[5] & 0x1FFFF) >> 11);
        v3 |= ((src[6] & 0x1FFFF) << 6);
        v3 |= ((src[7] & 0x1FFFF) << 23);

        v4 = ((src[7] & 0x1FFFF) >> 9);
        v4 |= ((src[8] & 0x1FFFF) << 8);
        v4 |= ((src[9] & 0x1FFFF) << 25);

        v5 = ((src[9] & 0x1FFFF) >> 7);
        v5 |= ((src[10] & 0x1FFFF) << 10);
        v5 |= ((src[11] & 0x1FFFF) << 27);

        v6 = ((src[11] & 0x1FFFF) >> 5);
        v6 |= ((src[12] & 0x1FFFF) << 12);
        v6 |= ((src[13] & 0x1FFFF) << 29);

        v7 = ((src[13] & 0x1FFFF) >> 3);
        v7 |= ((src[14] & 0x1FFFF) << 14);
        v7 |= ((src[15] & 0x1FFFF) << 31);

        v8 = ((src[15] & 0x1FFFF) >> 1);
        v8 |= ((src[16] & 0x1FFFF) << 16);

        v9 = ((src[16] & 0x1FFFF) >> 16);
        v9 |= ((src[17] & 0x1FFFF) << 1);
        v9 |= ((src[18] & 0x1FFFF) << 18);

        v10 = ((src[18] & 0x1FFFF) >> 14);
        v10 |= ((src[19] & 0x1FFFF) << 3);
        v10 |= ((src[20] & 0x1FFFF) << 20);

        v11 = ((src[20] & 0x1FFFF) >> 12);
        v11 |= ((src[21] & 0x1FFFF) << 5);
        v11 |= ((src[22] & 0x1FFFF) << 22);

        v12 = ((src[22] & 0x1FFFF) >> 10);
        v12 |= ((src[23] & 0x1FFFF) << 7);
        v12 |= ((src[24] & 0x1FFFF) << 24);

        v13 = ((src[24] & 0x1FFFF) >> 8);
        v13 |= ((src[25] & 0x1FFFF) << 9);
        v13 |= ((src[26] & 0x1FFFF) << 26);

        v14 = ((src[26] & 0x1FFFF) >> 6);
        v14 |= ((src[27] & 0x1FFFF) << 11);
        v14 |= ((src[28] & 0x1FFFF) << 28);

        v15 = ((src[28] & 0x1FFFF) >> 4);
        v15 |= ((src[29] & 0x1FFFF) << 13);
        v15 |= ((src[30] & 0x1FFFF) << 30);

        v16 = ((src[30] & 0x1FFFF) >> 2);
        v16 |= ((src[31] & 0x1FFFF) << 15);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 17);
    }
}

void Pack::pack18(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 18) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;

        v0 = (src[0] & 0x3FFFF);
        v0 |= ((src[1] & 0x3FFFF) << 18);

        v1 = ((src[1] & 0x3FFFF) >> 14);
        v1 |= ((src[2] & 0x3FFFF) << 4);
        v1 |= ((src[3] & 0x3FFFF) << 22);

        v2 = ((src[3] & 0x3FFFF) >> 10);
        v2 |= ((src[4] & 0x3FFFF) << 8);
        v2 |= ((src[5] & 0x3FFFF) << 26);

        v3 = ((src[5] & 0x3FFFF) >> 6);
        v3 |= ((src[6] & 0x3FFFF) << 12);
        v3 |= ((src[7] & 0x3FFFF) << 30);

        v4 = ((src[7] & 0x3FFFF) >> 2);
        v4 |= ((src[8] & 0x3FFFF) << 16);

        v5 = ((src[8] & 0x3FFFF) >> 16);
        v5 |= ((src[9] & 0x3FFFF) << 2);
        v5 |= ((src[10] & 0x3FFFF) << 20);

        v6 = ((src[10] & 0x3FFFF) >> 12);
        v6 |= ((src[11] & 0x3FFFF) << 6);
        v6 |= ((src[12] & 0x3FFFF) << 24);

        v7 = ((src[12] & 0x3FFFF) >> 8);
        v7 |= ((src[13] & 0x3FFFF) << 10);
        v7 |= ((src[14] & 0x3FFFF) << 28);

        v8 = ((src[14] & 0x3FFFF) >> 4);
        v8 |= ((src[15] & 0x3FFFF) << 14);

        v9 = (src[16] & 0x3FFFF);
        v9 |= ((src[17] & 0x3FFFF) << 18);

        v10 = ((src[17] & 0x3FFFF) >> 14);
        v10 |= ((src[18] & 0x3FFFF) << 4);
        v10 |= ((src[19] & 0x3FFFF) << 22);

        v11 = ((src[19] & 0x3FFFF) >> 10);
        v11 |= ((src[20] & 0x3FFFF) << 8);
        v11 |= ((src[21] & 0x3FFFF) << 26);

        v12 = ((src[21] & 0x3FFFF) >> 6);
        v12 |= ((src[22] & 0x3FFFF) << 12);
        v12 |= ((src[23] & 0x3FFFF) << 30);

        v13 = ((src[23] & 0x3FFFF) >> 2);
        v13 |= ((src[24] & 0x3FFFF) << 16);

        v14 = ((src[24] & 0x3FFFF) >> 16);
        v14 |= ((src[25] & 0x3FFFF) << 2);
        v14 |= ((src[26] & 0x3FFFF) << 20);

        v15 = ((src[26] & 0x3FFFF) >> 12);
        v15 |= ((src[27] & 0x3FFFF) << 6);
        v15 |= ((src[28] & 0x3FFFF) << 24);

        v16 = ((src[28] & 0x3FFFF) >> 8);
        v16 |= ((src[29] & 0x3FFFF) << 10);
        v16 |= ((src[30] & 0x3FFFF) << 28);

        v17 = ((src[30] & 0x3FFFF) >> 4);
        v17 |= ((src[31] & 0x3FFFF) << 14);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 18);
    }
}

void Pack::pack19(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 19) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;

        v0 = (src[0] & 0x7FFFF);
        v0 |= ((src[1] & 0x7FFFF) << 19);

        v1 = ((src[1] & 0x7FFFF) >> 13);
        v1 |= ((src[2] & 0x7FFFF) << 6);
        v1 |= ((src[3] & 0x7FFFF) << 25);

        v2 = ((src[3] & 0x7FFFF) >> 7);
        v2 |= ((src[4] & 0x7FFFF) << 12);
        v2 |= ((src[5] & 0x7FFFF) << 31);

        v3 = ((src[5] & 0x7FFFF) >> 1);
        v3 |= ((src[6] & 0x7FFFF) << 18);

        v4 = ((src[6] & 0x7FFFF) >> 14);
        v4 |= ((src[7] & 0x7FFFF) << 5);
        v4 |= ((src[8] & 0x7FFFF) << 24);

        v5 = ((src[8] & 0x7FFFF) >> 8);
        v5 |= ((src[9] & 0x7FFFF) << 11);
        v5 |= ((src[10] & 0x7FFFF) << 30);

        v6 = ((src[10] & 0x7FFFF) >> 2);
        v6 |= ((src[11] & 0x7FFFF) << 17);

        v7 = ((src[11] & 0x7FFFF) >> 15);
        v7 |= ((src[12] & 0x7FFFF) << 4);
        v7 |= ((src[13] & 0x7FFFF) << 23);

        v8 = ((src[13] & 0x7FFFF) >> 9);
        v8 |= ((src[14] & 0x7FFFF) << 10);
        v8 |= ((src[15] & 0x7FFFF) << 29);

        v9 = ((src[15] & 0x7FFFF) >> 3);
        v9 |= ((src[16] & 0x7FFFF) << 16);

        v10 = ((src[16] & 0x7FFFF) >> 16);
        v10 |= ((src[17] & 0x7FFFF) << 3);
        v10 |= ((src[18] & 0x7FFFF) << 22);

        v11 = ((src[18] & 0x7FFFF) >> 10);
        v11 |= ((src[19] & 0x7FFFF) << 9);
        v11 |= ((src[20] & 0x7FFFF) << 28);

        v12 = ((src[20] & 0x7FFFF) >> 4);
        v12 |= ((src[21] & 0x7FFFF) << 15);

        v13 = ((src[21] & 0x7FFFF) >> 17);
        v13 |= ((src[22] & 0x7FFFF) << 2);
        v13 |= ((src[23] & 0x7FFFF) << 21);

        v14 = ((src[23] & 0x7FFFF) >> 11);
        v14 |= ((src[24] & 0x7FFFF) << 8);
        v14 |= ((src[25] & 0x7FFFF) << 27);

        v15 = ((src[25] & 0x7FFFF) >> 5);
        v15 |= ((src[26] & 0x7FFFF) << 14);

        v16 = ((src[26] & 0x7FFFF) >> 18);
        v16 |= ((src[27] & 0x7FFFF) << 1);
        v16 |= ((src[28] & 0x7FFFF) << 20);

        v17 = ((src[28] & 0x7FFFF) >> 12);
        v17 |= ((src[29] & 0x7FFFF) << 7);
        v17 |= ((src[30] & 0x7FFFF) << 26);

        v18 = ((src[30] & 0x7FFFF) >> 6);
        v18 |= ((src[31] & 0x7FFFF) << 13);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 19);
    }

}

void Pack::pack20(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 20) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;

        v0 = (src[0] & 0xFFFFF);
        v0 |= ((src[1] & 0xFFFFF) << 20);

        v1 = ((src[1] & 0xFFFFF) >> 12);
        v1 |= ((src[2] & 0xFFFFF) << 8);
        v1 |= ((src[3] & 0xFFFFF) << 28);

        v2 = ((src[3] & 0xFFFFF) >> 4);
        v2 |= ((src[4] & 0xFFFFF) << 16);

        v3 = ((src[4] & 0xFFFFF) >> 16);
        v3 |= ((src[5] & 0xFFFFF) << 4);
        v3 |= ((src[6] & 0xFFFFF) << 24);

        v4 = ((src[6] & 0xFFFFF) >> 8);
        v4 |= ((src[7] & 0xFFFFF) << 12);

        v5 = (src[8] & 0xFFFFF);
        v5 |= ((src[9] & 0xFFFFF) << 20);

        v6 = ((src[9] & 0xFFFFF) >> 12);
        v6 |= ((src[10] & 0xFFFFF) << 8);
        v6 |= ((src[11] & 0xFFFFF) << 28);

        v7 = ((src[11] & 0xFFFFF) >> 4);
        v7 |= ((src[12] & 0xFFFFF) << 16);

        v8 = ((src[12] & 0xFFFFF) >> 16);
        v8 |= ((src[13] & 0xFFFFF) << 4);
        v8 |= ((src[14] & 0xFFFFF) << 24);

        v9 = ((src[14] & 0xFFFFF) >> 8);
        v9 |= ((src[15] & 0xFFFFF) << 12);

        v10 = (src[16] & 0xFFFFF);
        v10 |= ((src[17] & 0xFFFFF) << 20);

        v11 = ((src[17] & 0xFFFFF) >> 12);
        v11 |= ((src[18] & 0xFFFFF) << 8);
        v11 |= ((src[19] & 0xFFFFF) << 28);

        v12 = ((src[19] & 0xFFFFF) >> 4);
        v12 |= ((src[20] & 0xFFFFF) << 16);

        v13 = ((src[20] & 0xFFFFF) >> 16);
        v13 |= ((src[21] & 0xFFFFF) << 4);
        v13 |= ((src[22] & 0xFFFFF) << 24);

        v14 = ((src[22] & 0xFFFFF) >> 8);
        v14 |= ((src[23] & 0xFFFFF) << 12);

        v15 = (src[24] & 0xFFFFF);
        v15 |= ((src[25] & 0xFFFFF) << 20);

        v16 = ((src[25] & 0xFFFFF) >> 12);
        v16 |= ((src[26] & 0xFFFFF) << 8);
        v16 |= ((src[27] & 0xFFFFF) << 28);

        v17 = ((src[27] & 0xFFFFF) >> 4);
        v17 |= ((src[28] & 0xFFFFF) << 16);

        v18 = ((src[28] & 0xFFFFF) >> 16);
        v18 |= ((src[29] & 0xFFFFF) << 4);
        v18 |= ((src[30] & 0xFFFFF) << 24);

        v19 = ((src[30] & 0xFFFFF) >> 8);
        v19 |= ((src[31] & 0xFFFFF) << 12);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 20);
    }

}

void Pack::pack21(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 21) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;

        v0 = (src[0] & 0x1FFFFF);
        v0 |= ((src[1] & 0x1FFFFF) << 21);

        v1 = ((src[1] & 0x1FFFFF) >> 11);
        v1 |= ((src[2] & 0x1FFFFF) << 10);
        v1 |= ((src[3] & 0x1FFFFF) << 31);

        v2 = ((src[3] & 0x1FFFFF) >> 1);
        v2 |= ((src[4] & 0x1FFFFF) << 20);

        v3 = ((src[4] & 0x1FFFFF) >> 12);
        v3 |= ((src[5] & 0x1FFFFF) << 9);
        v3 |= ((src[6] & 0x1FFFFF) << 30);

        v4 = ((src[6] & 0x1FFFFF) >> 2);
        v4 |= ((src[7] & 0x1FFFFF) << 19);

        v5 = ((src[7] & 0x1FFFFF) >> 13);
        v5 |= ((src[8] & 0x1FFFFF) << 8);
        v5 |= ((src[9] & 0x1FFFFF) << 29);

        v6 = ((src[9] & 0x1FFFFF) >> 3);
        v6 |= ((src[10] & 0x1FFFFF) << 18);

        v7 = ((src[10] & 0x1FFFFF) >> 14);
        v7 |= ((src[11] & 0x1FFFFF) << 7);
        v7 |= ((src[12] & 0x1FFFFF) << 28);

        v8 = ((src[12] & 0x1FFFFF) >> 4);
        v8 |= ((src[13] & 0x1FFFFF) << 17);

        v9 = ((src[13] & 0x1FFFFF) >> 15);
        v9 |= ((src[14] & 0x1FFFFF) << 6);
        v9 |= ((src[15] & 0x1FFFFF) << 27);

        v10 = ((src[15] & 0x1FFFFF) >> 5);
        v10 |= ((src[16] & 0x1FFFFF) << 16);

        v11 = ((src[16] & 0x1FFFFF) >> 16);
        v11 |= ((src[17] & 0x1FFFFF) << 5);
        v11 |= ((src[18] & 0x1FFFFF) << 26);

        v12 = ((src[18] & 0x1FFFFF) >> 6);
        v12 |= ((src[19] & 0x1FFFFF) << 15);

        v13 = ((src[19] & 0x1FFFFF) >> 17);
        v13 |= ((src[20] & 0x1FFFFF) << 4);
        v13 |= ((src[21] & 0x1FFFFF) << 25);

        v14 = ((src[21] & 0x1FFFFF) >> 7);
        v14 |= ((src[22] & 0x1FFFFF) << 14);

        v15 = ((src[22] & 0x1FFFFF) >> 18);
        v15 |= ((src[23] & 0x1FFFFF) << 3);
        v15 |= ((src[24] & 0x1FFFFF) << 24);

        v16 = ((src[24] & 0x1FFFFF) >> 8);
        v16 |= ((src[25] & 0x1FFFFF) << 13);

        v17 = ((src[25] & 0x1FFFFF) >> 19);
        v17 |= ((src[26] & 0x1FFFFF) << 2);
        v17 |= ((src[27] & 0x1FFFFF) << 23);

        v18 = ((src[27] & 0x1FFFFF) >> 9);
        v18 |= ((src[28] & 0x1FFFFF) << 12);

        v19 = ((src[28] & 0x1FFFFF) >> 20);
        v19 |= ((src[29] & 0x1FFFFF) << 1);
        v19 |= ((src[30] & 0x1FFFFF) << 22);
        
        v20 = ((src[30] & 0x1FFFFF) >> 10);
        v20 |= ((src[31] & 0x1FFFFF) << 11);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 21);
    }

}

void Pack::pack22(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 22) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;

        v0 = (src[0] & 0x3FFFFF);
        v0 |= ((src[1] & 0x3FFFFF) << 22);

        v1 = ((src[1] & 0x3FFFFF) >> 10);
        v1 |= ((src[2] & 0x3FFFFF) << 12);

        v2 = ((src[2] & 0x3FFFFF) >> 20);
        v2 |= ((src[3] & 0x3FFFFF) << 2);
        v2 |= ((src[4] & 0x3FFFFF) << 24);

        v3 = ((src[4] & 0x3FFFFF) >> 8);
        v3 |= ((src[5] & 0x3FFFFF) << 14);

        v4 = ((src[5] & 0x3FFFFF) >> 18);
        v4 |= ((src[6] & 0x3FFFFF) << 4);
        v4 |= ((src[7] & 0x3FFFFF) << 26);

        v5 = ((src[7] & 0x3FFFFF) >> 6);
        v5 |= ((src[8] & 0x3FFFFF) << 16);

        v6 = ((src[8] & 0x3FFFFF) >> 16);
        v6 |= ((src[9] & 0x3FFFFF) << 6);
        v6 |= ((src[10] & 0x3FFFFF) << 28);

        v7 = ((src[10] & 0x3FFFFF) >> 4);
        v7 |= ((src[11] & 0x3FFFFF) << 18);

        v8 = ((src[11] & 0x3FFFFF) >> 14);
        v8 |= ((src[12] & 0x3FFFFF) << 8);
        v8 |= ((src[13] & 0x3FFFFF) << 30);

        v9 = ((src[13] & 0x3FFFFF) >> 2);
        v9 |= ((src[14] & 0x3FFFFF) << 20);

        v10 = ((src[14] & 0x3FFFFF) >> 12);
        v10 |= ((src[15] & 0x3FFFFF) << 10);

        v11 = (src[16] & 0x3FFFFF);
        v11 |= ((src[17] & 0x3FFFFF) << 22);

        v12 = ((src[17] & 0x3FFFFF) >> 10);
        v12 |= ((src[18] & 0x3FFFFF) << 12);

        v13 = ((src[18] & 0x3FFFFF) >> 20);
        v13 |= ((src[19] & 0x3FFFFF) << 2);
        v13 |= ((src[20] & 0x3FFFFF) << 24);

        v14 = ((src[20] & 0x3FFFFF) >> 8);
        v14 |= ((src[21] & 0x3FFFFF) << 14);

        v15 = ((src[21] & 0x3FFFFF) >> 18);
        v15 |= ((src[22] & 0x3FFFFF) << 4);
        v15 |= ((src[23] & 0x3FFFFF) << 26);

        v16 = ((src[23] & 0x3FFFFF) >> 6);
        v16 |= ((src[24] & 0x3FFFFF) << 16);

        v17 = ((src[24] & 0x3FFFFF) >> 16);
        v17 |= ((src[25] & 0x3FFFFF) << 6);
        v17 |= ((src[26] & 0x3FFFFF) << 28);

        v18 = ((src[26] & 0x3FFFFF) >> 4);
        v18 |= ((src[27] & 0x3FFFFF) << 18);

        v19 = ((src[27] & 0x3FFFFF) >> 14);
        v19 |= ((src[28] & 0x3FFFFF) << 8);
        v19 |= ((src[29] & 0x3FFFFF) << 30);

        v20 = ((src[29] & 0x3FFFFF) >> 2);
        v20 |= ((src[30] & 0x3FFFFF) << 20);

        v21 = ((src[30] & 0x3FFFFF) >> 12);
        v21 |= ((src[31] & 0x3FFFFF) << 10);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 22);
    }

}

void Pack::pack23(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 23) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;

        v0 = (src[0] & 0x7FFFFF);
        v0 |= ((src[1] & 0x7FFFFF) << 23);

        v1 = ((src[1] & 0x7FFFFF) >> 9);
        v1 |= ((src[2] & 0x7FFFFF) << 14);

        v2 = ((src[2] & 0x7FFFFF) >> 18);
        v2 |= ((src[3] & 0x7FFFFF) << 5);
        v2 |= ((src[4] & 0x7FFFFF) << 28);

        v3 = ((src[4] & 0x7FFFFF) >> 4);
        v3 |= ((src[5] & 0x7FFFFF) << 19);

        v4 = ((src[5] & 0x7FFFFF) >> 13);
        v4 |= ((src[6] & 0x7FFFFF) << 10);

        v5 = ((src[6] & 0x7FFFFF) >> 22);
        v5 |= ((src[7] & 0x7FFFFF) << 1);
        v5 |= ((src[8] & 0x7FFFFF) << 24);

        v6 = ((src[8] & 0x7FFFFF) >> 8);
        v6 |= ((src[9] & 0x7FFFFF) << 15);

        v7 = ((src[9] & 0x7FFFFF) >> 17);
        v7 |= ((src[10] & 0x7FFFFF) << 6);
        v7 |= ((src[11] & 0x7FFFFF) << 29);

        v8 = ((src[11] & 0x7FFFFF) >> 3);
        v8 |= ((src[12] & 0x7FFFFF) << 20);

        v9 = ((src[12] & 0x7FFFFF) >> 12);
        v9 |= ((src[13] & 0x7FFFFF) << 11);

        v10 = ((src[13] & 0x7FFFFF) >> 21);
        v10 |= ((src[14] & 0x7FFFFF) << 2);
        v10 |= ((src[15] & 0x7FFFFF) << 25);

        v11 = ((src[15] & 0x7FFFFF) >> 7);
        v11 |= ((src[16] & 0x7FFFFF) << 16);

        v12 = ((src[16] & 0x7FFFFF) >> 16);
        v12 |= ((src[17] & 0x7FFFFF) << 7);
        v12 |= ((src[18] & 0x7FFFFF) << 30);

        v13 = ((src[18] & 0x7FFFFF) >> 2);
        v13 |= ((src[19] & 0x7FFFFF) << 21);

        v14 = ((src[19] & 0x7FFFFF) >> 11);
        v14 |= ((src[20] & 0x7FFFFF) << 12);

        v15 = ((src[20] & 0x7FFFFF) >> 20);
        v15 |= ((src[21] & 0x7FFFFF) << 3);
        v15 |= ((src[22] & 0x7FFFFF) << 26);

        v16 = ((src[22] & 0x7FFFFF) >> 6);
        v16 |= ((src[23] & 0x7FFFFF) << 17);

        v17 = ((src[23] & 0x7FFFFF) >> 15);
        v17 |= ((src[24] & 0x7FFFFF) << 8);
        v17 |= ((src[25] & 0x7FFFFF) << 31);

        v18 = ((src[25] & 0x7FFFFF) >> 1);
        v18 |= ((src[26] & 0x7FFFFF) << 22);

        v19 = ((src[26] & 0x7FFFFF) >> 10);
        v19 |= ((src[27] & 0x7FFFFF) << 13);

        v20 = ((src[27] & 0x7FFFFF) >> 19);
        v20 |= ((src[28] & 0x7FFFFF) << 4);
        v20 |= ((src[29] & 0x7FFFFF) << 27);

        v21 = ((src[29] & 0x7FFFFF) >> 5);
        v21 |= ((src[30] & 0x7FFFFF) << 18);

        v22 = ((src[30] & 0x7FFFFF) >> 14);
        v22 |= ((src[31] & 0x7FFFFF) << 9);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 23);
    }

}

void Pack::pack24(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 24) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;

        v0 = (src[0] & 0xFFFFFF);
        v0 |= ((src[1] & 0xFFFFFF) << 24);

        v1 = ((src[1] & 0xFFFFFF) >> 8);
        v1 |= ((src[2] & 0xFFFFFF) << 16);

        v2 = ((src[2] & 0xFFFFFF) >> 16);
        v2 |= ((src[3] & 0xFFFFFF) << 8);

        v3 = (src[4] & 0xFFFFFF);
        v3 |= ((src[5] & 0xFFFFFF) << 24);

        v4 = ((src[5] & 0xFFFFFF) >> 8);
        v4 |= ((src[6] & 0xFFFFFF) << 16);

        v5 = ((src[6] & 0xFFFFFF) >> 16);
        v5 |= ((src[7] & 0xFFFFFF) << 8);

        v6 = (src[8] & 0xFFFFFF);
        v6 |= ((src[9] & 0xFFFFFF) << 24);

        v7 = ((src[9] & 0xFFFFFF) >> 8);
        v7 |= ((src[10] & 0xFFFFFF) << 16);

        v8 = ((src[10] & 0xFFFFFF) >> 16);
        v8 |= ((src[11] & 0xFFFFFF) << 8);

        v9 = (src[12] & 0xFFFFFF);
        v9 |= ((src[13] & 0xFFFFFF) << 24);

        v10 = ((src[13] & 0xFFFFFF) >> 8);
        v10 |= ((src[14] & 0xFFFFFF) << 16);

        v11 = ((src[14] & 0xFFFFFF) >> 16);
        v11 |= ((src[15] & 0xFFFFFF) << 8);

        v12 = (src[16] & 0xFFFFFF);
        v12 |= ((src[17] & 0xFFFFFF) << 24);

        v13 = ((src[17] & 0xFFFFFF) >> 8);
        v13 |= ((src[18] & 0xFFFFFF) << 16);

        v14 = ((src[18] & 0xFFFFFF) >> 16);
        v14 |= ((src[19] & 0xFFFFFF) << 8);

        v15 = (src[20] & 0xFFFFFF);
        v15 |= ((src[21] & 0xFFFFFF) << 24);

        v16 = ((src[21] & 0xFFFFFF) >> 8);
        v16 |= ((src[22] & 0xFFFFFF) << 16);

        v17 = ((src[22] & 0xFFFFFF) >> 16);
        v17 |= ((src[23] & 0xFFFFFF) << 8);

        v18 = (src[24] & 0xFFFFFF);
        v18 |= ((src[25] & 0xFFFFFF) << 24);

        v19 = ((src[25] & 0xFFFFFF) >> 8);
        v19 |= ((src[26] & 0xFFFFFF) << 16);

        v20 = ((src[26] & 0xFFFFFF) >> 16);
        v20 |= ((src[27] & 0xFFFFFF) << 8);

        v21 = (src[28] & 0xFFFFFF);
        v21 |= ((src[29] & 0xFFFFFF) << 24);

        v22 = ((src[29] & 0xFFFFFF) >> 8);
        v22 |= ((src[30] & 0xFFFFFF) << 16);

        v23 = ((src[30] & 0xFFFFFF) >> 16);
        v23 |= ((src[31] & 0xFFFFFF) << 8);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 24);
    }

}

void Pack::pack25(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 25) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;
        uint32_t v24;

        v0 = (src[0] & 0x1FFFFFF);
        v0 |= ((src[1] & 0x1FFFFFF) << 25);

        v1 = ((src[1] & 0x1FFFFFF) >> 7);
        v1 |= ((src[2] & 0x1FFFFFF) << 18);

        v2 = ((src[2] & 0x1FFFFFF) >> 14);
        v2 |= ((src[3] & 0x1FFFFFF) << 11);

        v3 = ((src[3] & 0x1FFFFFF) >> 21);
        v3 |= ((src[4] & 0x1FFFFFF) << 4);
        v3 |= ((src[5] & 0x1FFFFFF) << 29);

        v4 = ((src[5] & 0x1FFFFFF) >> 3);
        v4 |= ((src[6] & 0x1FFFFFF) << 22);

        v5 = ((src[6] & 0x1FFFFFF) >> 10);
        v5 |= ((src[7] & 0x1FFFFFF) << 15);

        v6 = ((src[7] & 0x1FFFFFF) >> 17);
        v6 |= ((src[8] & 0x1FFFFFF) << 8);

        v7 = ((src[8] & 0x1FFFFFF) >> 24);
        v7 |= ((src[9] & 0x1FFFFFF) << 1);
        v7 |= ((src[10] & 0x1FFFFFF) << 26);

        v8 = ((src[10] & 0x1FFFFFF) >> 6);
        v8 |= ((src[11] & 0x1FFFFFF) << 19);

        v9 = ((src[11] & 0x1FFFFFF) >> 13);
        v9 |= ((src[12] & 0x1FFFFFF) << 12);

        v10 = ((src[12] & 0x1FFFFFF) >> 20);
        v10 |= ((src[13] & 0x1FFFFFF) << 5);
        v10 |= ((src[14] & 0x1FFFFFF) << 30);

        v11 = ((src[14] & 0x1FFFFFF) >> 2);
        v11 |= ((src[15] & 0x1FFFFFF) << 23);

        v12 = ((src[15] & 0x1FFFFFF) >> 9);
        v12 |= ((src[16] & 0x1FFFFFF) << 16);

        v13 = ((src[16] & 0x1FFFFFF) >> 16);
        v13 |= ((src[17] & 0x1FFFFFF) << 9);

        v14 = ((src[17] & 0x1FFFFFF) >> 23);
        v14 |= ((src[18] & 0x1FFFFFF) << 2);
        v14 |= ((src[19] & 0x1FFFFFF) << 27);

        v15 = ((src[19] & 0x1FFFFFF) >> 5);
        v15 |= ((src[20] & 0x1FFFFFF) << 20);

        v16 = ((src[20] & 0x1FFFFFF) >> 12);
        v16 |= ((src[21] & 0x1FFFFFF) << 13);

        v17 = ((src[21] & 0x1FFFFFF) >> 19);
        v17 |= ((src[22] & 0x1FFFFFF) << 6);
        v17 |= ((src[23] & 0x1FFFFFF) << 31);

        v18 = ((src[23] & 0x1FFFFFF) >> 1);
        v18 |= ((src[24] & 0x1FFFFFF) << 24);

        v19 = ((src[24] & 0x1FFFFFF) >> 8);
        v19 |= ((src[25] & 0x1FFFFFF) << 17);

        v20 = ((src[25] & 0x1FFFFFF) >> 15);
        v20 |= ((src[26] & 0x1FFFFFF) << 10);

        v21 = ((src[26] & 0x1FFFFFF) >> 22);
        v21 |= ((src[27] & 0x1FFFFFF) << 3);
        v21 |= ((src[28] & 0x1FFFFFF) << 28);

        v22 = ((src[28] & 0x1FFFFFF) >> 4);
        v22 |= ((src[29] & 0x1FFFFFF) << 21);

        v23 = ((src[29] & 0x1FFFFFF) >> 11);
        v23 |= ((src[30] & 0x1FFFFFF) << 14);

        v24 = ((src[30] & 0x1FFFFFF) >> 18);
        v24 |= ((src[31] & 0x1FFFFFF) << 7);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;
        dest[24] = v24;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 25);
    }

}

void Pack::pack26(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 26) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;
        uint32_t v24;
        uint32_t v25;

        v0 = (src[0] & 0x3FFFFFF);
        v0 |= ((src[1] & 0x3FFFFFF) << 26);

        v1 = ((src[1] & 0x3FFFFFF) >> 6);
        v1 |= ((src[2] & 0x3FFFFFF) << 20);

        v2 = ((src[2] & 0x3FFFFFF) >> 12);
        v2 |= ((src[3] & 0x3FFFFFF) << 14);

        v3 = ((src[3] & 0x3FFFFFF) >> 18);
        v3 |= ((src[4] & 0x3FFFFFF) << 8);

        v4 = ((src[4] & 0x3FFFFFF) >> 24);
        v4 |= ((src[5] & 0x3FFFFFF) << 2);
        v4 |= ((src[6] & 0x3FFFFFF) << 28);

        v5 = ((src[6] & 0x3FFFFFF) >> 4);
        v5 |= ((src[7] & 0x3FFFFFF) << 22);

        v6 = ((src[7] & 0x3FFFFFF) >> 10);
        v6 |= ((src[8] & 0x3FFFFFF) << 16)
;
        v7 = ((src[8] & 0x3FFFFFF) >> 16);
        v7 |= ((src[9] & 0x3FFFFFF) << 10);

        v8 = ((src[9] & 0x3FFFFFF) >> 22);
        v8 |= ((src[10] & 0x3FFFFFF) << 4);
        v8 |= ((src[11] & 0x3FFFFFF) << 30);

        v9 = ((src[11] & 0x3FFFFFF) >> 2);
        v9 |= ((src[12] & 0x3FFFFFF) << 24);

        v10 = ((src[12] & 0x3FFFFFF) >> 8);
        v10 |= ((src[13] & 0x3FFFFFF) << 18);

        v11 = ((src[13] & 0x3FFFFFF) >> 14);
        v11 |= ((src[14] & 0x3FFFFFF) << 12);

        v12 = ((src[14] & 0x3FFFFFF) >> 20);
        v12 |= ((src[15] & 0x3FFFFFF) << 6);

        v13 = (src[16] & 0x3FFFFFF);
        v13 |= ((src[17] & 0x3FFFFFF) << 26);

        v14 = ((src[17] & 0x3FFFFFF) >> 6);
        v14 |= ((src[18] & 0x3FFFFFF) << 20);

        v15 = ((src[18] & 0x3FFFFFF) >> 12);
        v15 |= ((src[19] & 0x3FFFFFF) << 14);

        v16 = ((src[19] & 0x3FFFFFF) >> 18);
        v16 |= ((src[20] & 0x3FFFFFF) << 8);

        v17 = ((src[20] & 0x3FFFFFF) >> 24);
        v17 |= ((src[21] & 0x3FFFFFF) << 2);
        v17 |= ((src[22] & 0x3FFFFFF) << 28);

        v18 = ((src[22] & 0x3FFFFFF) >> 4);
        v18 |= ((src[23] & 0x3FFFFFF) << 22);

        v19 = ((src[23] & 0x3FFFFFF) >> 10);
        v19 |= ((src[24] & 0x3FFFFFF) << 16);

        v20 = ((src[24] & 0x3FFFFFF) >> 16);
        v20 |= ((src[25] & 0x3FFFFFF) << 10);

        v21 = ((src[25] & 0x3FFFFFF) >> 22);
        v21 |= ((src[26] & 0x3FFFFFF) << 4);
        v21 |= ((src[27] & 0x3FFFFFF) << 30);

        v22 = ((src[27] & 0x3FFFFFF) >> 2);
        v22 |= ((src[28] & 0x3FFFFFF) << 24);

        v23 = ((src[28] & 0x3FFFFFF) >> 8);
        v23 |= ((src[29] & 0x3FFFFFF) << 18);

        v24 = ((src[29] & 0x3FFFFFF) >> 14);
        v24 |= ((src[30] & 0x3FFFFFF) << 12);

        v25 = ((src[30] & 0x3FFFFFF) >> 20);
        v25 |= ((src[31] & 0x3FFFFFF) << 6);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;
        dest[24] = v24;
        dest[25] = v25;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 26);
    }

}

void Pack::pack27(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 27) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;
        uint32_t v24;
        uint32_t v25;
        uint32_t v26;

        v0 = (src[0] & 0x7FFFFFF);
        v0 |= ((src[1] & 0x7FFFFFF) << 27);

        v1 = ((src[1] & 0x7FFFFFF) >> 5);
        v1 |= ((src[2] & 0x7FFFFFF) << 22);

        v2 = ((src[2] & 0x7FFFFFF) >> 10);
        v2 |= ((src[3] & 0x7FFFFFF) << 17);

        v3 = ((src[3] & 0x7FFFFFF) >> 15);
        v3 |= ((src[4] & 0x7FFFFFF) << 12);

        v4 = ((src[4] & 0x7FFFFFF) >> 20);
        v4 |= ((src[5] & 0x7FFFFFF) << 7);

        v5 = ((src[5] & 0x7FFFFFF) >> 25);
        v5 |= ((src[6] & 0x7FFFFFF) << 2);
        v5 |= ((src[7] & 0x7FFFFFF) << 29);

        v6 = ((src[7] & 0x7FFFFFF) >> 3);
        v6 |= ((src[8] & 0x7FFFFFF) << 24);

        v7 = ((src[8] & 0x7FFFFFF) >> 8);
        v7 |= ((src[9] & 0x7FFFFFF) << 19);

        v8 = ((src[9] & 0x7FFFFFF) >> 13);
        v8 |= ((src[10] & 0x7FFFFFF) << 14);

        v9 = ((src[10] & 0x7FFFFFF) >> 18);
        v9 |= ((src[11] & 0x7FFFFFF) << 9);

        v10 = ((src[11] & 0x7FFFFFF) >> 23);
        v10 |= ((src[12] & 0x7FFFFFF) << 4);
        v10 |= ((src[13] & 0x7FFFFFF) << 31);

        v11 = ((src[13] & 0x7FFFFFF) >> 1);
        v11 |= ((src[14] & 0x7FFFFFF) << 26);

        v12 = ((src[14] & 0x7FFFFFF) >> 6);
        v12 |= ((src[15] & 0x7FFFFFF) << 21);

        v13 = ((src[15] & 0x7FFFFFF) >> 11);
        v13 |= ((src[16] & 0x7FFFFFF) << 16);

        v14 = ((src[16] & 0x7FFFFFF) >> 16);
        v14 |= ((src[17] & 0x7FFFFFF) << 11);

        v15 = ((src[17] & 0x7FFFFFF) >> 21);
        v15 |= ((src[18] & 0x7FFFFFF) << 6);

        v16 = ((src[18] & 0x7FFFFFF) >> 26);
        v16 |= ((src[19] & 0x7FFFFFF) << 1);
        v16 |= ((src[20] & 0x7FFFFFF) << 28);

        v17 = ((src[20] & 0x7FFFFFF) >> 4);
        v17 |= ((src[21] & 0x7FFFFFF) << 23);

        v18 = ((src[21] & 0x7FFFFFF) >> 9);
        v18 |= ((src[22] & 0x7FFFFFF) << 18);

        v19 = ((src[22] & 0x7FFFFFF) >> 14);
        v19 |= ((src[23] & 0x7FFFFFF) << 13);

        v20 = ((src[23] & 0x7FFFFFF) >> 19);
        v20 |= ((src[24] & 0x7FFFFFF) << 8);

        v21 = ((src[24] & 0x7FFFFFF) >> 24);
        v21 |= ((src[25] & 0x7FFFFFF) << 3);
        v21 |= ((src[26] & 0x7FFFFFF) << 30);

        v22 = ((src[26] & 0x7FFFFFF) >> 2);
        v22 |= ((src[27] & 0x7FFFFFF) << 25);

        v23 = ((src[27] & 0x7FFFFFF) >> 7);
        v23 |= ((src[28] & 0x7FFFFFF) << 20);

        v24 = ((src[28] & 0x7FFFFFF) >> 12);
        v24 |= ((src[29] & 0x7FFFFFF) << 15);

        v25 = ((src[29] & 0x7FFFFFF) >> 17);
        v25 |= ((src[30] & 0x7FFFFFF) << 10);

        v26 = ((src[30] & 0x7FFFFFF) >> 22);
        v26 |= ((src[31] & 0x7FFFFFF) << 5);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;
        dest[24] = v24;
        dest[25] = v25;
        dest[26] = v26;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 27);
    }

}

void Pack::pack28(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 28) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;
        uint32_t v24;
        uint32_t v25;
        uint32_t v26;
        uint32_t v27;

        v0 = (src[0] & 0xFFFFFFF);
        v0 |= ((src[1] & 0xFFFFFFF) << 28);

        v1 = ((src[1] & 0xFFFFFFF) >> 4);
        v1 |= ((src[2] & 0xFFFFFFF) << 24);

        v2 = ((src[2] & 0xFFFFFFF) >> 8);
        v2 |= ((src[3] & 0xFFFFFFF) << 20);

        v3 = ((src[3] & 0xFFFFFFF) >> 12);
        v3 |= ((src[4] & 0xFFFFFFF) << 16);

        v4 = ((src[4] & 0xFFFFFFF) >> 16);
        v4 |= ((src[5] & 0xFFFFFFF) << 12);

        v5 = ((src[5] & 0xFFFFFFF) >> 20);
        v5 |= ((src[6] & 0xFFFFFFF) << 8);

        v6 = ((src[6] & 0xFFFFFFF) >> 24);
        v6 |= ((src[7] & 0xFFFFFFF) << 4);

        v7 = (src[8] & 0xFFFFFFF);
        v7 |= ((src[9] & 0xFFFFFFF) << 28);

        v8 = ((src[9] & 0xFFFFFFF) >> 4);
        v8 |= ((src[10] & 0xFFFFFFF) << 24);

        v9 = ((src[10] & 0xFFFFFFF) >> 8);
        v9 |= ((src[11] & 0xFFFFFFF) << 20);

        v10 = ((src[11] & 0xFFFFFFF) >> 12);
        v10 |= ((src[12] & 0xFFFFFFF) << 16);

        v11 = ((src[12] & 0xFFFFFFF) >> 16);
        v11 |= ((src[13] & 0xFFFFFFF) << 12);

        v12 = ((src[13] & 0xFFFFFFF) >> 20);
        v12 |= ((src[14] & 0xFFFFFFF) << 8);

        v13 = ((src[14] & 0xFFFFFFF) >> 24);
        v13 |= ((src[15] & 0xFFFFFFF) << 4);

        v14 = (src[16] & 0xFFFFFFF);
        v14 |= ((src[17] & 0xFFFFFFF) << 28);

        v15 = ((src[17] & 0xFFFFFFF) >> 4);
        v15 |= ((src[18] & 0xFFFFFFF) << 24);

        v16 = ((src[18] & 0xFFFFFFF) >> 8);
        v16 |= ((src[19] & 0xFFFFFFF) << 20);

        v17 = ((src[19] & 0xFFFFFFF) >> 12);
        v17 |= ((src[20] & 0xFFFFFFF) << 16);

        v18 = ((src[20] & 0xFFFFFFF) >> 16);
        v18 |= ((src[21] & 0xFFFFFFF) << 12);

        v19 = ((src[21] & 0xFFFFFFF) >> 20);
        v19 |= ((src[22] & 0xFFFFFFF) << 8);

        v20 = ((src[22] & 0xFFFFFFF) >> 24);
        v20 |= ((src[23] & 0xFFFFFFF) << 4);

        v21 = (src[24] & 0xFFFFFFF);
        v21 |= ((src[25] & 0xFFFFFFF) << 28);

        v22 = ((src[25] & 0xFFFFFFF) >> 4);
        v22 |= ((src[26] & 0xFFFFFFF) << 24);

        v23 = ((src[26] & 0xFFFFFFF) >> 8);
        v23 |= ((src[27] & 0xFFFFFFF) << 20);

        v24 = ((src[27] & 0xFFFFFFF) >> 12);
        v24 |= ((src[28] & 0xFFFFFFF) << 16);

        v25 = ((src[28] & 0xFFFFFFF) >> 16);
        v25 |= ((src[29] & 0xFFFFFFF) << 12);

        v26 = ((src[29] & 0xFFFFFFF) >> 20);
        v26 |= ((src[30] & 0xFFFFFFF) << 8);

        v27 = ((src[30] & 0xFFFFFFF) >> 24);
        v27 |= ((src[31] & 0xFFFFFFF) << 4);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;
        dest[24] = v24;
        dest[25] = v25;
        dest[26] = v26;
        dest[27] = v27;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 28);
    }

}

void Pack::pack29(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 29) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;
        uint32_t v24;
        uint32_t v25;
        uint32_t v26;
        uint32_t v27;
        uint32_t v28;

        v0 = (src[0] & 0x1FFFFFFF);
        v0 |= ((src[1] & 0x1FFFFFFF) << 29);

        v1 = ((src[1] & 0x1FFFFFFF) >> 3);
        v1 |= ((src[2] & 0x1FFFFFFF) << 26);

        v2 = ((src[2] & 0x1FFFFFFF) >> 6);
        v2 |= ((src[3] & 0x1FFFFFFF) << 23);

        v3 = ((src[3] & 0x1FFFFFFF) >> 9);
        v3 |= ((src[4] & 0x1FFFFFFF) << 20);

        v4 = ((src[4] & 0x1FFFFFFF) >> 12);
        v4 |= ((src[5] & 0x1FFFFFFF) << 17);

        v5 = ((src[5] & 0x1FFFFFFF) >> 15);
        v5 |= ((src[6] & 0x1FFFFFFF) << 14);

        v6 = ((src[6] & 0x1FFFFFFF) >> 18);
        v6 |= ((src[7] & 0x1FFFFFFF) << 11);

        v7 = ((src[7] & 0x1FFFFFFF) >> 21);
        v7 |= ((src[8] & 0x1FFFFFFF) << 8);

        v8 = ((src[8] & 0x1FFFFFFF) >> 24);
        v8 |= ((src[9] & 0x1FFFFFFF) << 5);

        v9 = ((src[9] & 0x1FFFFFFF) >> 27);
        v9 |= ((src[10] & 0x1FFFFFFF) << 2);
        v9 |= ((src[11] & 0x1FFFFFFF) << 31);

        v10 = ((src[11] & 0x1FFFFFFF) >> 1);
        v10 |= ((src[12] & 0x1FFFFFFF) << 28);

        v11 = ((src[12] & 0x1FFFFFFF) >> 4);
        v11 |= ((src[13] & 0x1FFFFFFF) << 25);

        v12 = ((src[13] & 0x1FFFFFFF) >> 7);
        v12 |= ((src[14] & 0x1FFFFFFF) << 22);

        v13 = ((src[14] & 0x1FFFFFFF) >> 10);
        v13 |= ((src[15] & 0x1FFFFFFF) << 19);

        v14 = ((src[15] & 0x1FFFFFFF) >> 13);
        v14 |= ((src[16] & 0x1FFFFFFF) << 16);

        v15 = ((src[16] & 0x1FFFFFFF) >> 16);
        v15 |= ((src[17] & 0x1FFFFFFF) << 13);

        v16 = ((src[17] & 0x1FFFFFFF) >> 19);
        v16 |= ((src[18] & 0x1FFFFFFF) << 10);

        v17 = ((src[18] & 0x1FFFFFFF) >> 22);
        v17 |= ((src[19] & 0x1FFFFFFF) << 7);

        v18 = ((src[19] & 0x1FFFFFFF) >> 25);
        v18 |= ((src[20] & 0x1FFFFFFF) << 4);

        v19 = ((src[20] & 0x1FFFFFFF) >> 28);
        v19 |= ((src[21] & 0x1FFFFFFF) << 1);
        v19 |= ((src[22] & 0x1FFFFFFF) << 30);

        v20 = ((src[22] & 0x1FFFFFFF) >> 2);
        v20 |= ((src[23] & 0x1FFFFFFF) << 27);

        v21 = ((src[23] & 0x1FFFFFFF) >> 5);
        v21 |= ((src[24] & 0x1FFFFFFF) << 24);

        v22 = ((src[24] & 0x1FFFFFFF) >> 8);
        v22 |= ((src[25] & 0x1FFFFFFF) << 21);

        v23 = ((src[25] & 0x1FFFFFFF) >> 11);
        v23 |= ((src[26] & 0x1FFFFFFF) << 18);

        v24 = ((src[26] & 0x1FFFFFFF) >> 14);
        v24 |= ((src[27] & 0x1FFFFFFF) << 15);

        v25 = ((src[27] & 0x1FFFFFFF) >> 17);
        v25 |= ((src[28] & 0x1FFFFFFF) << 12);

        v26 = ((src[28] & 0x1FFFFFFF) >> 20);
        v26 |= ((src[29] & 0x1FFFFFFF) << 9);

        v27 = ((src[29] & 0x1FFFFFFF) >> 23);
        v27 |= ((src[30] & 0x1FFFFFFF) << 6);

        v28 = ((src[30] & 0x1FFFFFFF) >> 26);
        v28 |= ((src[31] & 0x1FFFFFFF) << 3);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;
        dest[24] = v24;
        dest[25] = v25;
        dest[26] = v26;
        dest[27] = v27;
        dest[28] = v28;

    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 29);
    }

}

void Pack::pack30(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 30) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;
        uint32_t v24;
        uint32_t v25;
        uint32_t v26;
        uint32_t v27;
        uint32_t v28;
        uint32_t v29;
        v0 = (src[0] & 0x3FFFFFFF);
        v0 |= ((src[1] & 0x3FFFFFFF) << 30);

        v1 = ((src[1] & 0x3FFFFFFF) >> 2);
        v1 |= ((src[2] & 0x3FFFFFFF) << 28);

        v2 = ((src[2] & 0x3FFFFFFF) >> 4);
        v2 |= ((src[3] & 0x3FFFFFFF) << 26);

        v3 = ((src[3] & 0x3FFFFFFF) >> 6);
        v3 |= ((src[4] & 0x3FFFFFFF) << 24);

        v4 = ((src[4] & 0x3FFFFFFF) >> 8);
        v4 |= ((src[5] & 0x3FFFFFFF) << 22);

        v5 = ((src[5] & 0x3FFFFFFF) >> 10);
        v5 |= ((src[6] & 0x3FFFFFFF) << 20);

        v6 = ((src[6] & 0x3FFFFFFF) >> 12);
        v6 |= ((src[7] & 0x3FFFFFFF) << 18);

        v7 = ((src[7] & 0x3FFFFFFF) >> 14);
        v7 |= ((src[8] & 0x3FFFFFFF) << 16);

        v8 = ((src[8] & 0x3FFFFFFF) >> 16);
        v8 |= ((src[9] & 0x3FFFFFFF) << 14);

        v9 = ((src[9] & 0x3FFFFFFF) >> 18);
        v9 |= ((src[10] & 0x3FFFFFFF) << 12);

        v10 = ((src[10] & 0x3FFFFFFF) >> 20);
        v10 |= ((src[11] & 0x3FFFFFFF) << 10);

        v11 = ((src[11] & 0x3FFFFFFF) >> 22);
        v11 |= ((src[12] & 0x3FFFFFFF) << 8);

        v12 = ((src[12] & 0x3FFFFFFF) >> 24);
        v12 |= ((src[13] & 0x3FFFFFFF) << 6);

        v13 = ((src[13] & 0x3FFFFFFF) >> 26);
        v13 |= ((src[14] & 0x3FFFFFFF) << 4);

        v14 = ((src[14] & 0x3FFFFFFF) >> 28);
        v14 |= ((src[15] & 0x3FFFFFFF) << 2);

        v15 = (src[16] & 0x3FFFFFFF);
        v15 |= ((src[17] & 0x3FFFFFFF) << 30);

        v16 = ((src[17] & 0x3FFFFFFF) >> 2);
        v16 |= ((src[18] & 0x3FFFFFFF) << 28);

        v17 = ((src[18] & 0x3FFFFFFF) >> 4);
        v17 |= ((src[19] & 0x3FFFFFFF) << 26);

        v18 = ((src[19] & 0x3FFFFFFF) >> 6);
        v18 |= ((src[20] & 0x3FFFFFFF) << 24);

        v19 = ((src[20] & 0x3FFFFFFF) >> 8);
        v19 |= ((src[21] & 0x3FFFFFFF) << 22);

        v20 = ((src[21] & 0x3FFFFFFF) >> 10);
        v20 |= ((src[22] & 0x3FFFFFFF) << 20);

        v21 = ((src[22] & 0x3FFFFFFF) >> 12);
        v21 |= ((src[23] & 0x3FFFFFFF) << 18);

        v22 = ((src[23] & 0x3FFFFFFF) >> 14);
        v22 |= ((src[24] & 0x3FFFFFFF) << 16);

        v23 = ((src[24] & 0x3FFFFFFF) >> 16);
        v23 |= ((src[25] & 0x3FFFFFFF) << 14);

        v24 = ((src[25] & 0x3FFFFFFF) >> 18);
        v24 |= ((src[26] & 0x3FFFFFFF) << 12);

        v25 = ((src[26] & 0x3FFFFFFF) >> 20);
        v25 |= ((src[27] & 0x3FFFFFFF) << 10);

        v26 = ((src[27] & 0x3FFFFFFF) >> 22);
        v26 |= ((src[28] & 0x3FFFFFFF) << 8);

        v27 = ((src[28] & 0x3FFFFFFF) >> 24);
        v27 |= ((src[29] & 0x3FFFFFFF) << 6);

        v28 = ((src[29] & 0x3FFFFFFF) >> 26);
        v28 |= ((src[30] & 0x3FFFFFFF) << 4);

        v29 = ((src[30] & 0x3FFFFFFF) >> 28);
        v29 |= ((src[31] & 0x3FFFFFFF) << 2);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;
        dest[24] = v24;
        dest[25] = v25;
        dest[26] = v26;
        dest[27] = v27;
        dest[28] = v28;
        dest[29] = v29;
     
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 30);
    }
}

void Pack::pack31(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 31) 
    {
        uint32_t v0;
        uint32_t v1;
        uint32_t v2;
        uint32_t v3;
        uint32_t v4;
        uint32_t v5;
        uint32_t v6;
        uint32_t v7;
        uint32_t v8;
        uint32_t v9;
        uint32_t v10;
        uint32_t v11;
        uint32_t v12;
        uint32_t v13;
        uint32_t v14;
        uint32_t v15;
        uint32_t v16;
        uint32_t v17;
        uint32_t v18;
        uint32_t v19;
        uint32_t v20;
        uint32_t v21;
        uint32_t v22;
        uint32_t v23;
        uint32_t v24;
        uint32_t v25;
        uint32_t v26;
        uint32_t v27;
        uint32_t v28;
        uint32_t v29;
        uint32_t v30;

        v0 = (src[0] & 0x7FFFFFFF);
        v0 |= ((src[1] & 0x7FFFFFFF) << 31);

        v1 = ((src[1] & 0x7FFFFFFF) >> 1);
        v1 |= ((src[2] & 0x7FFFFFFF) << 30);

        v2 = ((src[2] & 0x7FFFFFFF) >> 2);
        v2 |= ((src[3] & 0x7FFFFFFF) << 29);

        v3 = ((src[3] & 0x7FFFFFFF) >> 3);
        v3 |= ((src[4] & 0x7FFFFFFF) << 28);

        v4 = ((src[4] & 0x7FFFFFFF) >> 4);
        v4 |= ((src[5] & 0x7FFFFFFF) << 27);

        v5 = ((src[5] & 0x7FFFFFFF) >> 5);
        v5 |= ((src[6] & 0x7FFFFFFF) << 26);

        v6 = ((src[6] & 0x7FFFFFFF) >> 6);
        v6 |= ((src[7] & 0x7FFFFFFF) << 25);

        v7 = ((src[7] & 0x7FFFFFFF) >> 7);
        v7 |= ((src[8] & 0x7FFFFFFF) << 24);

        v8 = ((src[8] & 0x7FFFFFFF) >> 8);
        v8 |= ((src[9] & 0x7FFFFFFF) << 23);

        v9 = ((src[9] & 0x7FFFFFFF) >> 9);
        v9 |= ((src[10] & 0x7FFFFFFF) << 22);

        v10 = ((src[10] & 0x7FFFFFFF) >> 10);
        v10 |= ((src[11] & 0x7FFFFFFF) << 21);

        v11 = ((src[11] & 0x7FFFFFFF) >> 11);
        v11 |= ((src[12] & 0x7FFFFFFF) << 20);

        v12 = ((src[12] & 0x7FFFFFFF) >> 12);
        v12 |= ((src[13] & 0x7FFFFFFF) << 19);

        v13 = ((src[13] & 0x7FFFFFFF) >> 13);
        v13 |= ((src[14] & 0x7FFFFFFF) << 18);

        v14 = ((src[14] & 0x7FFFFFFF) >> 14);
        v14 |= ((src[15] & 0x7FFFFFFF) << 17);

        v15 = ((src[15] & 0x7FFFFFFF) >> 15);
        v15 |= ((src[16] & 0x7FFFFFFF) << 16);

        v16 = ((src[16] & 0x7FFFFFFF) >> 16);
        v16 |= ((src[17] & 0x7FFFFFFF) << 15);

        v17 = ((src[17] & 0x7FFFFFFF) >> 17);
        v17 |= ((src[18] & 0x7FFFFFFF) << 14);

        v18 = ((src[18] & 0x7FFFFFFF) >> 18);
        v18 |= ((src[19] & 0x7FFFFFFF) << 13);

        v19 = ((src[19] & 0x7FFFFFFF) >> 19);
        v19 |= ((src[20] & 0x7FFFFFFF) << 12);

        v20 = ((src[20] & 0x7FFFFFFF) >> 20);
        v20 |= ((src[21] & 0x7FFFFFFF) << 11);

        v21 = ((src[21] & 0x7FFFFFFF) >> 21);
        v21 |= ((src[22] & 0x7FFFFFFF) << 10);

        v22 = ((src[22] & 0x7FFFFFFF) >> 22);
        v22 |= ((src[23] & 0x7FFFFFFF) << 9);

        v23 = ((src[23] & 0x7FFFFFFF) >> 23);
        v23 |= ((src[24] & 0x7FFFFFFF) << 8);

        v24 = ((src[24] & 0x7FFFFFFF) >> 24);
        v24 |= ((src[25] & 0x7FFFFFFF) << 7);

        v25 = ((src[25] & 0x7FFFFFFF) >> 25);
        v25 |= ((src[26] & 0x7FFFFFFF) << 6);

        v26 = ((src[26] & 0x7FFFFFFF) >> 26);
        v26 |= ((src[27] & 0x7FFFFFFF) << 5);

        v27 = ((src[27] & 0x7FFFFFFF) >> 27);
        v27 |= ((src[28] & 0x7FFFFFFF) << 4);

        v28 = ((src[28] & 0x7FFFFFFF) >> 28);
        v28 |= ((src[29] & 0x7FFFFFFF) << 3);

        v29 = ((src[29] & 0x7FFFFFFF) >> 29);
        v29 |= ((src[30] & 0x7FFFFFFF) << 2);

        v30 = ((src[30] & 0x7FFFFFFF) >> 30);
        v30 |= ((src[31] & 0x7FFFFFFF) << 1);

        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
        dest[8] = v8;
        dest[9] = v9;
        dest[10] = v10;
        dest[11] = v11;
        dest[12] = v12;
        dest[13] = v13;
        dest[14] = v14;
        dest[15] = v15;
        dest[16] = v16;
        dest[17] = v17;
        dest[18] = v18;
        dest[19] = v19;
        dest[20] = v20;
        dest[21] = v21;
        dest[22] = v22;
        dest[23] = v23;
        dest[24] = v24;
        dest[25] = v25;
        dest[26] = v26;
        dest[27] = v27;
        dest[28] = v28;
        dest[29] = v29;
        dest[30] = v30;
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 31);
    }

}

void Pack::pack32(uint32_t* dest, const uint32_t* src, size_t n)
{
    for (uint32_t i = 32; i <= n; i += 32, src += 32, dest += 32) 
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        dest[3] = src[3];
        dest[4] = src[4];
        dest[5] = src[5];
        dest[6] = src[6];
        dest[7] = src[7];
        dest[8] = src[8];
        dest[9] = src[9];
        dest[10] = src[10];
        dest[11] = src[11];
        dest[12] = src[12];
        dest[13] = src[13];
        dest[14] = src[14];
        dest[15] = src[15];
        dest[16] = src[16];
        dest[17] = src[17];
        dest[18] = src[18];
        dest[19] = src[19];
        dest[20] = src[20];
        dest[21] = src[21];
        dest[22] = src[22];
        dest[23] = src[23];
        dest[24] = src[24];
        dest[25] = src[25];
        dest[26] = src[26];
        dest[27] = src[27];
        dest[28] = src[28];
        dest[29] = src[29];
        dest[30] = src[30];
        dest[31] = src[31];
    }
    size_t left = n % 32;
    if (left > 0)
    {
        packTail(dest, src, left, 32);
    }
}

FX_NS_END

