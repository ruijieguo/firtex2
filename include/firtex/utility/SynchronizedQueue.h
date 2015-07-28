//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-02-15 19:49:36

#ifndef __FX_SYNCHRONIZEDQUEUE_H
#define __FX_SYNCHRONIZEDQUEUE_H

#include <deque>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/Event.h"
#include "firtex/thread/FastMutex.h"

FX_NS_DEF(utility);

template <typename T>
class SynchronizedQueue
{
public:
    typedef T ElemType;

public:
    SynchronizedQueue();
    ~SynchronizedQueue();

public:
    /**
     * Enqueues the element by adding it to
     * the end of the queue (FIFO). 
     * @param elem element to enqueue.
     */
    void enqueue(const ElemType& elem);

    /**
     * Dequeues the element at the front of the queue
     * @return the front element in queue, or NULL if no element
     * is available. 
     */
    ElemType dequeue();

    /**
     * Dequeues the element at the front of queue
     * If no element is available, waits for an element
     * to be enqueued. 
     * @return the front element
     */
    ElemType waitDequeue();

    /**
     * Dequeues the element at the front of queue
     * If no notification is available, waits for a notification
     * to be enqueued up to the specified time.
     * @param milliseconds wait time
     * @return the front element
     */
    ElemType waitDequeue(int64_t milliseconds);

    /**
     * Wakes up all threads that wait for an element.
     */
    void wakeUpAll();

    /** 
     * Returns true iff the queue is empty.
     */
    bool empty() const;

    /** 
     * Returns the number of elements in the queue.
     */
    size_t size() const;

    /** 
     * Removes all elements from the queue.
     */
    void clear();

protected:
    ElemType dequeueOne();

private:
    typedef std::deque<ElemType> ElemQueue;

    struct WaitInfo
    {
        ElemType element;
        FX_NS(thread)::Event availEvent;
    };

    typedef std::deque<WaitInfo*> WaitQueue;

    ElemQueue m_elemQueue;
    WaitQueue m_waitQueue;
    mutable FX_NS(thread)::FastMutex m_mutex;
};

//////////////////////////////////////////
//
template <typename T>
SynchronizedQueue<T>::SynchronizedQueue()
{
}

template <typename T>
SynchronizedQueue<T>::~SynchronizedQueue()
{
    clear();
}

template <typename T>
void SynchronizedQueue<T>::enqueue(const ElemType& elem)
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    if (m_waitQueue.empty())
    {
        m_elemQueue.push_back(elem);
    }
    else
    {
        WaitInfo* pWI = m_waitQueue.front();
        m_waitQueue.pop_front();
        pWI->element = elem;
        pWI->availEvent.set();
    }
}

template <typename T>
typename SynchronizedQueue<T>::ElemType SynchronizedQueue<T>::dequeue()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    return dequeueOne();
}

template <typename T>
typename SynchronizedQueue<T>::ElemType SynchronizedQueue<T>::waitDequeue()
{
    ElemType elem;
    WaitInfo* pWI = NULL;
    {
        FX_NS(thread)::FastMutex::Guard lock(m_mutex);
        elem = dequeueOne();
        if (elem)
        {
            return elem;
        }
        pWI = new WaitInfo;
        m_waitQueue.push_back(pWI);
    }
    pWI->availEvent.wait();
    elem = pWI->element;
    delete pWI;
    return elem;
}

template <typename T>
typename SynchronizedQueue<T>::ElemType SynchronizedQueue<T>::waitDequeue(int64_t milliseconds)
{
    ElemType elem;
    WaitInfo* pWI = NULL;
    {
        FX_NS(thread)::FastMutex::Guard lock(m_mutex);
        elem = dequeueOne();
        if (elem) 
        {
            return elem;
        }
        pWI = new WaitInfo;
        m_waitQueue.push_back(pWI);
    }
    if (pWI->availEvent.tryWait(milliseconds))
    {
        elem = pWI->element;
    }
    else
    {
        FX_NS(thread)::FastMutex::Guard lock(m_mutex);
        elem = pWI->element;
        for (typename WaitQueue::iterator it = m_waitQueue.begin();
             it != m_waitQueue.end(); ++it)
        {
            if (*it == pWI)
            {
                m_waitQueue.erase(it);
                break;
            }
        }
    }
    delete pWI;
    return elem;
}

template <typename T>
void SynchronizedQueue<T>::wakeUpAll()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    for (typename WaitQueue::iterator it = m_waitQueue.begin();
         it != m_waitQueue.end(); ++it)
    {
        (*it)->availEvent.set();
    }
    m_waitQueue.clear();
}

template <typename T>
bool SynchronizedQueue<T>::empty() const
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    return m_elemQueue.empty();
}

template <typename T>
size_t SynchronizedQueue<T>::size() const
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    return m_elemQueue.size();
}

template <typename T>
void SynchronizedQueue<T>::clear()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    m_elemQueue.clear();	
}

template <typename T>
typename SynchronizedQueue<T>::ElemType SynchronizedQueue<T>::dequeueOne()
{
    ElemType elem;
    if (!m_elemQueue.empty())
    {
        elem = m_elemQueue.front();
        m_elemQueue.pop_front();
    }
    return elem;
}

FX_NS_END

#endif //__FX_SYNCHRONIZEDQUEUE_H
