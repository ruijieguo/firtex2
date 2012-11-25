#ifndef __FX_ATOMIC_GCC_IA64_H
#define __FX_ATOMIC_GCC_IA64_H

#include "firtex/common/StdHeader.h"

/**
 * Reference: http://www.decadent.org.uk/pipermail/cpp-threads/2008-December/001932.html
 */

// For atomic operations the template-based alternatives are defined
#define FX_ATOMIC_TEMPLATE_DEFINED 1

FX_NS_DEF(thread);

namespace atomic 
{


template <typename MB>
static inline void fence()
{}

template <>
inline void fence<membar_seq_cst>()
{
    asm volatile (
            "mf ;;  \n\t"
            : : : "memory"
                  );
}

template <>
inline void fence<membar_acquire>()
{
    //fence<membar_seq_cst>();
    atomic64_t val   ;
    asm volatile (
            "ld8.acq %[val] = [%[pMem]] \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (&val)
            : "memory"
                  );
}

template <>
inline void fence<membar_release>()
{
    //fence<membar_seq_cst>() ;
    atomic64_t val  ;
    asm volatile (
            "st8.rel [%[pMem]] = r0  \n\t"
            :
            : [pMem] "r" (&val)
            : "memory"
                  );
}

template <>
inline void fence<membar_acq_rel>()
{
    fence<membar_seq_cst>();
}

static inline void fence(memory_order order)
{
    switch( order ) {
    case membar_relaxed::order:
        fence<membar_relaxed>();
        break;
    case membar_release::order:
        fence<membar_release>();
        break;
    case membar_consume::order:
        fence<membar_consume>();
        break;
    case membar_acquire::order:
        fence<membar_acquire>();
        break;
    case membar_acq_rel::order:
        fence<membar_acq_rel>();
        break;
    case membar_seq_cst::order:
        fence<membar_seq_cst>();
        break;
    default:
        assert( false );
    }
}

//
// 32bit primitives
//

//////////////////////////////////////////////////////////////////////////
// load32

template <typename ORDER>
static inline atomic32_t load32( atomic32_t volatile const * pMem );

template <>
inline atomic32_t load32<membar_relaxed>( atomic32_t volatile const * pMem )
{
    atomic32_t val;
    asm volatile (
            "ld4 %[val] = [%[pMem]]  \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory"
                  );
    return val;
}

template <>
inline atomic32_t load32<membar_acquire>( atomic32_t volatile const * pMem )
{
    atomic32_t val;
    asm volatile (
            "ld4.acq %[val] = [%[pMem]]  \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory"
                  );
    return val;
}

template <>
inline atomic32_t load32<membar_consume>( atomic32_t volatile const * pMem )
{
    return load32<membar_relaxed>( pMem );
}

template <>
inline atomic32_t load32<membar_seq_cst>( atomic32_t volatile const * pMem )
{
    atomic32_t val;
    asm volatile (
            "mf \n\t"
            "ld4.acq %[val] = [%[pMem]] \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory"
                  );
    return val;
}

static inline atomic32_t load32( atomic32_t volatile const * pMem, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        return load32<membar_relaxed>( pMem );
        //case membar_release::order:
        //    return load32<membar_release>( pMem );
    case membar_consume::order:
        return load32<membar_consume>( pMem );
    case membar_acquire::order:
        return load32<membar_acquire>( pMem );
        //case membar_acq_rel::order:
        //    return load32<membar_acq_rel>( pMem );
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return load32<membar_seq_cst>( pMem );
    }
}

//////////////////////////////////////////////////////////////////////////
// store32

template <typename ORDER>
static inline void store32( atomic32_t volatile * pMem, atomic32_t val );

template <>
inline void store32<membar_relaxed>( atomic32_t volatile * pMem, atomic32_t val )
{
    asm volatile (
            "st4 [%[pMem]] = %[val]  \n\t"
            :
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
}

/*
  template <>
  inline void store32<membar_acquire>( atomic32_t volatile * pMem, atomic32_t val )
  {
  // Store with acquire semantics?..
  //FX_STATIC_ASSERT( false );

  asm volatile (
  "st4 [%[pMem]] = %[val];;  \n\t"
  "mf \n\t"
  :
  : [pMem] "r" (pMem), [val] "r" (val)
  : "memory"
  );
  }

  template <>
  inline void store32<membar_consume>( atomic32_t volatile * pMem, atomic32_t val )
  {
  // Store with consume semantics?..
  //FX_STATIC_ASSERT( false );

  store32<membar_acquire>( pMem, val ) ;
  }
*/
template <>
inline void store32<membar_release>( atomic32_t volatile * pMem, atomic32_t val )
{
    asm volatile (
            "st4.rel [%[pMem]] = %[val]  \n\t"
            :
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
}
/*
  template <>
  inline void store32<membar_acq_rel>( atomic32_t volatile * pMem, atomic32_t val )
  {
  // Store with acquire/release semantics?..
  //FX_STATIC_ASSERT( false );

  store32<membar_release>( pMem, val ) ;
  }
*/
template <>
inline void store32<membar_seq_cst>( atomic32_t volatile * pMem, atomic32_t val )
{
    asm volatile (
            "st4.rel [%[pMem]] = %[val] \n\t"
            "mf     \n\t"
            :
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
}


static inline void store32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        store32<membar_relaxed>( pMem, val ) ;
        break;
        //case membar_consume::order:
        //    store32<membar_consume>( pMem, val ) ;
        //    break;
        //case membar_acquire::order:
        //    store32<membar_acquire>( pMem, val ) ;
        //    break;
    case membar_release::order:
        store32<membar_release>( pMem, val ) ;
        break;
        //case membar_acq_rel::order:
        //    store32<membar_acq_rel>( pMem, val ) ;
        //    break;
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        store32<membar_seq_cst>( pMem, val ) ;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
// vcas32

template <typename SUCCESS_ORDER>
static inline atomic32_t vcas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired ) ;

template <>
inline atomic32_t vcas32<membar_acquire>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
{
    atomic32_t current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg4.acq %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}
template <>
inline atomic32_t vcas32<membar_relaxed>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
{
    return vcas32<membar_acquire>( pMem, expected, desired );
}
template <>
inline atomic32_t vcas32<membar_consume>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
{
    return vcas32<membar_acquire>( pMem, expected, desired );
}

template <>
inline atomic32_t vcas32<membar_release>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
{
    atomic32_t current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg4.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

template <>
inline atomic32_t vcas32<membar_acq_rel>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
{
    atomic32_t current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg4.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            "mf;; \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

template <>
inline atomic32_t vcas32<membar_seq_cst>( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
{
    atomic32_t current;
    asm volatile (
            "mf \n\t"
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg4.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            "mf \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

static inline atomic32_t vcas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order )
{
    switch (success_order) {
    case membar_relaxed::order:
        return vcas32<membar_relaxed>( pMem, expected, desired );
    case membar_consume::order:
        return vcas32<membar_consume>( pMem, expected, desired );
    case membar_acquire::order:
        return vcas32<membar_acquire>( pMem, expected, desired );
    case membar_release::order:
        return vcas32<membar_release>( pMem, expected, desired );
    case membar_acq_rel::order:
        return vcas32<membar_acq_rel>( pMem, expected, desired );
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return vcas32<membar_seq_cst>( pMem, expected, desired );
    }
}

//////////////////////////////////////////////////////////////////////////
// cas32

template <typename SUCCESS_ORDER>
static inline bool cas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired )
{
    return vcas32<SUCCESS_ORDER>( pMem, expected, desired ) == expected;
}

static inline bool cas32( atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order )
{
    return vcas32( pMem, expected, desired, success_order, failure_order ) == expected;
}

//////////////////////////////////////////////////////////////////////////
// xchg32
//
// Itanium xchg is performed with acquire semantics
//

#       define FX_xchg32_defined

template <typename ORDER>
static inline atomic32_t xchg32( atomic32_t volatile * pMem, atomic32_t val )
{
    atomic32_t current;
    asm volatile (
            "xchg4 %[current] = [%[pMem]], %[val]\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}
template <>
inline atomic32_t xchg32<membar_release>( atomic32_t volatile * pMem, atomic32_t val )
{
    atomic32_t current;
    asm volatile (
            "mf;;\n\t"
            "xchg4 %[current] = [%[pMem]], %[val]   \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}

template <>
inline atomic32_t xchg32<membar_acq_rel>( atomic32_t volatile * pMem, atomic32_t val )
{
    return xchg32<membar_release>( pMem, val );
}

template <>
inline atomic32_t xchg32<membar_seq_cst>( atomic32_t volatile * pMem, atomic32_t val )
{
    atomic32_t current;
    asm volatile (
            "mf;;\n\t"
            "xchg4 %[current] = [%[pMem]], %[val]   \n\t"
            "mf \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}

static inline atomic32_t xchg32( atomic32_t volatile * pMem, atomic32_t val, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        return xchg32<membar_relaxed>( pMem, val );
    case membar_consume::order:
        return xchg32<membar_consume>( pMem, val );
    case membar_acquire::order:
        return xchg32<membar_acquire>( pMem, val );
    case membar_release::order:
        return xchg32<membar_release>( pMem, val );
    case membar_acq_rel::order:
        return xchg32<membar_acq_rel>( pMem, val );
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return xchg32<membar_seq_cst>( pMem, val );
    }
}

//////////////////////////////////////////////////////////////////////////
// inc32

#       define FX_inc32_defined
template <typename ORDER>
static inline atomic32_t inc32( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "fetchadd4.acq %[ret] = [%[pMem]], 1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic32_t inc32<membar_release>( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "fetchadd4.rel %[ret] = [%[pMem]], 1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic32_t inc32<membar_acq_rel>( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "fetchadd4.rel %[ret] = [%[pMem]], 1;;\n\t"
            "mf \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic32_t inc32<membar_seq_cst>( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "mf;;  \n\t"
            "fetchadd4.acq %[ret] = [%[pMem]], 1    \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}

static inline atomic32_t inc32( atomic32_t volatile * pMem, memory_order order )
{
    atomic32_t ret;
    switch (order) {
    case membar_relaxed::order:
        return inc32<membar_relaxed>( pMem ) ;
    case membar_consume::order:
        return inc32<membar_consume>( pMem ) ;
    case membar_acquire::order:
        return inc32<membar_acquire>( pMem ) ;
    case membar_release::order:
        return inc32<membar_release>( pMem ) ;
    case membar_acq_rel::order:
        return inc32<membar_acq_rel>( pMem ) ;
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return inc32<membar_seq_cst>( pMem ) ;
    }
    return (ret);
}

//////////////////////////////////////////////////////////////////////////
// dec32

#       define FX_dec32_defined
template <typename ORDER>
static inline atomic32_t dec32( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "fetchadd4.acq %[ret] = [%[pMem]], -1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic32_t dec32<membar_release>( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "fetchadd4.rel %[ret] = [%[pMem]], -1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic32_t dec32<membar_acq_rel>( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "fetchadd4.rel %[ret] = [%[pMem]], -1;;\n\t"
            "mf \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic32_t dec32<membar_seq_cst>( atomic32_t volatile * pMem )
{
    atomic32_t ret;
    asm volatile (
            "mf;;  \n\t"
            "fetchadd4.acq %[ret] = [%[pMem]], -1    \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}

static inline atomic32_t dec32( atomic32_t volatile * pMem, memory_order order )
{
    atomic32_t ret;
    switch (order) {
    case membar_relaxed::order:
        return dec32<membar_relaxed>( pMem ) ;
    case membar_consume::order:
        return dec32<membar_consume>( pMem ) ;
    case membar_acquire::order:
        return dec32<membar_acquire>( pMem ) ;
    case membar_release::order:
        return dec32<membar_release>( pMem ) ;
    case membar_acq_rel::order:
        return dec32<membar_acq_rel>( pMem ) ;
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return dec32<membar_seq_cst>( pMem ) ;
    }
    return (ret);
}



//
// 64bit primitives
//

//////////////////////////////////////////////////////////////////////////
// load64

template <typename ORDER>
static inline atomic64_t load64( atomic64_t volatile const * pMem );

template <>
inline atomic64_t load64<membar_relaxed>( atomic64_t volatile const * pMem )
{
    atomic64_t val;
    asm volatile (
            "ld8 %[val] = [%[pMem]]  \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory"
                  );
    return val;
}
/*
  template <>
  inline atomic64_t load64<membar_release>( atomic64_t volatile const * pMem )
  {
  // Load with release semantics?..
  //FX_STATIC_ASSERT( false );

  atomic64_t val;
  asm volatile (
  "mf;;  \n\t"
  "ld8 %[val] = [%[pMem]]  \n\t"
  : [val] "=r" (val)
  : [pMem] "r" (pMem)
  :
  );
  return val;
  }
*/
template <>
inline atomic64_t load64<membar_acquire>( atomic64_t volatile const * pMem )
{
    atomic64_t val;
    asm volatile (
            "ld8.acq %[val] = [%[pMem]] \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory"
                  );
    return val;
}
template <>
inline atomic64_t load64<membar_consume>( atomic64_t volatile const * pMem )
{
    return load64<membar_relaxed>( pMem );
}
/*
  template <>
  inline atomic64_t load64<membar_acq_rel>( atomic64_t volatile const * pMem )
  {
  // Load with acquire/release semantics?..
  //FX_STATIC_ASSERT( false );

  return load64<membar_acquire>( pMem );
  }
*/
template <>
inline atomic64_t load64<membar_seq_cst>( atomic64_t volatile const * pMem )
{
    atomic64_t val;
    asm volatile (
            "mf;;  \n\t"
            "ld8.acq %[val] = [%[pMem]] \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory"
                  );
    return val;
}

static inline atomic64_t load64( atomic64_t volatile const * pMem, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        return load64<membar_relaxed>( pMem );
        //case membar_release::order:
        //    return load64<membar_release>( pMem );
    case membar_consume::order:
        return load64<membar_consume>( pMem );
    case membar_acquire::order:
        return load64<membar_acquire>( pMem );
        //case membar_acq_rel::order:
        //    return load64<membar_acq_rel>( pMem );
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return load64<membar_seq_cst>( pMem );
    }
}

//////////////////////////////////////////////////////////////////////////
// store64

template <typename ORDER>
static inline void store64( atomic64_t volatile * pMem, atomic64_t val );

template <>
inline void store64<membar_relaxed>( atomic64_t volatile * pMem, atomic64_t val )
{
    asm volatile  (
            "st8 [%[pMem]] = %[val]  \n\t"
            :
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                   );
}
/*
  template <>
  inline void store64<membar_acquire>( atomic64_t volatile * pMem, atomic64_t val )
  {
  // Store with acquire semantics?..
  //FX_STATIC_ASSERT( false );

  asm volatile (
  "st8 [%[pMem]] = %[val];;  \n\t"
  "mf \n\t"
  :
  : [pMem] "r" (pMem), [val] "r" (val)
  : "memory"
  );
  }
  template <>
  inline void store64<membar_consume>( atomic64_t volatile * pMem, atomic64_t val )
  {
  // Store with consume semantics?..
  //FX_STATIC_ASSERT( false );

  store64<membar_acquire>( pMem, val ) ;
  }
*/

template <>
inline void store64<membar_release>( atomic64_t volatile * pMem, atomic64_t val )
{
    asm volatile (
            "st8.rel [%[pMem]] = %[val]  \n\t"
            :
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
}
/*
  template <>
  inline void store64<membar_acq_rel>( atomic64_t volatile * pMem, atomic64_t val )
  {
  store64<membar_release>( pMem, val ) ;
  }
*/
template <>
inline void store64<membar_seq_cst>( atomic64_t volatile * pMem, atomic64_t val )
{
    asm volatile (
            "st8.rel [%[pMem]] = %[val] \n\t"
            "mf     \n\t"
            :
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
}

static inline void store64( atomic64_t volatile * pMem, atomic64_t val, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        store64<membar_relaxed>( pMem, val ) ;
        break;
        //case membar_consume::order:
        //    store64<membar_consume>( pMem, val ) ;
        //    break;
        //case membar_acquire::order:
        //    store64<membar_acquire>( pMem, val ) ;
        //    break;
    case membar_release::order:
        store64<membar_release>( pMem, val ) ;
        break;
        //case membar_acq_rel::order:
        //    store64<membar_acq_rel>( pMem, val ) ;
        //    break;
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        store64<membar_seq_cst>( pMem, val ) ;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
// vcas64

template <typename SUCCESS_ORDER>
static inline atomic64_t vcas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired ) ;

template <>
inline atomic64_t vcas64<membar_acquire>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
{
    atomic64_t current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg8.acq %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}
template <>
inline atomic64_t vcas64<membar_relaxed>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
{
    return vcas64<membar_acquire>( pMem, expected, desired );
}
template <>
inline atomic64_t vcas64<membar_consume>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
{
    return vcas64<membar_acquire>( pMem, expected, desired );
}

template <>
inline atomic64_t vcas64<membar_release>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
{
    atomic64_t current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg8.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

template <>
inline atomic64_t vcas64<membar_acq_rel>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
{
    atomic64_t current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg8.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            "mf \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

template <>
inline atomic64_t vcas64<membar_seq_cst>( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
{
    atomic64_t current;
    asm volatile (
            "mf \n\t"
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg8.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            "mf \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

static inline atomic64_t vcas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order success_order, memory_order failure_order )
{
    switch (success_order) {
    case membar_relaxed::order:
        return vcas64<membar_relaxed>( pMem, expected, desired );
    case membar_consume::order:
        return vcas64<membar_consume>( pMem, expected, desired );
    case membar_acquire::order:
        return vcas64<membar_acquire>( pMem, expected, desired );
    case membar_release::order:
        return vcas64<membar_release>( pMem, expected, desired );
    case membar_acq_rel::order:
        return vcas64<membar_acq_rel>( pMem, expected, desired );
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return vcas64<membar_seq_cst>( pMem, expected, desired );
    }
}

//////////////////////////////////////////////////////////////////////////
// cas64

template <typename SUCCESS_ORDER>
static inline bool cas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired )
{
    return vcas64<SUCCESS_ORDER>( pMem, expected, desired ) == expected;
}

static inline bool cas64( atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order success_order, memory_order failure_order )
{
    return vcas64( pMem, expected, desired, success_order, failure_order ) == expected;
}

//////////////////////////////////////////////////////////////////////////
// xchg64
//
// Itanium xchg is performed with acquire semantics
//

#       define FX_xchg64_defined

template <typename ORDER>
static inline atomic64_t xchg64( atomic64_t volatile * pMem, atomic64_t val )
{
    atomic64_t current;
    asm volatile (
            "xchg8 %[current] = [%[pMem]], %[val]\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}
template <>
inline atomic64_t xchg64<membar_release>( atomic64_t volatile * pMem, atomic64_t val )
{
    atomic64_t current;
    asm volatile (
            "mf;;\n\t"
            "xchg8 %[current] = [%[pMem]], %[val]   \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}

template <>
inline atomic64_t xchg64<membar_acq_rel>( atomic64_t volatile * pMem, atomic64_t val )
{
    return xchg64<membar_release>( pMem, val );
}

template <>
inline atomic64_t xchg64<membar_seq_cst>( atomic64_t volatile * pMem, atomic64_t val )
{
    atomic64_t current;
    asm volatile (
            "mf;;\n\t"
            "xchg8 %[current] = [%[pMem]], %[val]   \n\t"
            "mf \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}

static inline atomic64_t xchg64( atomic64_t volatile * pMem, atomic64_t val, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        return xchg64<membar_relaxed>( pMem, val );
    case membar_consume::order:
        return xchg64<membar_consume>( pMem, val );
    case membar_acquire::order:
        return xchg64<membar_acquire>( pMem, val );
    case membar_release::order:
        return xchg64<membar_release>( pMem, val );
    case membar_acq_rel::order:
        return xchg64<membar_acq_rel>( pMem, val );
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return xchg64<membar_seq_cst>( pMem, val );
    }
}

//////////////////////////////////////////////////////////////////////////
// inc64

#       define FX_inc64_defined
template <typename ORDER>
static inline atomic64_t inc64( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "fetchadd8.acq %[ret] = [%[pMem]], 1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic64_t inc64<membar_release>( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "fetchadd8.rel %[ret] = [%[pMem]], 1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic64_t inc64<membar_acq_rel>( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "fetchadd8.rel %[ret] = [%[pMem]], 1;;\n\t"
            "mf \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic64_t inc64<membar_seq_cst>( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "mf;;  \n\t"
            "fetchadd8.acq %[ret] = [%[pMem]], 1    \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}

static inline atomic64_t inc64( atomic64_t volatile * pMem, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        return inc64<membar_relaxed>( pMem ) ;
    case membar_consume::order:
        return inc64<membar_consume>( pMem ) ;
    case membar_acquire::order:
        return inc64<membar_acquire>( pMem ) ;
    case membar_release::order:
        return inc64<membar_release>( pMem ) ;
    case membar_acq_rel::order:
        return inc64<membar_acq_rel>( pMem ) ;
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return inc64<membar_seq_cst>( pMem ) ;
    }
}

//////////////////////////////////////////////////////////////////////////
// dec64

#       define FX_dec64_defined
template <typename ORDER>
static inline atomic64_t dec64( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "fetchadd8.acq %[ret] = [%[pMem]], -1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic64_t dec64<membar_release>( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "fetchadd8.rel %[ret] = [%[pMem]], -1\n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic64_t dec64<membar_acq_rel>( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "fetchadd8.rel %[ret] = [%[pMem]], -1;;\n\t"
            "mf \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}
template <>
inline atomic64_t dec64<membar_seq_cst>( atomic64_t volatile * pMem )
{
    atomic64_t ret;
    asm volatile (
            "mf;;  \n\t"
            "fetchadd8.acq %[ret] = [%[pMem]], -1    \n\t"
            : [ret] "=r" (ret)
            : [pMem] "r" (pMem)
                  );
    return ret;
}

static inline atomic64_t dec64( atomic64_t volatile * pMem, memory_order order )
{
    switch (order) {
    case membar_relaxed::order:
        return dec64<membar_relaxed>( pMem ) ;
    case membar_consume::order:
        return dec64<membar_consume>( pMem ) ;
    case membar_acquire::order:
        return dec64<membar_acquire>( pMem ) ;
    case membar_release::order:
        return dec64<membar_release>( pMem ) ;
    case membar_acq_rel::order:
        return dec64<membar_acq_rel>( pMem ) ;
    default:
        assert( false );   // Are we forgot anything?..
    case membar_seq_cst::order:
        return dec64<membar_seq_cst>( pMem ) ;
    }
}


//
// Atomic pointer primitives
//
template <typename SUCCESS_ORDER>
static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
{
    return cas64<SUCCESS_ORDER>( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired );
}
static inline bool casptr( pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order success_order, memory_order failure_order )
{
    return cas64( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired, success_order, failure_order );
}

template <typename SUCCESS_ORDER>
static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired )
{
    return (pointer_t) vcas64<SUCCESS_ORDER>( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired );
}
static inline pointer_t vcasptr(  pointer_t volatile * pMem, pointer_t expected, pointer_t desired, memory_order success_order, memory_order failure_order )
{
    return (pointer_t) vcas64( (atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired, success_order, failure_order );
}

#       define FX_xchgptr_defined
template <typename ORDER>
static inline pointer_t xchgptr( pointer_t volatile * pMem, pointer_t val )
{
    return (pointer_t) xchg64<ORDER>( (atomic64_t volatile *) pMem, *((atomic64_t *) &val) );
}
static inline pointer_t xchgptr( pointer_t volatile * pMem, pointer_t val, memory_order order )
{
    return (pointer_t) xchg64( (atomic64_t volatile *) pMem, *((atomic64_t *) &val), order );
}

template <typename ORDER>
static inline pointer_t loadptr( pointer_t volatile const * pMem )
{
    return (pointer_t) load64<ORDER>( (atomic64_t volatile const *) pMem );
}
static inline pointer_t loadptr( pointer_t volatile const * pMem, memory_order order )
{
    return (pointer_t) load64( (atomic64_t volatile const *) pMem, order );
}

template <typename ORDER>
static inline void storeptr( pointer_t volatile * pMem, pointer_t val )
{
    store64<ORDER>( (atomic64_t volatile *) pMem, *((atomic64_t *) &val) );
}
static inline void storeptr( pointer_t volatile * pMem, pointer_t val, memory_order order )
{
    store64( (atomic64_t volatile *) pMem, *((atomic64_t *) &val), order );
}


//
// Spin-lock primitives
//
#        define FX_spinlock_defined
typedef unsigned char    spinlock_t;
static inline bool spinlock_try_acquire( spinlock_t volatile * pDest )
{
    spinlock_t nCur;
    asm volatile ("mov ar.ccv = r0;;\n\t"
                  "cmpxchg1.acq %[curVal] = [%[dest]], %[one] , ar.ccv    \n\t"
                  : [curVal] "=r" (nCur)
                  : [dest] "r" (pDest), [one] "r" (1)
                  : "memory", "ar.ccv"
                  );
    return nCur == 0;
}
static inline void spinlock_release( spinlock_t volatile * pDest )
{
    asm volatile (
            "st1.rel [%[dest]]=r0"
            :
            : [dest] "r" (pDest)
            : "memory"
                  );
}


//////////////////////////////////////////////////////////////////////////
// Atomic bool
#       define FX_atomic_bool_defined

template <typename ORDER>
static inline bool load_bool( bool volatile const * pBool );
template <>
inline bool load_bool<membar_relaxed>( bool volatile const * pBool )
{
    bool val;
    asm volatile (
            "ld1 %[val] = [%[pMem]]  \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pBool)
            : "memory"
                  );
    return val;
}
template <>
inline bool load_bool<membar_acquire>( bool volatile const * pBool )
{
    bool val;
    asm volatile (
            "ld1.acq %[val] = [%[pMem]];;  \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pBool)
            : "memory"
                  );
    return val;
}
template <>
inline bool load_bool<membar_consume>( bool volatile const * pBool )
{
    return load_bool<membar_acquire>( pBool );
}
/*
  template <>
  inline bool load_bool<membar_release>( bool volatile const * pBool )
  {
  // Load with release semantics?..
  //FX_STATIC_ASSERT( false );

  bool val;
  asm volatile (
  "mf;;  \n\t"
  "ld1 %[val] = [%[pMem]] \n\t"
  : [val] "=r" (val)
  : [pMem] "r" (pBool)
  : "memory"
  );
  return val;
  }
  template <>
  inline bool load_bool<membar_acq_rel>( bool volatile const * pBool )
  {
  // Load with acquire/release semantics?..
  //FX_STATIC_ASSERT( false );

  bool val;
  asm volatile (
  "mf                           \n\t"
  "ld1.acq %[val] = [%[pMem]];;\n\t"
  : [val] "=r" (val)
  : [pMem] "r" (pBool)
  : "memory"
  );
  return val;
  }
*/
template <>
inline bool load_bool<membar_seq_cst>( bool volatile const * pBool )
{
    bool val;
    asm volatile (
            "mf;;  \n\t"
            "ld1 %[val] = [%[pMem]];;\n\t"
            "mf                       \n\t"
            : [val] "=r" (val)
            : [pMem] "r" (pBool)
            : "memory"
                  );
    return val;
}

template <typename ORDER>
static inline void store_bool( bool volatile * pBool, bool val ) ;
template <>
inline void store_bool<membar_relaxed>( bool volatile * pBool, bool val )
{
    asm volatile (
            "st1 [%[pMem]] = %[val]  \n\t"
            :
            : [val] "r" (val), [pMem] "r" (pBool)
            : "memory"
                  );
}
/*
  template <>
  inline void store_bool<membar_acquire>( bool volatile * pBool, bool val )
  {
  // Store with acquire semantics?..
  //FX_STATIC_ASSERT( false );

  asm volatile (
  "st1 [%[pMem]] = %[val] ;;\n\t"
  "mf  \n\t"
  :
  : [val] "r" (val), [pMem] "r" (pBool)
  : "memory"
  );
  }
  template <>
  inline void store_bool<membar_consume>( bool volatile * pBool, bool val )
  {
  // Store with consume semantics?..
  //FX_STATIC_ASSERT( false );

  store_bool<membar_acquire>( pBool, val ) ;
  }
*/
template <>
inline void store_bool<membar_release>( bool volatile * pBool, bool val )
{
    asm volatile (
            "st1.rel [%[pMem]] = %[val]  \n\t"
            :
            : [val] "r" (val), [pMem] "r" (pBool)
            : "memory"
                  );
}
/*
  template <>
  inline void store_bool<membar_acq_rel>( bool volatile * pBool, bool val )
  {
  // Store with acquire/release semantics?..
  //FX_STATIC_ASSERT( false );

  asm volatile (
  "st1.rel [%[pMem]] = %[val]  \n\t"
  "mf;; \n\t"
  :
  : [val] "r" (val), [pMem] "r" (pBool)
  : "memory"
  );
  }
*/
template <>
inline void store_bool<membar_seq_cst>( bool volatile * pBool, bool val )
{
    asm volatile (
            "mf;;  \n\t"
            "st1 [%[pMem]] = %[val];;\n\t"
            "mf                       \n\t"
            :
            : [val] "r" (val), [pMem] "r" (pBool)
            : "memory"
                  );
}

template <typename ORDER>
static inline bool xchg_bool( bool volatile * pMem, bool val )
{
    // Itanium xchg is performed with acquire semantics

    bool current;
    asm volatile (
            "xchg1 %[current] = [%[pMem]], %[val]\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}
template <>
inline bool xchg_bool<membar_release>( bool volatile * pMem, bool val )
{
    bool current;
    asm volatile (
            "mf;;\n\t"
            "xchg1 %[current] = [%[pMem]], %[val]   \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}

template <>
inline bool xchg_bool<membar_acq_rel>( bool volatile * pMem, bool val )
{
    return xchg_bool<membar_release>( pMem, val );
}

template <>
inline bool xchg_bool<membar_seq_cst>( bool volatile * pMem, bool val )
{
    bool current;
    asm volatile (
            "mf;;\n\t"
            "xchg1 %[current] = [%[pMem]], %[val]   \n\t"
            "mf \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [val] "r" (val)
            : "memory"
                  );
    return current;
}

template <typename SUCCESS_ORDER>
static inline bool vcas_bool( bool volatile * pMem, bool expected, bool desired ) ;
template <>
inline bool vcas_bool<membar_acquire>( bool volatile * pMem, bool expected, bool desired )
{
    bool current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg1.acq %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}
template <>
inline bool vcas_bool<membar_relaxed>( bool volatile * pMem, bool expected, bool desired )
{
    return vcas_bool<membar_acquire>( pMem, expected, desired );
}
template <>
inline bool vcas_bool<membar_consume>( bool volatile * pMem, bool expected, bool desired )
{
    return vcas_bool<membar_acquire>( pMem, expected, desired );
}

template <>
inline bool vcas_bool<membar_release>( bool volatile * pMem, bool expected, bool desired )
{
    bool current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg1.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

template <>
inline bool vcas_bool<membar_acq_rel>( bool volatile * pMem, bool expected, bool desired )
{
    bool current;
    asm volatile (
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg1.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            "mf;; \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

template <>
inline bool vcas_bool<membar_seq_cst>( bool volatile * pMem, bool expected, bool desired )
{
    bool current;
    asm volatile (
            "mf \n\t"
            "mov ar.ccv = %[expected];;\n\t"
            "cmpxchg1.rel %[current] = [%[pMem]], %[desired], ar.ccv\n\t"
            "mf;; \n\t"
            : [current] "=r" (current)
            : [pMem] "r" (pMem), [expected] "r" (expected), [desired] "r" (desired)
            : "ar.ccv", "memory"
                  );
    return current;
}

class atomic_bool
{
public:
    atomic_bool()
        : m_atomic( false )
    {}

    explicit atomic_bool( bool bVal )
        : m_atomic( bVal )
    {}

    bool    load( memory_order order ) const volatile
    {
        bool val;
        switch (order) {
        case membar_relaxed::order:
            return load_bool<membar_relaxed>( &m_atomic );
        case membar_consume::order:
            return load_bool<membar_consume>( &m_atomic );
        case membar_acquire::order:
            return load_bool<membar_acquire>( &m_atomic );
            //case membar_release::order:
            //    return load_bool<membar_release>( &m_atomic );
            //case membar_acq_rel::order:
            //    return load_bool<membar_acq_rel>( &m_atomic );
        default:
            assert( false );   // Are we forgot anything?..
        case membar_seq_cst::order:
            return load_bool<membar_seq_cst>( &m_atomic );
        }
        return val;
    }

    template <typename ORDER>
    bool    load() const volatile
    {
        return load_bool<ORDER>( &m_atomic );
    }

    void store( bool val, memory_order order ) volatile
    {
        bool volatile * pThis = &m_atomic;
        switch (order) {
        case membar_relaxed::order:
            store_bool<membar_relaxed>( &m_atomic, val );
            break;
            //case membar_consume::order:
            //    store_bool<membar_consume>( &m_atomic, val );
            //    break;
            //case membar_acquire::order:
            //    store_bool<membar_acquire>( &m_atomic, val );
            //    break;
        case membar_release::order:
            store_bool<membar_release>( &m_atomic, val );
            break;
            //case membar_acq_rel::order:
            //    store_bool<membar_acq_rel>( &m_atomic, val );
            //    break;
        default:
            assert( false );   // Are we forgot anything?..
        case membar_seq_cst::order:
            store_bool<membar_seq_cst>( &m_atomic, val );
            break;
        }
    }

    template <typename ORDER>
    void store( bool val ) volatile
    {
        store_bool<ORDER>( &m_atomic, val );
    }

    bool exchange(bool val, memory_order order ) volatile
    {
        switch (order) {
        case membar_relaxed::order:
            return xchg_bool<membar_relaxed>(&m_atomic, val );
        case membar_consume::order:
            return xchg_bool<membar_consume>(&m_atomic, val );
        case membar_acquire::order:
            return xchg_bool<membar_acquire>(&m_atomic, val );
        case membar_release::order:
            return xchg_bool<membar_release>(&m_atomic, val );
        case membar_acq_rel::order:
            return xchg_bool<membar_acq_rel>(&m_atomic, val );
        default:
            assert( false );   // Are we forgot anything?..
        case membar_seq_cst::order:
            return xchg_bool<membar_seq_cst>(&m_atomic, val );
        }
    }

    template <typename ORDER>
    bool exchange(bool val ) volatile
    {
        return xchg_bool<ORDER>(&m_atomic, val);
    }

    bool vcas( bool expected, bool desired, memory_order success_order, memory_order failure_order ) volatile
    {
        switch (success_order) {
        case membar_relaxed::order:
            return vcas_bool<membar_relaxed>( &m_atomic, expected, desired );
        case membar_consume::order:
            return vcas_bool<membar_consume>( &m_atomic, expected, desired );
        case membar_acquire::order:
            return vcas_bool<membar_acquire>( &m_atomic, expected, desired );
        case membar_release::order:
            return vcas_bool<membar_release>( &m_atomic, expected, desired );
        case membar_acq_rel::order:
            return vcas_bool<membar_acq_rel>( &m_atomic, expected, desired );
        default:
            assert( false );   // Are we forgot anything?..
        case membar_seq_cst::order:
            return vcas_bool<membar_seq_cst>( &m_atomic, expected, desired );
        }
    }

    template <typename SUCCESS_ORDER>
    bool vcas( bool expected, bool desired ) volatile
    {
        return vcas_bool<SUCCESS_ORDER>( &m_atomic, expected, desired );
    }

    bool cas( bool expected, bool desired, memory_order success_order, memory_order failure_order ) volatile
    {
        return vcas( expected, desired, success_order, failure_order ) == expected;
    }

    template <typename SUCCESS_ORDER>
    bool cas( bool expected, bool desired ) volatile
    {
        return vcas<SUCCESS_ORDER>( expected, desired ) == expected;
    }

private:
    bool volatile m_atomic;
};

} //end namespace atomic


FX_NS_END

#endif // #ifndef __FX_ATOMIC_GCC_IA64_H
