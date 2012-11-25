#ifndef __FX_ATOMIC_GCC_AMD64_H
#define __FX_ATOMIC_GCC_AMD64_H

#include "firtex/common/StdHeader.h"

/**
 * Reference: A.Terekhov http://www.decadent.org.uk/pipermail/cpp-threads/2008-December/001933.html
 */

// For atomic operations the template-based alternatives are defined
#define FX_ATOMIC_TEMPLATE_DEFINED 1

FX_NS_DEF(thread);

namespace atomic 
{


///fence
#include "firtex/thread/atomic/Atomic_GCC_X86_fence.h"

//
// 32bit atomic primitives
//
#include "firtex/thread/atomic/Atomic_GCC_X86_cas32.h"
#include "firtex/thread/atomic/Atomic_GCC_X86_xchg32.h"
#include "firtex/thread/atomic/Atomic_GCC_X86_xadd32.h"
#include "firtex/thread/atomic/Atomic_GCC_X86_load32.h"
#include "firtex/thread/atomic/Atomic_GCC_X86_store32.h"

//
// 64bit atomic primitives
//
template <typename SUCCESS_ORDER>
static inline bool cas64(atomic64_t volatile * pMem,
                         atomic64_unaligned expected,
                         atomic64_unaligned desired)
{
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8)
    return __sync_bool_compare_and_swap(pMem, expected, desired);
#else
    bool bRet;
    asm volatile (
            "lock; cmpxchgq %2, %0; setz %1"
            : "+m" (*pMem), "=q" (bRet)
            : "r" (desired), "m" (*pMem), "a" (expected)
            : "cc", "memory"
                  );
    return bRet;
#endif
}

static inline bool cas64(atomic64_t volatile * pMem, atomic64_unaligned expected,
                         atomic64_unaligned desired, memory_order success_order,
                         memory_order failure_order)
{
    return cas64<membar_relaxed>(pMem, expected, desired);
}

template <typename SUCCESS_ORDER>
static inline atomic64_t vcas64( atomic64_t volatile * pMem,
                                atomic64_unaligned expected,
                                atomic64_unaligned desired)
{
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8)
    return __sync_val_compare_and_swap(pMem, expected, desired);
#else
    atomic64_t nRet;
    asm volatile (
            "lock; cmpxchgq %2, %0;"
            : "+m" (*pMem), "=a" (nRet)
            : "r" (desired), "m" (*pMem), "a" (expected)
            : "cc", "memory"
                  );
    return nRet;
#endif
}

static inline atomic64_t vcas64( atomic64_t volatile * pMem, atomic64_unaligned expected,
                                atomic64_unaligned desired, memory_order success_order,
                                memory_order failure_order)
{
    return vcas64<membar_relaxed>(pMem, expected, desired);
}

#define FX_XCHG64_DEFINED
template <typename ORDER>
static inline atomic64_t xchg64(atomic64_t volatile * pMem, atomic64_t val)
{
    atomic64_t ret;
    asm volatile (
            " xchgq %2, %0"
            : "=m" (*pMem), "=a" (ret)
            : "a" (val), "m" (*pMem)
            : "memory"
                  );
    return (ret);
}

static inline atomic64_t xchg64(atomic64_t volatile * pMem, atomic64_t val, memory_order order)
{
    return xchg64<membar_relaxed>(pMem, val) ;
}

#define FX_XADD64_DEFINED
template <typename ORDER>
static inline atomic64_t xadd64(atomic64_t volatile * pMem, atomic64_t val)
{
    atomic64_t ret;
    asm volatile (
            " lock; xaddq %2, %0"
            : "=m" (*pMem), "=a" (ret)
            : "a" (val), "m" (*pMem)
            : "cc", "memory"
                  );
    return (ret);
}

static inline atomic64_t xadd64(atomic64_t volatile * pMem, atomic64_t val, memory_order order)
{
    return xadd64<membar_relaxed>(pMem, val);
}

template <typename ORDER>
static inline atomic64_t load64(atomic64_t volatile const * pMem);

template <>
inline atomic64_t load64<membar_relaxed>(atomic64_t volatile const * pMem)
{
    FX_COMPILER_RW_BARRIER;
    return *pMem     ;
}

template <>
inline atomic64_t load64<membar_consume>(atomic64_t volatile const * pMem)
{
    return load64<membar_relaxed>(pMem);
}

template <>
inline atomic64_t load64<membar_acquire>(atomic64_t volatile const * pMem)
{
    FX_COMPILER_RW_BARRIER  ;
    atomic64_t ret =  *pMem  ;
    FX_COMPILER_RW_BARRIER  ;
    return ret;
}

template <>
inline atomic64_t load64<membar_seq_cst>(atomic64_t volatile const * pMem)
{
    atomic64_t ret;
    asm volatile (
            " lock; xaddq %1, %2"
            : "=a" (ret)
            : "a" ((atomic64_t)0), "m" (*pMem)
            : "cc", "memory"
                  );
    return (ret);
}
static inline atomic64_t load64(atomic64_t volatile const * pMem, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        return load64<membar_relaxed>(pMem);
    case membar_consume::order:
        return load64<membar_consume>(pMem);
    case membar_acquire::order:
        return load64<membar_acquire>(pMem);
    case membar_seq_cst::order:
        return load64<membar_seq_cst>(pMem);
    default:
        assert(false);
        return load64<membar_seq_cst>(pMem);
    }
}

template <typename ORDER>
static inline void store64(atomic64_t volatile * pMem, atomic64_t val);

template <>
inline void store64<membar_relaxed>(atomic64_t volatile * pMem, atomic64_t val)
{
    assert(pMem != NULL);
    FX_COMPILER_RW_BARRIER;
    *pMem = val;
}

template <>
inline void store64<membar_release>(atomic64_t volatile * pMem, atomic64_t val)
{
    assert(pMem != NULL);
    FX_COMPILER_RW_BARRIER;
    *pMem = val;
    FX_COMPILER_RW_BARRIER;
}

template <>
inline void store64<membar_seq_cst>(atomic64_t volatile * pMem, atomic64_t val)
{
    xchg64<membar_seq_cst>(pMem, val);
}

static inline void store64(atomic64_t volatile * pMem, atomic64_t val, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        store64<membar_relaxed>(pMem, val);
        break;
    case membar_release::order:
        store64<membar_release>(pMem, val);
        break;
    case membar_seq_cst::order:
        store64<membar_seq_cst>(pMem, val);
        break;
    default:
        assert(false);
        store64<membar_seq_cst>(pMem, val);
        break;
    }
}

//
// Atomic pointer primitives
//
template <typename SUCCESS_ORDER>
static inline bool casptr(pointer_t volatile * pMem, pointer_t expected, pointer_t desired)
{
    return cas64<SUCCESS_ORDER>((atomic64_t volatile *) pMem, (atomic64_t) expected,
                                (atomic64_t) desired);
}

static inline bool casptr(pointer_t volatile * pMem, pointer_t expected,
                          pointer_t desired, memory_order success_order,
                          memory_order failure_order)
{
    return cas64((atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired, success_order, failure_order);
}

template <typename SUCCESS_ORDER>
static inline pointer_t vcasptr( pointer_t volatile * pMem, pointer_t expected,
                                pointer_t desired)
{
    return (pointer_t) vcas64<SUCCESS_ORDER>((atomic64_t volatile *) pMem,
            (atomic64_t) expected, (atomic64_t) desired);
}

static inline pointer_t vcasptr( pointer_t volatile * pMem, pointer_t expected,
                                pointer_t desired, memory_order success_order,
                                memory_order failure_order)
{
    return (pointer_t) vcas64((atomic64_t volatile *) pMem, (atomic64_t) expected,
                              (atomic64_t) desired, success_order, failure_order);
}

#define FX_XCHGPTR_DEFINED
template <typename ORDER>
static inline pointer_t xchgptr(pointer_t volatile * pMem, pointer_t val)
{
    return (pointer_t) xchg64<ORDER>((atomic64_t volatile *) pMem, *((atomic64_t *) &val));
}

static inline pointer_t xchgptr(pointer_t volatile * pMem, pointer_t val, memory_order order)
{
    return (pointer_t) xchg64((atomic64_t volatile *) pMem, *((atomic64_t *) &val), order);
}

#include <firtex/thread/atomic/Atomic_GCC_X86_loadptr.h>
#include <firtex/thread/atomic/Atomic_GCC_X86_storeptr.h>

//
// Spin-lock primitives
//
#include <firtex/thread/atomic/Atomic_GCC_X86_spinlock.h>

//
// Atomic 128bit primitives
//
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
#    ifndef __SSE2__
#        error SSE2 is not detected. FX library must be compiled with SSE2 instruction set
#    endif

/**
 * _GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 is defined for 64bit compiler starting with g++ version 4.3
 * for -march= core2, amdfam10 (barcelona) and higher
 */

#define FX_DWORD_CAS_SUPPORTED    1
#define FX_128BIT_ATOMIC_DEFINED
#define FX_CAS128_DEFINED

struct atomic128_t 
{
    atomic64_t  lo;
    atomic64_t  hi;
} __attribute__ ((__aligned__(16)));

typedef __uint128_t gcc_uint128_t __attribute__((__aligned__(16)));

template <typename SUCCESS_ORDER>
static inline bool cas128(volatile atomic128_t * pDest, const atomic128_t& oldVal, const atomic128_t& newVal)
{
    FIRTEX_ASSERT2(sizeof(atomic128_t) == 16);
    FIRTEX_ASSERT2(sizeof(atomic128_t) == sizeof(gcc_uint128_t));
    return __sync_bool_compare_and_swap(reinterpret_cast<gcc_uint128_t volatile *>(pDest), 
            reinterpret_cast<const gcc_uint128_t&>(oldVal), reinterpret_cast<const gcc_uint128_t&>(newVal));
    //return __sync_bool_compare_and_swap(pDest, oldVal, newVal);
}
static inline bool cas128(volatile atomic128_t * pDest, const atomic128_t& oldVal,
                          const atomic128_t& newVal, memory_order success_order,
                          memory_order failure_order)
{
    return cas128<membar_relaxed>(pDest, oldVal, newVal) ;
}

template <typename SUCCESS_ORDER>
static inline atomic128_t vcas128(volatile atomic128_t * pDest,
                                  const atomic128_t& oldVal,
                                  const atomic128_t& newVal)
{
    FIRTEX_ASSERT2(sizeof(atomic128_t) == 16);
    FIRTEX_ASSERT2(sizeof(atomic128_t) == sizeof(gcc_uint128_t));
    gcc_uint128_t nRet = __sync_val_compare_and_swap(reinterpret_cast<gcc_uint128_t volatile *>(pDest), 
            reinterpret_cast<const gcc_uint128_t&>(oldVal), reinterpret_cast<const gcc_uint128_t&>(newVal));
    return *reinterpret_cast<atomic128_t *>(&nRet);
}
static inline atomic128_t vcas128(volatile atomic128_t * pDest, const atomic128_t& oldVal,
                                  const atomic128_t& newVal, memory_order success_order,
                                  memory_order failure_order)
{
    return vcas128<membar_relaxed>(pDest, oldVal, newVal);
}

template <typename ORDER>
static inline atomic128_t load128(atomic128_t volatile const * pMem);
template <>
inline atomic128_t load128<membar_relaxed>(atomic128_t volatile const * pMem)
{
    gcc_uint128_t nVal;
    asm volatile (
            "movdqa %[pMem], %%xmm0;   \n\t"
            "movdqa %%xmm0, %[nVal];   \n\t"
            : [nVal] "=m" (nVal)
            : [pMem] "m" (*pMem)
            : "xmm0", "memory"
                  );
    return *((atomic128_t *) &nVal);
}
template <>
inline atomic128_t load128<membar_consume>(atomic128_t volatile const * pMem)
{
    return load128<membar_relaxed>(pMem);
}
template <>
inline atomic128_t load128<membar_acquire>(atomic128_t volatile const * pMem)
{
    gcc_uint128_t nVal;
    asm volatile (
            "movdqa %[pMem], %%xmm0;   \n\t"
            "movdqa %%xmm0, %[nVal];   \n\t"
            : [nVal] "=m" (nVal)
            : [pMem] "m" (*pMem)
            : "xmm0", "memory"
                  );
    return *((atomic128_t *) &nVal);
}
template <>
inline atomic128_t load128<membar_seq_cst>(atomic128_t volatile const * pMem)
{
    atomic128_t nVal = load128<membar_acquire>(pMem) ;
    fence<membar_seq_cst>();
    return nVal;
}
static inline atomic128_t load128(atomic128_t volatile const * pMem, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        return load128<membar_relaxed>(pMem);
    case membar_consume::order:
        return load128<membar_consume>(pMem);
    case membar_acquire::order:
        return load128<membar_acquire>(pMem);
    case membar_seq_cst::order:
        return load128<membar_seq_cst>(pMem);
    default:
        assert(false);
        return load128<membar_seq_cst>(pMem);
    }
}

template <typename ORDER>
static inline void store128(atomic128_t volatile * pMem, const atomic128_t& val);

template <>
inline void store128<membar_relaxed>(atomic128_t volatile * pMem, const atomic128_t& val)
{
    // Atomically stores 128bit value by SSE instruction movdqa
    asm volatile (
            "movdqa   %[val], %%xmm0  ;   \n\t"
            "movdqa   %%xmm0, %[pMem] ;   \n\t"
            : [pMem] "=m" (*pMem)
            : [val] "m" (val)
            : "xmm0", "memory"
                  );
}

template <>
inline void store128<membar_release>(atomic128_t volatile * pMem, const atomic128_t& val)
{
    // Atomically stores 128bit value by SSE instruction movdqa
    asm volatile (
            "movdqa   %[val], %%xmm0  ;   \n\t"
            "movdqa   %%xmm0, %[pMem] ;   \n\t"
            : [pMem] "=m" (*pMem)
            : [val] "m" (val)
            : "xmm0", "memory"
                  );
}

template <>
inline void store128<membar_seq_cst>(atomic128_t volatile * pMem, const atomic128_t& val)
{
    // CAS-based emulation xchg128
    atomic128_t cur;
    FX_COMPILER_RW_BARRIER;
    do 
    {
        cur = load128<membar_relaxed>(pMem);
    } while (!cas128<membar_seq_cst>(pMem, cur, val));
}

static inline void store128(atomic128_t volatile * pMem,
                            const atomic128_t& val, 
                            memory_order order)
{
    switch (order) 
    {
    case membar_relaxed::order:
        store128<membar_relaxed>(pMem, val);
        break;
    case membar_release::order:
        store128<membar_release>(pMem, val);
        break;
    case membar_seq_cst::order:
        store128<membar_seq_cst>(pMem, val);
        break;
    default:
        assert(false);
        store128<membar_seq_cst>(pMem, val);
        break;
    }
}

#endif  // __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16

#include "firtex/thread/atomic/Atomic_GCC_X86_atomic_bool.h"

} //end namespace atomic


FX_NS_END

#endif //__FX_ATOMIC_GCC_AMD64_H
