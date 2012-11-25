#ifndef __FXATOMICCOUNT_WIN32_H
#define __FXATOMICCOUNT_WIN32_H

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "firtex/common/StdHeader.h"
#include "firtex/thread/interlocked.h"

FX_NS_DEF(thread);

class AtomicCount
{
public:
    AtomicCount() : m_value(0) {}
    explicit AtomicCount( long v ): m_value( v )
    {
    }

    long operator++()
    {
        return FX_INTERLOCKED_INCREMENT( &m_value );
    }

    long operator--()
    {
        return FX_INTERLOCKED_DECREMENT( &m_value );
    }

    operator long() const
    {
        return static_cast<long const volatile &>( m_value );
    }

private:

    AtomicCount( AtomicCount const & );
    AtomicCount & operator=( AtomicCount const & );

    long m_value;
};

FX_NS_END

#endif // #ifndef __FXATOMICCOUNT_WIN32_H
