#ifndef __FXATOMICCOUNT_SYNC_H
#define __FXATOMICCOUNT_SYNC_H

#if defined( __ia64__ ) && defined( __INTEL_COMPILER )
# include <ia64intrin.h>
#endif

#include "firtex/common/StdHeader.h"

FX_NS_DEF(thread);

class AtomicCount
{
public:
    AtomicCount() : m_value(0) {}
    explicit AtomicCount( long v ) : m_value( v ) {}

    long operator++()
    {
        return __sync_add_and_fetch( &m_value, 1 );
    }

    long operator--()
    {
        return __sync_add_and_fetch( &m_value, -1 );
    }

    operator long() const
    {
        return __sync_fetch_and_add( &m_value, 0 );
    }

private:

    AtomicCount(AtomicCount const &);
    AtomicCount & operator=(AtomicCount const &);

    mutable long m_value;
};

FX_NS_END

#endif // #ifndef __FXATOMICCOUNT_SYNC_H
