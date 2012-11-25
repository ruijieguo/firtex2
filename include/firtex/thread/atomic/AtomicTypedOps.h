#ifndef __FX_ATOMIC_TYPED_OPS_H
#define __FX_ATOMIC_TYPED_OPS_H

#include "firtex/thread/atomic/AtomicOperations.h"
#include "firtex/thread/atomic/IsAligned.h"

FX_NS_DEF(thread);

namespace atomic 
{

template <typename ORDER, typename T, unsigned int SIZE>
struct TypedOps;

// 32bit atomic operations
template <typename ORDER, typename T>
struct TypedOps<ORDER, T, sizeof(atomic32_t)>
{
    typedef IntegralOperations<T, sizeof(atomic32_t)> StdOps;
    typedef typename StdOps::OperandType OperandType;
    typedef typename StdOps::AtomicType AtomicType;

    static const unsigned int OperandSize = StdOps::OperandSize;

    template <typename OTHER_ORDER>
    struct RebindOrder 
    {
        typedef TypedOps<OTHER_ORDER, T, sizeof(T)> other;
    };

    // Mandatory atomic primitives

    static inline T load(T volatile const * pAddr)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        AtomicType n = load32<ORDER>(reinterpret_cast< AtomicType volatile const *>(pAddr));
        return *(T *) &n;
#else
        return StdOps::load(pAddr, ORDER::order);
#endif
    }

    static inline T& load(T& dest, T volatile const * pAddr)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        *((AtomicType *) &dest) = load32<ORDER>(reinterpret_cast< AtomicType volatile const *>(pAddr));
        return dest;
#else
        return StdOps::load(dest, pAddr, ORDER::order);
#endif
    }

    static inline T store(T volatile * pAddr, const T val)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        store32<ORDER>(reinterpret_cast< AtomicType volatile *>(pAddr), atomic_cast<AtomicType>(val));
#else
        StdOps::store(pAddr, val, ORDER::order);
#endif
        return val;
    }

    static inline bool cas(T volatile * pDest, const T expected, const T desired)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        return cas32<ORDER>(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired)
                            );
#else
        return StdOps::cas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    static inline T vcas(T volatile * pDest, const T expected, const T desired)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType)) ;
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        return (T) vcas32<ORDER>(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired)
                                 );
#else
        return StdOps::vcas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T xchg(T volatile * pDest, const T val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_XCHG32_DEFINED
        // Processor has native xchg implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType)) ;
        return (T) xchg32<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, val));
        return cur;
#endif
#else
        return StdOps::xchg(pDest, val, ORDER::order);
#endif
    }

    static inline T xadd(T volatile * pDest, const T val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
#ifdef FX_XADD32_DEFINED
        // Processor has native xadd implementation
        return (T) xadd32<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, cur + val));
        return cur ;
#endif
#else
        return StdOps::xadd(pDest, val, ORDER::order);
#endif
    }

    static inline T inc(T volatile * pDest)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_INC32_DEFINED
        // Processor has native inc implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) inc32<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest));
#else
        return xadd(pDest, 1);
#endif
#else
        return StdOps::inc(pDest, ORDER::order);
#endif
    }

    static inline T dec(T volatile * pDest)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_DEC32_DEFINED
        // Processor has native dec implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) dec32<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest));
#else
        return xadd(pDest, T(0-1));
#endif
#else
        return StdOps::dec(pDest, ORDER::order);
#endif
    }

    static inline T bitwise_and(T volatile * pDest, const T val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_AND32_DEFINED
        // Processor has native xand implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType)) ;
        return (T) and32<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, cur & val));
        return cur ;
#endif
#else
        return StdOps::bitwise_and(pDest, val, ORDER::order);
#endif
    }

    static inline T bitwise_or(T volatile * pDest, const T val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_OR32_DEFINED
        // Processor has native bitwise_or implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) or32<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, cur | val));
        return cur ;
#endif
#else
        return StdOps::bitwise_or(pDest, val, ORDER::order);
#endif
    }

    static inline T bitwise_xor(T volatile * pDest, const T val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_XOR32_DEFINED
        // Processor has native bitwise_or implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType)) ;
        return (T) xor32<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest) ;
        } while (!cas(pDest, cur, cur ^ val));
        return cur ;
#endif
#else
        return StdOps::bitwise_xor(pDest, val, ORDER::order);
#endif
    }
};

// Pointer atomic operations
template <typename ORDER, typename T>
struct TypedOps<ORDER, T *, sizeof(pointer_t)>
{
    typedef Operations<T *, sizeof(pointer_t)> StdOps;

    typedef typename StdOps::OperandType OperandType;
    typedef typename StdOps::AtomicType AtomicType;

    static const unsigned int OperandSize = StdOps::OperandSize;

    template <typename OTHER_ORDER>
    struct RebindOrder 
    {
        typedef TypedOps<OTHER_ORDER, T *, sizeof(pointer_t)> other;
    };

    // Mandatory atomic primitives

    static inline T * load(T * volatile const * pAddr)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        return reinterpret_cast< T *>(loadptr<ORDER>(
                        reinterpret_cast< pointer_t volatile const *>(pAddr)));
#else
        return StdOps::load(pAddr, ORDER::order);
#endif
    }

    static inline T * store(T * volatile * pAddr, T * val)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        storeptr<ORDER>(reinterpret_cast< pointer_t volatile *>(pAddr), reinterpret_cast<pointer_t>(val));
#else
        StdOps::store(pAddr, val, ORDER::order);
#endif
        return val;
    }

    static inline bool cas(T * volatile * pDest, T * expected, T * desired)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        return casptr<ORDER>(
                reinterpret_cast< pointer_t volatile *>(pDest),
                reinterpret_cast<pointer_t>(expected),
                reinterpret_cast<pointer_t>(desired)
                             );
#else
        return StdOps::cas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    static inline T * vcas(T * volatile * pDest, T * expected, T * desired)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        return reinterpret_cast<T *>(vcasptr<ORDER>(
                        reinterpret_cast< pointer_t volatile *>(pDest),
                        reinterpret_cast<pointer_t>(expected),
                        reinterpret_cast<pointer_t>(desired)
                                                    ));
#else
        return StdOps::vcas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T * xchg(T * volatile * pDest, T * val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_XCHGPTR_DEFINED
        // Processor has native xchgptr implementation
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
        return reinterpret_cast<T *>(xchgptr<ORDER>(
                        reinterpret_cast< pointer_t volatile *>(pDest),
                        reinterpret_cast<T *>(val)
                                                    ));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, val));
        return cur;
#endif
#else
        return StdOps::xchg(pDest, val, ORDER::order);
#endif
    }
};

//64bit atomic operations
template <typename ORDER, typename T>
struct TypedOps<ORDER, T, sizeof(atomic64_t)>
{
    typedef IntegralOperations<T, sizeof(atomic64_t)> StdOps;
    typedef typename StdOps::OperandType OperandType;
    typedef typename StdOps::AtomicType AtomicType;
    //typedef typename StdOps::atomic_unaligned  atomic_unaligned;

    static const unsigned int OperandSize = StdOps::OperandSize;

    template <typename OTHER_ORDER>
    struct RebindOrder 
    {
        typedef TypedOps<OTHER_ORDER, T, sizeof(T)> other;
    };

    // Mandatory atomic primitives

    static inline T load(T volatile const * pAddr)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pAddr));
        AtomicType v = load64<ORDER>(reinterpret_cast< AtomicType volatile const *>(pAddr));
        return *((T *) &v);
#else
        return StdOps::load(pAddr, ORDER::order);
#endif
    }

    static inline T& load(T& dest, T volatile const * pAddr)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pAddr));
        *((AtomicType *) &dest) = load64<ORDER>(reinterpret_cast< AtomicType volatile const *>(pAddr));
        return dest;
#else
        return StdOps::load(dest, pAddr, ORDER::order);
#endif
    }

    static inline const T& store(T volatile * pAddr, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pAddr));
        store64<ORDER>(reinterpret_cast< AtomicType volatile *>(pAddr), atomic_cast<AtomicType>(val));
#else
        return StdOps::store(pAddr, val, ORDER::order);
#endif
        return val;
    }

    static inline bool cas(T volatile * pDest, const T& expected, const T& desired)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return cas64<ORDER>(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired)
                            );
#else
        return StdOps::cas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    static inline T vcas(T volatile * pDest, const T& expected, const T& desired)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return (T) vcas64<ORDER>(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired)
                                 );
#else
        return StdOps::vcas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T xchg(T volatile * pDest, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_XCHG64_DEFINED
        // Processor has native xchg implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return (T) xchg64<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, val));
        return cur;
#endif
#else
        return StdOps::xchg(pDest, val, ORDER::order);
#endif
    }

    static inline T xadd(T volatile * pDest, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
#ifdef FX_XADD64_DEFINED
        // Processor has native xadd implementation
        return (T) xadd64<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, cur + val));
        return cur;
#endif
#else
        return StdOps::xadd(pDest, val, ORDER::order);
#endif
    }

    static inline T inc(T volatile * pDest)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_INC64_DEFINED
        // Processor has native inc implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) inc64<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest));
#else
        return xadd(pDest, 1);
#endif
#else
        return StdOps::inc(pDest, ORDER::order);
#endif
    }

    static inline T dec(T volatile * pDest)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_DEC64_DEFINED
        // Processor has native dec implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) dec64<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest));
#else
        return xadd(pDest, T(0-1));
#endif
#else
        return StdOps::dec(pDest, ORDER::order);
#endif
    }

    static inline T bitwise_and(T volatile * pDest, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_AND64_DEFINED
        // Processor has native xand implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) and64<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, cur & val));
        return cur;
#endif
#else
        return StdOps::bitwise_and(pDest, val, ORDER::order);
#endif
    }

    static inline T bitwise_or(T volatile * pDest, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_OR64_DEFINED
        // Processor has native bitwise_or implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) or64<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, cur | val));
        return cur;
#endif
#else
        return StdOps::bitwise_or(pDest, val, ORDER::order);
#endif
    }

    static inline T bitwise_xor(T volatile * pDest, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_XOR64_DEFINED
        // Processor has native bitwise_or implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) xor64<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest), atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, cur ^ val));
        return cur;
#endif
#else
        return StdOps::bitwise_xor(pDest, val, ORDER::order);
#endif
    }
};

#ifdef FX_128BIT_ATOMIC_DEFINED
// 128bit atomic operations for user-defined aligned types

template <typename ORDER, typename T>
struct TypedOps<ORDER, T, sizeof(atomic128_t)>
{
    typedef operations<T, sizeof(atomic128_t)>   StdOps;
    typedef typename StdOps::OperandType OperandType;
    typedef typename StdOps::AtomicType AtomicType;

    static const unsigned int OperandSize = StdOps::OperandSize;

    template <typename OTHER_ORDER>
    struct RebindOrder 
    {
        typedef TypedOps<OTHER_ORDER, T, sizeof(T)> other;
    };

    // Mandatory atomic primitives

    static inline T load(T volatile const * pAddr)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pAddr));
        AtomicType v = load128<ORDER>(reinterpret_cast< AtomicType volatile const *>(pAddr));
        return *((T *) &v);
#else
        return StdOps::load(pAddr, ORDER::order);
#endif
    }

    static inline T& load(T& dest, T volatile const * pAddr)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pAddr));
        *((AtomicType *) &dest) = load128<ORDER>(reinterpret_cast< AtomicType volatile const *>(pAddr));
        return dest;
#else
        return StdOps::load(dest, pAddr, ORDER::order);
#endif
    }

    static inline void store(T volatile * pAddr, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pAddr));
        store128<ORDER>(reinterpret_cast< AtomicType volatile *>(pAddr), atomic_cast<AtomicType>(val));
#else
        return StdOps::store(pAddr, val, ORDER::order);
#endif
    }

    static inline bool cas(T volatile * pDest, const T& expected, const T& desired)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pDest));
        return cas128<ORDER>(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired)
                             );
#else
        return StdOps::cas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    static inline T vcas(T volatile * pDest, const T& expected, const T& desired)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pDest));
        return (T) vcas128<ORDER>(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired)
                                  );
#else
        return StdOps::vcas(pDest, expected, desired, ORDER::order, membar_relaxed::order);
#endif
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T xchg(T volatile * pDest, const T& val)
    {
#ifdef FX_ATOMIC_TYPED_OPS_DEFINED
#ifdef FX_XCHG128_DEFINED
        // Processor has native xchg implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pDest));
        return (T) xchg128<ORDER>(reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val));
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = RebindOrder<membar_relaxed>::other::load(pDest);
        } while (!cas(pDest, cur, val));
        return cur;
#endif
#else
        return StdOps::xchg(pDest, val, ORDER::order);
#endif
    }
};
#endif  // #ifdef FX_128BIT_ATOMIC_DEFINED


/// Memory fence of type \p ORDER
// template <typename ORDER>
// static inline void fence()
// {
// #ifdef FX_ATOMIC_TYPED_OPS_DEFINED
//     fence<ORDER>();
// #else
//     fence(ORDER::order);
// #endif
// }

/// Atomic load value of address \p p with explicit memory order
template <typename ORDER, typename T>
static inline T load(T volatile const * p)
{
    return TypedOps<ORDER,T,sizeof(T)>::load(p);
}

/// Atomic load value of address \p p with explicit memory order
template <typename ORDER, typename T>
static inline T& load(T& dest, T volatile const * p)
{
    return TypedOps<ORDER,T,sizeof(T)>::load(dest, p);
}

/// Atomic store a value \p val to memory address \p p with explicit memory order
template <typename ORDER, typename T>
static inline void store(T volatile * p, const T val)
{
    TypedOps<ORDER,T,sizeof(T)>::store(p, val);
}

/// Atomic exchange
template <typename ORDER, typename T>
static inline T exchange(T volatile * p, const T val)
{
    return TypedOps<ORDER,T,sizeof(T)>::xchg(p, val);
}

/// Atomic compare and swap
template <typename SUCCESS_ORDER, typename T>
static inline bool cas(T volatile * p, const T expected, const T desired)
{
    return TypedOps<SUCCESS_ORDER,T,sizeof(T)>::cas(p, expected, desired);
}

/// Atomic compare and swap, returns current value
template <typename SUCCESS_ORDER, typename T>
static inline T vcas(T volatile * p, const T expected, const T desired)
{
    return TypedOps<SUCCESS_ORDER,T,sizeof(T)>::vcas(p, expected, desired);
}

/// Atomic fetch and add
template <typename ORDER, typename T>
static inline T xadd(T volatile * p, const T val)
{
    return TypedOps<ORDER,T,sizeof(T)>::xadd(p, val);
}

/// Atomic post-increment
template <typename ORDER, typename T>
static inline T inc(T volatile * p)
{
    return TypedOps<ORDER,T,sizeof(T)>::inc(p);
}

/// Atomic post-decrement
template <typename ORDER, typename T>
static inline T dec(T volatile * p)
{
    return TypedOps<ORDER,T,sizeof(T)>::dec(p);
}

/// Atomic bitwise and
template <typename ORDER, typename T>
static inline T bitwise_and(T volatile * p, const T val)
{
    return TypedOps<ORDER,T,sizeof(T)>::bitwise_and(p, val);
}

/// Atomic bitwise or
template <typename ORDER, typename T>
static inline T bitwise_or(T volatile * p, const T val)
{
    return TypedOps<ORDER,T,sizeof(T)>::bitwise_or(p, val);
}

/// Atomic bitwise xor
template <typename ORDER, typename T>
static inline T bitwise_xor(T volatile * p, const T val)
{
    return TypedOps<ORDER,T,sizeof(T)>::bitwise_xor(p, val);
}

} //end namespace atomic


FX_NS_END

#endif  // #ifndef __FX_ATOMIC_TYPED_OPS_H
