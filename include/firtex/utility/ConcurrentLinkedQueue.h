//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-28 20:44:39

#ifndef __FX_CONCURRENTLINKEDQUEUE_H
#define __FX_CONCURRENTLINKEDQUEUE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/thread/Atomic.h"
#include "firtex/thread/HPSMR.h"

FX_NS_DEF(utility);

/// Queue internal statistics. Used for debugging or profiling
struct QueueStatistics 
{
    typedef FX_NS(thread)::EventCounter EventCounter;

    EventCounter m_enqueueCount;    ///< Enqueue call count
    EventCounter m_dequeueCount;    ///< Dequeue call count
    EventCounter m_enqueueRace;    ///< Count of enqueue race conditions encountered
    EventCounter m_dequeueRace;    ///< Count of dequeue race conditions encountered
    EventCounter m_advanceTailError; ///< Count of "advance tail failed" events
    EventCounter m_badTail;         ///< Count of events "Tail is not pointed to the last item in the queue"

    /// Register enqueue call
    void onEnqueue() { ++m_enqueueCount; }

    /// Register dequeue call
    void onDequeue() { ++m_dequeueCount; }

    /// Register enqueue race event
    void onEnqueueRace() { ++m_enqueueRace; }

    /// Register dequeue race event
    void onDequeueRace() { ++m_dequeueRace; }

    /// Register "advance tail failed" event
    void onAdvanceTailFailed() { ++m_advanceTailError; }

    /// Register event "Tail is not pointed to last item in the queue"
    void onBadTail()  { ++m_badTail; }
};

/// Empty queue statistics
struct DummyQueueStatistics 
{
    void onEnqueue() {}
    void onDequeue() {}
    void onEnqueueRace() {}
    void onDequeueRace() {}
    void onAdvanceTailFailed() {}
    void onBadTail() {}
};

/// Traits struct defines types for queue implementations
struct Traits
{
    typedef FX_NS(thread)::AtomicCounter<FX_NS(thread)::membar_relaxed, FX_NS(thread)::membar_relaxed> AtomicCounterType;
    typedef DummyQueueStatistics StatType;
};

/**
 * @brief implement of non-blocking concurrent queue algorithm
 *  (http://www.cs.rochester.edu/research/synchronization/pseudocode/queues.html)
 */

template <typename T, typename TRAITS = Traits>
class ConcurrentLinkedQueue
{
public:
    typedef T ElemType;
    typedef Traits::AtomicCounterType AtomicCounterType;
    typedef Traits::StatType StatType;

    template <typename K>
    struct Node;

    typedef Node<ElemType> NodeType;

    typedef FX_NS(thread)::Atomic<NodeType*> AtomicNodeType;

    template <typename K>
    struct Node
    {
        AtomicNodeType nextNode;
        FX_NS(thread)::Atomic<K> data;

        Node() : nextNode(NULL) {}
        Node(const K& v) 
            : nextNode(NULL)
        {
            lazySetData(v);
        }

        K& getData()
        {
            return data;
        }

        const K& getData() const
        {
            return data;
        }
        
        bool casData(const K& cmp, const K& val)
        {
            return data.template cas<FX_NS(thread)::membar_release>(cmp, val);
        }
        
        void setData(const K& dat)
        {
            data = dat;
        }

        void lazySetData(const K& dat)
        {
            data.template store<FX_NS(thread)::membar_relaxed>(dat);
        }

        void lazySetNext(Node* n) 
        {
            nextNode.template store<FX_NS(thread)::membar_relaxed>(n);
        } 

        // Node* getNext() 
        // {
        //     return nextNode;
        // }

        template <typename ORDER>
        Node* getNext()
        {
            return nextNode.template load<ORDER>();
        }

        template <typename ORDER>
        bool casNext(Node* cmp, Node* val) 
        {
            return nextNode.template cas<ORDER>(cmp, val);
        }
    };
    
    static NodeType* allocateNode(const ElemType& v)
    {
        return new NodeType(v);
    }

    static NodeType* allocateNode()
    {
        return new NodeType();
    }

    static void deallocateNode(NodeType* pNode)
    {
        delete pNode;
    }

    /**
     * @brief a state holder for storing the pointer of the current node and 
     * the pointers of the current node's previous and next in it.
     */
    struct FindStateHolder 
    {
    public:
        NodeType* prev;
        NodeType* current;
        NodeType* next;
        bool isFound;

        FindStateHolder() 
            : prev(NULL), current(NULL), next(NULL), isFound(false) 
        {
        }
    };

    typedef FX_NS(thread)::EventCounter EventCounter;
    typedef typename FX_NS(thread)::AutoHPArray<2>::template rebind<2>::other HPGuardArray2;
    typedef typename FX_NS(thread)::AutoHPArray<3>::template rebind<3>::other HPGuardArray3;

    /// Generic algorithm for clearing queue \p q in lock-free manner
    template <class Q>
    size_t genericClear(Q& q)
    {
        typename Q::ElemType dest;
        size_t nRemoved = 0;
        while (q.dequeue(dest))
            ++nRemoved;
        return nRemoved;
    }

public:
    ConcurrentLinkedQueue()
    {
        NodeType* pNode = allocateNode();
        m_pHead.template store<FX_NS(thread)::membar_relaxed>(pNode);
        m_pTail.template store<FX_NS(thread)::membar_relaxed>(pNode);
    }

    ~ConcurrentLinkedQueue()
    {
        clear();

        NodeType* pHead = m_pHead.template load<FX_NS(thread)::membar_relaxed>();
        FIRTEX_ASSERT2(pHead != NULL);
        FIRTEX_ASSERT2(pHead == m_pTail.template load<FX_NS(thread)::membar_relaxed>());
        
        getGC().retirePtr(pHead, &ConcurrentLinkedQueue::deallocateNode);
        m_pHead.template store<FX_NS(thread)::membar_relaxed>(reinterpret_cast<NodeType*>(NULL));
        m_pTail.template store<FX_NS(thread)::membar_relaxed>(reinterpret_cast<NodeType*>(NULL));
    }

public:
    size_t size() const
    {
        return m_itemCounter.value();
    }

    void enqueue(const ElemType& data)
    {
        FX_NS(thread)::AutoHPGuard grd(getGC());

        NodeType* pNew = allocateNode(data);
        NodeType* pTail;
        while (true) 
        {
            grd = pTail = getTail<FX_NS(thread)::membar_relaxed>();
            if (getTail<FX_NS(thread)::membar_acquire>() != pTail) 
            {
                continue;
            }

            NodeType* pNext = pTail->template getNext<FX_NS(thread)::membar_acquire>();
            if (pNext != NULL) 
            {
                // Tail is misplaced, advance it
                casTail<FX_NS(thread)::membar_release>(pTail, pNext);
                m_stat.onBadTail();
                continue;
            }
            if (pTail->template casNext<FX_NS(thread)::membar_release>(
                            static_cast<NodeType*>(NULL), pNew))
            {
                break;
            }
            m_stat.onEnqueueRace();
        }
        ++m_itemCounter;
        m_stat.onEnqueue();

        if (!casTail<FX_NS(thread)::membar_acq_rel>(pTail, pNew))
            m_stat.onAdvanceTailFailed();
    }

    bool dequeue(ElemType& data)
    {
        HPGuardArray2 hpArr(getGC());

        NodeType* pHeadNext;
        NodeType* pHead;
        while (true) 
        {
            pHead = getHead<FX_NS(thread)::membar_relaxed>();
            hpArr[0] = pHead;
            if (getHead<FX_NS(thread)::membar_acquire>() != pHead)
            {
                continue;
            }

            pHeadNext = pHead->template getNext<FX_NS(thread)::membar_acquire>();
            hpArr[1] = pHeadNext;
            if (pHeadNext == NULL)
                return false;    // empty queue

            if (getHead<FX_NS(thread)::membar_acquire>() != pHead)
                continue;

            NodeType* pTail = getTail<FX_NS(thread)::membar_acquire>();
            if (pHead == pTail) 
            {
                m_stat.onBadTail();
                //try to advance pTail
                casTail<FX_NS(thread)::membar_release>(pTail, pHeadNext);
                continue;
            }
            if (casHead<FX_NS(thread)::membar_release>(pHead, pHeadNext))
                break;
                                                                
            m_stat.onDequeueRace();
        }
                            
        data = pHeadNext->data.template load<FX_NS(thread)::membar_relaxed>();
        --m_itemCounter;
        m_stat.onDequeue();
        hpArr.getGC().retirePtr(pHead, deallocateNode);
        return true;
    }

    /**
     * Remove a specified element
     *
     * @param e The element to be removed
     * @return Remove the data and return true if exists, otherwise return false.
     */
    bool remove(const ElemType& e) 
    {
        HPGuardArray3 hpArr(getGC());

        FindStateHolder holder = FindStateHolder();
        bool result;
        while (true) 
        {
            if (!find(e, hpArr, holder))
            {
                result = false;
                break;
            }
		
            /**
             * CAS operation, mark current as deleted.
             * if successful, the thread attempts to remove it at next step.
             * else, it implies that one or more of three events must have taken place since the snapshot
             * in Find method was taken. Then goto the start of this while loop
             */
            if (!holder.current->template casNext<FX_NS(thread)::membar_release>(holder.next,
                            markPointer(holder.next))) 
            {
                continue;
            }

            if (holder.next == NULL)
            {
                casTail<FX_NS(thread)::membar_release>(holder.current, holder.prev);
            }

            /// Set the next pointer of prev to the next pointer of current.
            /// If successful, delete current in SMR.
            /// If failed, it implies that another thread must have removed the 
            /// node from the list after the success of the MARK CAS before.
            assert(!isMarked(holder.next));
            if (holder.prev->template casNext<FX_NS(thread)::membar_release>(
                            holder.current, holder.next))
            {
                hpArr.getGC().retirePtr(holder.current, deallocateNode);
            }
            else 
            {
                find(e, hpArr, holder);
            }
            result = true;
            --m_itemCounter;
            break;
        }
        //retireAll(hp);
        return result;

    }

    bool push(const ElemType& data)
    {
        return enqueue(data);
    }
    
    bool pop(ElemType& data)
    {
        return dequeue(data);
    }
    
    bool empty() const
    {
        return m_pTail.template load<FX_NS(thread)::membar_acquire>() == 
            m_pHead.template load<FX_NS(thread)::membar_acquire>();
    }

    size_t clear()
    {
        return genericClear(*this);
    }

    const StatType& getStatistics() const
    {
        return m_stat;
    }

private:
    /// Clear the last two bits of pointers
    inline static NodeType* restorePointer(NodeType* p)
    {
        return ((NodeType*)(((long)(p))&(~3)));
    }

    /// Set the last bit of pointers */
    inline static NodeType* markPointer(NodeType* p)
    {
        return ((NodeType*)(((long)(p))|(1)));
    }
    
    /// Get the last bit of pointers */
    inline static bool isMarked(NodeType* p) 
    {
        return ((long)(p)) & (1);
    }


private:
    template <typename ORDER>
    inline NodeType* getTail()
    {
        return m_pTail.template load<ORDER>();
    }
    
    template <typename ORDER>
    inline bool casTail(NodeType* cmp, NodeType* val)
    {
        return m_pTail.template cas<ORDER>(cmp, val);
    }

    template <typename ORDER>
    inline NodeType* getHead()
    {
        return m_pHead.template load<ORDER>();
    }

    template <typename ORDER>
    inline bool casHead(NodeType* cmp, NodeType* val)
    {
        return m_pHead.template cas<ORDER>(cmp, val);
    }

    inline bool find(const ElemType& key, HPGuardArray3& hpArr,
                     FindStateHolder& holder) 
    {
        while (true)
        {
            NodeType* pNext = NULL;
            NodeType* pPrev = getHead<FX_NS(thread)::membar_acquire>();
            hpArr[0] = pPrev;
            if (getHead<FX_NS(thread)::membar_acquire>() != pPrev)
            {
                continue;
            }

            NodeType* pCur = pPrev->template getNext<FX_NS(thread)::membar_acquire>();
            if (pCur == NULL)
            {
                return false;
            }

            assert(!isMarked(pCur));
            hpArr[1] = pCur;
            
            if (pPrev->template getNext<FX_NS(thread)::membar_relaxed>() != pCur)
                continue;

            while (true)
            {
                if (NULL == pCur)
                {
                    return false;
                }

                assert(!isMarked(pCur));
                NodeType* markedNext = pCur->template getNext<FX_NS(thread)::membar_relaxed>();
                bool cmark = isMarked(markedNext);
                pNext = restorePointer(markedNext);
                hpArr[2] = pNext;

                if (pCur->template getNext<FX_NS(thread)::membar_relaxed>() != markedNext) 
                    break;

                ElemType cKey = pCur->data.template load<FX_NS(thread)::membar_relaxed>();
                // If the holder.pre changed, start over from the beginning
                if (pPrev->template getNext<FX_NS(thread)::membar_relaxed>() != pCur)
                    break;

                if (!cmark)
                {
                    if (cKey == key) 
                    {
                        holder.isFound = true;
                        holder.prev = pPrev;
                        holder.current = pCur;
                        holder.next = pNext;
                        return true;
                    }
                    pPrev = pCur;
                    hpArr[0] = pPrev;
                }
                else 
                {
                    /// The thread encounters a node marked to be deleted, 
                    /// it attempts to remove it from the list
                    assert(!isMarked(pNext));
                    if (pPrev->template casNext<FX_NS(thread)::membar_relaxed>(pCur, pNext)) 
                    {
                        hpArr.getGC().retirePtr(pCur, deallocateNode);
                    }
                    else 
                    {
                        break;
                    }
                }
                pCur = pNext;
                hpArr[1] = pCur;
            }//end while
        }//end while
    }

private:
    inline FX_NS(thread)::ThreadGC& getGC()
    {
        return m_smr.getHPSMR();
    }

private:
    // const static size_t CACHE_LINE_SIZE = 64;
    // typedef typename FX_NS(thread)::AlignedType<Atomic<NodeType*>, c_nCacheLineSize >::type  AlignedNodePtr;
    
    AtomicNodeType m_pHead;
    AtomicNodeType m_pTail;
    
    StatType m_stat;
    AtomicCounterType m_itemCounter;

    FX_NS(thread)::HPSMR m_smr;
};

FX_NS_END

#endif //__FX_CONCURRENTLINKEDQUEUE_H
