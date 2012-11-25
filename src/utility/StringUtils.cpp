//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/20
//
#include "firtex/utility/StringUtils.h"
#include <string.h>
#include <stdio.h>

using namespace std;

FX_NS_DEF(utility)

size_t strLength(const char *s) 
{
    register const char *pos = s;
    while (*pos) 
    {
        pos++;
    }
    return pos - s;
}

int strCompare(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
    }
    return (unsigned char) *s1 - (unsigned char) *s2;
}

int strCompare(const char* s1, const char* s2, size_t size) 
{
    while (size && *s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
        size--;
    }

    if (size) 
    {
        return (unsigned char) *s1 - (unsigned char) *s2;
    } 
    else 
    {
        return 0;
    }
}


int strCompare(const char* s1, size_t size1, const char* s2, size_t size2) 
{
    int def;
    size_t len;

    if (size1 < size2) 
    {
        def = -1;
        len = size1;
    } 
    else if (size1 > size2)
    {
        def = 1;
        len = size2;
    } 
    else 
    {
        len = size1;
        def = 0;
    }

    while (len && *s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
        len--;
    }

    if (len) 
    {
        return (unsigned char) *s1 - (unsigned char) *s2;
    } 
    else 
    {
        return def;
    }
}

/**
 * Lookup array of lowercase converted characters (note that we'll offset
 * lookups into this array by 128 to get rid of signed/unsigned problems) 
 */
static char lookup[] = 
{
    /* first comes the extended character set in case chars are signed */
    (char) 128, (char) 129, (char) 130, (char) 131, (char) 132, (char) 133, 
    (char) 134, (char) 135, (char) 136, (char) 137, (char) 138, (char) 139, 
    (char) 140, (char) 141, (char) 142, (char) 143, (char) 144, (char) 145, 
    (char) 146, (char) 147, (char) 148, (char) 149, (char) 150, (char) 151, 
    (char) 152, (char) 153, (char) 154, (char) 155, (char) 156, (char) 157, 
    (char) 158, (char) 159, (char) 160, (char) 161, (char) 162, (char) 163, 
    (char) 164, (char) 165, (char) 166, (char) 167, (char) 168, (char) 169, 
    (char) 170, (char) 171, (char) 172, (char) 173, (char) 174, (char) 175, 
    (char) 176, (char) 177, (char) 178, (char) 179, (char) 180, (char) 181, 
    (char) 182, (char) 183, (char) 184, (char) 185, (char) 186, (char) 187, 
    (char) 188, (char) 189, (char) 190, (char) 191, (char) 192, (char) 193, 
    (char) 194, (char) 195, (char) 196, (char) 197, (char) 198, (char) 199, 
    (char) 200, (char) 201, (char) 202, (char) 203, (char) 204, (char) 205, 
    (char) 206, (char) 207, (char) 208, (char) 209, (char) 210, (char) 211, 
    (char) 212, (char) 213, (char) 214, (char) 215, (char) 216, (char) 217, 
    (char) 218, (char) 219, (char) 220, (char) 221, (char) 222, (char) 223, 
    (char) 224, (char) 225, (char) 226, (char) 227, (char) 228, (char) 229, 
    (char) 230, (char) 231, (char) 232, (char) 233, (char) 234, (char) 235, 
    (char) 236, (char) 237, (char) 238, (char) 239, (char) 240, (char) 241, 
    (char) 242, (char) 243, (char) 244, (char) 245, (char) 246, (char) 247, 
    (char) 248, (char) 249, (char) 250, (char) 251, (char) 252, (char) 253, 
    (char) 254, (char) 255,

    /* then the regular 7-bit character set */
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, '!', '"', '#', '$', '%', 
    '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', 
    '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'a', 'b', 'c', 
    'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', '\\', ']', '^', '_', '`', 'a', 
    'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', 127, 

    /* then the extended character set again in case chars are unsigned */
    (char) 128, (char) 129, (char) 130, (char) 131, (char) 132, (char) 133, 
    (char) 134, (char) 135, (char) 136, (char) 137, (char) 138, (char) 139, 
    (char) 140, (char) 141, (char) 142, (char) 143, (char) 144, (char) 145, 
    (char) 146, (char) 147, (char) 148, (char) 149, (char) 150, (char) 151, 
    (char) 152, (char) 153, (char) 154, (char) 155, (char) 156, (char) 157, 
    (char) 158, (char) 159, (char) 160, (char) 161, (char) 162, (char) 163, 
    (char) 164, (char) 165, (char) 166, (char) 167, (char) 168, (char) 169, 
    (char) 170, (char) 171, (char) 172, (char) 173, (char) 174, (char) 175, 
    (char) 176, (char) 177, (char) 178, (char) 179, (char) 180, (char) 181, 
    (char) 182, (char) 183, (char) 184, (char) 185, (char) 186, (char) 187, 
    (char) 188, (char) 189, (char) 190, (char) 191, (char) 192, (char) 193, 
    (char) 194, (char) 195, (char) 196, (char) 197, (char) 198, (char) 199, 
    (char) 200, (char) 201, (char) 202, (char) 203, (char) 204, (char) 205, 
    (char) 206, (char) 207, (char) 208, (char) 209, (char) 210, (char) 211, 
    (char) 212, (char) 213, (char) 214, (char) 215, (char) 216, (char) 217, 
    (char) 218, (char) 219, (char) 220, (char) 221, (char) 222, (char) 223, 
    (char) 224, (char) 225, (char) 226, (char) 227, (char) 228, (char) 229, 
    (char) 230, (char) 231, (char) 232, (char) 233, (char) 234, (char) 235, 
    (char) 236, (char) 237, (char) 238, (char) 239, (char) 240, (char) 241, 
    (char) 242, (char) 243, (char) 244, (char) 245, (char) 246, (char) 247, 
    (char) 248, (char) 249, (char) 250, (char) 251, (char) 252, (char) 253, 
    (char) 254, (char) 255
};

int strCompareNoCase(const char* s1, const char* s2)
{
    const char*us1 = (const char*) s1,*us2 = (const char*) s2;
    while (*us1 && (lookup[*us1 + 128] == lookup[*us2 + 128])) 
    {
        us1++;
        us2++;
    }

    return (unsigned char) lookup[*us1 + 128] - (unsigned char) lookup[*us2 + 128];
}

int strCompareNoCase(const char* s1, const char* s2, size_t size) 
{
    const char *us1 = (const char*) s1,*us2 = (const char*) s2;

    while (size && *us1 && (lookup[*us1 + 128] == lookup[*us2 + 128])) 
    {
        us1++;
        us2++;
        size--;
    }

    if (size) 
    {
        return (unsigned char) lookup[*us1 + 128] - 
            (unsigned char) lookup[*us2 + 128];
    }
    else 
    {
        return 0;
    }
}

char* strFind(const char* s, int c) 
{
    while (*s && *s != (char)c)
    {
        s++;
    }
    if (*s == (char)c)
    {
        return((char *)s);
    }
    return NULL;
}

char* strFind(const char* str1, const char* str2)
{
    char* cp = (char*) str1;
    char* s1, *s2;

    if ( !*str2 )
        return((char*)str1);

    while (*cp)
    {
        s1 = cp;
        s2 = (char*) str2;

        while (*s1 && *s2 && !(*s1 - *s2))
            s1++, s2++;

        if (!*s2)
            return(cp);

        cp++;
    }
    return NULL;
}

char* strFindInRange(const char* strB, const char* strE, const char* str)
{
    char *cp = (char *) strB;
    char *s1, *s2;

    if ( !*str )
    {
        return((char *)strB);
    }

//    while ((cp < strE) && *cp)
    while ((cp < strE))
    {
        s1 = cp;
        s2 = (char *) str;

        while ( *s1 && *s2 && !(*s1-*s2) )
            s1++, s2++;

        if (!*s2)
            return(cp);

        cp++;
    }
    return NULL;
}

FX_NS_END

