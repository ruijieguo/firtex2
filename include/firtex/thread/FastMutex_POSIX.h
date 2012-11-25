#ifndef __FASTMUTEX_POSIX_H
#define __FASTMUTEX_POSIX_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/thread/Mutex_POSIX.h"
#include <pthread.h>
#include <errno.h>

FX_NS_DEF(thread);

class FastMutexImpl: public MutexImpl
{
public:
	FastMutexImpl();
	~FastMutexImpl();
};

FX_NS_END

#endif 
