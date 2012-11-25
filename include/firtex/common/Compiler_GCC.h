#ifndef __FX_COMPILER_GCC_H
#define __FX_COMPILER_GCC_H

// Compiler version
#define FX_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if FX_COMPILER_VERSION < 40000
#   error "Compiler version error. GCC version 4.0.0 and above is supported"
#endif

// Compiler name
#ifdef __VERSION__
#   define FX_COMPILER__NAME   ("GNU C++ " __VERSION__)
#else
#   define FX_COMPILER__NAME   "GNU C++"
#endif
#define FX_COMPILER__NICK      "gcc"

// OS interface && OS name
#if defined(__linux__)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_LINUX
#   define FX_OS__NAME         "linux"
#   define FX_OS__NICK         "linux"
#   define FX_POSIX
#elif defined(__sun__)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_SUN_SOLARIS
#   define FX_OS__NAME         "Sun Solaris"
#   define FX_OS__NICK         "sun"
#   define FX_POSIX
#elif defined(__hpux__)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_HPUX
#   define FX_OS__NAME         "HP-UX"
#   define FX_OS__NICK         "hpux"
#   define FX_POSIX
#elif defined(_AIX)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_AIX
#   define FX_OS__NAME         "AIX"
#   define FX_OS__NICK         "aix"
#elif defined(__FreeBSD__)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_FREE_BSD
#   define FX_OS__NAME         "FreeBSD"
#   define FX_OS__NICK         "freebsd"
#   define FX_POSIX
#elif defined(__OpenBSD__)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_OPEN_BSD
#   define FX_OS__NAME         "OpenBSD"
#   define FX_OS__NICK         "openbsd"
#   define FX_POSIX
#elif defined(__NetBSD__)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_NET_BSD
#   define FX_OS__NAME         "NetBSD"
#   define FX_OS__NICK         "netbsd"
#   define FX_POSIX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#   define FX_OS_INTERFACE     FX_OSI_UNIX
#   define FX_OS_TYPE          FX_OS_MACOS
#   define FX_OS__NAME         "MacOS"
#   define FX_OS__NICK         "macos"
#   define FX_MACOS
#else
#    error GCC: unknown OS type. Compilation aborted
#endif

// Processor architecture
#if defined(__x86_64__) || defined(__amd64__) || defined(__amd64)
#   define FX_PROCESSOR_ARCH    FX_PROCESSOR_X86_64
#   define FX_BUILD_BITS        64
#   define FX_PROCESSOR__NAME   "X86-64"
#   define FX_PROCESSOR__NICK   "x86-64"
#elif defined(__i386__)
#   define FX_PROCESSOR_ARCH    FX_PROCESSOR_X86
#   define FX_BUILD_BITS        32
#   define FX_PROCESSOR__NAME   "Intel x86"
#   define FX_PROCESSOR__NICK   "x86"
#elif defined(sparc) || defined (__sparc__)
#   define FX_PROCESSOR_ARCH    FX_PROCESSOR_SPARC
#   define FX_PROCESSOR__NAME   "Sparc"
#   define FX_PROCESSOR__NICK   "sparc"
#   ifdef __arch64__
#       define FX_BUILD_BITS   64
#    else
#       error Sparc 32bit is not supported
#    endif
#elif defined(__ia64__)
#    define FX_PROCESSOR_ARCH    FX_PROCESSOR_IA64
#    define FX_BUILD_BITS        64
#    define FX_PROCESSOR__NAME   "Intel IA64"
#    define FX_PROCESSOR__NICK   "ia64"
#elif defined(_ARCH_PPC64)
#    define FX_PROCESSOR_ARCH    FX_PROCESSOR_PPC64
#    define FX_BUILD_BITS        64
#    define FX_PROCESSOR__NAME   "IBM PowerPC64"
#    define FX_PROCESSOR__NICK   "ppc64"
#else
#    error "GCC: unknown processor architecture. Compilation aborted"
#endif

#ifndef __declspec
#   define __declspec( _x )
#endif

#if FX_PROCESSOR_ARCH == FX_PROCESSOR_X86
#   define FX_STDCALL __attribute__((stdcall))
#else
#   define FX_STDCALL
#endif

#define alignof __alignof__

// constexpr is not yet supported 
#define FX_CONSTEXPR

// Variadic template support (only if -std=c++0x compile-line option provided)
#ifdef __GXX_EXPERIMENTAL_CXX0X__
#define FX_COMPILER_SUPPORTS_VARIADIC_TEMPLATE
#endif

// *************************************************
// Alignment macro

#define FX_TYPE_ALIGNMENT(n)   __attribute__ ((aligned (n)))
#define FX_CLASS_ALIGNMENT(n)  __attribute__ ((aligned (n)))
#define FX_DATA_ALIGNMENT(n)   __attribute__ ((aligned (n)))

#endif // #ifndef __FX_COMPILER_GCC_H
