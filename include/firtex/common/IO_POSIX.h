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
// Created	: 2010-07-24 10:57:06

#ifndef __FX_IO_POSIX_H
#define __FX_IO_POSIX_H

#include "firtex/common/StdHeader.h"
#include <unistd.h>
#include <sys/time.h>  
#include <sys/types.h>
#include <sys/stat.h>  
#include <fcntl.h>
#include <sys/mman.h>
#include <string>

typedef offset_t FX_OS_OFFSET;

enum FX_OS_MAPPING_FLAG 
{
    FX_OS_FLAG_USE_MAPPING_FALSE,
    FX_OS_FLAG_USE_MAPPING_TRUE
};

enum FX_OS_FLAG 
{
    FX_OS_FLAG_SEEK_SET = SEEK_SET,
    FX_OS_FLAG_SEEK_CUR = SEEK_CUR,
    FX_OS_FLAG_SEEK_END = SEEK_END,
    FX_OS_FLAG_PROT_READ = PROT_READ,
    FX_OS_FLAG_PROT_WRITE = PROT_WRITE,
    FX_OS_FLAG_MAP_SHARED = MAP_SHARED,
    FX_OS_FLAG_MS_SYNC = MS_SYNC,
    FX_OS_FLAG_MS_ASYNC = MS_ASYNC,
    FX_OS_FLAG_MAP_FIXED = MAP_FIXED
};

typedef int FX_OS_FILE_DESCRIPTOR;

inline bool FX_OS_IS_VALID_FILE_DESCRIPTOR(const FX_OS_FILE_DESCRIPTOR& fd) 
{
    return (fd == -1 ? false : true);
}

inline FX_OS_OFFSET FX_OS_LSEEK(FX_OS_FILE_DESCRIPTOR &fd, FX_OS_OFFSET offset, FX_OS_FLAG origin) 
{
    return ::lseek(fd, offset, origin);
}

inline ssize_t FX_OS_WRITE(FX_OS_FILE_DESCRIPTOR fd, const void* buffer, size_t count) 
{
    return ::write(fd,buffer,count);
}

inline ssize_t FX_OS_READ(FX_OS_FILE_DESCRIPTOR fd, void* buffer, size_t count) 
{
    return ::read(fd, buffer, count);
}

inline void* FX_OS_MMAP(void* addr, size_t len, int prot, int flags,
                        FX_OS_FILE_DESCRIPTOR fildes, FX_OS_OFFSET off) 
{
    return mmap(static_cast<caddr_t>(addr), len, prot, flags, fildes, off);
}

inline int FX_OS_MUNMAP(void* addr, size_t len) 
{
    return munmap(static_cast<caddr_t>(addr), len);
}

inline int FX_OS_MSYNC(char* addr, size_t len, int flags) 
{
    return msync(addr, len, flags);
}

inline bool FX_OS_EXISTS(const std::string & path) 
{
    return access(path.c_str(),R_OK) == 0 || errno == EACCES;
}

inline int FX_OS_FTRUNCATE(FX_OS_FILE_DESCRIPTOR& fd, FX_OS_OFFSET length) 
{
    return ftruncate(fd, length);
}

inline int FX_OS_CLOSE(FX_OS_FILE_DESCRIPTOR fd) 
{
    return ::close(fd);
}

#ifdef _SC_PAGE_SIZE
inline size_t FX_OS_PAGESIZE() 
{
    return (size_t)sysconf(_SC_PAGE_SIZE);
}
#else
inline size_t  FX_OS_PAGESIZE() 
{
    return (size_t)getpagesize();
}
#endif

#ifdef _SC_PAGE_SIZE
inline size_t FX_OS_BLOCKSIZE() 
{
    return (size_t)sysconf (_SC_PAGE_SIZE);
}
#else
inline size_t  FX_OS_BLOCKSIZE() {
    return (size_t)getpagesize();
}
#endif

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_READ(
    const std::string& name, FX_OS_MAPPING_FLAG = FX_OS_FLAG_USE_MAPPING_FALSE /* mappingFlag */) 
{
    return ::open(name.c_str(), O_RDONLY);
}

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_EXCL(
    const std::string& name, FX_OS_MAPPING_FLAG = FX_OS_FLAG_USE_MAPPING_FALSE) 
{
    return ::open(name.c_str(), O_RDWR | O_CREAT | O_EXCL,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_CRTR(
    const std::string& name, FX_OS_MAPPING_FLAG = FX_OS_FLAG_USE_MAPPING_FALSE) 
{
    return ::open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

inline FX_OS_FILE_DESCRIPTOR FX_OS_OPEN_FOR_RDWR(
    const std::string&  name, FX_OS_MAPPING_FLAG = FX_OS_FLAG_USE_MAPPING_FALSE) 
{
    return ::open(name.c_str(), O_RDWR);
}

#endif //__FX_IO_POSIX_H
