#include "firtex/thread/RWLock_WIN32.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(utility);
FX_NS_DEF(thread);


RWLockImpl::RWLockImpl(): m_nReaders(0), m_nWritersWaiting(0), m_nWriters(0)
{
	m_hMutex = CreateMutexW(NULL, FALSE, NULL);
	if (m_hMutex == NULL)
		throw SystemException("cannot create reader/writer lock");

	m_hReadEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (m_hReadEvent == NULL)
		throw SystemException("cannot create reader/writer lock");

	m_hWriteEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (m_hWriteEvent == NULL)
		throw SystemException("cannot create reader/writer lock");
}

RWLockImpl::~RWLockImpl()
{
	CloseHandle(m_hMutex);
	CloseHandle(m_hReadEvent);
	CloseHandle(m_hWriteEvent);
}

inline void RWLockImpl::addWriter()
{
	switch (WaitForSingleObject(m_hMutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (++m_nWritersWaiting == 1) ResetEvent(m_hReadEvent);
		ReleaseMutex(m_hMutex);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}

inline void RWLockImpl::removeWriter()
{
	switch (WaitForSingleObject(m_hMutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (--m_nWritersWaiting == 0 && m_nWriters == 0) SetEvent(m_hReadEvent);
		ReleaseMutex(m_hMutex);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


void RWLockImpl::readLock()
{
	HANDLE h[2];
	h[0] = m_hMutex;
	h[1] = m_hReadEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++m_nReaders;
		ResetEvent(m_hWriteEvent);
		ReleaseMutex(m_hMutex);
		FIRTEX_ASSERT2(m_nWriters == 0);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


bool RWLockImpl::tryReadLock()
{
	HANDLE h[2];
	h[0] = m_hMutex;
	h[1] = m_hReadEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, 1))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++m_nReaders;
		ResetEvent(m_hWriteEvent);
		ReleaseMutex(m_hMutex);
		FIRTEX_ASSERT2(m_nWriters == 0);
		return true;
	case WAIT_TIMEOUT:
		return false;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


void RWLockImpl::writeLock()
{
	addWriter();
	HANDLE h[2];
	h[0] = m_hMutex;
	h[1] = m_hWriteEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--m_nWritersWaiting;
		++m_nReaders;
		++m_nWriters;
		ResetEvent(m_hReadEvent);
		ResetEvent(m_hWriteEvent);
		ReleaseMutex(m_hMutex);
		FIRTEX_ASSERT2(m_nWriters == 1);
		break;
	default:
		removeWriter();
		throw SystemException("cannot lock reader/writer lock");
	}
}


bool RWLockImpl::tryWriteLock()
{
	addWriter();
	HANDLE h[2];
	h[0] = m_hMutex;
	h[1] = m_hWriteEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, 1))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--m_nWritersWaiting;
		++m_nReaders;
		++m_nWriters;
		ResetEvent(m_hReadEvent);
		ResetEvent(m_hWriteEvent);
		ReleaseMutex(m_hMutex);
		FIRTEX_ASSERT2(m_nWriters == 1);
		return true;
	case WAIT_TIMEOUT:
		removeWriter();
		return false;
	default:
		removeWriter();
		throw SystemException("cannot lock reader/writer lock");
	}
}


void RWLockImpl::unlock()
{
	switch (WaitForSingleObject(m_hMutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		m_nWriters = 0;
		if (m_nWritersWaiting == 0) SetEvent(m_hReadEvent);
		if (--m_nReaders == 0) SetEvent(m_hWriteEvent);
		ReleaseMutex(m_hMutex);
		break;
	default:
		throw SystemException("cannot unlock reader/writer lock");
	}
}

FX_NS_END
