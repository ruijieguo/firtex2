#ifndef __FX_ATOMIC_INT_H
#define __FX_ATOMIC_INT_H

#include "firtex/thread/atomic/AtomicTypedOps.h"
#include "firtex/thread/atomic/AtomicOperations.h"

FX_NS_DEF(thread);

namespace atomic 
{

/// Atomic type for integers
template <typename T>
class AtomicInt
{
public:
    typedef typename AtomicTypeSelector<T>::Type AtomicType;
    typedef typename AtomicTypeSelector<T>::UnalignedType  UnalignedAtomicType;
    typedef typename AtomicTypeSelector<T>::AlignedValueType AlignedValueType;
    typedef typename AtomicTypeSelector<T>::ValueType ValueType;

private:
    typedef typename IntegralOperationsSelector<T, sizeof(AtomicType)>::op_impl Operations;

    template <typename ORDER>
    struct TypedOperations: public TypedOps<ORDER, ValueType, sizeof(ValueType)>
    {};

    AlignedValueType volatile m_val;

public:
    AtomicInt()
        : m_val(0)  
    {}
    explicit AtomicInt(T val) { m_val = val; }

    /// Load value
    T  load(memory_order order) const volatile
    {
        return Operations::load(&m_val, order);
    }

    /// Load value
    template <typename ORDER>
    T  load() const volatile
    {
        return TypedOperations<ORDER>::load(&m_val);
    }

    /// Store value
    T store(T val, memory_order order) volatile
    {
        return Operations::store(&m_val, val, order);
    }

    /// Store value
    template <typename ORDER>
    T store(T val) volatile
    {
        return TypedOperations<ORDER>::store(&m_val, val);
    }

    /// CAS
    bool cas(T expected, T desired, memory_order success_order, memory_order failure_order) volatile
    {
        return Operations::cas(&m_val, expected, desired, success_order, failure_order);
    }

    /// CAS
    template <typename SUCCESS_ORDER>
    bool cas(T expected, T desired) volatile
    {
        return TypedOperations<SUCCESS_ORDER>::cas(&m_val, expected, desired);
    }

    /// Valued CAS
    T vcas(T expected, T desired, memory_order success_order, memory_order failure_order) volatile
    {
        return Operations::vcas(&m_val, expected, desired, success_order, failure_order);
    }

    /// Valued CAS
    template <typename SUCCESS_ORDER>
    T vcas(T expected, T desired) volatile
    {
        return TypedOperations<SUCCESS_ORDER>::vcas(&m_val, expected, desired);
    }

    /// exchange
    T xchg(T val, memory_order order) volatile
    {
        return Operations::xchg(&m_val, val, order);
    }

    /// exchange
    template <typename ORDER>
    T xchg(T val) volatile
    {
        return TypedOperations<ORDER>::xchg(&m_val, val);
    }

    /// Atomic add
    T xadd(T val, memory_order order) volatile
    {
        return Operations::xadd(&m_val, val, order);
    }

    /// Atomic add
    template <typename ORDER>
    T xadd(T val) volatile
    {
        return TypedOperations<ORDER>::xadd(&m_val, val);
    }

    /// Atomic increment
    T inc(memory_order order) volatile
    {
        return AtomicInt::Operations::inc(&m_val, order);
    }

    /// Atomic increment
    template <typename ORDER>
    T inc() volatile
    {
        return TypedOperations<ORDER>::inc(&m_val);
    }

    /// Atomic decrement
    T dec(memory_order order) volatile
    {
        return AtomicInt::Operations::dec(&m_val, order);
    }

    /// Atomic decrement
    template <typename ORDER>
    T dec() volatile
    {
        return TypedOperations<ORDER>::dec(&m_val);
    }
};

} //end namespace atomic

FX_NS_END

#endif // #ifndef __FX_ATOMICINT_H
