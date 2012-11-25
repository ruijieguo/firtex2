#ifndef __FX_ATOMIC_OPERATIONS_H
#define __FX_ATOMIC_OPERATIONS_H

#include "firtex/thread/atomic/AtomicImpl.h"
#include "firtex/thread/atomic/MemoryOrder.h"
#include "firtex/thread/atomic/IsAligned.h"

FX_NS_DEF(thread);

namespace atomic 
{

template <typename T, unsigned int SIZE>
struct Operations;

// 32bit atomic operations
template <typename T>
struct Operations<T, sizeof(atomic32_t)>
{
    typedef T OperandType;
    typedef atomic32_t AtomicType;
    typedef atomic32_t AtomicUnaligned;
    static const unsigned int OperandSize = sizeof(AtomicType);

    // Mandatory atomic primitives

    static inline T load(T volatile const * pAddr, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        AtomicType n = atomic::load32(reinterpret_cast<AtomicType volatile const *>(pAddr), order);
        return *(T *) &n;
    }

    static inline T& load(T& dest, T volatile const * pAddr, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        *((AtomicType *) &dest) = atomic::load32(reinterpret_cast< AtomicType volatile const *>(pAddr), order);
        return dest;
    }

    static inline T store(T volatile * pAddr, const T val, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        store32(reinterpret_cast< AtomicType volatile *>(pAddr),
                atomic_cast<AtomicType>(val), order);
        return val;
    }

    static inline bool cas(T volatile * pDest, const T expected, const T desired,
                           memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return cas32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired),
                success_order,
                failure_order);
    }

    static inline T vcas(T volatile * pDest, const T expected, const T desired,
                         memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) vcas32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired),
                success_order,
                failure_order);
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T xchg(T volatile * pDest, const T val, memory_order order)
    {
#ifdef FX_XCHG32_DEFINED
        // Processor has native xchg implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) xchg32(reinterpret_cast< AtomicType volatile *>(pDest),
                          atomic_cast<AtomicType>(val), order);
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, val, order, membar_relaxed::order));
        return cur;
#endif
    }
};

// 64bit atomic operations for user-defined aligned types
template <typename T>
struct Operations<T, sizeof(atomic64_t)>
{
    typedef T OperandType;
    typedef atomic64_t AtomicType;
    typedef atomic64_unaligned  AtomicUnaligned;

    static const unsigned int OperandSize = sizeof(AtomicType);

    // Mandatory atomic primitives

    static inline T load(T volatile const * pAddr, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        FIRTEX_ASSERT2(atomic::isAligned<8>(pAddr));
        AtomicType v = atomic::load64(reinterpret_cast< AtomicType volatile const *>(pAddr), order);
        return *((T *) &v);
    }

    static inline T& load(T& dest, T volatile const * pAddr, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pAddr));
        *((AtomicType *) &dest) = atomic::load64(reinterpret_cast< AtomicType volatile const *>(pAddr), order);
        return dest;
    }

    static inline const T& store(T volatile * pAddr, const T& val, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pAddr));
        store64(reinterpret_cast< AtomicType volatile *>(pAddr), atomic_cast<AtomicType>(val), order);
        return val;
    }

    static inline bool cas(T volatile * pDest, const T& expected, const T& desired, memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return cas64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired),
                success_order,
                failure_order
                              );
    }

    static inline T vcas(T volatile * pDest, const T& expected, const T& desired, memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return (T) vcas64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired),
                success_order,
                failure_order
                                   );
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T xchg(T volatile * pDest, const T& val, memory_order order)
    {
#ifdef FX_xchg64_defined
        // Processor has native xchg implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return (T) xchg64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val),
                order
                                   );
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, val, order, membar_relaxed::order));
        return cur;
#endif
    }
};

#ifdef FX_128BIT_ATOMIC_DEFINED
// 128bit atomic operations for user-defined aligned types
using atomic128_t;

template <typename T>
struct Operations<T, sizeof(atomic128_t)> {
    typedef T            OperandType;
    typedef atomic128_t  AtomicType;
    static const unsigned int OperandSize = sizeof(AtomicType);

    // Mandatory atomic primitives

    static inline T load(T volatile const * pAddr, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pAddr));
        AtomicType v = load128(reinterpret_cast< AtomicType volatile const *>(pAddr), order);
        return *((T *) &v);
    }

    static inline T& load(T& dest, T volatile const * pAddr, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pAddr));
        *((AtomicType *) &dest) = load128(reinterpret_cast< AtomicType volatile const *>(pAddr), order);
        return dest;
    }

    static inline void store(T volatile * pAddr, const T& val, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pAddr));
        store128(reinterpret_cast< AtomicType volatile *>(pAddr), atomic_cast<AtomicType>(val), order);
    }

    static inline bool cas(T volatile * pDest, const T& expected, const T& desired, memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pDest));
        return cas128(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired),
                success_order,
                failure_order
                               );
    }

    static inline T vcas(T volatile * pDest, const T& expected, const T& desired, memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pDest));
        return (T) vcas128(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(expected),
                atomic_cast<AtomicType>(desired),
                success_order,
                failure_order
                                    );
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T xchg(T volatile * pDest, const T& val, memory_order order)
    {
#ifdef FX_xchg128_defined
        // Processor has native xchg implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<16>(pDest));
        return (T) xchg128(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val),
                order
                                    );
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, val, order, membar_relaxed::order));
        return cur;
#endif
    }
};
#endif  // #ifdef FX_128BIT_ATOMIC_DEFINED

template <typename T, int>  struct IntegralOperations;

// 32bit atomic operations for predefined integral types
template <typename T>
struct IntegralOperations<T, sizeof(atomic32_t)> : public Operations<T, sizeof(atomic32_t)>
{
    typedef Operations<T, sizeof(atomic32_t)> base_class;

    typedef typename base_class::OperandType OperandType;
    typedef typename base_class::AtomicType AtomicType;
    typedef typename base_class::AtomicUnaligned AtomicUnaligned;

    static inline T xadd(T volatile * pDest, const T val, memory_order order)
    {
#ifdef FX_XADD32_DEFINED
        // Processor has native xadd implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) xadd32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val),
                order
                                   );
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur + val, order, membar_relaxed::order));
        return cur;
#endif
    }

    static inline T inc(T volatile * pDest, memory_order order)
    {
#ifdef FX_INC32_DEFINED
        // Processor has native inc implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) inc32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                order
                                  );
#else
        return xadd(pDest, 1, order);
#endif
    }

    static inline T dec(T volatile * pDest, memory_order order)
    {
#ifdef FX_DEC32_DEFINED
        // Processor has native dec implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) dec32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                order
                                  );
#else
        return xadd(pDest, T(0-1), order);
#endif
    }

    static inline T bitwise_and(T volatile * pDest, const T val, memory_order order)
    {
#ifdef FX_AND32_DEFINED
        // Processor has native xand implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) and32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val),
                order
                                  );
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur & val, order, membar_relaxed::order));
        return cur;
#endif
    }

    static inline T bitwise_or(T volatile * pDest, const T val, memory_order order)
    {
#ifdef FX_OR32_DEFINED
        // Processor has native xand implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) or32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val),
                order
                                 );
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur | val, order, membar_relaxed::order));
        return cur;
#endif
    }

    static inline T bitwise_xor(T volatile * pDest, const T val, memory_order order)
    {
#ifdef FX_XOR32_DEFINED
        // Processor has native xand implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        return (T) xor32(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicType>(val),
                order
                                  );
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur ^ val, order, membar_relaxed::order));
        return cur;
#endif
    }
};

// 64bit atomic operations for predefined integral types
template <typename T>
struct IntegralOperations<T, sizeof(atomic64_t)>: public Operations<T, sizeof(atomic64_t)>
{
    typedef Operations<T, sizeof(atomic64_t)>        base_class;

    typedef typename base_class::OperandType        OperandType;
    typedef typename base_class::AtomicType         AtomicType;
    typedef typename base_class::AtomicUnaligned    AtomicUnaligned;

    static inline T xadd(T volatile * pDest, const T& val, memory_order order)
    {
#ifdef FX_XADD64_DEFINED
        // Processor has native xadd implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return (T) xadd64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicUnaligned>(val),
                order
                                   );
#else
        // CAS-based emulation
        OperandType cur;
        do {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur + val, order, membar_relaxed::order));
        return cur;
#endif
    }

    static inline T inc(T volatile * pDest, memory_order order)
    {
#ifdef FX_INC64_DEFINED
        // Processor has native xadd implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return (T) inc64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                order
                                  );
#else
        return xadd(pDest, 1, order);
#endif
    }

    static inline T dec(T volatile * pDest, memory_order order)
    {
#ifdef FX_DEC64_DEFINED
        // Processor has native xadd implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));
        return (T) dec64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                order
                                  );
#else
        return xadd(pDest, T(0-1), order);
#endif
    }

    static inline T bitwise_and(T volatile * pDest, const T& val, memory_order order)
    {
#ifdef FX_and64_defined
        // Processor has native atomic AND implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));

        return (T) and64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicUnaligned>(val), order);
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur & val, order, membar_relaxed::order));
        return cur;
#endif
    }

    static inline T bitwise_or(T volatile * pDest, const T& val, memory_order order)
    {
#ifdef FX_OR64_DEFINED
        // Processor has native atomic OR implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));

        return (T) or64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicUnaligned>(val),
                order
                                 );
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur | val, order, membar_relaxed::order));
        return cur;
#endif
    }

    static inline T bitwise_xor(T volatile * pDest, const T& val, memory_order order)
    {
#ifdef FX_XOR64_DEFINED
        // Processor has native atomic XOR implementation
        FIRTEX_ASSERT2(sizeof(T) == sizeof(AtomicType));
        assert(isAligned<8>(pDest));

        return (T) xor64(
                reinterpret_cast< AtomicType volatile *>(pDest),
                atomic_cast<AtomicUnaligned>(val), order);
#else
        // CAS-based emulation
        OperandType cur;
        do 
        {
            cur = load(pDest, membar_relaxed::order);
        } while (!cas(pDest, cur, cur ^ val, order, membar_relaxed::order));
        return cur;
#endif
    }
};

template <typename T, int> struct IntegralOperationsSelector;

template <typename T>
struct IntegralOperationsSelector<T, 4>
{
    typedef IntegralOperations<T, 4> op_impl;
};

template <typename T>
struct IntegralOperationsSelector<T, 8>
{
    typedef IntegralOperations<T, 8>    op_impl;
};


// Pointer atomic operations
template <typename T>
struct Operations<T*, sizeof(pointer_t)>
{
    typedef T* OperandType;
    typedef pointer_t   AtomicType;
    static const unsigned int OperandSize = sizeof(pointer_t);

    // Mandatory atomic primitives

    static inline T * load(T * volatile const * pAddr, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
        return reinterpret_cast< T *>(atomic::loadptr(reinterpret_cast< pointer_t volatile const *>(pAddr), order));
    }

    static inline T * store(T * volatile * pAddr, T * val, memory_order order)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
        atomic::storeptr(reinterpret_cast< pointer_t volatile *>(pAddr),
                 reinterpret_cast<pointer_t>(val), order);
        return val;
    }

    static inline bool cas(T * volatile * pDest, T * expected, T * desired,
                           memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
        return atomic::casptr(
                reinterpret_cast< pointer_t volatile *>(pDest),
                reinterpret_cast<pointer_t>(expected),
                reinterpret_cast<pointer_t>(desired),
                success_order, failure_order);
    }

    static inline T * vcas(T * volatile * pDest, T * expected, T * desired,
                           memory_order success_order, memory_order failure_order)
    {
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
        return reinterpret_cast<T *>(atomic::vcasptr(
                        reinterpret_cast< pointer_t volatile *>(pDest),
                        reinterpret_cast<pointer_t>(expected),
                        reinterpret_cast<pointer_t>(desired),
                        success_order,
                        failure_order));
    }

    // Optional atomic primitives (can be emulated by CAS)

    static inline T * xchg(T * volatile * pDest, T * val, memory_order order)
    {
#ifdef FX_XCHGPTR_DEFINED
        // Processor has native xchgptr implementation
        FIRTEX_ASSERT2(sizeof(T *) == sizeof(AtomicType));
        return reinterpret_cast<T *>(xchgptr(reinterpret_cast< pointer_t volatile *>(pDest),
                        reinterpret_cast<T *>(val),order));
#else
        // CAS-based emulation
        AtomicType cur;
        do 
        {
            cur = load(pDest, membar_relaxed::order);
        } while(!cas(pDest, cur, val, order, membar_relaxed::order));
        return cur;
#endif
    }
};

template <typename T, int SIZE> struct AtomicType;

template <typename T>
struct AtomicType<T, 4>
{
    typedef atomic32_t Type;
    typedef atomic32_t UnalignedType;
    typedef T ValueType;
    typedef T AlignedValueType;
};

template <typename T>
struct AtomicType<T, 8>
{
    typedef atomic64_t Type;
    typedef atomic64_unaligned UnalignedType;
    typedef T ValueType;
    //TODO: GCC 4.1, GCC 4.2: error: sorry, applying attributes to template parameter is not implemented
    // Any workaround?..
    // typedef T FX_TYPE_ALIGNMENT(8) AlignedValueType;
};

#define DECLARE_ATOMIC_TYPE_8(_type)                            \
    template <> struct AtomicType<_type, 8>                     \
    {                                                           \
        typedef atomic64_t Type;                                \
        typedef atomic64_unaligned UnalignedType;               \
        typedef _type ValueType;                                \
        typedef _type FX_TYPE_ALIGNMENT(8) AlignedValueType;    \
    }

DECLARE_ATOMIC_TYPE_8(long int);
DECLARE_ATOMIC_TYPE_8(long unsigned int);
DECLARE_ATOMIC_TYPE_8(long long int);
DECLARE_ATOMIC_TYPE_8(long long unsigned int);

#ifdef FX_128BIT_ATOMIC_DEFINED
template <typename T>
struct AtomicType<T, 16>
{
    typedef atomic128_t Type;
    typedef atomic128_t UnalignedType;
    typedef T ValueType;
    typedef T FX_TYPE_ALIGNMENT(16) AlignedValueType;
};

#define DECLARE_ATOMIC_TYPE_16(_type)                            \
    template <> struct AtomicType<_type, 16>                     \
    {                                                           \
        typedef atomic64_t Type;                                \
        typedef atomic64_unaligned UnalignedType;               \
        typedef _type ValueType;                                \
        typedef _type FX_TYPE_ALIGNMENT(16) AlignedValueType;    \
    }

DECLARE_ATOMIC_TYPE_16(long int);
DECLARE_ATOMIC_TYPE_16(long unsigned int);
DECLARE_ATOMIC_TYPE_16(long long int);
DECLARE_ATOMIC_TYPE_16(long long unsigned int);

#endif

template <typename T>
struct AtomicTypeSelector 
{
    typedef typename AtomicType<T, sizeof(T)>::Type Type;
    typedef typename AtomicType<T, sizeof(T)>::UnalignedType UnalignedType;
    typedef typename AtomicType<T, sizeof(T)>::ValueType ValueType;
    typedef typename AtomicType<T, sizeof(T)>::AlignedValueType AlignedValueType;
};

} // end namespace atomic

FX_NS_END

#endif  // #ifndef __FX_ATOMIC_OPERATIONS_H
