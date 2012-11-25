#ifndef __FX_ATOMIC_GCC_X86_LOAD32_H
#define __FX_ATOMIC_GCC_X86_LOAD32_H

template <typename ORDER>
static inline atomic32_t load32(atomic32_t volatile const * pMem);
template <>
inline atomic32_t load32<membar_relaxed>(atomic32_t volatile const * pMem)
{
    FX_COMPILER_RW_BARRIER;
    return *pMem;
}

template <>
inline atomic32_t load32<membar_consume>(atomic32_t volatile const * pMem)
{
    return load32<membar_relaxed>(pMem);
}

template <>
inline atomic32_t load32<membar_acquire>(atomic32_t volatile const * pMem)
{
    FX_COMPILER_RW_BARRIER ;
    atomic32_t ret =  *pMem ;
    FX_COMPILER_RW_BARRIER ;
    return ret ;
}

template <>
inline atomic32_t load32<membar_seq_cst>(atomic32_t volatile const * pMem)
{
    atomic32_t ret;
    asm volatile (
        " lock; xaddl %1, %2"
        : "=a" (ret)
        : "a" (0), "m" (*pMem)
        : "cc", "memory"
      );
    return (ret);
/*
    fence<membar_seq_cst>();
    return *pMem    ;
*/
}

static inline atomic32_t load32(atomic32_t volatile const * pMem, memory_order order)
{
    switch (order) {
        case membar_relaxed::order:
            return load32<membar_relaxed>(pMem);
        case membar_consume::order:
            return load32<membar_consume>(pMem);
        case membar_acquire::order:
            return load32<membar_acquire>(pMem);
        case membar_seq_cst::order:
            return load32<membar_seq_cst>(pMem);
        default:
            assert(false)  ;
            return load32<membar_seq_cst>(pMem);
    }
}

#endif //__FX_ATOMIC_GCC_X86_LOAD32_H
