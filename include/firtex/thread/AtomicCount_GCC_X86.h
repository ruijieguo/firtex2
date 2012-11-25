#ifndef __FXATOMICCOUNT_GCC_X86_H
#define __FXATOMICCOUNT_GCC_X86_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(thread);

class AtomicCount
{
public:
    explicit AtomicCount() : m_value(0) {}
    explicit AtomicCount( long v ) : m_value( static_cast< int >( v ) ) {}

    long operator++()
    {
        return atomic_exchange_and_add( &m_value, +1 ) + 1;
    }

    long operator--()
    {
        return atomic_exchange_and_add( &m_value, -1 ) - 1;
    }

    operator long() const
    {
        return atomic_exchange_and_add( &m_value, 0 );
    }

private:

    AtomicCount(AtomicCount const &);
    AtomicCount & operator=(AtomicCount const &);

    mutable int m_value;

private:

    static int atomic_exchange_and_add( int * pw, int dv )
    {
        // int r = *pw;
        // *pw += dv;
        // return r;

        int r;

        __asm__ __volatile__
        (
            "lock\n\t"
            "xadd %1, %0":
            "+m"( *pw ), "=r"( r ): // outputs (%0, %1)
            "1"( dv ): // inputs (%2 == %1)
            "memory", "cc" // clobbers
        );

        return r;
    }
};

FX_NS_END

#endif // #ifndef __FXATOMICCOUNT_GCC_X86_H
