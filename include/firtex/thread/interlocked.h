#ifndef __FX_INTERLOCKED_H
#define __FX_INTERLOCKED_H

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#if defined(FX_WINDOWS)

# include <windows.h>

# define FX_INTERLOCKED_INCREMENT InterlockedIncrement
# define FX_INTERLOCKED_DECREMENT InterlockedDecrement
# define FX_INTERLOCKED_COMPARE_EXCHANGE InterlockedCompareExchange
# define FX_INTERLOCKED_EXCHANGE InterlockedExchange
# define FX_INTERLOCKED_EXCHANGE_ADD InterlockedExchangeAdd
# define FX_INTERLOCKED_COMPARE_EXCHANGE_POINTER InterlockedCompareExchangePointer
# define FX_INTERLOCKED_EXCHANGE_POINTER InterlockedExchangePointer

#elif defined(_WIN32_WCE)

// under Windows CE we still have old-style Interlocked* functions

extern "C" long __cdecl InterlockedIncrement( long* );
extern "C" long __cdecl InterlockedDecrement( long* );
extern "C" long __cdecl InterlockedCompareExchange( long*, long, long );
extern "C" long __cdecl InterlockedExchange( long*, long );
extern "C" long __cdecl InterlockedExchangeAdd( long*, long );

# define FX_INTERLOCKED_INCREMENT InterlockedIncrement
# define FX_INTERLOCKED_DECREMENT InterlockedDecrement
# define FX_INTERLOCKED_COMPARE_EXCHANGE InterlockedCompareExchange
# define FX_INTERLOCKED_EXCHANGE InterlockedExchange
# define FX_INTERLOCKED_EXCHANGE_ADD InterlockedExchangeAdd

# define FX_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
    ((void*)FX_INTERLOCKED_COMPARE_EXCHANGE((long*)(dest),(long)(exchange),(long)(compare)))
# define FX_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
    ((void*)FX_INTERLOCKED_EXCHANGE((long*)(dest),(long)(exchange)))

#elif defined(_MSC_VER)

#if defined( __CLRCALL_PURE_OR_CDECL )

extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedIncrement( long volatile * );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedDecrement( long volatile * );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedCompareExchange( long volatile *, long, long );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedExchange( long volatile *, long );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedExchangeAdd( long volatile *, long );

#else

extern "C" long __cdecl _InterlockedIncrement( long volatile * );
extern "C" long __cdecl _InterlockedDecrement( long volatile * );
extern "C" long __cdecl _InterlockedCompareExchange( long volatile *, long, long );
extern "C" long __cdecl _InterlockedExchange( long volatile *, long );
extern "C" long __cdecl _InterlockedExchangeAdd( long volatile *, long );

#endif

# pragma intrinsic( _InterlockedIncrement )
# pragma intrinsic( _InterlockedDecrement )
# pragma intrinsic( _InterlockedCompareExchange )
# pragma intrinsic( _InterlockedExchange )
# pragma intrinsic( _InterlockedExchangeAdd )

# if defined(_M_IA64) || defined(_M_AMD64)

extern "C" void* __cdecl _InterlockedCompareExchangePointer( void* volatile *, void*, void* );
extern "C" void* __cdecl _InterlockedExchangePointer( void* volatile *, void* );

#  pragma intrinsic( _InterlockedCompareExchangePointer )
#  pragma intrinsic( _InterlockedExchangePointer )

#  define FX_INTERLOCKED_COMPARE_EXCHANGE_POINTER _InterlockedCompareExchangePointer
#  define FX_INTERLOCKED_EXCHANGE_POINTER _InterlockedExchangePointer

# else

#  define FX_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
    ((void*)FX_INTERLOCKED_COMPARE_EXCHANGE((long volatile*)(dest),(long)(exchange),(long)(compare)))
#  define FX_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
    ((void*)FX_INTERLOCKED_EXCHANGE((long volatile*)(dest),(long)(exchange)))

# endif

# define FX_INTERLOCKED_INCREMENT _InterlockedIncrement
# define FX_INTERLOCKED_DECREMENT _InterlockedDecrement
# define FX_INTERLOCKED_COMPARE_EXCHANGE _InterlockedCompareExchange
# define FX_INTERLOCKED_EXCHANGE _InterlockedExchange
# define FX_INTERLOCKED_EXCHANGE_ADD _InterlockedExchangeAdd

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || defined( __CYGWIN__ )

FX_NS_DEF(thread);

extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement( long volatile * );
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement( long volatile * );
extern "C" __declspec(dllimport) long __stdcall InterlockedCompareExchange( long volatile *, long, long );
extern "C" __declspec(dllimport) long __stdcall InterlockedExchange( long volatile *, long );
extern "C" __declspec(dllimport) long __stdcall InterlockedExchangeAdd( long volatile *, long );

FX_NS_END

# define FX_INTERLOCKED_INCREMENT FX_NS(thread)::InterlockedIncrement
# define FX_INTERLOCKED_DECREMENT FX_NS(thread)::InterlockedDecrement
# define FX_INTERLOCKED_COMPARE_EXCHANGE FX_NS(thread)::InterlockedCompareExchange
# define FX_INTERLOCKED_EXCHANGE FX_NS(thread)::InterlockedExchange
# define FX_INTERLOCKED_EXCHANGE_ADD FX_NS(thread)::InterlockedExchangeAdd

# define FX_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
    ((void*)FX_INTERLOCKED_COMPARE_EXCHANGE((long volatile*)(dest),(long)(exchange),(long)(compare)))
# define FX_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
    ((void*)FX_INTERLOCKED_EXCHANGE((long volatile*)(dest),(long)(exchange)))

#else

# error "Interlocked intrinsics not available"

#endif

#endif // #ifndef __FX_INTERLOCKED_H
