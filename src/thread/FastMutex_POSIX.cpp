#include "firtex/thread/FastMutex_POSIX.h"


FX_NS_DEF(thread);

FastMutexImpl::FastMutexImpl(): MutexImpl(true)
{
}

FastMutexImpl::~FastMutexImpl()
{
}

FX_NS_END
