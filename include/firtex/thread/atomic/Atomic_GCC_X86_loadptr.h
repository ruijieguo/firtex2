#ifndef __FX_ATOMIC_GCC_X86_LOADPTR_H
#define __FX_ATOMIC_GCC_X86_LOADPTR_H

template <typename ORDER>
static inline pointer_t loadptr(pointer_t volatile const * pMem);
template <>
inline pointer_t loadptr<membar_relaxed>(pointer_t volatile const * pMem)
{
    FX_COMPILER_RW_BARRIER;
    return *pMem;
}

template <>
inline pointer_t loadptr<membar_consume>(pointer_t volatile const * pMem)
{
    return loadptr<membar_relaxed>(pMem);
}
template <>
inline pointer_t loadptr<membar_acquire>(pointer_t volatile const * pMem)
{
    return loadptr<membar_relaxed>(pMem);
}
template <>
inline pointer_t loadptr<membar_seq_cst>(pointer_t volatile const * pMem)
{
    fence<membar_seq_cst>();
    return *pMem;
}
static inline pointer_t loadptr(pointer_t volatile const * pMem, memory_order order)
{
    switch (order) {
        case membar_relaxed::order:
            return loadptr<membar_relaxed>(pMem);
        case membar_consume::order:
            return loadptr<membar_consume>(pMem);
        case membar_acquire::order:
            return loadptr<membar_acquire>(pMem);
        case membar_seq_cst::order:
            return loadptr<membar_seq_cst>(pMem);
        default:
            assert(false)  ;
            return loadptr<membar_seq_cst>(pMem);
    }
}

#endif //__FX_ATOMIC_GCC_X86_LOADPTR_H
