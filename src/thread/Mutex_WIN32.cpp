#include "firtex/thread/Mutex_WIN32.h"
#include "firtex/utility/Timestamp.h"

FX_NS_USE(utility);
FX_NS_DEF(thread)


MutexImpl::MutexImpl()
{
	// the fct has a boolean return value under WInnNt/2000/XP but not on Win98
	// the return only checks if the input address of &m_cs was valid, so it is safe to omit it
#if(_WIN32_WINNT >= 0x0400)
	InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
#else
	InitializeCriticalSection(&m_cs);
#endif
}

MutexImpl::~MutexImpl()
{
	DeleteCriticalSection(&m_cs);
}

bool MutexImpl::tryLock(long milliseconds)
{
	const int sleepMillis = 5;
	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);
	do
	{
		try
		{
#if(_WIN32_WINNT >= 0x0400)
			if (TryEnterCriticalSection(&m_cs) == TRUE)
				return true;
#else
            return true;
#endif
		}
		catch (...)
		{
			throw SystemException("cannot lock mutex");
		}
		Sleep(sleepMillis);
	} while (!now.isElapsed(diff));
	return false;
}

FX_NS_END
