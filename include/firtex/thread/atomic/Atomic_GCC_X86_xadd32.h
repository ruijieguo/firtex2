#ifndef __FX_ATOMIC_GCC_X86_XADD32_H
#define __FX_ATOMIC_GCC_X86_XADD32_H

#define FX_XADD32_DEFINED
template <typename ORDER>
static inline atomic32_t xadd32(atomic32_t volatile * pMem, atomic32_t val)
{
    atomic32_t ret;
    asm volatile (
        " lock; xaddl %2, %0"
        : "=m" (*pMem), "=a" (ret)
        : "a" (val), "m" (*pMem)
        : "cc", "memory"
      );
    return (ret);
}

static inline atomic32_t xadd32(atomic32_t volatile * pMem, atomic32_t val, memory_order order)
{
    return xadd32<membar_relaxed>(pMem, val);
}

#endif //__FX_ATOMIC_GCC_X86_XADD32_H
