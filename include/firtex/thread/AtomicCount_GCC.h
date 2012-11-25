#ifndef __FXATOMICCOUNT_GCC_H
#define __FXATOMICCOUNT_GCC_H

#if __GNUC__ * 100 + __GNUC_MINOR__ >= 402
# include <ext/atomicity.h> 
#else 
# include <bits/atomicity.h>
#endif

#include "firtex/common/StdHeader.h"

FX_NS_DEF(thread);

#if defined(__GLIBCXX__) // g++ 3.4+

using __gnu_cxx::__atomic_add;
using __gnu_cxx::__exchange_and_add;

#endif

class AtomicCount
{
public:
    AtomicCount() : m_value(0) {}
    explicit AtomicCount( long v ) : m_value( v ) {}

    long operator++()
    {
        return __exchange_and_add( &m_value, +1 ) + 1;
    }

    long operator--()
    {
        return __exchange_and_add( &m_value, -1 ) - 1;
    }

    operator long() const
    {
        return __exchange_and_add( &m_value, 0 );
    }

private:

    AtomicCount(AtomicCount const &);
    AtomicCount & operator=(AtomicCount const &);

    mutable _Atomic_word m_value;
};

FX_NS_END

#endif // #ifndef __FXATOMICCOUNT_GCC_H
