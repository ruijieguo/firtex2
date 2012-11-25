#ifndef __FX_ATOMIC_GCC_X86_SPINLOCK_H
#define __FX_ATOMIC_GCC_X86_SPINLOCK_H

#define FX_SPINLOCK_DEFINED
typedef unsigned char    spinlock_t;
static inline bool spinlock_try_acquire(spinlock_t volatile * pDest)
{
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
    return __sync_bool_compare_and_swap_1(pDest, 0, 1);
#else
    bool bRet;
    asm volatile (
        "xorw %%ax, %%ax;    \n\t"
        "orb  $1, %%ah     ;    \n\t"
        "lock; cmpxchgb %%ah, %[dest];    \n\t"
        "setz %[ret]    ;   \n\t"
        : [dest] "+m"(*pDest), [ret] "=a"(bRet)
        : "m"(*pDest)
        : "memory", "cc"
      );
    return bRet;
#endif
}

static inline void spinlock_release(spinlock_t volatile * pDest)
{
    FX_COMPILER_RW_BARRIER;
    *pDest = 0;
    FX_COMPILER_RW_BARRIER;
}

#endif //__FX_ATOMIC_GCC_X86_SPINLOCK_H
