//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2006/5/20
//
#ifndef __FX_PRIORITYQUEUE_H
#define __FX_PRIORITYQUEUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(utility);

template <typename T>
struct DefaultDestroyer
{
    void operator() (T& elem)
    {
        delete elem;
        elem = NULL;
    }
};

template <typename T>
struct DummyDestroyer
{
    void operator() (T& elem)
    {
        //Nothing to do
    }
};

/**
 * A PriorityQueue maintains a partial ordering of its elements such that the
 * least element can always be found in constant time.  Put()'s and pop()'s
 *  require log(size) time. 
 */
template <typename T, typename Comp, typename Destroyer = DefaultDestroyer<T> > 
class PriorityQueue
{
public:
    typedef T ElemType;
    typedef Comp CompType;
    typedef Destroyer DestroyerType;
public:
    PriorityQueue(size_t maxSize, bool deleteOnClear, 
                  DestroyerType* pDestroyer = NULL,
                  const Comp& comp = Comp())
        : m_heap(NULL)
        , m_size(0)
        , m_bDelete(false)
        , m_maxSize(0)
        , m_pComparator(new Comp(comp))
        , m_pDestryer(pDestroyer)
        , m_bOwnDestroyer(false)
    {
        initialize(maxSize, deleteOnClear);
    }

    PriorityQueue(size_t maxSize, bool deleteOnClear, 
                  DestroyerType* pDestroyer,
                  Comp* pComp)
        : m_heap(NULL)
        , m_size(0)
        , m_bDelete(false)
        , m_maxSize(0)
        , m_pComparator(pComp)
        , m_pDestryer(pDestroyer)
        , m_bOwnDestroyer(false)
    {
        initialize(maxSize, deleteOnClear);
    }

    virtual ~PriorityQueue()
    {
        clear();
        delete[] m_heap;

        delete m_pComparator;

        if (m_bOwnDestroyer)
        {
            delete m_pDestryer;
        }
    }

private:
    void upHeap()
    {
        size_t i = m_size;
        ElemType node = m_heap[i];   // save bottom node (WAS object)
        int32_t j = ((uint32_t)i) >> 1;
        while (j > 0 && (*m_pComparator)(node, m_heap[j]))
        {
            m_heap[i] = m_heap[j];  // shift parents down
            i = j;
            j = ((uint32_t)j) >> 1;
        }
        m_heap[i] = node;	// install saved node
    }

    void downHeap()
    {
        size_t i = 1;
        ElemType node = m_heap[i]; // save top node
        size_t j = i << 1;	   // find smaller child
        size_t k = j + 1;
        if (k <= m_size && (*m_pComparator)(m_heap[k], m_heap[j])) 
        {
            j = k;
        }
        while (j <= m_size && (*m_pComparator)(m_heap[j],node)) 
        {
            m_heap[i] = m_heap[j];	 // shift up child
            i = j;
            j = i << 1;
            k = j + 1;
            if (k <= m_size && (*m_pComparator)(m_heap[k], m_heap[j]))
            {
                j = k;
            }
        }
        m_heap[i] = node;
    }

protected:
    /**
     * Subclass constructors must call this. 
     */
    void initialize(size_t maxSize, bool deleteOnClear)
    {
        m_size = 0;
        m_bDelete = deleteOnClear;
        m_maxSize = maxSize;
        size_t heapSize = m_maxSize + 1;
        m_heap = new ElemType[heapSize];			
    }

public:
    /**
     * Adds an Object to a PriorityQueue in log(size) time.
     * If one tries to add more objects than m_maxSize from initialize
     * a RuntimeException (ArrayIndexOutOfBound) is thrown.
     */
    void put(ElemType element)
    {
        if(m_size >= m_maxSize)
        {
            FIRTEX_THROW(OutOfRangeException,
                         _T("PriorityQueue::put():add is out of bounds"));
        }

        m_size++;	
        m_heap[m_size] = element;		
        upHeap();
    }

    /**
     * Adds element to the PriorityQueue in log(size) time if either
     * the PriorityQueue is not full, or not comparator(element, top()).
     * @param element
     * @return true if element is added, false otherwise.
     */
    bool insert(ElemType element)
    {
        if(m_size < m_maxSize)
        {
            put(element);
            return true;
        }
        else if(m_size > 0 && !(*m_pComparator)(element, top()))
        {
            if (m_bDelete)
            {
                (*m_pDestryer)(m_heap[1]);
            }
            m_heap[1] = element;
            adjustTop();
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * Returns the least element of the PriorityQueue in constant time. 
     */
    ElemType top() const
    {
        if (m_size > 0)
            return m_heap[1];
        else
            return NULL;
    }

    /** 
     * Removes and returns the least element of the PriorityQueue in log(size) time.  
     */
    ElemType pop()
    {
        if (m_size > 0) 
        {
            ElemType result = m_heap[1];			  // save first value
            m_heap[1] = m_heap[m_size];			  // move last to first

            m_heap[m_size] = (ElemType)0;			  // permit GC of objects
            m_size--;
            downHeap();				  // adjust m_heap
            return result;
        }
        else
            return (ElemType)NULL;
    }

    /**
     * Should be called when the object at top changes values.  Still log(n)
     * worst case, but it's at least twice as fast to <pre>
     * { pq.top().change(); pq.adjustTop(); }
     * </pre> instead of <pre>
     * { o = pq.pop(); o.change(); pq.push(o); }
     * </pre>
     */
    void adjustTop()
    {
        downHeap();
    }


    /**
     * Returns the number of elements currently stored in the PriorityQueue.
     */ 
    size_t size() const
    {
        return m_size;
    }

    /** 
     * Removes all entries from the PriorityQueue. 
     */
    void clear()
    {
        for (size_t i = 1; i <= m_size; i++)
        {
            if (m_bDelete)
            {
                (*m_pDestryer)(m_heap[i]);
            }
        }
        m_size = 0;
    }

    /** return element by position */
    ElemType operator [] (size_t _pos) const
    {
        return m_heap[_pos+1];
    }

    ElemType getAt(size_t _pos) const
    {
        return m_heap[_pos+1];
    }

private:
    ElemType* m_heap; 
    size_t m_size;
    bool m_bDelete;
    size_t m_maxSize;
    CompType* m_pComparator;
    DestroyerType* m_pDestryer;
    bool m_bOwnDestroyer;
};

FX_NS_END

#endif
