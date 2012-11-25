//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/5/20
//

#include <string.h>
#include <stdio.h>
#include "firtex/utility/MiscUtil.h"
#include "firtex/utility/IconvWrapper.h"
#include "firtex/utility/StringUtils.h"

#include <stdarg.h>
#ifdef WIN32 
#include <windows.h>
# include <sys/timeb.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

FX_NS_DEF(utility)

//static
int64_t MiscUtil::currentTimeMillis() 
{
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__BORLANDC__)
    struct _timeb tstruct;
    _ftime(&tstruct);

    return (((uint64_t) tstruct.time) * 1000) + tstruct.millitm;
#else
    struct timeval tstruct;
    if (gettimeofday(&tstruct, NULL) < 0) 
    {
        return 0;				
    }

    return (((uint64_t) tstruct.tv_sec) * 1000) + tstruct.tv_usec / 1000;
#endif			
}

std::string MiscUtil::getSelfPath()
{
#ifdef FX_WINDOWS
    tchar szExePath[512];
    GetModuleFileName(NULL, szExePath, 512);
    return std::string(szExePath);
#elif defined(FX_POSIX)
    std::string sFileName;
    char sLine[1024] = { 0 };
    void* pSymbol = (void*)"";
    FILE *fp;
    char *pPath;
    fp = fopen ("/proc/self/maps", "r");
    if ( fp != NULL )
    {
        while (!feof (fp))
        {
            unsigned long start, end;
            if ( !fgets (sLine, sizeof (sLine), fp))
                continue;
            if ( !strstr (sLine, " r-xp ") || !strchr (sLine, '/'))
                continue;
            sscanf (sLine, "%lx-%lx ", &start, &end);
            if (pSymbol >= (void *) start && pSymbol < (void *) end)
            {
                char *tmp;
                /* Extract the filename; it is always an absolute path */
                pPath = strchr (sLine, '/');
                /* Get rid of the newline */
                tmp = strrchr (pPath, '\n');
                if (tmp) *tmp = 0;
                sFileName = pPath;
            }
        }
        fclose (fp);
    }
    return sFileName;
#else
    char buff[512];
    if (getcwd(buff, 512))
    {
        std::string str(buff);
        return str;
    }
    return "";
#endif
}

uint32_t MiscUtil::log2UpperBound(uint32_t val)
{
    for (uint32_t i = 0; i < sizeof(POW_TABLE); i++)
    {
        if ((uint64_t)val <= POW_TABLE[i])
            return i;
    }
    return 0;
}

uint32_t MiscUtil::log2UpperBound(uint64_t val)
{
    for (uint32_t i = 0; i < sizeof(POW_TABLE); i++)
    {
        if (val <= POW_TABLE[i])
            return i;
    }
    return 0;
}

uint32_t MiscUtil::log2LowerBound(uint32_t val)
{
    for (uint32_t i = 1; i < sizeof(POW_TABLE); i++)
    {
        if ((uint64_t)val == POW_TABLE[i])
            return i;
        else if((uint64_t)val < POW_TABLE[i])
            return i - 1;
    }
    return 0;
}

uint32_t MiscUtil::log2LowerBound(uint64_t val)
{
    for (uint32_t i = 1; i < sizeof(POW_TABLE); i++)
    {
        if (val == POW_TABLE[i])
            return i;
        else if(val < POW_TABLE[i])
            return i - 1;
    }
    return 0;
}

int32_t MiscUtil::convertRequest(std::string& sResult, 
                                 const std::string& sRequet, 
                                 const std::string& sDstEncode,
                                 const std::string& sSrcEncodePrefix)
{
    char* pEncodeStr = FX_NS(utility)::strFind(sRequet.c_str(),
            sSrcEncodePrefix.c_str());
    if (pEncodeStr)
    {
        std::string sEncodeName;
        pEncodeStr += sSrcEncodePrefix.length();
        char* pEncodeStrEnd = FX_NS(utility)::strFind(pEncodeStr, ";");
        if (!pEncodeStrEnd)
        {
            sEncodeName.assign(pEncodeStr);
        }
        else
        {
            sEncodeName.assign(pEncodeStr, pEncodeStrEnd - pEncodeStr);
        }

        if (FX_NS(utility)::strCompareNoCase(sEncodeName, sDstEncode))
        {
            FX_NS(utility)::IconvWrapper converter;
            try
            {
                converter.open(sDstEncode, sEncodeName);
            }
            catch(const FX_NS(utility)::FirteXException&)
            {
                return -1;
            }
         
            if (!converter.convert(sResult, sRequet))
            {
                return -1;
            }
            return 1;
        }
    }

    return 0;
}

FX_NS_END
