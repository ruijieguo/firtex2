#ifndef __FX_MEMORY_ORDER_H
#define __FX_MEMORY_ORDER_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(thread);

/// C++0x memory order enum
enum memory_order 
{
    memory_order_relaxed,   /// relaxed memory semantics
    memory_order_consume,   /// consume memory semantics
    memory_order_acquire,   /// acquire memory semantics
    memory_order_release,   /// release memory semantics
    memory_order_acq_rel,   /// acquire-release memory semantics
    memory_order_seq_cst,   /// sequentially consistent memory semantics

    END_OF_MEMORY_ORDER     /// helper value
};

/// Wrapper for \ref memory_order_relaxed
struct membar_relaxed 
{
    static const memory_order order = memory_order_relaxed;     /// Corresponding memory order constant
};
/// Wrapper for \ref memory_order_consume
struct membar_consume 
{
    static const memory_order order = memory_order_consume;     /// Corresponding memory order constant
};
/// Wrapper for \ref memory_order_acquire
struct membar_acquire 
{
    static const memory_order order = memory_order_acquire;     /// Corresponding memory order constant
};
/// Wrapper for \ref memory_order_release
struct membar_release 
{
    static const memory_order order = memory_order_release;     /// Corresponding memory order constant
};
/// Wrapper for \ref memory_order_acq_rel
struct membar_acq_rel 
{
    static const memory_order order = memory_order_acq_rel;     /// Corresponding memory order constant
};
/// Wrapper for \ref memory_order_seq_cst
struct membar_seq_cst 
{
    static const memory_order order = memory_order_seq_cst;     /// Corresponding memory order constant
};

FX_NS_END

#endif //__FX_MEMORY_ORDER_H
