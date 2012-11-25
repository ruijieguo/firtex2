#ifndef __FX_ATOMIC_GCC_X86_STORE32_H
#define __FX_ATOMIC_GCC_X86_STORE32_H

template <typename ORDER>
static inline void store32(atomic32_t volatile * pMem, atomic32_t val);
template <>
inline void store32<membar_relaxed>(atomic32_t volatile * pMem, atomic32_t val)
{
    assert(pMem != NULL) ;
    FX_COMPILER_RW_BARRIER;
    *pMem = val;
}

template <>
inline void store32<membar_release>(atomic32_t volatile * pMem, atomic32_t val)
{
    assert(pMem != NULL) ;
    FX_COMPILER_RW_BARRIER;
    *pMem = val;
    FX_COMPILER_RW_BARRIER;
}

template <>
inline void store32<membar_seq_cst>(atomic32_t volatile * pMem, atomic32_t val)
{
    xchg32<membar_seq_cst>(pMem, val) ;
}

static inline void store32(atomic32_t volatile * pMem, atomic32_t val, memory_order order)
{
    switch (order) {
        case membar_relaxed::order:
            store32<membar_relaxed>(pMem, val);
            break;
        case membar_release::order:
            store32<membar_release>(pMem, val);
            break;
        case membar_seq_cst::order:
            store32<membar_seq_cst>(pMem, val);
            break;
        default:
            assert(false)  ;
            store32<membar_seq_cst>(pMem, val);
            break;
    }
}

#endif //__FX_ATOMIC_GCC_X86_STORE32_H
