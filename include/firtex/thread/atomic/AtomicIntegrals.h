#ifndef __FX_ATOMIC_INTEGRALS_H
#define __FX_ATOMIC_INTEGRALS_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(thread);

namespace atomic
{

/// Atomic pointer
typedef void* pointer_t;

#if FX_BUILD_BITS == 32
/// 64bit unaligned int
typedef long long atomic64_unaligned;

/// 64bit unaligned unsigned int
typedef unsigned long long  atomic64u_unaligned;

/// 64bit aligned int
typedef atomic64_unaligned FX_TYPE_ALIGNMENT(8) atomic64_aligned;

/// 64bit aligned unsigned int
typedef atomic64u_unaligned FX_TYPE_ALIGNMENT(8) atomic64u_aligned;

/// 64bit atomic int (aligned)
typedef atomic64_aligned atomic64_t;

/// 64bit atomic unsigned int (aligned)
typedef atomic64u_aligned atomic64u_t;

/// 32bit atomic int
typedef long atomic32_t;

/// 32bit atomic unsigned int
typedef unsigned long atomic32u_t;

/// atomic int
typedef atomic32_t atomic_t;

/// atomic unsigned int
typedef atomic32u_t unsigned_atomic_t;

/// atomic int sized as pointer
/**
   This type is synonym for atomic32_t for 32bit platforms and for atomic64_t for 64bit platforms
*/
typedef atomic32_t ptr_atomic_t;

/// atomic unsigned int sized as pointer
/**
   This type is synonym for atomic32u_t for 32bit platforms and for atomic64u_t for 64bit platforms
*/
typedef atomic32u_t uptr_atomic_t;

#   elif FX_BUILD_BITS == 64

typedef long long               atomic64_unaligned;  // 64bit unaligned int
typedef unsigned long long      atomic64u_unaligned; // 64bit unaligned unsigned int
typedef atomic64_unaligned      atomic64_aligned;    // 64bit aligned int
typedef atomic64u_unaligned     atomic64u_aligned;   // 64bit aligned unsigned int

typedef atomic64_aligned        atomic64_t;    // 64bit atomic int
typedef atomic64u_aligned       atomic64u_t;   // 64bit atomic unsigned int

typedef int                     atomic32_t;    // 32bit atomic int
typedef unsigned int            atomic32u_t;    // 32bit atomic unsigned int

typedef atomic32_t        atomic_t;    // atomic int
typedef atomic32u_t       unsigned_atomic_t;    // atomic unsigned int
typedef atomic64_t        ptr_atomic_t;    // atomic int sized as pointer
typedef atomic64u_t       uptr_atomic_t;    // atomic unsigned int sized as pointer

#endif

template <int> struct AtomicLongSelector;

template <> struct AtomicLongSelector<4>
{
    typedef atomic32_t  atomic_long ;
};

template <> struct AtomicLongSelector<8>
{
    typedef atomic64_t  atomic_long ;
};

/// Atomic type for long int
/**
 *  The typedef selects appropriate atomic type depends on sizeof(long):
 * \li if sizeof(long) == 4 (for 32bit platforms) then \p atomic_long is \ref atomic32_t
 * \li if sizeof(long) == 8 (for 32bit platforms) then \p atomic_long is \ref atomic64_t
 */
typedef AtomicLongSelector<sizeof(long)>::atomic_long atomic_long;

/************************************************************************/
/* atomic_cast conversions                                              */
/************************************************************************/
//@cond none
template <typename ATOMIC, typename T>
static inline ATOMIC atomic_cast(T val)
{
    FIRTEX_ASSERT2(sizeof(T) == sizeof(ATOMIC));
    return *(ATOMIC *) &val;
}

template <typename P, typename T>
static inline P * atomic_cast(T * val)
{
    return (P *) val  ;
}

template <>
inline atomic32_t atomic_cast<atomic32_t>(int val)
{
    return (atomic32_t) val ;
}

template <>
inline atomic32_t atomic_cast<atomic32_t>(unsigned int val)
{
    return (atomic32_t) val ;
}

template <>
inline atomic64_unaligned atomic_cast<atomic64_unaligned>(long long val)
{
    return (atomic64_unaligned) val ;
}

template <>
inline atomic64_unaligned atomic_cast<atomic64_unaligned>(unsigned long long val)
{
    return (atomic64_unaligned) val ;
}

template <>
inline atomic_long atomic_cast<atomic_long>(long val)
{
    return (atomic_long) val ;
}

template <>
inline atomic_long atomic_cast<atomic_long>(unsigned long val)
{
    return (atomic_long) val ;
}

} // end namespace atomic

FX_NS_END

#endif // #ifndef __FX_ATOMIC_INTEGRALS_H
