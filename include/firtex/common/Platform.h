#ifndef __FX_PLATFORM_H
#define __FX_PLATFORM_H

#if defined(_DEBUG) || !defined(NDEBUG)
#    define    FX_BUILD_DEBUG
#    define    FX_BUILD_TYPE    "DEBUG"
#else
#    define    FX_BUILD_TYPE    "RELEASE"
#    define    FX_BUILD_RELEASE
#endif

// Supported compilers:
#define FX_COMPILER_MSVC       1
#define FX_COMPILER_GCC        2
#define FX_COMPILER_INTEL      3
#define FX_COMPILER_UNKNOWN    -1

// Supported processor architectures:
#define FX_PROCESSOR_X86       1
#define FX_PROCESSOR_IA64      2
#define FX_PROCESSOR_SPARC     3
#define FX_PROCESSOR_AMD64     4
#define FX_PROCESSOR_PPC64     5   // PowerPC 64bit
#define FX_PROCESSOR_UNKNOWN   -1

// Supported OS interfaces
#define FX_OSI_UNKNOWN         0
#define FX_OSI_UNIX            1
#define FX_OSI_WINDOWS         2

// Supported operating systems (value of FX_OS_TYPE):
#define FX_OS_UNKNOWN          -1
#define FX_OS_WIN32            1
#define FX_OS_WIN64            5
#define FX_OS_LINUX            10
#define FX_OS_SUN_SOLARIS      20
#define FX_OS_HPUX             30
#define FX_OS_AIX              50  // IBM AIX
#define FX_OS_FREE_BSD         61  
#define FX_OS_OPEN_BSD         62  
#define FX_OS_NET_BSD          63  
#define FX_OS_MACOS            64  

#if defined(_MSC_VER)
#   if defined(__INTEL_COMPILER)
#       define FX_COMPILER FX_COMPILER_INTEL
#   else
#       define FX_COMPILER FX_COMPILER_MSVC
#   endif
#elif defined(__GCC__) || defined(__GNUC__)
#    define FX_COMPILER FX_COMPILER_GCC
#else
#    define FX_COMPILER FX_COMPILER_UNKNOWN
#endif  // Compiler choice


#if FX_COMPILER == FX_COMPILER_MSVC
#   include <firtex/common/Compiler_VC.h>
#elif FX_COMPILER == FX_COMPILER_GCC
#   include <firtex/common/Compiler_GCC.h>
#elif FX_COMPILER == FX_COMPILER_INTEL
#   include <firtex/common/Compiler_IC.h>
#elif FX_COMPILER == FX_COMPILER_UNKNOWN
#    error Unknown compiler. Compilation aborted
#else
#   error Unknown value of FX_COMPILER macro
#endif

#ifndef FX_STDCALL
#   define FX_STDCALL
#endif

#ifndef FX_EXPORT_API
#   define FX_EXPORT_API
#endif

#endif //__FX_PLATFORM_H


