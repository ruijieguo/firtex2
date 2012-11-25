#ifndef __FX_PLATFORM_VC_H
#define __FX_PLATFORM_VC_H

// Compiler version
#define FX_COMPILER_VERSION _MSC_VER

// Compiler name
// Supported compilers: MS VC 2005 (?), 2008, 2010
//
#if _MSC_VER == 1400
#   define  FX_COMPILER__NAME  "MS Visual C++ 2005"
#   define  FX_COMPILER__NICK  "msvc2005"
#elif _MSC_VER == 1500
#   define  FX_COMPILER__NAME  "MS Visual C++ 2008"
#   define  FX_COMPILER__NICK  "msvc2008"
#elif _MSC_VER == 1600
#   define  FX_COMPILER__NAME  "MS Visual C++ 2010"
#   define  FX_COMPILER__NICK  "msvc2010"
#else
#   define  FX_COMPILER__NAME  "MS Visual C++"
#   define  FX_COMPILER__NICK  "msvc"
#endif

// OS interface
#define FX_OS_INTERFACE FX_OSI_WINDOWS
#define FX_WINDOWS

// OS name
#if defined(_WIN64)
#   define FX_OS_TYPE      FX_OS_WIN64
#   define FX_OS__NAME     "Win64"
#   define FX_OS__NICK     "Win64"
#elif defined(_WIN32)
#   define FX_OS_TYPE      FX_OS_WIN32
#   define FX_OS__NAME     "Win32"
#   define FX_OS__NICK     "Win32"
#endif

// Processor architecture
#ifdef _M_IX86
#   define FX_BUILD_BITS       32
#   define FX_PROCESSOR_ARCH   FX_PROCESSOR_X86
#   define FX_PROCESSOR__NAME  "Intel x86"
#   define FX_PROCESSOR__NICK  "x86"
#elif _M_X64
#   define FX_BUILD_BITS       64
#   define FX_PROCESSOR_ARCH   FX_PROCESSOR_X86_64
#   define FX_PROCESSOR__NAME  "X86_64"
#   define FX_PROCESSOR__NICK  "x86_64"
#else
#   define FX_BUILD_BITS        -1
#   define FX_PROCESSOR_ARCH    FX_PROCESSOR_UNKNOWN
#   define FX_PROCESSOR__NAME    "<<Undefined>>"
#   error Microsoft Visual C++ compiler is supported for x86 only
#endif


#define  __attribute__( _x )

#define  FX_STDCALL    __stdcall

#ifdef FX_BUILD_LIB_SHARED
#   define FX_EXPORT_API __declspec(dllimport)
#elif FX_BUILD_LIB_STATIC
#   define FX_EXPORT_API __declspec(dllexport)
#else
#   define FX_EXPORT_API
#endif

#define alignof  __alignof

// Memory leaks detection (debug build only)
#ifdef _DEBUG
//#   define _CRTDBG_MAP_ALLOC
#   define _CRTDBG_MAPALLOC
#   include <stdlib.h>
#   include <crtdbg.h>
#   define FX_MSVC_MEMORY_LEAKS_DETECTING_ENABLED
#endif

// constexpr is not yet supported 
#define FX_CONSTEXPR

// Variadic template support
//#define FX_COMPILER_SUPPORTS_VARIADIC_TEMPLATE     1

// *************************************************
// Alignment macro

// VC 2005 generates error C2719 "formal parameter with __declspec(align('#')) won't be aligned"
// for function's formal parameter with align declspec
#define FX_TYPE_ALIGNMENT(n)     __declspec( align(n) )
#define FX_DATA_ALIGNMENT(n)     __declspec( align(n) )
#define FX_CLASS_ALIGNMENT(n)    __declspec( align(n) )

#endif//__FX_PLATFORM_VC_H
