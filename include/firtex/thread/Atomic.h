#ifndef __FX_ATOMIC_H
#define __FX_ATOMIC_H

#include "firtex/common/StdHeader.h"
#include "firtex/thread/atomic/MemoryOrder.h"
#include "firtex/thread/atomic/AtomicOperations.h"
#include "firtex/thread/atomic/AtomicTypedOps.h"
#include "firtex/thread/atomic/AtomicIntegrals.h"
#include "firtex/thread/atomic/AtomicInt.h"

#ifdef FX_WINDOWS
#pragma warning(push)
#pragma warning(disable:4267)
#endif

FX_NS_DEF(thread);

/// Memory fence of type \p order
static inline void fence(memory_order order)
{
    atomic::fence(order);
}

/// Atomic load value of address \p p with explicit memory order
template <typename T>
static inline T load(T volatile const * p, memory_order order)
{
    return atomic::Operations<T, sizeof(T)>::load(p, order);
}

/// Atomic load value of address \p p with explicit memory order
template <typename T>
static inline T& load(T& dest, T volatile const * p, memory_order order)
{
    return atomic::Operations<T, sizeof(T)>::load(dest, p, order);
}

/// Atomic store a value \p val to memory address \p p with explicit memory order
template <typename T>
static inline void store(T volatile * p, const T val, memory_order order)
{
    atomic::Operations<T, sizeof(T)>::store(p, val, order);
}

/// Atomic exchange
template <typename T>
static inline T exchange(T volatile * p, const T val, memory_order order)
{
    return atomic::Operations<T, sizeof(T)>::xchg(p, val, order);
}

/// Atomic compare and swap
template <typename T>
static inline bool cas(T volatile * p, const T expected, const T desired,
                       memory_order success_order, memory_order failure_order)
{
    return atomic::Operations<T, sizeof(T)>::cas(p, expected, desired, success_order, failure_order);
}

/// Atomic compare and swap, returns current value
template <typename T>
static inline T vcas(T volatile * p, const T expected, const T desired, 
                     memory_order success_order, memory_order failure_order)
{
    return atomic::Operations<T, sizeof(T)>::vcas(p, expected, desired, success_order, failure_order);
}

/// Atomic fetch and add
template <typename T>
static inline T xadd(T volatile * p, const T val, memory_order order)
{
    return atomic::IntegralOperationsSelector<T, sizeof(T)>::op_impl::xadd(p, val, order);
}

/// Atomic post-increment
template <typename T>
static inline T inc(T volatile * p, memory_order order)
{
    return atomic::IntegralOperationsSelector<T, sizeof(T)>::op_impl::inc(p, order);
}

/// Atomic post-decrement
template <typename T>
static inline T dec(T volatile * p, memory_order order)
{
    return atomic::IntegralOperationsSelector<T, sizeof(T)>::op_impl::dec(p, order);
}

/// Atomic bitwise and
template <typename T>
static inline T bitwise_and(T volatile * p, const T val, memory_order order)
{
    return atomic::IntegralOperationsSelector<T, sizeof(T)>::op_impl::bitwise_and(p, val, order);
}

/// Atomic bitwise or
template <typename T>
static inline T bitwise_or(T volatile * p, const T val, memory_order order)
{
    return atomic::IntegralOperationsSelector<T, sizeof(T)>::op_impl::bitwise_or(p, val, order);
}

/// Atomic bitwise xor
template <typename T>
static inline T bitwise_xor(T volatile * p, const T val, memory_order order)
{
    return atomic::IntegralOperationsSelector<T, sizeof(T)>::op_impl::bitwise_xor(p, val, order);
}

/// Atomic type
template <typename T>
class Atomic
{
    typedef typename atomic::AtomicTypeSelector<T>::Type NativeAtomicType;
    typedef atomic::Operations<T, sizeof(NativeAtomicType)> Operations;

    T volatile m_val;
public:
    typedef T value_type;

    Atomic() {}

    explicit Atomic(T v)
        : m_val(v)
    {}

    T value() const
    {
        return m_val;
    }

    T load(memory_order order) const volatile
    {
        return Operations::load(&m_val, order);
    }

    /// Read pointer value, metaprogramming version
    template <typename ORDER>
    T  load() volatile const
    {
        return atomic::TypedOps<ORDER, T, sizeof(T)>::load(&m_val);
    }

    /// Store value
    void store(const T v, memory_order order) volatile
    {
        Operations::store(&m_val, v, order);
    }

    /// Store value, metaprogramming version
    template <typename ORDER>
    void store(const T v) volatile
    {
        atomic::TypedOps<ORDER, T, sizeof(T)>::store(&m_val, v);
    }

    /// CAS
    bool cas(const T expected, const T desired,
             memory_order success_order,
             memory_order failure_order) volatile
    {
        return Operations::cas(&m_val, expected, desired,
                               success_order, failure_order);
    }

    /// CAS, metaprogramming version
    template <typename SUCCESS_ORDER>
    bool cas(const T expected, const T desired) volatile
    {
        return atomic::TypedOps<SUCCESS_ORDER, T, sizeof(T)>::cas(&m_val, expected, desired);
    }

    /// Valued CAS
    T vcas(const T expected, const T desired, memory_order success_order,
           memory_order failure_order) volatile
    {
        return Operations::vcas(&m_val, expected, desired, 
                                success_order, failure_order);
    }

    /// Valued CAS, metaprogramming version
    template <typename SUCCESS_ORDER>
    T vcas(const T expected, const T desired) volatile
    {
        return atomic::TypedOps<SUCCESS_ORDER, T, sizeof(T)>::vcas(&m_val, expected, desired);
    }

    /// Value exchange
    T xchg(const T v, memory_order order) volatile
    {
        return Operations::xchg(&m_val, v, order);
    }

    /// Value exchange, metaprogramming version
    template <typename ORDER>
    T xchg(const T v) volatile
    {
        return atomic::TypedOps<ORDER, T, sizeof(T)>::xchg(&m_val, v);
    }
};

/// Atomic pointer
template<typename T>
class Atomic<T*>
{
    T* volatile m_ptr;

    typedef atomic::Operations<T*, sizeof(atomic::pointer_t)> Operations;

public:
    typedef T ValueType;     /// Type that pointer pointed to
    typedef T* PointerType;  /// Pointer type

    Atomic() : m_ptr(NULL)
    {}

    explicit Atomic(T * p)
        : m_ptr(p)
    {}

    T * load(memory_order order) volatile const
    {
        return Operations::load(&m_ptr, order);
    }

    /// Read pointer value
    template <typename ORDER>
    T * load() volatile const
    {
        return atomic::TypedOps<ORDER, T*, sizeof(PointerType)>::load(&m_ptr);
    }

    /// Store pointer value
    T * store(T * p, memory_order order) volatile
    {
        return Operations::store(&m_ptr, p, order);
    }

    /// Store pointer value
    template <typename ORDER>
    T * store(T * p) volatile
    {
        return atomic::TypedOps<ORDER, T*, sizeof(PointerType)>::store(&m_ptr, p);
    }

    /// CAS
    bool cas(T* expected, T* desired, memory_order success_order, 
             memory_order failure_order) volatile
    {
        return Operations::cas(&m_ptr, expected, desired,
                               success_order, failure_order);
    }

    template <typename SUCCESS_ORDER>
    bool cas(T* expected, T* desired) volatile
    {
        return atomic::TypedOps<SUCCESS_ORDER, T*, 
                                sizeof(PointerType)>::cas(&m_ptr, expected, desired);
    }

    T* vcas(T* expected, T* desired, memory_order success_order,
            memory_order failure_order) volatile
    {
        return Operations::vcas(&m_ptr, expected, desired,
                                success_order, failure_order);
    }

    template <typename SUCCESS_ORDER>
    T* vcas(T* expected, T* desired) volatile
    {
        return atomic::TypedOps<SUCCESS_ORDER, T*, 
                                sizeof(atomic::pointer_t)>::vcas(&m_ptr, expected, desired);
    }

    T* xchg(T* p, memory_order order) volatile
    {
        return Operations::xchg(&m_ptr, p, order);
    }

    /// Pointer exchange
    template <typename ORDER>
    T* xchg(T* p) volatile
    {
        return atomic::TypedOps<ORDER, T*, sizeof(PointerType)>::xchg(&m_ptr, p);
    }

    /// Returns pointer to atomic pointer
    T* volatile *  operator &()
    {
        return &m_ptr;
    }
};

#define _FX_DEFINE_ATOMIC_INT(_type)                            \
    template <>                                                 \
    class Atomic<_type>: public atomic::AtomicInt<_type>        \
    {                                                           \
        typedef atomic::AtomicInt<_type> BaseClass;             \
    public:                                                     \
        Atomic() {}                                             \
        explicit Atomic(_type v)                                \
            : BaseClass(v)                                      \
        {}                                                      \
    };

_FX_DEFINE_ATOMIC_INT(int)
_FX_DEFINE_ATOMIC_INT(unsigned int)
_FX_DEFINE_ATOMIC_INT(long)
_FX_DEFINE_ATOMIC_INT(unsigned long)
_FX_DEFINE_ATOMIC_INT(long long)
_FX_DEFINE_ATOMIC_INT(unsigned long long)


#ifdef FX_ATOMIC_BOOL_DEFINED
/// Atomic bool
template <>
class Atomic<bool> : public atomic::atomic_bool
{
    typedef atomic_bool BaseClass;

public:
    Atomic()
        : BaseClass()
    {}

    explicit Atomic(bool bVal)
        : BaseClass(bVal)
    {}
};
#else

// Emulate atomic bool through atomic32_t
template <>
class Atomic<bool>
{
    Atomic<atomic32_t>  m_bool;
public:
    Atomic()
        : m_bool(0)
    {}

    explicit Atomic(bool bVal)
        : m_bool(bVal ? 1 : 0)
    {}

    bool load(memory_order order) const volatile
    {
        return m_bool.load(order) != 0;
    }

    template <typename ORDER>
    bool    load() const volatile
    {
        return m_bool.load<ORDER>() != 0;
    }

    void store(bool v, memory_order order) volatile
    {
        m_bool.store(v, order);
    }

    template <typename ORDER>
    void store(bool v) volatile
    {
        m_bool.store<ORDER>(v);
    }

    bool exchange(bool val, memory_order order) volatile
    {
        return m_bool.xchg(val, order) != 0;
    }

    template <typename ORDER>
    bool exchange(bool val) volatile
    {
        return m_bool.xchg<ORDER>(val) != 0;
    }

    bool cas(bool bExpected, bool bDesired, memory_order success_order,
             memory_order failure_order) volatile
    {
        return m_bool.cas(bExpected, bDesired, success_order, failure_order);
    }

    template <typename SUCCESS_ORDER>
    bool cas(bool bExpected, bool bDesired) volatile
    {
        return m_bool.cas<SUCCESS_ORDER>(bExpected, bDesired);
    }

    bool vcas(bool bExpected, bool bDesired, memory_order success_order,
              memory_order failure_order) volatile
    {
        return m_bool.vcas(bExpected, bDesired, success_order, failure_order) != 0;
    }

    template <typename SUCCESS_ORDER>
    bool vcas(bool bExpected, bool bDesired) volatile
    {
        return m_bool.vcas<SUCCESS_ORDER>(bExpected, bDesired) != 0;
    }
};

#endif


/**
 * @brief Atomic counter
 * This class is simplified interface around \ref atomic<size_t>.
 * The class supports getting of current value of the counter and increment/decrement its value.
 * \par Template parameters:
 * \li INC_ORDER - default memory order for increment/decrement operations
 * \li READ_ORDER - default memory order for \ref value() method
 */
template <typename INC_ORDER = membar_relaxed, typename READ_ORDER = membar_relaxed>
class AtomicCounter
{
    typedef Atomic<size_t> AtomicType;
    Atomic<size_t> m_counter;

public:
    typedef typename AtomicType::UnalignedAtomicType CounterType;

public:
    AtomicCounter(size_t v)
        : m_counter(v)
    {}

    AtomicCounter()
        : m_counter(0)
    {}

    /// Returns current value of the counter
    CounterType value(memory_order order = READ_ORDER::order) const
    {
        return m_counter.load(order);
    }

    /// Same as \ref value()
    operator CounterType() const
    {
        return value();
    }

    /// Returns underlying atomic interface
    AtomicType& getAtomic()
    {
        return m_counter;
    }

    /// Returns underlying atomic interface (const)
    const AtomicType& getAtomic() const
    {
        return m_counter;
    }

    /// Increments the counter. Semantics: postincrement
    size_t inc(memory_order order = INC_ORDER::order)
    {
        return m_counter.inc(order);
    }

    /// Decrements the counter. Semantics: postdecrement
    size_t dec(memory_order order = INC_ORDER::order)
    {
        return m_counter.dec(order);
    }

    /// Preincrement
    size_t operator ++()
    {
        return inc() + 1;
    }

    /// Postincrement
    size_t operator ++(int)
    {
        return inc();
    }

    /// Predecrement
    size_t operator --()
    {
        return dec() - 1;
    }

    /// Postdecrement
    size_t operator --(int)
    {
        return dec();
    }

    /// Resets count to 0
    void reset(memory_order order = INC_ORDER::order)
    {
        m_counter.store(0, order);
    }
};

/**
 * @brief Empty item counter
 * This class may be used instead of \ref AtomicCounter when you
 * do not need full \ref AtomicCounter interface.
 * All methods of the class is empty and returns 0.
 * 
 * The object of this class should not be used in data structure 
 * that behavior significantly depends on item counting
 * (for example, in many hash map implementation).
 */
class EmptyAtomicCounter 
{
public:
    typedef AtomicCounter<>::CounterType CounterType;

public:
    /// Returns 0
    CounterType value(memory_order /*order*/ = membar_relaxed::order) const
    {
        return 0;
    }

    /// Same as \ref value(), always returns 0.
    operator CounterType() const
    {
        return value();
    }

    /// Dummy increment. Always returns 0
    size_t inc(memory_order /*order*/ = membar_relaxed::order)
    {
        return 0;
    }

    /// Dummy increment. Always returns 0
    size_t dec(memory_order /*order*/ = membar_relaxed::order)
    {
        return 0;
    }

    /// Dummy pre-increment. Always returns 0
    size_t operator ++()
    {
        return 0;
    }
    /// Dummy post-increment. Always returns 0
    size_t operator ++(int)
    {
        return 0;
    }

    /// Dummy pre-decrement. Always returns 0
    size_t operator --()
    {
        return 0;
    }
    /// Dummy post-decrement. Always returns 0
    size_t operator --(int)
    {
        return 0;
    }

    /// Dummy function
    void reset(memory_order = membar_relaxed::order)
    {}
};

/**
 * @brief /// Atomic event counter.
 *
 * This class uses weak memory ordering (memory_order_relaxed) 
 * and may be used for statistical purposes
 */
class EventCounter : private Atomic<size_t>
{
    typedef Atomic<size_t> BaseClass;

public:
    typedef size_t ValueType;

public:
    EventCounter()
        : BaseClass(0)
    {}

    /// Assign operator
    size_t operator = (size_t n)
    {
        BaseClass::store(n, membar_relaxed::order);
        return n;
    }

    /// Addition
    size_t operator +=(size_t n)
    {
        return BaseClass::xadd(n, membar_relaxed::order);
    }

    /// Substraction
    size_t operator -=(size_t n)
    {
        return BaseClass::xadd(0-n, membar_relaxed::order);
    }

    /// Get current value of the counter
    operator size_t () const
    {
        return get();
    }

    /// Preincrement
    size_t operator ++()
    {
        return BaseClass::inc(membar_relaxed::order) + 1;
    }
    /// Postincrement
    size_t operator ++(int)
    {
        return BaseClass::inc(membar_relaxed::order);
    }

    /// Predecrement
    size_t operator --()
    {
        return BaseClass::dec(membar_relaxed::order) - 1;
    }
    /// Postdecrement
    size_t operator --(int)
    {
        return BaseClass::dec(membar_relaxed::order);
    }

    /// Get current value of the counter
    size_t get() const
    {
        return BaseClass::load(membar_relaxed::order);
    }
};

#ifdef FX_128BIT_ATOMIC_DEFINED
    using atomic::atomic128_t;
#endif

FX_NS_END

#ifdef FX_WINDOWS
#pragma warning(pop)
#endif

#endif //__FX_ATOMIC_H
