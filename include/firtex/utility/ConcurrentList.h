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
// Created : 2011-08-13 00:32:00

#ifndef __FX_CONCURRENTLIST_H
#define __FX_CONCURRENTLIST_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(utility);

/*
 * Number of pointers protected by HPSMR.
 */
#define NHPOINTER 3

/* clear the last two bits of pointers */
#define POINTER(p) ((NodeType<KeyType>*)(((long)(p))&(~3)))

/* set the last bit of pointers */
#define MARK(p) ((NodeType<KeyType>*)(((long)(p))|(1)))

/* get the last bit of pointers */
#define MARKED(p) (((long)(p))&(1))


/**
 * @brief This is an implementation of a lock-free unbounded linked list data structure. 
 * 
 * Reference: High Performance Dynamic Lock-Free Hash Tables and List-Based Sets
 */
template<typename T>
class List 
{
public:
    typedef T KeyType;
    typedef typename SMR<Node<KeyType>, NHPOINTER>::HP_Rec HazardP;

    struct Node
    {
    public:
        typedef T KeyType;

    public:
        KeyType data;
        FX_NS(thread)::Atomic<Node*> next;

        Node() 
        {
            setNext<FX_NS(thread)::membar_relaxed>(NULL);
        }

        Node(const KeyType& val) 
            : data(val) 
        {
            setNext<FX_NS(thread)::membar_relaxed>(NULL);
        }

        template<typename ORDER>
        inline void setNext(Node* p)
        {
            next.template store<ORDER>(p);
        }

        template <typename ORDER>
        Node* getNext()
        {
            return next.template load<ORDER>();
        }
    };

    static Node* allocateNode()
    {
        return new Node();
    }

    static void deallocateNode(Node* pNode)
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
        FX_NS(thread)::Atomic<Node*>* prev;
        Node* cur;
        Node* next;
        bool isFound;

        FindStateHolder() 
            : prev(NULL), cur(NULL), next(NULL), isFound(false) 
        {
        }
    };


public:
    List() 
    {
        //mm = getSMR<Node<KeyType> , NHPOINTER> (); //for memory management, 1 hazard pointer is OK
        m_pDummyNode = allocateNode();
        setHead<FX_NS(thread)::membar_relaxed>(m_pDummyNode->getNext<FX_NS(thread)::membar_relaxed>());
    }

    ~List() 
    {
        Node* pTmp = getHead<FX_NS(thread)::membar_relaxed>();
        while (pTmp != NULL)
        {
            Node* pTmpKeep = POINTER(pTmp);
            FIRTEX_ASSERT2(NULL != pTmpKeep);
            pTmp = pTmpKeep->getNext<FX_NS(thread)::membar_relaxed>();
            FIRTEX_ASSERT2(!MARKED(pTmpKeep));
            deallocateNode(pTmpKeep);
        }

        deallocateNode(m_pDummyNode);
    }

    bool front(KeyType& ret) 
    {
        typename FX_NS(thread)::AutoHPArray<2>::template rebind<2>::other hpArr(getGC());

        HazardP * hp = mm->getHPRec();
            
        Node* first = getHead<FX_NS(thread)::membar_relaxed>();
        while(true)
        {
            if (first == NULL)
                return false;
            
            if (MARKED(first))
            { 
                mm->employ(hp, 0, first);
                if (first != getHead<FX_NS(thread)::membar_relaxed>())
                {
                    first = getHead<FX_NS(thread)::membar_relaxed>();
                    continue;
                }

                first = first->getNext<FX_NS(thread)::membar_relaxed>();
                ///TODO
                mm->retire(hp, 0);
                continue;
            }
            FIRST_ASSERT2(!MARKED(first));
            ret = first->data;
            return true;
        }
    }

    size_t size() 
    {
        size_t ret = 0;
        Node* curr = getHead<FX_NS(thread)::membar_relaxed>();
        while (curr != NULL) 
        {
            if (!MARKED(curr))
                ++ret;
            curr = POINTER(curr)->getNext<FX_NS(thread)::membar_relaxed>();
        }
        return ret;
    }

    bool pushFront(const KeyType& e) 
    {
        Node* node = allocateNode();

        bool result;
        while (true)
        {
            Node* cur = getHead<FX_NS(thread)::membar_relaxed>();
            node->setNext<FX_NS(thread)::membar_relaxed>(cur);

            if (casHead<FX_NS(thread)::membar_release>(cur, node))
            {
                result = true;
                break;
            }
        }
        return result;
    }

    bool empty() 
    {
        return getHead<FX_NS(thread)::membar_relaxed>() == NULL;
    }

    /**
     * Remove a specified element by value
     *
     * @param e The element to be removed
     * @return Remove the value and return true if exists, otherwise return false.
     */
    bool remove(const KeyType& e) 
    {
        return remove(e, &m_head);
    }

    /**
     * @brief Determine if the element is in the list
     *
     * @param e The element to be searched
     * @return Return true if exists. otherwise return false
     */
    bool find(const KeyType& e)
    {
        return find(e, &m_head);
    }

private:

    /**
     * @brief Judge if the element is in the list
     *
     * @param e
     * 			The element to be searched
     * @param start
     * 			Address of the begin position
     * @return
     * 			If the element exists in the list, return true. else return false
     */
    bool search(const KeyType& e, atomic<Node*>* start) {
        FindStateHolder<KeyType> holder = FindStateHolder<KeyType> ();
        bool result = find(e, start, holder);
        HazardP * hp = mm->getHPRec();	
        retireAll(hp);
        return result;
    }

    /**
     * @brief Remove a specified element by value
     *
     * @param e
     * 			The element to be removed
     * @param start
     * 			address of the begin position
     * @return
     * 			If the value exists in the list, remove it and return true. else return false.
     */
    bool remove(const KeyType& e, atomic<Node*>* start) 
    {
        FindStateHolder<KeyType> holder = FindStateHolder<KeyType> ();
        bool result;
        HazardP * hp = mm->getHPRec();

        while (true) 
        {
            if (!find(e, start, holder)) 
            {/* return false if the element is not found in the list*/
                result = false;
                break;
            }
		
            /**
             * CAS operation, mark cur as deleted.
             * if successful, the thread attempts to remove it at next step.
             * else, it implies that one or more of three events must have taken place since the snapshot
             * in Find method was taken. Then goto the start of this while loop
             */
            if (!holder.cur->next.compare_swap(holder.next, MARK(holder.next))) 
            {
                continue;
            }

            /**
             *CAS operation, set the next pointer of prev to the next pointer of cur.
             *If successful, delete cur by delNode of SMR.
             *If failed, ,it implies that another thread must have removed the node from the list 
             *after the success of the MARK CAS before.
             *and a new Find is invoked in order to guarantee that the number of deleted nodes not yet i
             *removed never exceeds the maximum number of concurrent threads operating on the object.
             */
            assert(!MARKED(holder.next));
            if (holder.prev->compare_swap(holder.cur, holder.next)) 
            {
                mm->delNode(hp, holder.cur);
            }
            else 
            {
                find(e, start, holder);
            }
            result = true;
            break;
        }
        retireAll(hp);
        return result;
    }

    /**
     * @brief Assign NULL to all the hazard pointer so that the original
     * pointer is not marked as "in-use" any more.
     */
    void retireAll(HazardP * hp) 
    {
        mm->retire(hp, 0);
        mm->retire(hp, 1);
        mm->retire(hp, 2);
    }

    /**
     * @brief Judge if a specified element is in the list
     *
     * @param key
     * 			A specified element
     * @param start
     * 			The address of the begin position
     * @param holder
     * 			A node Keep the snapshot of a specified state
     * @return
     * 			If the element is existed in the list, return true, else return false
     */
    bool find(const KeyType& key, atomic<Node*>* start, FindStateHolder<KeyType>& holder) 
    {
        HazardP * hp = mm->getHPRec();

    try_again:

        Node* next = NULL;
        atomic<Node*>* prev = start;

        Node* cur = prev->load(memory_order_relaxed);//memory_order_relaxed
        assert(!MARKED(cur));
        mm->employ(hp, 1, cur);

        if (prev->load(memory_order_relaxed) != cur)/* if the holder.pre changed, start over from the beginning */
            goto try_again;

        while (true) 
        {
            if (NULL == cur) 
            {
                /*set value of state holder*/
                holder.isFound = false;
                holder.prev = prev;
                holder.cur = cur;
                holder.next = next;
                return false;
            }

            assert(!MARKED(cur));
            Node* markedNext = cur->next.load(memory_order_relaxed);
            bool cmark = MARKED(markedNext);
            next = POINTER(markedNext);

            mm->employ(hp, 0, next);
            if (cur->next.load(memory_order_relaxed) != markedNext) /*If the next node changed, start over from the beginning*/
                goto try_again;

            KeyType cKey = cur->data;
            if (prev->load(memory_order_relaxed) != cur) /*If the holder.pre changed, start over from the beginning*/
                goto try_again;

            if (!cmark) {
                if (cKey == key) {
                    /*set value of state holder*/
                    holder.isFound = true;
                    holder.prev = prev;
                    holder.cur = cur;
                    holder.next = next;
                    return true;
                }
                prev = &(cur->next);
                //mm->employ(hp, 2, cur);
            } else {/* if the thread encounters a node marked to be deleted, it attempts to remove it from the list*/
                /**
                 *CAS operation, set the next pointer of prew to the next pointer of cur.
                 *If successful, delete holder.cur by delNode of SMR.
                 *If failed, ,it implies that another thread must have removed the node from the list 
                 *after the success of the MARK CAS before.
                 *goto the start position.
                 */
                assert(!MARKED(next));
                if (prev->compare_swap(cur, next)) {
                    mm->delNode(hp, cur);
                } else {
                    goto try_again;
                }
            }

            cur = next;
            mm->employ(hp, 1, next);
        }
    }
private:
protected:
    /// Pointer to header node, initialized by a dummy node.
    FX_NS(thread)::Atomic<Node*> m_head;

    /**
     *a pointer created for initializing of head pointer, for the performance reason
     */
    Node* m_pDummyNode;

    /**
     * for memory management
     */
    SMR<Node, NHPOINTER>* mm;

    FX_NS(thread)::HPSMR m_smr;

private:

    List(const List& s) 
    {
    }

    List& operator=(const List& st) {
    }
};

FX_NS_END

#endif //__FX_CONCURRENTLIST_H
