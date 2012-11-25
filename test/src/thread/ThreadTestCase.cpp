#include "ThreadTestCase.h"
#include "firtex/thread/Thread.h"
#include "firtex/thread/Event.h"
#include <stdio.h>
#include "TestHelper.h"

FX_NS_DEF(thread);

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadTestCase);

class MyRunner: public Runnable
{
public:
	MyRunner(): _ran(false)
	{
	}
	
	void run()
	{
		_ran = true;
		_event.wait();
	}
	
	bool ran() const
	{
		return _ran;
	}
	
	void notify()
	{
		_event.set();
	}
private:
	bool _ran;
	Event _event;
};

ThreadTestCase::ThreadTestCase(void)
{	
}

ThreadTestCase::~ThreadTestCase(void)
{	
}

void ThreadTestCase::setUp()
{
}

void ThreadTestCase::tearDown()
{
}

void ThreadTestCase::testThread()
{
	Thread thread;
	MyRunner r;
	CPPUNIT_ASSERT (!thread.isRunning());
	thread.start(r);
	Thread::sleep(200);
	CPPUNIT_ASSERT (thread.isRunning());
	r.notify();
	thread.join();
	CPPUNIT_ASSERT (!thread.isRunning());
	CPPUNIT_ASSERT (r.ran());
}

void ThreadTestCase::testNamedThread()
{
	Thread thread("MyThread");
	MyRunner r;
	thread.start(r);
	r.notify();
	thread.join();
	CPPUNIT_ASSERT (r.ran());
}

void ThreadTestCase::testThreads()
{
	Thread thread1("Thread1");
	Thread thread2("Thread2");
	Thread thread3("Thread3");
	Thread thread4("Thread4");

	MyRunner r1;
	MyRunner r2;
	MyRunner r3;
	MyRunner r4;
	CPPUNIT_ASSERT (!thread1.isRunning());
	CPPUNIT_ASSERT (!thread2.isRunning());
	CPPUNIT_ASSERT (!thread3.isRunning());
	CPPUNIT_ASSERT (!thread4.isRunning());
	thread1.start(r1);
	Thread::sleep(200);
	CPPUNIT_ASSERT (thread1.isRunning());
	CPPUNIT_ASSERT (!thread2.isRunning());
	CPPUNIT_ASSERT (!thread3.isRunning());
	CPPUNIT_ASSERT (!thread4.isRunning());
	thread2.start(r2);
	thread3.start(r3);
	thread4.start(r4);
	Thread::sleep(200);
	CPPUNIT_ASSERT (thread1.isRunning());
	CPPUNIT_ASSERT (thread2.isRunning());
	CPPUNIT_ASSERT (thread3.isRunning());
	CPPUNIT_ASSERT (thread4.isRunning());
	r4.notify();
	thread4.join();
	CPPUNIT_ASSERT (!thread4.isRunning());
	CPPUNIT_ASSERT (thread1.isRunning());
	CPPUNIT_ASSERT (thread2.isRunning());
	CPPUNIT_ASSERT (thread3.isRunning());
	r3.notify();
	thread3.join();
	CPPUNIT_ASSERT (!thread3.isRunning());
	r2.notify();
	thread2.join();
	CPPUNIT_ASSERT (!thread2.isRunning());
	r1.notify();
	thread1.join();
	CPPUNIT_ASSERT (!thread1.isRunning());
	CPPUNIT_ASSERT (r1.ran());
	CPPUNIT_ASSERT (r2.ran());
	CPPUNIT_ASSERT (r3.ran());
	CPPUNIT_ASSERT (r4.ran());
}

void ThreadTestCase::testJoin()
{
	Thread thread;
	MyRunner r;
	CPPUNIT_ASSERT (!thread.isRunning());
	thread.start(r);
	Thread::sleep(200);
	CPPUNIT_ASSERT (thread.isRunning());
	CPPUNIT_ASSERT (!thread.tryJoin(100));
	r.notify();
	CPPUNIT_ASSERT (thread.tryJoin(500));
	CPPUNIT_ASSERT (!thread.isRunning());
}

void ThreadTestCase::testThreadStackSize()
{
	size_t stackSize = 50000000;

	Thread thread;
	CPPUNIT_ASSERT (0 == thread.getStackSize());
	thread.setStackSize(stackSize);
#if !defined(PTHREAD_STACK_MIN) && !defined(FX_WINDOWS)
	CPPUNIT_ASSERT (0 == thread.getStackSize());
#else
	CPPUNIT_ASSERT (stackSize <= thread.getStackSize());
#endif

    MyRunner r1;
	thread.start(r1);
    r1.notify();
	thread.join();
	CPPUNIT_ASSERT (r1.ran());

	stackSize = 1;
	thread.setStackSize(stackSize);
#ifdef PTHREAD_STACK_MIN
	CPPUNIT_ASSERT (PTHREAD_STACK_MIN == thread.getStackSize());
#else
	CPPUNIT_ASSERT (stackSize >= thread.getStackSize());
#endif
    MyRunner r2;
	thread.start(r2);
    r2.notify();
	thread.join();
	CPPUNIT_ASSERT (r2.ran());

	thread.setStackSize(0);
	CPPUNIT_ASSERT (0 == thread.getStackSize());
    MyRunner r3;
	thread.start(r3);
    r3.notify();
	thread.join();
	CPPUNIT_ASSERT (r3.ran());
}

FX_NS_END
