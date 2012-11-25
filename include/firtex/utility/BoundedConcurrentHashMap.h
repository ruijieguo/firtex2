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
// Created : 2011-07-09 17:03:46

#ifndef __FX_BOUNDEDCONCURRENTHASHMAP_H
#define __FX_BOUNDEDCONCURRENTHASHMAP_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Hash.h"
#include "firtex/utility/ConcurrentLinkedQueue.h"
#include "firtex/utility/HashComparator.h"
#include "firtex/utility/LinkedHashMap.h"
#include "firtex/thread/Mutex.h"
#include <vector>
#include <map>
#include <list>
#include <set>

FX_NS_DEF(utility);

template <typename K, typename V, typename HashFunc = Hasher<K>,
          typename Comp = HashComparator<K> >
class BoundedConcurrentHashMap
{
public:
    typedef K KeyType;
    typedef V ValueType;
    typedef Comp KeyComparator;

public:
    /**
     * The default initial capacity for this table,
     * used when not otherwise specified in a constructor.
     */
    const static size_t DEFAULT_MAXIMUM_CAPACITY = 512;
    
    /**
     * The default load factor for this table, used when not
     * otherwise specified in a constructor.
     */
    const static float DEFAULT_LOAD_FACTOR = 0.75f;

    /**
     * The default concurrency level for this table, used when not
     * otherwise specified in a constructor.
     */
    const static size_t DEFAULT_CONCURRENCY_LEVEL = 16;

    /**
     * The maximum capacity, used if a higher value is implicitly
     * specified by either of the constructors with arguments.  MUST
     * be a power of two <= 1<<30 to ensure that entries are indexable
     * using ints.
     */
    const static size_t MAXIMUM_CAPACITY = (1 << 30);

    /**
     * The maximum number of segments to allow; used to bound
     * constructor arguments.
     */
    const static size_t MAX_SEGMENTS = (1 << 16); // slightly conservative

    /**
     * Number of unsynchronized retries in size and containsValue
     * methods before resorting to locking. This is used to avoid
     * unbounded retries if tables undergo continuous modification
     * which would make it impossible to obtain an accurate result.
     */
    const static size_t RETRIES_BEFORE_LOCK = 2;

public:
    enum Recency 
    {
        HIR_RESIDENT, LIR_RESIDENT, HIR_NONRESIDENT
    };

    enum Eviction 
    {
        EVICTION_NONE, 
        EVICTION_LRU,
        EVICTION_LIRS,
    };

    /**
     * ConcurrentHashMap list entry. Note that this is never exported
     * out as a user-visible Map.Entry.
     *
     * Because the value field is volatile, not final, it is legal wrt
     * the Java Memory Model for an unsynchronized reader to see NULL
     * instead of initial value when read via a data race.  Although a
     * reordering leading to this is not likely to ever actually
     * occur, the Segment.readValueUnderLock method is used as a
     * backup in case a NULL (pre-initialized) value is ever seen in
     * an unsynchronized access method.
     */
    class HashEntry
    {
    public:
        KeyType key;
        uint32_t hash;
        volatile ValueType value;
        HashEntry* next;
        Recency state;

    public:
        HashEntry(KeyType k, size_t h, HashEntry* n, ValueType v) 
            : key(k)
            , hash(h)
            , value(v)
            , next(n)
            , state(HIR_RESIDENT)
        {
        }

        void transitionToLIRResident() 
        {
            state = LIR_RESIDENT;
        }

        void transitionHIRResidentToHIRNonResident() 
        {
            state = HIR_NONRESIDENT;
        }

        void transitionLIRResidentToHIRResident() 
        {
            state = HIR_RESIDENT;
        }

        Recency recency() const
        {
            return state;
        }
    };

    typedef std::set<HashEntry*> HashEntrySet;
    typedef std::vector<HashEntry*> HashEntryVect;

    class EvictionListener
    {
    public:
        virtual ~EvictionListener() {}
        virtual void onEntryEviction(const std::map<KeyType, ValueType>& evicted) = 0;
        virtual void onEntryChosenForEviction(ValueType internalCacheEntry) = 0;
    };
    DEFINE_TYPED_PTR(EvictionListener);

    class EvictionPolicy
    {
    public:
        const static size_t MAX_BATCH_SIZE = 64;
    public:
        virtual ~EvictionPolicy() {}

    public:
        /**
         * Invokes eviction policy algorithm and returns set of evicted entries.
         *
         * <p>
         * Set cannot be NULL but could possibly be an empty set.
         *
         * @return set of evicted entries.
         */
        virtual void execute(HashEntrySet& evicted) = 0;

        /**
         * Invoked to notify EvictionPolicy implementation that there has been an attempt to access
         * an entry in Segment, however that entry was not present in Segment.
         *
         * @param e
         *            accessed entry in Segment
         *
         * @return non NULL set of evicted entries.
         */
        virtual void onEntryMiss(HashEntry* e, HashEntrySet& evicted) = 0;

        /**
         * Invoked to notify EvictionPolicy implementation that an entry in Segment has been
         * accessed. Returns true if batching threshold has been reached, false otherwise.
         * <p>
         * Note that this method is potentially invoked without holding a lock on Segment.
         *
         * @return true if batching threshold has been reached, false otherwise.
         *
         * @param e
         *            accessed entry in Segment
         */
        virtual bool onEntryHit(HashEntry* e) = 0;

        /**
         * Invoked to notify EvictionPolicy implementation that an entry e has been removed from
         * Segment.
         *
         * @param e
         *            removed entry in Segment
         */
        virtual void onEntryRemove(HashEntry* e) = 0;

        /**
         * Invoked to notify EvictionPolicy implementation that all Segment entries have been
         * cleared.
         *
         */
        virtual void clear() = 0;

        /**
         * Returns type of eviction algorithm (strategy).
         *
         * @return type of eviction algorithm
         */
        virtual Eviction strategy() = 0;

        /**
         * Returns true if batching threshold has expired, false otherwise.
         * <p>
         * Note that this method is potentially invoked without holding a lock on Segment.
         *
         * @return true if batching threshold has expired, false otherwise.
         */
        virtual bool thresholdExpired() = 0;
    };
    DEFINE_TYPED_PTR(EvictionPolicy);

    template <typename T>
    class LinkedList : public std::list<T>
    {
    public:
        typedef T ElemType;
        typedef std::list<T> BaseClass;
        typedef typename BaseClass::iterator Iterator;
        typedef typename BaseClass::const_iterator ConstIterator;

    public:
        bool contains(const ElemType& e) const
        {
            for (typename BaseClass::const_iterator it = BaseClass::begin();
                 it != BaseClass::end(); ++it)
            {
                if (e == *it)
                    return true;
            }
            return false;
        }

        bool removeElem(const ElemType& e)
        {
            for (Iterator it = this->begin();
                 it != this->end(); ++it)
            {
                if (e == *it)
                {
                    this->erase(it);
                    return true;
                }
            }
            return false;
        }
    };

    /**
     * Segments are specialized versions of hash tables.  This
     * subclasses from ReentrantLock opportunistically, just to
     * simplify some locking and avoid separate construction.
     */
    class Segment
    {
    public:
        /*
         * Segments maintain a table of entry lists that are ALWAYS
         * kept in a consistent state, so can be read without locking.
         * Next fields of nodes are immutable (final).  All list
         * additions are performed at the front of each bin. This
         * makes it easy to check changes, and also fast to traverse.
         * When nodes would otherwise be changed, new nodes are
         * created to replace them. This works well for hash tables
         * since the bin lists tend to be short. (The average length
         * is less than two for the default load factor threshold.)
         *
         * Read operations can thus proceed without locking, but rely
         * on selected uses of volatiles to ensure that completed
         * write operations performed by other threads are
         * noticed. For most purposes, the "count" field, tracking the
         * number of elements, serves as that volatile variable
         * ensuring visibility.  This is convenient because this field
         * needs to be read in many read operations anyway:
         *
         *   - All (unsynchronized) read operations must first read the
         *     "count" field, and should not look at table entries if
         *     it is 0.
         *
         *   - All (synchronized) write operations should write to
         *     the "count" field after structurally changing any bin.
         *     The operations must not take any action that could even
         *     momentarily cause a concurrent read operation to see
         *     inconsistent data. This is made easier by the nature of
         *     the read operations in Map. For example, no operation
         *     can reveal that the table has grown but the threshold
         *     has not yet been updated, so there are no atomicity
         *     requirements for this with respect to reads.
         *
         * As a guide, all critical volatile reads and writes to the
         * count field are marked in code comments.
         */

        Segment(size_t cap, float lf, Eviction es, 
                const EvictionListenerPtr& pListener,
                const ValueType& emptyValue) 
            : m_nCount(0)
            , m_nModCount(0)
        {
            m_fLoadFactor = lf;
            m_pEviction = MapType::makeEvictionPolicy(es, *this, cap, lf);
            m_pEvictionListener = pListener;

            m_nThreshold = (size_t)(cap * m_fLoadFactor);
            m_table.resize(cap);

            m_emptyValue = emptyValue;
        }
        
        ~Segment()
        {
            clear();
        }

        void lock()
        {
            m_mutex.lock();
        }
            
        void unlock()
        {
            m_mutex.unlock();
        }

        bool tryLock()
        {
            return m_mutex.tryLock();
        }

        EvictionListenerPtr getEvictionListener() const
        {
            return m_pEvictionListener;
        }

        /**
         * Returns properly casted first entry of bin for given hash.
         */
        HashEntry* getFirst(size_t hash) 
        {
            return m_table[hash & m_table.size() - 1];
        }

        /**
         * Reads value field of an entry under lock. Called if value
         * field ever appears to be NULL. This is possible only if a
         * compiler happens to reorder a HashEntry initialization with
         * its table assignment, which is legal under memory model
         * but is not known to ever occur.
         */
        ValueType readValueUnderLock(HashEntry* e) 
        {
            FX_NS(thread)::Mutex::Guard g(m_mutex);
            return e->value;
        }

        /* Specialized implementations of map methods */
        ValueType get(const KeyType& key, uint32_t hash) 
        {
            size_t c = m_nCount;
            if (c != 0) 
            { // read-volatile
                ValueType result = m_emptyValue;
                HashEntry* e = getFirst(hash);
                while (e != NULL) 
                {
                    if (e->hash == hash && m_comparator(key, e->key)) 
                    {
                        ValueType v = e->value;
                        if (v != m_emptyValue) 
                        {
                            result = v;
                            break;
                        }
                        else 
                        {
                            result = readValueUnderLock(e);// recheck
                            break;
                        }
                    }
                    e = e->next;
                }

                // a hit
                if (result != m_emptyValue) 
                {
                    if (m_pEviction->onEntryHit(e)) 
                    {
                        HashEntrySet evicted;
                        attemptEviction(false, evicted);
                        notifyEvictionListener(evicted);
                    }
                }
                return result;
            }
            return m_emptyValue;
        }

        bool containsKey(const KeyType& key, uint32_t hash)
        {
            if (m_nCount != 0) 
            { // read-volatile
                HashEntry* e = getFirst(hash);
                while (e != NULL) 
                {
                    if (e->hash == hash && m_comparator(key, e->key)) 
                    {
                        return true;
                    }
                    e = e->next;
                }
            }
            return false;
        }

        bool replace(const KeyType& key, uint32_t hash, const ValueType& oldValue, 
                     const ValueType& newValue) 
        {
            FX_NS(thread)::Mutex::Guard g(m_mutex);
            HashEntrySet evicted;
            HashEntry* e = getFirst(hash);
            while (e != NULL && (e->hash != hash || !m_comparator(key, e->key))) 
            {
                e = e->next;
            }

            bool replaced = false;
            if (e != NULL && m_comparator(oldValue, e->value)) 
            {
                replaced = true;
                e->value = newValue;
                if (m_pEviction->onEntryHit(e)) 
                {
                    attemptEviction(true, evicted);
                }
            }
            notifyEvictionListener(evicted);
            return replaced;
        }

        ValueType replace(const KeyType& key, uint32_t hash, const ValueType& newValue) 
        {
            FX_NS(thread)::Mutex::Guard g(m_mutex);

            HashEntrySet evicted;
            HashEntry* e = getFirst(hash);
            while (e != NULL && (e.hash != hash || !m_comparator(key, e->key))) 
            {
                e = e->next;
            }

            ValueType oldValue = m_emptyValue;
            if (e != NULL) 
            {
                oldValue = e->value;
                e->value = newValue;
                if (m_pEviction->onEntryHit(e)) 
                {
                    attemptEviction(true, evicted);
                }
            }
            notifyEvictionListener(evicted);
            return oldValue;
        }

        const ValueType& put(const KeyType& key, uint32_t hash,
                             const ValueType& value, bool onlyIfAbsent)
        {
            FX_NS(thread)::Mutex::Guard g(m_mutex);
            HashEntrySet evicted;

            size_t c = m_nCount;
            if (c++ > m_nThreshold && m_pEviction->strategy() == EVICTION_NONE) 
            {
                rehash();
            }
            size_t index = hash & m_table.size() - 1;
            HashEntry* first = m_table[index];
            HashEntry* e = first;
            while (e != NULL && (e->hash != hash || !KeyComparator()(key, e->key))) 
            {
                e = e->next;
            }

            ValueType& oldValue = m_emptyValue;
            if (e != NULL) 
            {
                oldValue = e->value;
                if (!onlyIfAbsent) 
                {
                    e->value = value;
                    m_pEviction->onEntryHit(e);
                }
            } 
            else 
            {
                ++m_nModCount;
                m_nCount = c; // write-volatile
                if (m_pEviction->strategy() != EVICTION_NONE) 
                {
                    if (c > m_table.size()) 
                    {
                        // remove entries;lower count
                        m_pEviction->execute(evicted);
                        // re-read first
                        first = m_table[index];
                    }
                    // add a new entry
                    m_table[index] = new HashEntry(key, hash, first, value);
                    // notify a miss
                    HashEntrySet newlyEvicted;
                    m_pEviction->onEntryMiss(m_table[index], newlyEvicted);
                    if (!newlyEvicted.empty()) 
                    {
                        if (!evicted.empty()) 
                        {
                            evicted.insert(newlyEvicted.begin(), newlyEvicted.end());
                        }
                        else 
                        {
                            evicted.swap(newlyEvicted);
                        }
                    }
                } 
                else 
                {
                    m_table[index] = new HashEntry(key, hash, first, value);
                }
            }
            notifyEvictionListener(evicted);
            destroyHashEntry(evicted);
            return oldValue;
        }

        void rehash() 
        {
            HashEntryVect& oldTable = m_table;
            size_t oldCapacity = oldTable.size();
            if (oldCapacity >= MAXIMUM_CAPACITY) 
            {
                return;
            }

            /**
             * Reclassify nodes in each list to new Map.  Because we are
             * using power-of-two expansion, the elements from each bin
             * must either stay at same index, or move with a power of two
             * offset. We eliminate unnecessary node creation by catching
             * cases where old nodes can be reused because their next
             * fields won't change. Statistically, at the default
             * threshold, only about one-sixth of them need cloning when
             * a table doubles. The nodes they replace will be garbage
             * collectable as soon as they are no longer referenced by any
             * reader thread that may be in the midst of traversing table
             * right now.
             */

            HashEntryVect newTable;
            newTable.resize(oldCapacity << 1);
            m_nThreshold = (int)(newTable.size() * m_fLoadFactor);
            size_t sizeMask = newTable.size() - 1;
            for (size_t i = 0; i < oldCapacity ; i++) 
            {
                // We need to guarantee that any existing reads of old Map can
                //  proceed. So we cannot yet NULL out each bin.
                HashEntry* e = oldTable[i];

                if (e != NULL) 
                {
                    HashEntry* next = e->next;
                    size_t idx = e->hash & sizeMask;

                    //  Single node on list
                    if (next == NULL) 
                    {
                        newTable[idx] = e;
                    }
                    else 
                    {
                        // Reuse trailing consecutive sequence at same slot
                        HashEntry* lastRun = e;
                        size_t lastIdx = idx;
                        for (HashEntry* last = next; last != NULL;
                             last = last->next) 
                        {
                            size_t k = last->hash & sizeMask;
                            if (k != lastIdx) 
                            {
                                lastIdx = k;
                                lastRun = last;
                            }
                        }
                        newTable[lastIdx] = lastRun;

                        // Clone all remaining nodes
                        for (HashEntry* p = e; p != lastRun; p = p->next) 
                        {
                            size_t k = p->hash & sizeMask;
                            HashEntry* n = newTable[k];
                            newTable[k] = new HashEntry(p->key, p->hash,
                                    n, p->value);
                        }
                    }
                }
            }
            m_table.swap(newTable);
            ///TODO: destroy newTable
        }

        /**
         * Remove; match on key only if value NULL, else match both.
         */
        ValueType remove(const KeyType& key, uint32_t hash, const ValueType& value) 
        {
            FX_NS(thread)::Mutex::Guard g(m_mutex);
            size_t c = m_nCount - 1;
            HashEntryVect& tab = m_table;
            size_t index = hash & tab.size() - 1;
            HashEntry* first = tab[index];
            HashEntry* e = first;
            while (e != NULL && (e->hash != hash || !m_comparator(key, e->key)))
            {
                e = e->next;
            }

            ValueType oldValue = m_emptyValue;
            if (e != NULL) 
            {
                //ValueType& v = e->value;
                ValueType v = e->value;
                if (value == m_emptyValue || value == v)
                {
                    oldValue = v;
                    // All entries following removed node can stay
                    // in list, but all preceding ones need to be
                    // cloned.
                    ++m_nModCount;

                    // e was removed
                    m_pEviction->onEntryRemove(e);

                    HashEntry* newFirst = e->next;
                    for (HashEntry* p = first; p != e; p = p->next) 
                    {
                        // allow p to be GC-ed
                        m_pEviction->onEntryRemove(p);
                        newFirst = new HashEntry(p->key, p->hash, newFirst, p->value);
                        // and notify eviction algorithm about new hash entries
                        HashEntrySet evicted;
                        m_pEviction->onEntryMiss(newFirst, evicted);
                    }

                    tab[index] = newFirst;
                    m_nCount = c; // write-volatile
                }
            }
            return oldValue;
        }

        void clear() 
        {
            if (m_nCount != 0) 
            {
                FX_NS(thread)::Mutex::Guard g(m_mutex);
                HashEntryVect& tab = m_table;
                for (size_t i = 0; i < tab.size(); i++) 
                {
                    HashEntry* e = tab[i];
                    while (e)
                    {
                        HashEntry* tmp = e;
                        e = e->next;
                        delete tmp;
                    }
                }
                tab.clear();

                ++m_nModCount;
                m_pEviction->clear();
                m_nCount = 0; // write-volatile
            }
        }

    private:
        void destroyHashEntry(HashEntrySet& evicted)
        {
            for (typename HashEntrySet::iterator it = evicted.begin();
                 it != evicted.end(); ++it)
            {
                delete *it;
            }
            evicted.clear();
        }

        void attemptEviction(bool lockedAlready, HashEntrySet& evicted)
        {
            bool obtainedLock = !lockedAlready ? tryLock() : true;
            if (!obtainedLock && m_pEviction->thresholdExpired()) 
            {
                lock();
                obtainedLock = true;
            }
            if (obtainedLock) 
            {
                try 
                {
                    m_pEviction->execute(evicted);
                }
                catch (...)
                {
                    if (!lockedAlready) 
                    {
                        unlock();
                    }
                    throw;
                }

                if (!lockedAlready) 
                {
                    unlock();
                }

            }
        }

        void notifyEvictionListener(HashEntrySet& evicted) 
        {
            // piggyback listener invocation on callers thread outside lock
            if (!evicted.empty()) 
            {
                std::map<KeyType, ValueType> evictedCopy;
                if (evicted.size() == 1) 
                {
                    HashEntry* evictedEntry = *(evicted.begin());
                    evictedCopy.insert(std::make_pair(evictedEntry->key, evictedEntry->value));
                }
                else 
                {
                    for (typename HashEntrySet::const_iterator it = evicted.begin();
                         it != evicted.end(); ++it) 
                    {
                        evictedCopy.insert(std::make_pair((*it)->key, (*it)->value));
                    }
                }
                m_pEvictionListener->onEntryEviction(evictedCopy);
            }
        }

    public:
        /**
         * The number of elements in this segment's region.
         */
        volatile size_t m_nCount;

        /**
         * Number of updates that alter the size of the table. This is
         * used during bulk-read methods to make sure they see a
         * consistent snapshot: If modCounts change during a traversal
         * of segments computing size or checking containsValue, then
         * we might have an inconsistent view of state so (usually)
         * must retry.
         */
        size_t m_nModCount;

        /**
         * The table is rehashed when its size exceeds this threshold.
         * (The value of this field is always <tt>(int)(capacity *
         * loadFactor)</tt>.)
         */
        size_t m_nThreshold;
            
        /**
         * The per-segment table.
         */
        //volatile HashEntryVect m_table;
        HashEntryVect m_table;

        /**
         * The load factor for the hash table.  Even though this value
         * is same for all segments, it is replicated to avoid needing
         * links to outer object.
         * @serial
         */
        float m_fLoadFactor;

        /// Eviction policy
        EvictionPolicyPtr m_pEviction;

        EvictionListenerPtr m_pEvictionListener;

        FX_NS(thread)::Mutex m_mutex;

        /// The key comparator
        Comp m_comparator;

        /// empty value
        ValueType m_emptyValue;
    };

public:
    typedef Segment SegmentType;
    DEFINE_TYPED_PTR(SegmentType);

    typedef BoundedConcurrentHashMap MapType;
 
public:
    /**
     * Returns the segment that should be used for key with given hash
     * @param hash the hash code for the key
     * @return the segment
     */
    SegmentType* segmentFor(uint32_t hash) 
    {
        return m_segments[(hash >> m_segmentShift) & m_segmentMask];
    }

public:
    class NULLEvictionListener : public EvictionListener
    {
    public:
        void onEntryEviction(const std::map<KeyType, ValueType>& evicted) 
        {
            // Do nothing.
        }
        void onEntryChosenForEviction(ValueType internalCacheEntry) 
        {
            // Do nothing.
        }
    };

    class NULLEvictionPolicy : public EvictionPolicy
    {
    public:
        void clear() 
        {
            // Do nothing.
        }

        void execute(HashEntrySet& evicted) 
        {
            // Do nothing.
        }

        bool onEntryHit(HashEntry* e) 
        {
            return false;
        }

        void onEntryMiss(HashEntry* e, HashEntrySet& evicted) 
        {
        }

        void onEntryRemove(HashEntry* e) 
        {
            // Do nothing.
        }

        bool thresholdExpired() 
        {
            return false;
        }

        Eviction strategy() 
        {
            return EVICTION_NONE;
        }
    };

    class LRU : public EvictionPolicy 
    {
    public:
        LRU(SegmentType& s, size_t capacity, float lf,
            size_t maxBatchSize, float batchThresholdFactor) 
            : m_segment(s)
            , m_nTrimDownSize((size_t) (capacity * lf))
        {
            m_nMaxBatchQueueSize = maxBatchSize > EvictionPolicy::MAX_BATCH_SIZE 
                                ? EvictionPolicy::MAX_BATCH_SIZE : maxBatchSize;
            m_fBatchThresholdFactor = batchThresholdFactor;
        }
            
        void execute(HashEntrySet& evicted) 
        {
            std::vector<HashEntry*> elems(m_accessQueue.size());
            HashEntry* e = NULL;
            while (m_accessQueue.dequeue(e))
            {
                elems.push_back(e);
            }

            for (size_t i = 0; i < elems.size(); ++i)
            {
                e = elems[i];
                if (m_lruQueue.removeElem(e)) 
                {
                    m_lruQueue.push_front(e);
                }
            }
            while (isOverflow())
            {
                HashEntry* first = m_lruQueue.back();
                m_segment.m_pEvictionListener->onEntryChosenForEviction(first->value);
                m_segment.remove(first->key, first->hash, m_segment.m_emptyValue);
                evicted.insert(first);
            }
        }

    private:
        bool isOverflow() const
        {
            return m_lruQueue.size() > m_nTrimDownSize;
        }

    public:
        void onEntryMiss(HashEntry* e, HashEntrySet& evicted) 
        {
            m_lruQueue.push_front(e);
        }

        /*
         * Invoked without holding a lock on Segment
         */
        bool onEntryHit(HashEntry* e) 
        {
            m_accessQueue.enqueue(e);
            return m_accessQueue.size() >= m_nMaxBatchQueueSize * m_fBatchThresholdFactor;
        }

        /*
         * Invoked without holding a lock on Segment
         */
        bool thresholdExpired() 
        {
            return m_accessQueue.size() >= m_nMaxBatchQueueSize;
        }

        void onEntryRemove(HashEntry* e) 
        {
            m_lruQueue.remove(e);
            // we could have multiple instances of e in m_accessQueue; remove them all
            while (m_accessQueue.remove(e)) 
            {
                continue;
            }
        }

        void clear() 
        {
            m_lruQueue.clear();
            m_accessQueue.clear();
        }

        Eviction strategy() 
        {
            return EVICTION_LRU;
        }

    private:
        ConcurrentLinkedQueue<HashEntry*> m_accessQueue;
        SegmentType& m_segment;
        LinkedList<HashEntry*> m_lruQueue;
            
        size_t m_nMaxBatchQueueSize;
        size_t m_nTrimDownSize;
        float m_fBatchThresholdFactor;
    };

    class LIRS : public EvictionPolicy
    {
    public:
        LIRS(SegmentType& s, size_t capacity, 
             size_t maxBatchSize,
             float batchThresholdFactor) 
            : m_segment(s)
            , m_stack(NULL)
            , m_nCurrentLIRSize(0)
        {
            size_t tmpLirSize = (size_t) (capacity * 0.9);
            size_t tmpHirSizeLimit = capacity - tmpLirSize;
            if (tmpHirSizeLimit < MIN_HIR_SIZE)
            {
                m_nHIRSizeLimit = MIN_HIR_SIZE;
                m_nLIRSizeLimit = capacity - m_nHIRSizeLimit;
            }
            else 
            {
                m_nHIRSizeLimit = tmpHirSizeLimit;
                m_nLIRSizeLimit = tmpLirSize;
            }
            m_nMaxBatchQueueSize = maxBatchSize > EvictionPolicy::MAX_BATCH_SIZE 
                                ? EvictionPolicy::MAX_BATCH_SIZE : maxBatchSize;
            m_fBatchThresholdFactor = batchThresholdFactor;
        }

    public:
        void execute(HashEntrySet& evicted) 
        {
            HashEntry* e = NULL;
            while (m_accessQueue.dequeue(e))
            {
                if (present(e)) 
                {
                    if (e->recency() == LIR_RESIDENT) 
                    {
                        handleLIRHit(e, evicted);
                    }
                    else if (e->recency() == HIR_RESIDENT) 
                    {
                        handleHIRHit(e, evicted);
                    }
                }
            }
            removeFromSegment(evicted);

            // for (HashEntry* e : m_accessQueue) 
            // {
            //     if (present(e)) 
            //     {
            //         if (e.recency() == Recency.LIR_RESIDENT) 
            //         {
            //             handleLIRHit(e, evicted);
            //         }
            //         else if (e.recency() == Recency.HIR_RESIDENT) 
            //         {
            //             handleHIRHit(e, evicted);
            //         }
            //     }
            // }
            // removeFromSegment(evicted);
            // m_accessQueue.clear();
            //return evicted;
        }

    private:
        void handleHIRHit(HashEntry* e, HashEntrySet& evicted) 
        {
            bool inStack = m_stack.containsKey(e->key);
            if (inStack) 
            {
                m_stack.remove(e->key);
            }

            // first put on top of the stack
            m_stack.put(e->key, e);

            if (inStack) 
            {
                m_queue.remove(e);
                e->transitionToLIRResident();
                switchBottomostLIRtoHIRAndPrune(evicted);
            }
            else 
            {
                m_queue.remove(e);
                m_queue.push_back(e);
            }
        }

        void handleLIRHit(HashEntry* e, HashEntrySet& evicted) 
        {
            m_stack.remove(e->key);
            m_stack.put(e->key, e);
            typename LinkedHashMap<KeyType, HashEntry*>::Iterator it = m_stack.iterator();
            while (it.hasNext())
            {
                HashEntry* next = it.nextValue();
                if (next->recency() == LIR_RESIDENT) 
                {
                    break;
                }
                else 
                {
                    it.remove();
                    evicted.insert(next);
                }
            }
        }

    private:
        bool present(HashEntry* e) 
        {
            return (m_stack.containsKey(e->key) || m_queue.contains(e));
        }

    public:
        void onEntryMiss(HashEntry* e, HashEntrySet& evicted) 
        {
            // initialization
            if (m_nCurrentLIRSize + 1 < m_nLIRSizeLimit) 
            {
                m_nCurrentLIRSize++;
                e->transitionToLIRResident();
                m_stack.put(e->key, e);
            }
            else 
            {
                if (m_queue.size() < m_nHIRSizeLimit) 
                {
                    m_queue.push_back(e);
                }
                else 
                {
                    bool inStack = m_stack.containsKey(e->key);
                    HashEntry* first = m_queue.front();
                    m_queue.pop_front();
                    first->transitionHIRResidentToHIRNonResident();

                    m_stack.put(e->key, e);

                    if (inStack) 
                    {
                        e->transitionToLIRResident();
                        switchBottomostLIRtoHIRAndPrune(evicted);
                    } 
                    else 
                    {
                        m_queue.push_back(e);
                        evicted.insert(first);
                    }
                    // evict from segment
                    removeFromSegment(evicted);
                }
            }
        }

    private:
        void removeFromSegment(HashEntrySet& evicted) 
        {
            for (typename HashEntrySet::const_iterator it = evicted.begin();
                 it != evicted.end(); ++it) 
            {
                HashEntry* e = *it;
                m_segment.m_pEvictionListener->onEntryChosenForEviction(e->value);
                m_segment.remove(e->key, e->hash, m_segment.m_emptyValue);
            }
        }

        void switchBottomostLIRtoHIRAndPrune(HashEntrySet& evicted) 
        {
            bool seenFirstLIR = false;
            typename LinkedHashMap<KeyType, HashEntry*>::Iterator it = m_stack.iterator();
            while (it.hasNext())
            {
                HashEntry* next = it.nextValue();
                if (next->recency() == LIR_RESIDENT) 
                {
                    if (!seenFirstLIR) 
                    {
                        seenFirstLIR = true;
                        it.remove();
                        next->transitionLIRResidentToHIRResident();
                        m_queue.push_back(next);
                    }
                    else 
                    {
                        break;
                    }
                } 
                else 
                {
                    it.remove();
                    evicted.insert(next);
                }
            }
        }

    public:
        /*
         * Invoked without holding a lock on Segment
         */
        bool onEntryHit(HashEntry* e) 
        {
            //m_accessQueue.add(e);
            m_accessQueue.enqueue(e);
            return m_accessQueue.size() >= m_nMaxBatchQueueSize * m_fBatchThresholdFactor;
        }

        /*
         * Invoked without holding a lock on Segment
         */
        bool thresholdExpired() 
        {
            return m_accessQueue.size() >= m_nMaxBatchQueueSize;
        }

        void onEntryRemove(HashEntry* e) 
        {
            HashEntry* removed = m_stack.remove(e->key);
            if (removed != NULL && removed->recency() == LIR_RESIDENT) 
            {
                m_nCurrentLIRSize--;
            }
            m_queue.remove(e);
            // we could have multiple instances of e in m_accessQueue; remove them all
            while (m_accessQueue.remove(e)) 
            {
                continue;
            }
        }

        void clear() 
        {
            m_stack.clear();
            m_accessQueue.clear();
        }

        Eviction strategy() 
        {
            return EVICTION_LIRS;
        }

    private:
        const static size_t MIN_HIR_SIZE = 2;
    
        SegmentType& m_segment;
        ConcurrentLinkedQueue<HashEntry*> m_accessQueue;
        LinkedHashMap<KeyType, HashEntry*> m_stack;
        LinkedList<HashEntry*> m_queue;
        size_t m_nMaxBatchQueueSize;
        size_t m_nLIRSizeLimit;
        size_t m_nHIRSizeLimit;
        size_t m_nCurrentLIRSize;
        float m_fBatchThresholdFactor;
    };

    static EvictionPolicy* makeEvictionPolicy(Eviction e, SegmentType& s, size_t cap, float lf)
    {
        switch (e)
        {
        case EVICTION_NONE:
            return new NULLEvictionPolicy();
        case EVICTION_LRU:
            return new LRU(s, cap, lf, cap * 10, lf);
        case EVICTION_LIRS:
            return new LIRS(s, cap, cap * 10, lf);
        default:
            return new NULLEvictionPolicy();
        }
    }

public:
    /**
     * Creates a new, empty map with the specified maximum capacity, load factor and concurrency
     * level.
     *
     * @param capacity
     *            is the upper bound capacity for the number of elements in this map
     *
     * @param concurrencyLevel
     *            the estimated number of concurrently updating threads. The implementation performs
     *            internal sizing to try to accommodate this many threads.
     *
     * @param evictionStrategy
     *            the algorithm used to evict elements from this map
     *
     * @param evictionListener
     *            the evicton listener callback to be notified about evicted elements
     */
    BoundedConcurrentHashMap(size_t capacity, size_t concurrencyLevel,
                             Eviction evictionStrategy, 
                             const EvictionListenerPtr& evictionListener,
                             const ValueType& emptyValue) 
        : m_emptyValue(emptyValue)
    {
        init(capacity, concurrencyLevel, evictionStrategy, evictionListener);
    }

    /**
     * Creates a new, empty map with the specified maximum capacity, load factor, concurrency
     * level and LRU eviction policy.
     *
     * @param capacity
     *            is the upper bound capacity for the number of elements in this map
     *
     * @param concurrencyLevel
     *            the estimated number of concurrently updating threads. The implementation performs
     *            internal sizing to try to accommodate this many threads.
     *
     * @throws IllegalArgumentException
     *             if the initial capacity is negative or the load factor or concurrencyLevel are
     *             nonpositive.
     */
    BoundedConcurrentHashMap(size_t capacity, size_t concurrencyLevel,
                             const ValueType& emptyValue)
        : m_emptyValue(emptyValue)
    {
        init(capacity, concurrencyLevel, EVICTION_LRU, new NULLEvictionListener());
    }

    /**
     * Creates a new, empty map with the specified maximum capacity, load factor, concurrency
     * level and eviction strategy.
     *
     * @param capacity
     *            is the upper bound capacity for the number of elements in this map
     *
     * @param concurrencyLevel
     *            the estimated number of concurrently updating threads. The implementation performs
     *            internal sizing to try to accommodate this many threads.
     *
     * @param evictionStrategy
     *            the algorithm used to evict elements from this map
     *
     * @throws IllegalArgumentException
     *             if the initial capacity is negative or the load factor or concurrencyLevel are
     *             nonpositive.
     */
    BoundedConcurrentHashMap(size_t capacity, size_t concurrencyLevel, 
                             Eviction evictionStrategy, const ValueType& emptyValue) 
        : m_emptyValue(emptyValue)
    {
        init(capacity, concurrencyLevel, evictionStrategy, new NULLEvictionListener());
    }

    /**
     * Creates a new, empty map with the specified maximum capacity, default concurrency
     * level and LRU eviction policy.
     *
     *  @param capacity
     *            is the upper bound capacity for the number of elements in this map
     */
    BoundedConcurrentHashMap(size_t capacity, const ValueType& emptyValue) 
        : m_emptyValue(emptyValue)
    {
        init(capacity, DEFAULT_CONCURRENCY_LEVEL, EVICTION_LRU, new NULLEvictionListener());
    }

    /**
     * Creates a new, empty map with the default maximum capacity
     */
    BoundedConcurrentHashMap(const ValueType& emptyValue) 
        : m_emptyValue(emptyValue)
    {
        init(DEFAULT_MAXIMUM_CAPACITY, DEFAULT_CONCURRENCY_LEVEL, 
             EVICTION_LRU, new NULLEvictionListener());
    }

    ~BoundedConcurrentHashMap()
    {
        for (size_t i = 0; i < m_segments.size(); ++ i) 
        {
            if (m_segments[i])
            {
                delete m_segments[i];
            }
        }    
        m_segments.clear();
    }

    void init(size_t capacity, size_t concurrencyLevel,
              Eviction evictionStrategy,
              const EvictionListenerPtr& evictionListener)
    {
        FIRTEX_ASSERT2(capacity >= 0 && concurrencyLevel > 0);

        concurrencyLevel = std::min(capacity / 2, concurrencyLevel); // concurrencyLevel cannot be > capacity/2
        concurrencyLevel = std::max(concurrencyLevel, (size_t)1); // concurrencyLevel cannot be less than 1

        // minimum two elements per segment
        if (capacity < concurrencyLevel * 2 && capacity != 1) 
        {
            FIRTEX_THROW(IllegalArgumentException, "Maximum capacity has to be at "
                         "least twice the concurrencyLevel");
        }

        FIRTEX_ASSERT2(evictionListener.isNotNull());

        if (concurrencyLevel > MAX_SEGMENTS) 
        {
            concurrencyLevel = MAX_SEGMENTS;
        }

        // Find power-of-two sizes best matching arguments
        int sshift = 0;
        size_t ssize = 1;
        while (ssize < concurrencyLevel) 
        {
            ++sshift;
            ssize <<= 1;
        }
        m_segmentShift = 32 - sshift;
        m_segmentMask = ssize - 1;
        m_segments.resize(ssize);

        if (capacity > MAXIMUM_CAPACITY) 
        {
            capacity = MAXIMUM_CAPACITY;
        }
        size_t c = capacity / ssize;
        if (c * ssize < capacity) 
        {
            ++c;
        }
        size_t cap = 1;
        while (cap < c) 
        {
            cap <<= 1;
        }

        for (size_t i = 0; i < m_segments.size(); ++i) 
        {
            m_segments[i] = new SegmentType(cap, DEFAULT_LOAD_FACTOR, 
                    evictionStrategy, evictionListener, m_emptyValue);
        }
    }

    /**
     * Returns <tt>true</tt> if this map contains no key-value mappings.
     *
     * @return <tt>true</tt> if this map contains no key-value mappings
     */
    bool isEmpty() 
    {
        /*
         * We keep track of per-segment modCounts to avoid ABA
         * problems in which an element in one segment was added and
         * in another removed during traversal, in which case the
         * table was never actually empty at any point. Note the
         * similar use of modCounts in the size() and containsValue()
         * methods, which are the only other methods also susceptible
         * to ABA problems.
         */
        std::vector<int> mc(m_segments.size());
        size_t mcsum = 0;
        for (size_t i = 0; i < m_segments.size(); ++i) 
        {
            if (m_segments[i]->count != 0) 
            {
                return false;
            }
            else 
            {
                mcsum += mc[i] = m_segments[i]->modCount;
            }
        }
        // If mcsum happens to be zero, then we know we got a snapshot
        // before any modifications at all were made.  This is
        // probably common enough to bother tracking.
        if (mcsum != 0) 
        {
            for (size_t i = 0; i < m_segments.length(); ++i) 
            {
                if (m_segments[i]->count != 0 || mc[i] != m_segments[i]->modCount) 
                {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Returns the number of key-value mappings in this map.  If the
     * map contains more than <tt>Integer.MAX_VALUE</tt> elements, returns
     * <tt>Integer.MAX_VALUE</tt>.
     *
     * @return the number of key-value mappings in this map
     */
    size_t size() 
    {
        long sum = 0;
        long check = 0;
        std::vector<size_t> mc(m_segments.size());
        // Try a few times to get accurate count. On failure due to
        // continuous async changes in table, resort to locking.
        for (size_t k = 0; k < RETRIES_BEFORE_LOCK; ++k) 
        {
            check = 0;
            sum = 0;
            size_t mcsum = 0;
            for (size_t i = 0; i < m_segments.size(); ++ i) 
            {
                sum += m_segments[i]->m_nCount;
                mcsum += mc[i] = m_segments[i]->m_nModCount;
            }
            if (mcsum != 0) 
            {
                for (size_t i = 0; i < m_segments.size(); ++ i) 
                {
                    check += m_segments[i]->m_nCount;
                    if (mc[i] != m_segments[i]->m_nModCount) 
                    {
                        check = -1; // force retry
                        break;
                    }
                }
            }
            if (check == sum) 
            {
                break;
            }
        }

        if (check != sum) 
        { // Resort to locking all segments
            sum = 0;
            for (size_t i = 0; i < m_segments.size(); ++ i) 
            {
                m_segments[i]->lock();
            }

            for (size_t i = 0; i < m_segments.size(); ++ i) 
            {
                sum += m_segments[i]->m_nCount;
            }

            for (size_t i = 0; i < m_segments.size(); ++ i) 
            {
                m_segments[i]->unlock();
            }
        }
        if (sum > std::numeric_limits<int>::max()) 
        {
            return std::numeric_limits<int>::max();
        }
        else 
        {
            return sum;
        }
    }

    /**
     * Returns the value to which the specified key is mapped,
     * or {@code NULL} if this map contains no mapping for the key.
     *
     * <p>More formally, if this map contains a mapping from a key
     * {@code k} to a value {@code v} such that {@code key.equals(k)},
     * then this method returns {@code v}; otherwise it returns
     * {@code NULL}.  (There can be at most one such mapping.)
     *
     * @throws NULLPointerException if the specified key is NULL
     */
    ValueType get(const KeyType& key) 
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->get(key, hash);
    }

    /**
     * Tests if the specified object is a key in this table.
     *
     * @param  key   possible key
     * @return <tt>true</tt> if and only if the specified object
     *         is a key in this table, as determined by the
     *         <tt>equals</tt> method; <tt>false</tt> otherwise.
     * @throws NULLPointerException if the specified key is NULL
     */
    bool containsKey(const KeyType& key) 
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->containsKey(key, hash);
    }

    /**
     * Returns <tt>true</tt> if this map maps one or more keys to the
     * specified value. Note: This method requires a full internal
     * traversal of the hash table, and so is much slower than
     * method <tt>containsKey</tt>.
     *
     * @param value value whose presence in this map is to be tested
     * @return <tt>true</tt> if this map maps one or more keys to the
     *         specified value
     * @throws NULLPointerException if the specified value is NULL
     */
    bool containsValue(const ValueType& value) 
    {
        // See explanation of modCount use above

        std::vector<int> mc(m_segments.size());

        // Try a few times without locking
        for (size_t k = 0; k < RETRIES_BEFORE_LOCK; ++ k) 
        {
            size_t mcsum = 0;
            for (size_t i = 0; i < m_segments.size(); ++ i) 
            {
                size_t c = m_segments[i]->count; // read-volatile
                (void)c;
                mcsum += mc[i] = m_segments[i]->modCount;
                if (m_segments[i]->containsValue(value)) 
                {
                    return true;
                }
            }
            bool cleanSweep = true;
            if (mcsum != 0) 
            {
                for (size_t i = 0; i < m_segments.size(); ++ i) 
                {
                    size_t c = m_segments[i]->count; // read-volatile
                    (void)c;
                    if (mc[i] != m_segments[i]->modCount) 
                    {
                        cleanSweep = false;
                        break;
                    }
                }
            }
            if (cleanSweep) 
            {
                return false;
            }
        }
        // Resort to locking all segments
        for (size_t i = 0; i < m_segments.size(); ++i) 
        {
            m_segments[i]->lock();
        }

        bool found = false;
        try 
        {
            for (size_t i = 0; i < m_segments.size(); ++i)
            {
                if (m_segments[i]->containsValue(value)) 
                {
                    found = true;
                    break;
                }
            }
        } 
        catch (...)
        {
            for (size_t i = 0; i < m_segments.length(); ++ i) 
            {
                m_segments[i]->unlock();
            }
            throw;
        }

        for (size_t i = 0; i < m_segments.length(); ++ i) 
        {
            m_segments[i]->unlock();
        }

        return found;
    }

    /**
     * Maps the specified key to the specified value in this table.
     * Neither the key nor the value can be NULL.
     *
     * <p> The value can be retrieved by calling the <tt>get</tt> method
     * with a key that is equal to the original key.
     *
     * @param key key with which the specified value is to be associated
     * @param value value to be associated with the specified key
     * @return the previous value associated with <tt>key</tt>, or
     *         <tt>NULL</tt> if there was no mapping for <tt>key</tt>
     * @throws NULLPointerException if the specified key or value is NULL
     */
    const ValueType& put(const KeyType& key, const ValueType& value) 
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->put(key, hash, value, false);
    }

    /**
     * {@inheritDoc}
     *
     * @return the previous value associated with the specified key,
     *         or <tt>NULL</tt> if there was no mapping for the key
     * @throws NULLPointerException if the specified key or value is NULL
     */
    const ValueType& putIfAbsent(const KeyType& key, const ValueType& value) 
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->put(key, hash, value, true);
    }

    /**
     * Removes the key (and its corresponding value) from this map.
     * This method does nothing if the key is not in the map.
     *
     * @param  key the key that needs to be removed
     * @return the previous value associated with <tt>key</tt>, or
     *         <tt>NULL</tt> if there was no mapping for <tt>key</tt>
     * @throws NULLPointerException if the specified key is NULL
     */
    ValueType remove(const KeyType& key) 
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->remove(key, hash, m_emptyValue);
    }

    /**
     * {@inheritDoc}
     *
     */
    bool remove(const KeyType& key, const ValueType& value) 
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->remove(key, hash, value) != NULL;
    }

    /**
     * {@inheritDoc}
     *
     * @throws NULLPointerException if any of the arguments are NULL
     */
    bool replace(const KeyType& key, const ValueType& oldValue, const ValueType& newValue)
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->replace(key, hash, oldValue, newValue);
    }

    /**
     * {@inheritDoc}
     *
     * @return the previous value associated with the specified key,
     *         or <tt>NULL</tt> if there was no mapping for the key
     * @throws NULLPointerException if the specified key or value is NULL
     */
    const ValueType& replace(const KeyType& key, const ValueType& value) 
    {
        uint32_t hash = m_hasher(key);
        return segmentFor(hash)->replace(key, hash, value);
    }

    /**
     * Removes all of the mappings from this map.
     */
    void clear() 
    {
        for (size_t i = 0; i < m_segments.size(); ++ i) 
        {
            m_segments[i]->clear();
        }
    }
public:
    /// The key comparator
    static Comp m_comparator;

private:
    /**
     * Mask value for indexing into segments. The upper bits of a
     * key's hash code are used to choose the segment.
     */
    int m_segmentMask;

    /// Shift value for indexing within segments.
    int m_segmentShift;

    /// The segments, each of which is a specialized hash table
    std::vector<SegmentType*> m_segments;

    /// The hash function for key
    HashFunc m_hasher;
    
    /// Empty value
    ValueType m_emptyValue;
};

FX_NS_END

#endif //__FX_BOUNDEDCONCURRENTHASHMAP_H
