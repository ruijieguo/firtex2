//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: 郭瑞杰(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2006/5/20
//
#ifndef __FXSTRINGUTILS_H
#define __FXSTRINGUTILS_H

#include "firtex/common/StdHeader.h"
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <sstream>
#include <algorithm>

FX_NS_DEF(utility);

template <class T>
T trimLeftInPlace(const T& str)
{
    typename T::const_iterator it  = str.begin();
    typename T::const_iterator end = str.end();
    
    while (it != end && std::isspace(*it)) 
    {
        ++it;
    }
    return T(it, end);
}


template <class T>
void trimLeftInPlaceInPlace(T& str)
{
    typename T::iterator it  = str.begin();
    typename T::iterator end = str.end();
	
    while (it != end && std::isspace(*it)) 
    {
        ++it;
    }
    str.erase(str.begin(), it);
}

template <class T>
T trimRightInPlace(const T& str)
{
    int32_t pos = int32_t(str.size()) - 1;
		
    while (pos >= 0 && std::isspace(str[pos]))
    {
        --pos;
    }
    return T(str, 0, (typename T::size_type)(pos + 1));
}

template <class T>
void trimRightInPlaceInPlace(T& str)
{
    int32_t pos = int32_t(str.size()) - 1;
		
    while (pos >= 0 && std::isspace(str[pos])) 
    {
        --pos;
    }
    str.resize(typename T::size_type(pos + 1));
}

template <class T>
T trim(const T& str)
{
    int32_t first = 0;
    int32_t last  = int32_t(str.size()) - 1;
	
    while (first <= last && std::isspace(str[first]))
    {
        ++first;
    }
    while (last >= first && std::isspace(str[last])) 
    {
        --last;
    }

    return T(str, (typename T::size_type)first, typename T::size_type(last - first + 1));
}


template <class T>
void trimInPlace(T& str)
{
    int32_t first = 0;
    int32_t last  = int32_t(str.size()) - 1;
	
    while (first <= last && std::isspace(str[first])) 
    {
        ++first;
    }
    while (last >= first && std::isspace(str[last]))
    {
        --last;
    }

    str.resize(typename T::size_type(last + 1));
    str.erase(0, (typename T::size_type)first);
}

template <typename T>
T toUpper(const T& str)
{
    typename T::const_iterator it  = str.begin();
    typename T::const_iterator end = str.end();

    T result;
    result.reserve(str.size());
    while (it != end) result += std::toupper(*it++);
    return result;
} 

template <typename T>
void toUpperInPlace(T& str)
{
    typename T::iterator it  = str.begin();
    typename T::iterator end = str.end();

    while (it != end) 
    { 
        *it = std::toupper(*it);
        ++it;
    }
}
	
template <typename T>
T toLower(const T& str)
{
    typename T::const_iterator it  = str.begin();
    typename T::const_iterator end = str.end();

    T result;
    result.reserve(str.size());
    while (it != end) 
    {
        result += std::tolower(*it++);
    }
    return result;
}

template <typename T>
void toLowerInPlace(T& str)
{
    typename T::iterator it  = str.begin();
    typename T::iterator end = str.end();

    while (it != end) 
    { 
        *it = std::tolower(*it);
        ++it;
    }
}

template <typename T>
bool startsWithNoCase(const T& longStr, const T& shortStr)
{
    typename T::const_iterator it1(longStr.begin());
    typename T::const_iterator end1(longStr.end());
    typename T::const_iterator it2(shortStr.begin());
    typename T::const_iterator end2(shortStr.end());
    while (it1 != end1 && it2 != end2)
    {
        typename T::value_type c1(std::tolower(*it1));
        typename T::value_type c2(std::tolower(*it2));
        if (c1 != c2)
        {
            return false;
        }
        ++it1; ++it2;
    }
    
    if (it2 == end2)
    {
        return true;
    }
    return false;
}

/** 
 * 判断shortStr是否是longStr的后缀,不区分大小写
 * @param longStr 长字符串
 * @param shortStr 需要判断后缀的短字符串
 * @return 当shortStr是longStr的后缀时返回true，否则返回false
 */
template <typename T>
bool endsWithNoCase(const T& longStr, const T& shortStr)
{
    typename T::const_reverse_iterator it1(longStr.rbegin());
    typename T::const_reverse_iterator end1(longStr.rend());
    typename T::const_reverse_iterator it2(shortStr.rbegin());
    typename T::const_reverse_iterator end2(shortStr.rend());
    while (it1 != end1 && it2 != end2)
    {
        typename T::value_type c1(std::tolower(*it1));
        typename T::value_type c2(std::tolower(*it2));
        if (c1 != c2)
        {
            return false;
        }
        ++it1; ++it2;
    }
    
    if (it2 == end2)
    {
        return true;
    }
    return false;
}

/** 
 * version of strlen
 */
size_t strLength(const char *s);

/**
 * version of _strcmp 
 */
int strCompare(const char* s1, const char* s2);

/**
 * version of strncmp. 
 */
int strCompare(const char* s1, const char* s2, size_t size);

/* Case-sensitive string comparison */
template <class T, class It>
int strCompare(const T& str,
               typename T::size_type pos, 
               typename T::size_type n,
               It it2, 	It end2)
{
    typename T::size_type sz = str.size();
    if (pos > sz) pos = sz;
    if (pos + n > sz) n = sz - pos;
    It it1  = str.begin() + pos; 
    It end1 = str.begin() + pos + n;
    while (it1 != end1 && it2 != end2)
    {
        if (*it1 < *it2)
            return -1;
        else if (it1 > it2)
            return 1;
        ++it1; ++it2;
    }
    
    if (it1 == end1)
    {
        return it2 == end2 ? 0 : -1;
    }
    else
    {
        return 1;
    }
}

template <typename T>
int strCompare(const T& s1, typename T::size_type size1, const T& s2, typename T::size_type size2)
{
    if (size2 > s2.size())
    {
        size2 = s2.size();
    }
    return strCompare(s1, 0, s2.begin(), s2.begin() + size2);
}

int strCompare(const tchar* s1, size_t size1, const tchar* s2, size_t size2);

/** Case-insensitive string comparison */
template <class T, class It>
int strCompareNoCase(const T& str,
                     typename T::size_type pos, 
                     typename T::size_type n,
                     It it2, It end2)
{
    typename T::size_type sz = str.size();
    if (pos > sz) pos = sz;
    if (pos + n > sz) n = sz - pos;
    It it1  = str.begin() + pos; 
    It end1 = str.begin() + pos + n;
    while (it1 != end1 && it2 != end2)
    {
        typename T::value_type c1(std::tolower(*it1));
        typename T::value_type c2(std::tolower(*it2));
        if (c1 < c2)
            return -1;
        else if (c1 > c2)
            return 1;
        ++it1; ++it2;
    }
    
    if (it1 == end1)
        return it2 == end2 ? 0 : -1;
    else
        return 1;
}


/** compare two sting case nonsensitive */
template <typename T>
inline int strCompareNoCase(const T& s1, const T& s2)
{
    return strCompareNoCase(s1, 0, s1.size(), s2.begin(), s2.end());
}

/**
 * version of strcasecmp.  return an integer less than, equal to or greater
 * than 0 if s1 is less than, equal to or greater than s2, ignoring case.
 * If a letter gets compared with []\^_` the letter will be evaluate to more
 * than these characters (equivalent to conversion to lowercase) 
 */
int strCompareNoCase(const char* s1, const char* s2);

/**
 * version of strncasecmp.  return an integer less than, equal to or greater
 * than 0 if the first size characters of s1 are less than, equal to or greater 
 * than the first size charactesr of s2, ignoring case.
 * If a letter gets compared with []\^_` the letter will be evaluate to more
 * than these characters (equivalent to conversion to lowercase) 
 */
int strCompareNoCase(const char* s1, const char* s2, size_t size);

/**
 * version of strchr.strstr
 **/
char* strFind(const char* s, int c);

/** 
 * version of strstr
 */
char* strFind(const char* str1, const char* str2);

/** 
 * search a sub-string in a range
 */
char* strFindInRange(const char* strB, const char* strE, const char* str);

FX_NS_END

#endif
