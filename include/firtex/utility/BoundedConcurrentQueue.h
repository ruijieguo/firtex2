//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-01-02 16:57:54

#ifndef __FX_BOUNDEDCONCURRENTQUEUE_H
#define __FX_BOUNDEDCONCURRENTQUEUE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/Condition.h"
#include <deque>

FX_NS_DEF(utility);

template<typename T>
class BoundedConcurrentQueue
{
public:
    BoundedConcurrentQueue(size_t nCapacity);
    ~BoundedConcurrentQueue();

public:
    /**
     * Return capacity of this queue
     */
    size_t getCapacity() const;

    /**
     * Push element to the back of queue. 
     * Blocked if the queue is full.
     */
    void enqueue(const T& element);
    
    /**
     * Return front element of queue
     */
    T getFront() const;

    /**
     * Pop front element of queue
     */
    void popFront();

    /**
     * Get and pop front element of queue
     */
    T dequeue();

    /**
     * Try to get and pop front element of queue
     * @param element returned element
     * @return false if no element in the queue
     */
    bool tryDequeue(T& element);

    /**
     * Wait and get front element. Blocked if no element in the queue
     */
    T waitDequeue();
    
    /// Wait until empty
    void waitEmpty();

    /// Wait until not empty
    void waitNotEmpty();

    /**
     * Return true if empty
     */ 
    bool isEmpty() const;

    /**
     * Return size of queue
     */
    uint32_t getSize() const;

    /**
     * Wake up all blocked thread
     */
    void wakeup();

    /*
     * Clear all elements in queue
     */
    void clear();

public:
    static const uint32_t DEF_WAIT_TIME = 500; // 0.5 second
    static const uint32_t DEF_CAPACITY = 10;

private:
    size_t m_nCapacity;
    std::deque<T> m_elements;
    FX_NS(thread)::Condition m_cond;
    mutable FX_NS(thread)::FastMutex m_mutex;
};

///////////////////////////////////////////////////
//

template<class T>
BoundedConcurrentQueue<T>::BoundedConcurrentQueue(size_t nCapacity)
    : m_nCapacity(nCapacity)
{
}

template<class T>
BoundedConcurrentQueue<T>::~BoundedConcurrentQueue() 
{
}

template<class T>
inline size_t BoundedConcurrentQueue<T>::getCapacity() const
{
    return m_nCapacity;
}

template<class T>
void BoundedConcurrentQueue<T>::enqueue(const T& element)
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    while (m_elements.size() >= m_nCapacity)
    {
        m_cond.wait(m_mutex);
    }
    m_elements.push_back(element);
    m_cond.signal();
}

template<class T>
T BoundedConcurrentQueue<T>::getFront() const
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    const T &element = m_elements.front();
    return element;
}

template<class T>
void BoundedConcurrentQueue<T>::popFront()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    m_elements.pop_front();
    m_cond.signal();
}

template<class T>
T BoundedConcurrentQueue<T>::dequeue()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);

    const T element = m_elements.front();
    m_elements.pop_front();
    m_cond.signal();

    return element;
}

template<class T>
bool BoundedConcurrentQueue<T>::tryDequeue(T& element)
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    
    if (m_elements.empty())
    {
        return false;
    }
    else
    {
        element = m_elements.front();
        m_elements.pop_front();
        m_cond.signal();
        
        return true;
    }
}

template<class T>
T BoundedConcurrentQueue<T>::waitDequeue()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    
    while (m_elements.empty())
    {
        m_cond.tryWait(m_mutex, DEF_WAIT_TIME);
    }

    T element = m_elements.front();
    m_elements.pop_front();
    m_cond.signal();
    return element;
}

template<class T>
void BoundedConcurrentQueue<T>::waitEmpty()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
   
    while (m_elements.size() > 0)
    {
        m_cond.tryWait(m_mutex, DEF_WAIT_TIME);
    }
}

template<class T>
void BoundedConcurrentQueue<T>::waitNotEmpty()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    
    while (m_elements.empty())
    {
        m_cond.tryWait(m_mutex, DEF_WAIT_TIME);
    }
}

template<class T>
bool BoundedConcurrentQueue<T>::isEmpty() const
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    return m_elements.empty();
}

template<class T>
uint32_t BoundedConcurrentQueue<T>::getSize() const
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    return (uint32_t)m_elements.size();
}

template<class T>
void BoundedConcurrentQueue<T>::wakeup()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    m_cond.broadcast();
}

template<class T>
void BoundedConcurrentQueue<T>::clear()
{
    FX_NS(thread)::FastMutex::Guard lock(m_mutex);
    m_elements.clear();
}

FX_NS_END

#endif //__FX_BOUNDEDCONCURRENTQUEUE_H
