#ifndef __FX_ATOMIC_GCC_X86_FENCE_H
#define __FX_ATOMIC_GCC_X86_FENCE_H

template <typename ORDER>
static inline void fence()
{
    FX_COMPILER_RW_BARRIER;
}

template <>
inline void fence<membar_seq_cst>()
{
    asm volatile ("mfence" : : : "memory");
}

static inline void fence(memory_order order)
{
    switch(order) 
    {
    case membar_relaxed::order:
    case membar_release::order:
    case membar_consume::order:
    case membar_acquire::order:
    case membar_acq_rel::order:
        break;
    case membar_seq_cst::order:
        fence<membar_seq_cst>();
        break;
    default:
        assert(false);
    }
}

#endif //FX_ATOMIC_GCC_X86_FENCE_H
