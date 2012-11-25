//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-07-24 10:55:40

#ifndef __FX_IO_WIN32_H
#define __FX_IO_WIN32_H

#include "firtex/common/StdHeader.h"
#include <time.h>
#include <io.h>
#include <time.h>
#include <windows.h>
#include <fcntl.h>
#include <string>

typedef __int64 FX_OS_OFFSET;
typedef SIZE_T FX_OS_SIZE_T;

typedef FX_OS_OFFSET offset_t;

enum FX_OS_MAPPING_FLAG 
{
    FX_OS_FLAG_USE_MAPPING_FALSE,
    FX_OS_FLAG_USE_MAPPING_TRUE
};

enum FX_OS_FLAG 
{
    FX_OS_FLAG_SEEK_SET = FILE_BEGIN,
    FX_OS_FLAG_SEEK_CUR = FILE_CURRENT,
    FX_OS_FLAG_SEEK_END = FILE_END,
    FX_OS_FLAG_PROT_READ = FILE_MAP_READ,
    FX_OS_FLAG_PROT_WRITE = FILE_MAP_WRITE,
    FX_OS_FLAG_MAP_SHARED,
    FX_OS_FLAG_MS_SYNC,
    FX_OS_FLAG_MS_ASYNC,
    FX_OS_FLAG_MAP_FIXED = 0
};

typedef struct 
{
    HANDLE fileHandle, mapFileHandle;
    BOOL RDWR;
    FX_OS_MAPPING_FLAG useFileMapping;
} FX_OS_FILE_DESCRIPTOR;

inline bool operator == (const FX_OS_FILE_DESCRIPTOR& s1, const FX_OS_FILE_DESCRIPTOR& s2) {return (s1.fileHandle == s2.fileHandle);}


inline LONG getHighOrderOff(FX_OS_OFFSET off) 
{
    return (LONG)((ULONGLONG)(off) >> 32);
}
 
inline LONG getLowOrderOff(FX_OS_OFFSET off) 
{
    return (LONG)((ULONGLONG)(off) % 0x000100000000ULL);
}

inline bool FX_OS_IS_VALID_FILE_DESCRIPTOR(const FX_OS_FILE_DESCRIPTOR& fd) 
{
    if (fd.fileHandle == INVALID_HANDLE_VALUE || fd.mapFileHandle == NULL) 
    {
        return false;
    }
    else 
    {
        return true;
    };
}

inline FX_OS_OFFSET FX_OS_LSEEK(FX_OS_FILE_DESCRIPTOR &fd, FX_OS_OFFSET offset, FX_OS_FLAG origin) 
{
    LONG highOrderOff = getHighOrderOff(offset);
    DWORD x = SetFilePointer(fd.fileHandle,getLowOrderOff(offset),&highOrderOff,origin);

    if (x==0xFFFFFFFF && (GetLastError() != NO_ERROR)) 
    {
        //Error
        return -1;
    }
    else
    {
        return FX_OS_OFFSET((((ULONGLONG) highOrderOff)<<32)+(ULONGLONG) x);
    }
}

inline ssize_t FX_OS_WRITE(FX_OS_FILE_DESCRIPTOR fd, const void* buffer, FX_OS_SIZE_T count) 
{
    DWORD bytesWritten = 0;
    ::WriteFile(fd.fileHandle, buffer, (DWORD)count, &bytesWritten, 0);
    return (ssize_t)(bytesWritten > 0 ? bytesWritten : -1);
}

inline ssize_t FX_OS_READ(FX_OS_FILE_DESCRIPTOR fd, void* buffer, FX_OS_SIZE_T count) 
{
    DWORD bytesRead = 0;
    if (!ReadFile(fd.fileHandle, buffer, (DWORD)count, &bytesRead, 0))
	{
		return -1;
	}
    return (bytesRead > 0 ? bytesRead : -1);
}

// The suggested starting address of the mmap call has to be
// a multiple of the systems granularity (else the mapping fails)
// Hence, the parameter addr is not used at present.
inline LPVOID FX_OS_MMAP(LPVOID /* addr */, 
                           FX_OS_SIZE_T  len, 
                           int     prot, 
                           int     /* flags */ , 
                           FX_OS_FILE_DESCRIPTOR fildes, 
                           FX_OS_OFFSET off) {
    return MapViewOfFileEx( fildes.mapFileHandle,
                            prot,
                            getHighOrderOff(off),       
                            getLowOrderOff(off),
                            len, NULL);
}

inline int FX_OS_MUNMAP(LPVOID addr, size_t /* len */) 
{
    return (UnmapViewOfFile(addr) == 0 ? -1 : 0);
}

inline int FX_OS_MSYNC(LPVOID addr, size_t len, int /* flags */) 
{
    return (FlushViewOfFile(addr,len) ? 0 : -1);
}

inline bool FX_OS_EXISTS(const std::string & fileName) 
{
    return (GetFileAttributes(fileName.c_str()) != 0xFFFFFFFF);
}

inline int FX_OS_FTRUNCATE(FX_OS_FILE_DESCRIPTOR& fd, FX_OS_OFFSET length) 
{
    // Save the offset
    FX_OS_OFFSET off = FX_OS_LSEEK(fd, 0, FX_OS_FLAG_SEEK_CUR);
    if (fd.useFileMapping == FX_OS_FLAG_USE_MAPPING_TRUE) 
    {
        CloseHandle(fd.mapFileHandle);
    }
    LONG highOrderOff = getHighOrderOff(length);
    int x = ((((fd).RDWR == false) || 
              (SetFilePointer((fd).fileHandle, getLowOrderOff(length), &highOrderOff, FILE_BEGIN) == 0xFFFFFFFF) || 
              (SetEndOfFile((fd).fileHandle) == 0)) ? -1 : 0);

    if (fd.useFileMapping == FX_OS_FLAG_USE_MAPPING_TRUE) 
    {
        fd.mapFileHandle= CreateFileMapping((fd).fileHandle,
                                            0,
                                            ((fd).RDWR ? PAGE_READWRITE : PAGE_READONLY),
                                            0, 0,
                                            NULL);
    }
    // Restore the offset, mimicking the ftruncate() behavior.
    FX_OS_LSEEK(fd, (off < length ? off : length), FX_OS_FLAG_SEEK_SET);
    return x;
}

inline int FX_OS_CLOSE(FX_OS_FILE_DESCRIPTOR fd) 
{
    if (fd.useFileMapping == FX_OS_FLAG_USE_MAPPING_TRUE) 
    {
        return (((CloseHandle(fd.mapFileHandle) != 0) &&
                   (CloseHandle(fd.fileHandle) != 0)) ? 0 : -1);
    }
    else 
    {
        return ((CloseHandle(fd.fileHandle) != 0) ? 0 : -1);
    } 
}

inline FX_OS_SIZE_T  FX_OS_PAGESIZE() 
{
    SYSTEM_INFO systemInfos;
    GetSystemInfo(&systemInfos);
    return (FX_OS_SIZE_T )systemInfos.dwPageSize;
}

inline FX_OS_SIZE_T FX_OS_BLOCKSIZE() 
{
    SYSTEM_INFO systemInfos;
    GetSystemInfo(&systemInfos);
    return systemInfos.dwAllocationGranularity;
}

inline FX_OS_FILE_DESCRIPTOR WIN32_OPEN(LPCTSTR name, int flag, FX_OS_MAPPING_FLAG mappingFlag) 
{
    DWORD createFlag = FILE_FLAG_SEQUENTIAL_SCAN;
    FX_OS_FILE_DESCRIPTOR hHandle;
    switch(flag) 
    {
    case _O_RDONLY: 
        hHandle.RDWR = false;
        hHandle.fileHandle = CreateFile(
            name,
            GENERIC_READ, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            0,
            OPEN_EXISTING, 
            createFlag,
            0);
        break;

    case _O_EXCL:
        hHandle.RDWR = true;
        hHandle.fileHandle = CreateFile(
            name,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            0,
            CREATE_NEW, 
            createFlag,
            0);
        break;
    case _O_TRUNC:
        hHandle.RDWR = true;
        hHandle.fileHandle = CreateFile(
            name,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            0,
            CREATE_ALWAYS, 
            createFlag,
            0);
        break;
    case _O_RDWR:
        hHandle.RDWR = true;
        hHandle.fileHandle = CreateFile(
            name,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            0,
            OPEN_EXISTING, 
            createFlag,
            0);
        break;
    default :
        hHandle.RDWR = false;
        hHandle.fileHandle = CreateFile(
            name,
            GENERIC_READ, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            0,
            OPEN_EXISTING, 
            createFlag,
            0);
    };
    hHandle.useFileMapping = mappingFlag;
    if (hHandle.fileHandle == INVALID_HANDLE_VALUE)
    {
        return hHandle;
    }

    if (hHandle.useFileMapping == FX_OS_FLAG_USE_MAPPING_TRUE) 
    {
        hHandle.mapFileHandle = 
            CreateFileMapping( 
                hHandle.fileHandle,
                0,  
                (hHandle.RDWR ? PAGE_READWRITE : PAGE_READONLY),
                0, 0,
                NULL);
    }
    else 
    {
        hHandle.mapFileHandle = (void*)1;
    }
    return hHandle;
}

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_READ(
    const std::string& name, FX_OS_MAPPING_FLAG mappingFlag = FX_OS_FLAG_USE_MAPPING_FALSE) 
{
    return WIN32_OPEN(name.c_str(), _O_RDONLY,mappingFlag);
}

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_EXCL(
    const std::string& name, FX_OS_MAPPING_FLAG mappingFlag = FX_OS_FLAG_USE_MAPPING_FALSE) 
{
    return WIN32_OPEN(name.c_str(), _O_EXCL, mappingFlag);
}

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_CRTR(
    const std::string& name, FX_OS_MAPPING_FLAG mappingFlag = FX_OS_FLAG_USE_MAPPING_FALSE) 
{
    return WIN32_OPEN(name.c_str(), _O_TRUNC, mappingFlag);
}

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_RDWR(
    const std::string& name, FX_OS_MAPPING_FLAG mappingFlag = FX_OS_FLAG_USE_MAPPING_FALSE) 
{
    return WIN32_OPEN(name.c_str(), _O_RDWR, mappingFlag);
}

#endif //__FX_IO_WIN32_H
