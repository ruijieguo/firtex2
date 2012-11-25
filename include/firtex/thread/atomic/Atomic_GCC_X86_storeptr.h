#ifndef __FX_ATOMIC_GCC_X86_STOREPTR_H
#define __FX_ATOMIC_GCC_X86_STOREPTR_H

template <typename ORDER>
static inline void storeptr(pointer_t volatile * pMem, pointer_t val);
template <>
inline void storeptr<membar_relaxed>(pointer_t volatile * pMem, pointer_t val)
{
    assert(pMem != NULL) ;
    FX_COMPILER_RW_BARRIER;
    *pMem = val;
}
template <>
inline void storeptr<membar_release>(pointer_t volatile * pMem, pointer_t val)
{
    assert(pMem != NULL) ;
    FX_COMPILER_RW_BARRIER;
    *pMem = val;
    FX_COMPILER_RW_BARRIER;
}
template <>
inline void storeptr<membar_seq_cst>(pointer_t volatile * pMem, pointer_t val)
{
    xchgptr<membar_seq_cst>(pMem, val);
}
static inline void storeptr(pointer_t volatile * pMem, pointer_t val, memory_order order)
{
    switch (order) {
        case membar_relaxed::order:
            storeptr<membar_relaxed>(pMem, val);
            break;
        case membar_release::order:
            storeptr<membar_release>(pMem, val);
            break;
        case membar_seq_cst::order:
            storeptr<membar_seq_cst>(pMem, val);
            break;
        default:
            assert(false)  ;
            storeptr<membar_seq_cst>(pMem, val);
            break;
    }
}
#endif //__FX_ATOMIC_GCC_X86_STOREPTR_H
