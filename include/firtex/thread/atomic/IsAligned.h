#ifndef __FX_IS_ALIGNED_H
#define __FX_IS_ALIGNED_H

#include "firtex/thread/atomic/AtomicIntegrals.h"

FX_NS_DEF(thread);

namespace atomic 
{

/**
 * \p ALIGN must be power of 2.
 *
 * The function is mostly intended for run-time assertion
*/
template <int ALIGN, typename T>
static inline bool isAligned(T const * p)
{
    return (((uptr_atomic_t)p) & uptr_atomic_t(ALIGN - 1)) == 0;
}

/// Checks if the pointer \p p \p nAlign is aligned
/**
 *  \p nAlign must be power of 2.
 *
 * The function is mostly intended for run-time assertion
 */
template <typename T>
static inline bool isAligned(T const * p, size_t nAlign)
{
    return (((uptr_atomic_t)p) & uptr_atomic_t(nAlign - 1)) == 0;
}

/*
 // TODO: GCC 4.1, GCC 4.2: error: sorry, applying attributes to template parameter is not implemented
template <typename T, size_t ALIGNMENT>
struct AlignedType;

#define FX_ALIGNEDTYPE_IMPL(nAlign)                                     \
    template <typename T> struct AlignedType<T, nAlign>                 \
    {                                                                   \
        typedef FX_TYPE_ALIGNMENT(nAlign) T type;                       \
    }

    FX_ALIGNEDTYPE_IMPL(1);
    FX_ALIGNEDTYPE_IMPL(2);
    FX_ALIGNEDTYPE_IMPL(4);
    FX_ALIGNEDTYPE_IMPL(8);
    FX_ALIGNEDTYPE_IMPL(16);
    FX_ALIGNEDTYPE_IMPL(32);
    FX_ALIGNEDTYPE_IMPL(64);
    FX_ALIGNEDTYPE_IMPL(128);
    FX_ALIGNEDTYPE_IMPL(256);
    FX_ALIGNEDTYPE_IMPL(512);
    FX_ALIGNEDTYPE_IMPL(1024);
#   undef FX_ALIGNEDTYPE_IMPL

template <typename T, typename ALIGN_AS>
struct AlignAs 
{
    /// Result of meta-algo: type \p T aligned like type \p ALIGN_AS
    typedef typename AlignedType<T, alignof(ALIGN_AS)>::type type;
};
*/

} //end namespace atomic


FX_NS_END

#endif // #ifndef __FX_IS_ALIGNED_H
