AM_CPPFLAGS = -I $(top_srcdir)/include

noinst_LTLIBRARIES = libthread.la

libthread_la_SOURCES = Event.cpp\
	FastMutex.cpp\
	Mutex_POSIX.cpp\
	RWLock_POSIX.cpp\
	Thread.cpp\
	ThreadLocal.cpp\
	Event_POSIX.cpp\
	FastMutex_POSIX.cpp\
	Mutex.cpp\
	RWLock.cpp\
	Condition.cpp\
	StoppableObject.cpp\
	ThreadPool.cpp


EXTRA_DIST = \
	Thread_POSIX.cpp\
	Thread_WIN32.cpp\
	Event_WIN32.cpp\
	Mutex_WIN32.cpp\
	RWLock_WIN32.cpp

