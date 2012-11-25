#ifndef __FX_ATOMIC_GCC_X86_CAS32_H
#define __FX_ATOMIC_GCC_X86_CAS32_H

template <typename SUCCESS_ORDER>
inline static bool cas32(atomic32_t volatile * pMem,
                         atomic32_t expected, atomic32_t desired)
{
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4) || FX_COMPILER_VERSION > 40099
    return __sync_bool_compare_and_swap(pMem, expected, desired) ;
#else
    bool bRet;
    asm volatile (
        "lock; cmpxchgl %2, %0; setz %1"
        : "+m" (*pMem), "=q" (bRet)
        : "r" (desired), "m" (*pMem), "a" (expected)
        : "cc", "memory"
      );
    return bRet;
#endif
}

inline static bool cas32(atomic32_t volatile * pMem, atomic32_t expected,
                         atomic32_t desired, memory_order success_order,
                         memory_order failure_order)
{
    return cas32<membar_relaxed>(pMem, expected, desired);
}

template <typename SUCCESS_ORDER>
static inline atomic32_t vcas32(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4) || FX_COMPILER_VERSION > 40099
    return __sync_val_compare_and_swap(pMem, expected, desired);
#else
    atomic32_t nRet;
    asm volatile (
        "lock; cmpxchgl %2, %0;"
        : "+m" (*pMem), "=a" (nRet)
        : "r" (desired), "m" (*pMem), "a" (expected)
        : "cc", "memory"
      );
    return nRet;
#endif
}

static inline atomic32_t vcas32(atomic32_t volatile * pMem, atomic32_t expected,
                                atomic32_t desired, memory_order success_order,
                                memory_order failure_order)
{
    return vcas32<membar_relaxed>(pMem, expected, desired);
}

#endif  //__FX_ATOMIC_GCC_X86_CAS32_H
