#ifndef __FX_COMPILER_IC_H
#define __FX_COMPILER_IC_H

#error "Intel C++ compiler is not supported yet"

#define  FX_COMPILER_VERSION __INTEL_COMPILER
#define  FX_COMPILER__NAME   "Intel C++ "
#define  FX_COMPILER__NICK   "ic"

#if defined( _WIN64 )
#   define FX_OS_INTERFACE FX_OSI_WINDOWS
#   define FX_OS_TYPE      FX_OS_WIN64
#   define FX_OS__NAME     "win64"
#   define FX_OS__NICK     "win64"
#elif defined( _WIN32 )
#   define FX_OS_INTERFACE FX_OSI_WINDOWS
#   define FX_OS_TYPE      FX_OS_WIN32
#   define FX_OS__NAME     "win32"
#   define FX_OS__NICK     "win32"
#else
#   error "Intel C++: unknown OS type. Compilation aborted"
#endif

#if defined(_M_AMD64)
#   define FX_PROCESSOR_ARCH    FX_PROCESSOR_X86_64
#   define FX_BUILD_BITS        64
#   define FX_PROCESSOR__NAME   "AMD x86-64"
#   define FX_PROCESSOR__NICK   "amd64"
#elif defined(_M_X86)
#   define FX_PROCESSOR_ARCH    FX_PROCESSOR_X86
#   define FX_BUILD_BITS        32
#   define FX_PROCESSOR__NAME   "Intel x86-32"
#   define FX_PROCESSOR__NICK   "x86"
#elif defined(_M_IX86)
#   define FX_PROCESSOR_ARCH    FX_PROCESSOR_X86
#   define FX_BUILD_BITS        32
#   define FX_PROCESSOR__NAME   "Intel x86"
#   define FX_PROCESSOR__NICK   "x86"
#else
#   error "Intel C++: unknown processor architecture. Compilation aborted"
#endif

#endif // #ifndef __FX_COMPILER_IC_H
