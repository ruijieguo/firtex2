#ifndef __FX_ATOMIC_GCC_PPC64_H
#define __FX_ATOMIC_GCC_PPC64_H

// For atomic operations the template-based alternatives are defined
#define CDS_ATOMIC_TEMPLATE_DEFINED 1

//@cond none
FX_NS_DEF(thread);

namespace atomic 
{

/**
 * PowerPC 64bit mode atomic primitives.
 * Reference:
 * [1] PowerPC Virtual Environment Architecture, Book II, Version 2.02. IBM
 * [2] Paul E. McKenney, Raul Silvera Example POWER Implementation for C/C++ Memory Model,
 *   ISO/IEC JTC1 SC22 WG21 N2745 = 08-0255 - 2008-08-22 (Revised 2009.02)
 * http://www.rdrop.com/users/paulmck/scalability/paper/N2745r.2009.02.22a.html
 * [3] PowerPC storage model and AIX programming, http://www.ibm.com/developerworks/systems/articles/powerpc.html
 * [4] Linux kernel source
 */

//////////////////////////////////////////////////////////////////////////
// fence
template <typename ORDER>
static inline void fence();
template <>

inline void fence<membar_relaxed>()
{}

template <>
inline void fence<membar_consume>()
{}

template <>
inline void fence<membar_acq_rel>()
{
    asm volatile (
            "lwsync \n"    // sync 1     // see [3] "Memory barrier instructions", "Storage barriers and ordering of storage accesses" table
            : : : "memory"
                );
}

template <>
inline void fence<membar_release>()
{
    fence<membar_acq_rel>();
}

template <>
inline void fence<membar_acquire>()
{
    fence<membar_acq_rel>();
}

template <>
inline void fence<membar_seq_cst>()
{
    asm volatile (
            "sync \n"    // sync 0         // see [3] "Memory barrier instructions", "Storage barriers and ordering of storage accesses" table
            : : : "memory"
                );
}

static inline void fence(memory_order order)
{
    switch(order) {
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
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        fence<membar_seq_cst>();
        break;
    }
}

//
// 32bit primitives
//

//////////////////////////////////////////////////////////////////////////
// load32
template <typename ORDER>
static inline atomic32_t load32(atomic32_t volatile const * pMem)
{ return *pMem; }

template <>
inline atomic32_t load32<membar_relaxed>(atomic32_t volatile const * pMem)
{
    CDS_COMPILER_RW_BARRIER;
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
    atomic32_t val;
    asm volatile (                 // from [2]
            "lwzx %[val], 0, %[pMem]\n"
            "cds_ld32_acq: cmpw %[val], %[val]\n"
            "bne- cds_ld32_acq\n" // branch never taken, for isync
            "isync\n"
            : [val] "=b" (val)
            : [pMem] "b" (pMem)
            : "memory", "cc"
                                  );
    return val;
}

template <>
inline atomic32_t load32<membar_seq_cst>(atomic32_t volatile const * pMem)
{
    atomic32_t val;
    asm volatile (             // see [2]
            "sync                       \n"
            "lwzx %[val], 0, %[pMem]    \n"
            "cds_ld32_seq: cmpw %[val], %[val]  \n"
            "bne- cds_ld32_seq     \n" // branch never taken, for isync
            "isync \n"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory", "cr0"
                              );
    return val;
}

static inline atomic32_t load32(atomic32_t volatile const * pMem, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        return load32<membar_relaxed>(pMem);
        //case membar_release::order:
        //    return load32<membar_release>(pMem);
    case membar_consume::order:
        return load32<membar_consume>(pMem);
    case membar_acquire::order:
        return load32<membar_acquire>(pMem);
        //case membar_acq_rel::order:
        //    return load32<membar_acq_rel>(pMem);
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        return load32<membar_seq_cst>(pMem);
    }
}

//////////////////////////////////////////////////////////////////////////
// store32
template <typename ORDER>
static inline void store32(atomic32_t volatile * pMem, atomic32_t val);

template <>
inline void store32<membar_relaxed>(atomic32_t volatile * pMem, atomic32_t val)
{
    CDS_COMPILER_RW_BARRIER;
    *pMem = val;
}

template <>
inline void store32<membar_release>(atomic32_t volatile * pMem, atomic32_t val)
{
    asm volatile (     // from [2]
            "lwsync \n\t"
            "stwx %[val], 0, %[pMem]  \n"
            : 
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory"
                      );
}

template <>
inline void store32<membar_seq_cst>(atomic32_t volatile * pMem, atomic32_t val)
{
    asm volatile (     // from [2]
            "sync \n"
            "stwx %[val], 0, %[pMem]  \n"
            : 
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory"
                      );
}

static inline void store32(atomic32_t volatile * pMem, atomic32_t val, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        store32<membar_relaxed>(pMem, val);
        break;
        //case membar_consume::order:
        //    store32<membar_consume>(pMem, val);
        //    break;
        //case membar_acquire::order:
        //    store32<membar_acquire>(pMem, val);
        //    break;
    case membar_release::order:
        store32<membar_release>(pMem, val);
        break;
        //case membar_acq_rel::order:
        //    store32<membar_acq_rel>(pMem, val);
        //    break;
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        store32<membar_seq_cst>(pMem, val);
        break;
    }
}


//////////////////////////////////////////////////////////////////////////
// vcas32

template <typename SUCCESS_ORDER>
static inline atomic32_t vcas32(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{return 0;}


template <>
inline atomic32_t vcas32<membar_relaxed>(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
    atomic32_t current;
    asm volatile ( // from [2], [1]
            "cds_vcas32_rlx_1: lwarx %[current], 0, %[pMem]   \n"
            "cmpw  %[current], %[expected]  \n"
            "bne-  cds_vcas32_rlx_2 \n"
            "stwcx. %[desired], 0, %[pMem]    \n"
            "bne-  cds_vcas32_rlx_1 \n"
            "cds_vcas32_rlx_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic32_t vcas32<membar_consume>(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
    return vcas32<membar_relaxed>(pMem, expected, desired);
}

template <>
inline atomic32_t vcas32<membar_acquire>(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "cds_vcas32_acq_1: lwarx %[current], 0, %[pMem]   \n\t"
            "cmpw %[current], %[expected]  \n\t"
            "bne-  cds_vcas32_acq_2 \n\t"
            "stwcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas32_acq_1 \n\t"
            "isync  \n\t"
            "cds_vcas32_acq_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic32_t vcas32<membar_release>(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_vcas32_rel_1: lwarx %[current], 0, %[pMem]   \n\t"
            "cmpw %[current], %[expected]  \n\t"
            "bne-  cds_vcas32_rel_2 \n\t"
            "stwcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas32_rel_1 \n\t"
            "cds_vcas32_rel_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic32_t vcas32<membar_acq_rel>(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_vcas32_aqr_1: lwarx %[current], 0, %[pMem]   \n\t"
            "cmpw  %[current], %[expected]  \n\t"
            "bne-  cds_vcas32_aqr_2 \n\t"
            "stwcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas32_aqr_1 \n\t"
            "isync  \n\t"
            "cds_vcas32_aqr_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic32_t vcas32<membar_seq_cst>(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "sync     \n\t"
            "cds_vcas32_seq_1: lwarx %[current], 0, %[pMem]   \n\t"
            "cmpw  %[current], %[expected]  \n\t"
            "bne-  cds_vcas32_seq_2 \n\t"
            "stwcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas32_seq_1 \n\t"
            "isync  \n\t"
            "cds_vcas32_seq_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

static inline atomic32_t vcas32(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order)
{
    switch (success_order) {
    case membar_relaxed::order:
        return vcas32<membar_relaxed>(pMem, expected, desired);
    case membar_consume::order:
        return vcas32<membar_consume>(pMem, expected, desired);
    case membar_acquire::order:
        return vcas32<membar_acquire>(pMem, expected, desired);
    case membar_release::order:
        return vcas32<membar_release>(pMem, expected, desired);
    case membar_acq_rel::order:
        return vcas32<membar_acq_rel>(pMem, expected, desired);
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        return vcas32<membar_seq_cst>(pMem, expected, desired);
    }
}

//////////////////////////////////////////////////////////////////////////
// cas32

template <typename SUCCESS_ORDER>
static inline bool cas32(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired)
{
    return vcas32<SUCCESS_ORDER>(pMem, expected, desired) == expected;
}

static inline bool cas32(atomic32_t volatile * pMem, atomic32_t expected, atomic32_t desired, memory_order success_order, memory_order failure_order)
{
    return vcas32(pMem, expected, desired, success_order, failure_order) == expected;
}

//////////////////////////////////////////////////////////////////////////
// xchg32

#       define CDS_xchg32_defined
template <typename ORDER>
static inline atomic32_t xchg32(atomic32_t volatile * pMem, atomic32_t val)
{return 0;}

template <>
inline atomic32_t xchg32<membar_relaxed>(atomic32_t volatile * pMem, atomic32_t val)
{
    atomic32_t current;
    asm volatile ( // from [2], [1]
            "cds_xchg32_rlx: lwarx %[current], 0, %[pMem]   \n\t"
            "stwcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg32_rlx \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic32_t xchg32<membar_consume>(atomic32_t volatile * pMem, atomic32_t val)
{
    return xchg32<membar_relaxed>(pMem, val);
}
template <>
inline atomic32_t xchg32<membar_acquire>(atomic32_t volatile * pMem, atomic32_t val)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "cds_xchg32_acq: lwarx %[current], 0, %[pMem]   \n\t"
            "stwcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg32_acq \n\t"
            "isync  \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic32_t xchg32<membar_release>(atomic32_t volatile * pMem, atomic32_t val)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_xchg32_rel: lwarx %[current], 0, %[pMem]   \n\t"
            "stwcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg32_rel \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic32_t xchg32<membar_acq_rel>(atomic32_t volatile * pMem, atomic32_t val)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_xchg32_aqr: lwarx %[current], 0, %[pMem]   \n\t"
            "stwcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg32_aqr \n\t"
            "isync  \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic32_t xchg32<membar_seq_cst>(atomic32_t volatile * pMem, atomic32_t val)
{
    atomic32_t current;
    asm volatile ( // from [2]
            "sync     \n\t"
            "cds_xchg32_seq: lwarx %[current], 0, %[pMem]   \n\t"
            "stwcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg32_seq \n\t"
            "isync  \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}

static inline atomic32_t xchg32(atomic32_t volatile * pMem, atomic32_t val, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        return xchg32<membar_relaxed>(pMem, order);
    case membar_consume::order:
        return xchg32<membar_consume>(pMem, order);
    case membar_acquire::order:
        return xchg32<membar_acquire>(pMem, order);
    case membar_release::order:
        return xchg32<membar_release>(pMem, order);
    case membar_acq_rel::order:
        return xchg32<membar_acq_rel>(pMem, order);
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        return xchg32<membar_seq_cst>(pMem, order);
    }
}




//
// 64bit primitives
//

//////////////////////////////////////////////////////////////////////////
// load64

template <typename ORDER>
static inline atomic64_t load64(atomic64_t volatile const * pMem);

template <>
inline atomic64_t load64<membar_relaxed>(atomic64_t volatile const * pMem)
{
    CDS_COMPILER_RW_BARRIER;
    return *pMem;
}
template <>
inline atomic64_t load64<membar_consume>(atomic64_t volatile const * pMem)
{
    return load64<membar_relaxed>(pMem);
}

template <>
inline atomic64_t load64<membar_acquire>(atomic64_t volatile const * pMem)
{
    atomic64_t val;
    asm volatile (                 // from [2]
            "ldx %[val], 0, %[pMem] \n"
            "cds_ld64_acq: cmpd %[val], %[val]    \n"
            "bne- cds_ld64_acq                \n" // branch never taken, for isync
            "isync \n"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory", "cr0"
                                  );
    return val;
}
/*
  template <>
  inline atomic64_t load64<membar_release>(atomic64_t volatile const * pMem)
  {
  // Load with release semantics?..
  //CDS_STATIC_ASSERT(false);

  atomic64_t val;
  asm volatile (
  "sync                   \n"
  "ldx %[val], 0, %[pMem] \n"
  : [val] "=r" (val)
  : [pMem] "r" (pMem)
  : "memory"
);
  return val;
  }
*/
template <>
inline atomic64_t load64<membar_seq_cst>(atomic64_t volatile const * pMem)
{
    atomic64_t val;
    asm volatile (             // see [2]
            "sync                   \n"
            "ldx %[val], 0, %[pMem] \n"
            "cds_ld64_seq: cmpd %[val], %[val]    \n"
            "bne- cds_ld64_seq                \n" // branch never taken, for isync
            "isync \n"
            : [val] "=r" (val)
            : [pMem] "r" (pMem)
            : "memory", "cr0"
                              );
    return val;
}
/*
  template <>
  inline atomic64_t load64<membar_acq_rel>(atomic64_t volatile const * pMem)
  {
  // Load with acquire/release semantics?..
  //CDS_STATIC_ASSERT(false);

  return load64<membar_seq_cst>(pMem);
  }
*/
static inline atomic64_t load64(atomic64_t volatile const * pMem, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        return load64<membar_relaxed>(pMem);
        //case membar_release::order:
        //    return load64<membar_release>(pMem);
    case membar_consume::order:
        return load64<membar_consume>(pMem);
    case membar_acquire::order:
        return load64<membar_acquire>(pMem);
        //case membar_acq_rel::order:
        //    return load64<membar_acq_rel>(pMem);
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        return load64<membar_seq_cst>(pMem);
    }
}

//////////////////////////////////////////////////////////////////////////
// store64
template <typename ORDER>
static inline void store64(atomic64_t volatile * pMem, atomic64_t val);

template <>
inline void store64<membar_relaxed>(atomic64_t volatile * pMem, atomic64_t val)
{
    CDS_COMPILER_RW_BARRIER;
    *pMem = val;
}
/*
  template <>
  inline void store64<membar_acquire>(atomic64_t volatile * pMem, atomic64_t val)
  {
  // Store with acquire semantics?..
  //CDS_STATIC_ASSERT(false);

  asm volatile (
  "stdx %[val], 0, %[pMem]  \n"
  "sync \n"
  : [pMem] "=m" (pMem)
  : [val] "r" (val)
  : "memory"
);
  }

  template <>
  inline void store64<membar_consume>(atomic64_t volatile * pMem, atomic64_t val)
  {
  // Store with consume semantics?..
  //CDS_STATIC_ASSERT(false);

  store64<membar_acquire>(pMem, val);
  }
*/

template <>
inline void store64<membar_release>(atomic64_t volatile * pMem, atomic64_t val)
{
    asm volatile (     // from [2]
            "lwsync \n\t"
            "stdx %[val], 0, %[pMem]  \n"
            : 
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory"
                      );
}

template <>
inline void store64<membar_seq_cst>(atomic64_t volatile * pMem, atomic64_t val)
{
    asm volatile (     // from [2]
            "sync \n\t"
            "stdx %[val], 0, %[pMem]  \n"
            : 
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory"
                      );
}
/*
  template <>
  inline void store64<membar_acq_rel>(atomic64_t volatile * pMem, atomic64_t val)
  {
  // Store with acquire/release semantics?..
  //CDS_STATIC_ASSERT(false);

  store64<membar_seq_cst>(pMem, val);
  }
*/

static inline void store64(atomic64_t volatile * pMem, atomic64_t val, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        store64<membar_relaxed>(pMem, val);
        break;
        //case membar_consume::order:
        //    store64<membar_consume>(pMem, val);
        //    break;
        //case membar_acquire::order:
        //    store64<membar_acquire>(pMem, val);
        //    break;
    case membar_release::order:
        store64<membar_release>(pMem, val);
        break;
        //case membar_acq_rel::order:
        //    store64<membar_acq_rel>(pMem, val);
        //    break;
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        store64<membar_seq_cst>(pMem, val);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
// vcas64

template <typename SUCCESS_ORDER>
static inline atomic64_t vcas64(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{ return 0; }

template <>
inline atomic64_t vcas64<membar_relaxed>(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{
    atomic64_t current;
    asm volatile ( // from [2], [1]
            "cds_vcas64_rlx_1: ldarx %[current], 0, %[pMem]   \n\t"
            "cmpd  %[current], %[expected]  \n\t"
            "bne-  cds_vcas64_rlx_2 \n\t"
            "stdcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas64_rlx_1 \n\t"
            "cds_vcas64_rlx_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic64_t vcas64<membar_consume>(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{
    return vcas64<membar_relaxed>(pMem, expected, desired);
}

template <>
inline atomic64_t vcas64<membar_acquire>(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "cds_vcas64_acq_1: ldarx %[current], 0, %[pMem]   \n\t"
            "cmpd  %[current], %[expected]  \n\t"
            "bne-  cds_vcas64_acq_2 \n\t"
            "stdcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas64_acq_1 \n\t"
            "isync  \n\t"
            "cds_vcas64_acq_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic64_t vcas64<membar_release>(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_vcas64_rel_1: ldarx %[current], 0, %[pMem]   \n\t"
            "cmpd %[current], %[expected]  \n\t"
            "bne- cds_vcas64_rel_2 \n\t"
            "stdcx. %[desired], 0, %[pMem]    \n\t"
            "bne- cds_vcas64_rel_1 \n\t"
            "cds_vcas64_rel_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic64_t vcas64<membar_acq_rel>(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_vcas64_aqr_1: ldarx %[current], 0, %[pMem]   \n\t"
            "cmpd  %[current], %[expected]  \n\t"
            "bne-  cds_vcas64_aqr_2 \n\t"
            "stdcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas64_aqr_1 \n\t"
            "isync  \n\t"
            "cds_vcas64_aqr_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}

template <>
inline atomic64_t vcas64<membar_seq_cst>(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "sync     \n\t"
            "cds_vcas64_seq_1: ldarx %[current], 0, %[pMem]   \n\t"
            "cmpd  %[current], %[expected]  \n\t"
            "bne-  cds_vcas64_seq_2 \n\t"
            "stdcx. %[desired], 0, %[pMem]    \n\t"
            "bne-  cds_vcas64_seq_1 \n\t"
            "isync  \n\t"
            "cds_vcas64_seq_2: \n"
            : [current] "=r" (current)
            : [desired] "r" (desired), [pMem] "r" (pMem), [expected] "r" (expected)
            : "memory", "cr0"
                  );
    return current;
}


static inline atomic64_t vcas64(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order success_order, memory_order failure_order)
{

    switch (success_order) {
    case membar_relaxed::order:
        return vcas64<membar_relaxed>(pMem, expected, desired);
    case membar_consume::order:
        return vcas64<membar_consume>(pMem, expected, desired);
    case membar_acquire::order:
        return vcas64<membar_acquire>(pMem, expected, desired);
    case membar_release::order:
        return vcas64<membar_release>(pMem, expected, desired);
    case membar_acq_rel::order:
        return vcas64<membar_acq_rel>(pMem, expected, desired);
    default:
        assert(false);   // Are we forgot anything?..
    case membar_seq_cst::order:
        return vcas64<membar_seq_cst>(pMem, expected, desired);
    }
}

//////////////////////////////////////////////////////////////////////////
// cas64

template <typename SUCCESS_ORDER>
static inline bool cas64(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired)
{
    return vcas64<SUCCESS_ORDER>(pMem, expected, desired) == expected;
}

static inline bool cas64(atomic64_t volatile * pMem, atomic64_t expected, atomic64_t desired, memory_order success_order, memory_order failure_order)
{
    return vcas64(pMem, expected, desired, success_order, failure_order) == expected;
}

//////////////////////////////////////////////////////////////////////////
// xchg64

#       define CDS_xchg64_defined
template <typename ORDER>
static inline atomic64_t xchg64(atomic64_t volatile * pMem, atomic64_t val)
{return 0; }


template <>
inline atomic64_t xchg64<membar_relaxed>(atomic64_t volatile * pMem, atomic64_t val)
{
    atomic64_t current;
    asm volatile ( // from [2], [1]
            "cds_xchg64_rlx: ldarx %[current], 0, %[pMem]   \n\t"
            "stdcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg64_rlx \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic64_t xchg64<membar_consume>(atomic64_t volatile * pMem, atomic64_t val)
{
    return xchg64<membar_relaxed>(pMem, val);
}
template <>
inline atomic64_t xchg64<membar_acquire>(atomic64_t volatile * pMem, atomic64_t val)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "cds_xchg64_acq: ldarx %[current], 0, %[pMem]   \n\t"
            "stdcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg64_acq \n\t"
            "isync  \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic64_t xchg64<membar_release>(atomic64_t volatile * pMem, atomic64_t val)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_xchg64_rel: ldarx %[current], 0, %[pMem]   \n\t"
            "stdcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg64_rel \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic64_t xchg64<membar_acq_rel>(atomic64_t volatile * pMem, atomic64_t val)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "lwsync     \n\t"
            "cds_xchg64_aqr: ldarx %[current], 0, %[pMem]   \n\t"
            "stdcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg64_aqr \n\t"
            "isync  \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}
template <>
inline atomic64_t xchg64<membar_seq_cst>(atomic64_t volatile * pMem, atomic64_t val)
{
    atomic64_t current;
    asm volatile ( // from [2]
            "sync     \n\t"
            "cds_xchg64_seq: ldarx %[current], 0, %[pMem]   \n\t"
            "stdcx. %[val], 0, %[pMem]    \n\t"
            "bne-  cds_xchg64_seq \n\t"
            "isync  \n\t"
            : [current] "=r" (current)
            : [val] "r" (val), [pMem] "r" (pMem)
            : "memory", "cr0"
                  );
    return current;
}


static inline atomic64_t xchg64(atomic64_t volatile * pMem, atomic64_t val, memory_order order)
{
    switch (order) {
    case membar_relaxed::order:
        return xchg64<membar_relaxed>(pMem, order);
    case membar_consume::order:
        return xchg64<membar_consume>(pMem, order);
    case membar_acquire::order:
        return xchg64<membar_acquire>(pMem, order);
    case membar_release::order:
        return xchg64<membar_release>(pMem, order);
    case membar_acq_rel::order:
        return xchg64<membar_acq_rel>(pMem, order);
    case membar_seq_cst::order:
        return xchg64<membar_seq_cst>(pMem, order);
    default:
        assert(false);
    }
}


///////////////////////////////////////////////////////////////////////////
// Atomic pointer primitives
//
template <typename SUCCESS_ORDER>
static inline bool casptr(pointer_t volatile * pMem, pointer_t expected, pointer_t desired)
{
    return cas64<SUCCESS_ORDER>((atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired);
}

static inline bool casptr(pointer_t volatile * pMem, pointer_t expected,
                          pointer_t desired, memory_order success_order,
                          memory_order failure_order)
{
    return cas64((atomic64_t volatile *) pMem, (atomic64_t) expected, (atomic64_t) desired, success_order, failure_order);
}

template <typename SUCCESS_ORDER>
static inline pointer_t vcasptr( pointer_t volatile * pMem,
                                pointer_t expected, pointer_t desired)
{
    return (pointer_t) vcas64<SUCCESS_ORDER>((atomic64_t volatile *) pMem,
            (atomic64_t) expected, (atomic64_t) desired);
}

static inline pointer_t vcasptr( pointer_t volatile * pMem, pointer_t expected,
                                pointer_t desired, memory_order success_order,
                                memory_order failure_order)
{
    return (pointer_t) vcas64((atomic64_t volatile *) pMem, (atomic64_t) expected,
                              (atomic64_t) desired, success_order, failure_order);
}

template <typename ORDER>
static inline pointer_t loadptr(pointer_t volatile const * pMem)
{
    return (pointer_t) load64<ORDER>((atomic64_t volatile const *) pMem);
}

static inline pointer_t loadptr(pointer_t volatile const * pMem, memory_order order)
{
    return (pointer_t) load64((atomic64_t volatile const *) pMem, order);
}

template <typename ORDER>
static inline void storeptr(pointer_t volatile * pMem, pointer_t val)
{
    store64<ORDER>((atomic64_t volatile *) pMem, *((atomic64_t *) &val));
}

static inline void storeptr(pointer_t volatile * pMem, pointer_t val, memory_order order)
{
    store64((atomic64_t volatile *) pMem, *((atomic64_t *) &val), order);
}

} //end namespace atomic

FX_NS_END

#endif // #ifndef __FX_ATOMIC_GCC_PPC64_H
