#ifndef __FX_NONCOPYABLE_H__
#define __FX_NONCOPYABLE_H__

#include "firtex/common/StdHeader.h"

FX_NS_DEF(utility);

class NonCopyable 
{
    NonCopyable(const NonCopyable&);
    const NonCopyable& operator=(const NonCopyable&);

protected:
    NonCopyable() { }
    virtual ~NonCopyable() { }
};

FX_NS_END

#endif // __FX_NONCOPYABLE_H__
