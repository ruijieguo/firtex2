#include "firtex/thread/Condition.h"

FX_NS_DEF(thread);

Condition::Condition()
{
}

Condition::~Condition()
{
}

void Condition::signal()
{
    FastMutex::Guard lock(m_mutex);
    
    if (!m_waitQueue.empty())
    {
        m_waitQueue.front()->set();
        dequeue();
    }
}

void Condition::broadcast()
{
    FastMutex::Guard lock(m_mutex);
    
    for (WaitQueue::iterator it = m_waitQueue.begin();
         it != m_waitQueue.end(); ++it)
    {
        (*it)->set();
    }
    m_waitQueue.clear();
}

void Condition::enqueue(Event& event)
{
    m_waitQueue.push_back(&event);
}

void Condition::dequeue()
{
    m_waitQueue.pop_front();
}

void Condition::dequeue(Event& event)
{
    for (WaitQueue::iterator it = m_waitQueue.begin();
         it != m_waitQueue.end(); ++it)
    {
        if (*it == &event)
        {
            m_waitQueue.erase(it);
            break;
        }
    }
}

FX_NS_END

