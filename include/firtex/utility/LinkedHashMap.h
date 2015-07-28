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
// Created : 2011-08-16 23:26:09

#ifndef __FX_LINKEDHASHMAP_H
#define __FX_LINKEDHASHMAP_H

#include <vector>
#include <limits>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/HashComparator.h"

FX_NS_DEF(utility);

template <typename K, typename V, typename HashFunc = Hasher<K>,
          typename Comp = HashComparator<K> >
class LinkedHashMap
{
    /**
     * The default initial capacity - MUST be a power of two.
     */
    static const size_t DEFAULT_INITIAL_CAPACITY = 16;

    /**
     * The maximum capacity, used if a higher value is implicitly specified
     * by either of the constructors with arguments.
     * MUST be a power of two <= 1<<30.
     */
    static const size_t MAXIMUM_CAPACITY = 1 << 30;

    /**
     * The load factor used when none specified in constructor.
     */
    static const float DEFAULT_LOAD_FACTOR;

public:
    typedef K KeyType;
    typedef V ValueType;
    
    typedef HashFunc HashType;
    typedef Comp CompType;

public:
    struct Entry 
    {
        typedef std::pair<KeyType, ValueType> KeyValuePair;
        
        KeyValuePair kv;
        uint32_t hash;
        Entry* next;

        Entry* before;
        Entry* after;

        /**
         * Creates new entry.
         */
        Entry(uint32_t h, const KeyType& k, const ValueType& v, Entry* n) 
            : kv(k, v)
            , hash(h)
            , next(n)
            , before(NULL)
            , after(NULL)
        {
        }

        const KeyType& getKey() const
        {
            return kv.first;
        }

        const ValueType& getValue() const 
        {
            return kv.second;
        }

        ValueType setValue(const ValueType& newValue) 
        {
	    ValueType oldValue = kv.second;
            kv.second = newValue;
            return oldValue;
        }

        bool equals(const Entry* e) 
        {
            const KeyType& k1 = getKey();
            const KeyType& k2 = e->getKey();
            if (m_comparator(k1, k2))
            {
                const ValueType& v1 = getValue();
                const ValueType& v2 = e->getValue();
                if (v1 == v2)
                    return true;
            }
            return false;
        }

        /**
         * This method is invoked whenever the value in an entry is
         * overwritten by an invocation of put(k,v) for a key k that's already
         * in the HashMap.
         */
        void recordAccess(LinkedHashMap* m) 
        {
            if (m->m_bAccessOrder) 
            {
                m->m_nModCount++;
                remove();
                addBefore(m->m_pHeader);
            }
        }

        /**
         * This method is invoked whenever the entry is
         * removed from the table.
         */
        void recordRemoval(LinkedHashMap* m) 
        {
            remove();
        }

        /**
         * Removes this entry from the linked list.
         */
        void remove() 
        {
            before->after = after;
            after->before = before;
        }

        /**
         * Inserts this entry before the specified existing entry in the list.
         */
        void addBefore(Entry* existingEntry) 
        {
            after  = existingEntry;
            before = existingEntry->before;
            before->after = this;
            after->before = this;
        }
    };

    typedef std::vector<Entry*> EntryVector;

public:
    /**
     * Constructs an empty <tt>LinkedHashMap</tt> with the specified initial
     * capacity and load factor.
     *
     * @param  initialCapacity the initial capacity
     * @param  loadFactor      the load factor
     * @throws IllegalArgumentException if the initial capacity is negative
     *         or the load factor is nonpositive
     */
    LinkedHashMap(size_t initialCapacity, float loadFactor, const ValueType& emptyValue)
    {
        FIRTEX_ASSERT2(initialCapacity > 0);
        FIRTEX_ASSERT2(loadFactor > 0.0f);

        if (initialCapacity > MAXIMUM_CAPACITY)
            initialCapacity = MAXIMUM_CAPACITY;

        // Find a power of 2 >= initialCapacity
        size_t capacity = 1;
        while (capacity < initialCapacity)
            capacity <<= 1;

        m_fLoadFactor = loadFactor;
        m_nThreshold = (size_t)(capacity * loadFactor);
        m_nSize = 0;
        m_table.resize(capacity);

        m_emptyValue = emptyValue;

        m_bAccessOrder = false;
        m_nModCount = 0;
        init();
    }

    /**
     * Constructs an empty <tt>LinkedHashMap</tt> with the specified initial
     * capacity and the default load factor (0.75).
     *
     * @param  initialCapacity the initial capacity.
     * @throws IllegalArgumentException if the initial capacity is negative.
     */
    LinkedHashMap(size_t initialCapacity, const ValueType& emptyValue) 
    {
        FIRTEX_ASSERT2(initialCapacity > 0);

        if (initialCapacity > MAXIMUM_CAPACITY)
            initialCapacity = MAXIMUM_CAPACITY;

        // Find a power of 2 >= initialCapacity
        size_t capacity = 1;
        while (capacity < initialCapacity)
            capacity <<= 1;

        m_fLoadFactor = DEFAULT_LOAD_FACTOR;
        m_nThreshold = (size_t)(capacity * m_fLoadFactor);
        m_nSize = 0;
        m_table.resize(capacity);
        m_emptyValue = emptyValue;
        m_bAccessOrder = false;
        m_nModCount = 0;

        init();
    }

    /**
     * Constructs an empty <tt>LinkedHashMap</tt> with the default initial capacity
     * (16) and the default load factor (0.75).
     */
    LinkedHashMap(const ValueType& emptyValue) 
    {
        m_fLoadFactor = DEFAULT_LOAD_FACTOR;
        m_nThreshold = (size_t)(DEFAULT_INITIAL_CAPACITY * DEFAULT_LOAD_FACTOR);
        m_nSize = 0;
        m_table.resize(DEFAULT_INITIAL_CAPACITY);
        m_emptyValue = emptyValue;
        m_bAccessOrder = false;
        m_nModCount = 0;

        init();
    }

    ~LinkedHashMap()
    {
        clear();
        delete m_pHeader;
        m_pHeader = NULL;
    }

public:
    class ConstIterator
    {
    public:
        ConstIterator(const LinkedHashMap* pMap, size_t nModCount)
            : m_pMap(pMap)
            , m_pNextEntry(pMap->m_pHeader->after)
            , m_pLastReturned(NULL)
            , m_nExpectedModCount(nModCount)
        {
        }

        bool hasNext() const
        {
            return m_pNextEntry != m_pMap->m_pHeader;
	}

	Entry* nextEntry()
        {
	    FIRTEX_ASSERT2(m_pMap->m_nModCount == m_nExpectedModCount);
            FIRTEX_ASSERT2(m_pNextEntry != m_pMap->m_pHeader);

            Entry* e = m_pLastReturned = m_pNextEntry;
            m_pNextEntry = e->after;
            return e;
	}

        const KeyType& nextKey()
        {
            return nextEntry()->kv.first;
        }

        const ValueType& nextValue()
        {
            return nextEntry()->kv.second;
        }

        const typename Entry::KeyValuePair& nextKeyValue()
        {
            return nextEntry()->kv;
        }

        size_t size() const
        {
            return m_pMap->size();
        }

    private:
        const LinkedHashMap* m_pMap;
	Entry* m_pNextEntry;
	Entry* m_pLastReturned;

	/**
	 * The modCount value that the iterator believes that the backing
	 * List should have.  If this expectation is violated, the iterator
	 * has detected concurrent modification.
	 */
	size_t m_nExpectedModCount;
    };

    class Iterator
    {
    public:
        Iterator(LinkedHashMap* pMap, size_t nModCount)
            : m_pMap(pMap)
            , m_pNextEntry(pMap->m_pHeader->after)
            , m_pLastReturned(NULL)
            , m_nExpectedModCount(nModCount)
        {
        }

        bool hasNext() const
        {
            return m_pNextEntry != m_pMap->m_pHeader;
	}

        void remove() 
        {
            FIRTEX_ASSERT2(m_pLastReturned != NULL);
            FIRTEX_ASSERT2(m_pMap->m_nModCount == m_nExpectedModCount);

            m_pMap->remove(m_pLastReturned->kv.first);
            m_pLastReturned = NULL;
            m_nExpectedModCount = m_pMap->m_nModCount;
	}

	Entry* nextEntry()
        {
	    FIRTEX_ASSERT2(m_pMap->m_nModCount == m_nExpectedModCount);
            FIRTEX_ASSERT2(m_pNextEntry != m_pMap->m_pHeader);

            Entry* e = m_pLastReturned = m_pNextEntry;
            m_pNextEntry = e->after;
            return e;
	}

        const KeyType& nextKey()
        {
            return nextEntry()->kv.first;
        }

        const ValueType& nextValue()
        {
            return nextEntry()->kv.second;
        }

        const typename Entry::KeyValuePair& nextKeyValue()
        {
            return nextEntry()->kv;
        }

        size_t size() const
        {
            return m_pMap->size();
        }

    private:
        LinkedHashMap* m_pMap;
	Entry* m_pNextEntry;
	Entry* m_pLastReturned;

	/**
	 * The modCount value that the iterator believes that the backing
	 * List should have.  If this expectation is violated, the iterator
	 * has detected concurrent modification.
	 */
	size_t m_nExpectedModCount;
    };

public:
    /**
     * Returns the number of key-value mappings in this map.
     *
     * @return the number of key-value mappings in this map
     */
    size_t size() const
    {
        return m_nSize;
    }

    /**
     * Returns <tt>true</tt> if this map contains no key-value mappings.
     *
     * @return <tt>true</tt> if this map contains no key-value mappings
     */
    bool isEmpty() const
    {
        return m_nSize == 0;
    }

    /**
     * Returns the value to which the specified key is mapped,
     * or {@code NULL} if this map contains no mapping for the key.
     *
     * <p>More formally, if this map contains a mapping from a key
     * {@code k} to a value {@code v} such that {@code (key==NULL ? k==NULL :
     * key.equals(k))}, then this method returns {@code v}; otherwise
     * it returns {@code NULL}.  (There can be at most one such mapping.)
     *
     * <p>A return value of {@code NULL} does not <i>necessarily</i>
     * indicate that the map contains no mapping for the key; it's also
     * possible that the map explicitly maps the key to {@code NULL}.
     * The {@link #containsKey containsKey} operation may be used to
     * distinguish these two cases.
     *
     * @see #put(Object, Object)
     */
    const ValueType& get(const KeyType& key)
    {
        Entry* e = getEntry(key);
        if (e == NULL)
        {
            return m_emptyValue;
        }

        e->recordAccess(this);
        return e->kv.second;
    }

    const ValueType& get(const KeyType& key) const
    {
        Entry* e = getEntry(key);
        if (e == NULL)
        {
            return m_emptyValue;
        }

        return e->kv.second;
    }


    /**
     * Returns <tt>true</tt> if this map contains a mapping for the
     * specified key.
     *
     * @param   key   The key whose presence in this map is to be tested
     * @return <tt>true</tt> if this map contains a mapping for the specified
     * key.
     */
    bool containsKey(const KeyType& key) const
    {
        return getEntry(key) != NULL;
    }

    /**
     * Removes all of the mappings from this map.
     * The map will be empty after this call returns.
     */
    void clear() 
    {
        m_nModCount++;
        EntryVector& tab = m_table;
        for (size_t i = 0; i < tab.size(); i++)
        {
            Entry* e = tab[i];
            while (e)
            {
                Entry* t = e;
                e = e->next;
                delete t;
            }
            tab[i] = NULL;
        }
        m_nSize = 0;
        
        m_pHeader->before = m_pHeader->after = m_pHeader;
    }

    /**
     * Associates the specified value with the specified key in this map.
     * If the map previously contained a mapping for the key, the old
     * value is replaced.
     *
     * @param key key with which the specified value is to be associated
     * @param value value to be associated with the specified key
     * @return the previous value associated with <tt>key</tt>, or
     *         <tt>NULL</tt> if there was no mapping for <tt>key</tt>.
     *         (A <tt>NULL</tt> return can also indicate that the map
     *         previously associated <tt>NULL</tt> with <tt>key</tt>.)
     */
    ValueType put(const KeyType& key, const ValueType& value) 
    {
        uint32_t hash = m_hasher(key);
        size_t i = indexFor(hash, m_table.size());
        for (Entry* e = m_table[i]; e != NULL; e = e->next) 
        {
            if (e->hash == hash && (m_comparator(e->kv.first, key)))
            {
                ValueType oldValue = e->kv.second;
                e->kv.second = value;
                e->recordAccess(this);
                return oldValue;
            }
        }

        m_nModCount++;
        addEntry(hash, key, value, i);
        return m_emptyValue;
    }

    /**
     * Removes the mapping for the specified key from this map if present.
     *
     * @param  key key whose mapping is to be removed from the map
     * @return the previous value associated with <tt>key</tt>, or
     *         <tt>NULL</tt> if there was no mapping for <tt>key</tt>.
     *         (A <tt>NULL</tt> return can also indicate that the map
     *         previously associated <tt>NULL</tt> with <tt>key</tt>.)
     */
    ValueType remove(const KeyType& key) 
    {
        Entry* e = removeEntryForKey(key);
        if (e)
        {
            ValueType v = e->kv.second;
            delete e;
            return v;
        }
        else 
        {
            return m_emptyValue;
        }
    }

    ConstIterator iterator() const
    {
        return ConstIterator(this, m_nModCount);
    }

    Iterator iterator()
    {
        return Iterator(this, m_nModCount);
    }

private:
    /**
     * Returns index for hash code h.
     */
    static size_t indexFor(uint32_t h, size_t length) 
    {
        return h & (length - 1);
    }

    /**
     * Initialization hook for subclasses. This method is called
     * in all constructors and pseudo-constructors (clone, readObject)
     * after HashMap has been initialized but before any entries have
     * been inserted.  (In the absence of this method, readObject would
     * require explicit knowledge of subclasses.)
     */
    void init() 
    {
        m_pHeader = new Entry((uint32_t)-1, KeyType(), m_emptyValue, NULL);
        m_pHeader->before = m_pHeader->after = m_pHeader;
    }

    /**
     * Returns the entry associated with the specified key in the
     * HashMap.  Returns NULL if the HashMap contains no mapping
     * for the key.
     */
    Entry* getEntry(const KeyType& key) const
    {
        uint32_t hash = m_hasher(key);
        for (Entry* e = m_table[indexFor(hash, m_table.size())];
             e != NULL; e = e->next) 
        {
            if (e->hash == hash && (m_comparator(e->kv.first, key)))
                return e;
        }
        return NULL;
    }

    /**
     * This method is used instead of put by constructors and
     * pseudoconstructors (clone, readObject).  It does not resize the table,
     * check for comodification, etc.  It calls createEntry rather than
     * addEntry.
     */
    void putForCreate(const KeyType& key, const ValueType& value) 
    {
        uint32_t hash = (key == NULL) ? 0 : m_hasher(key);
        size_t i = indexFor(hash, m_table.size());

        /**
         * Look for preexisting entry for key.  This will never happen for
         * clone or deserialize.  It will only happen for construction if the
         * input Map is a sorted map whose ordering is inconsistent w/ equals.
         */
        for (Entry* e = m_table[i]; e != NULL; e = e->next) 
        {
            if (e->hash == hash && (m_comparator(e->kv.first, key)))
            {
                e->kv.second = value;
                return;
            }
        }

        createEntry(hash, key, value, i);
    }

    /**
     * Rehashes the contents of this map into a new array with a
     * larger capacity.  This method is called automatically when the
     * number of keys in this map reaches its threshold.
     *
     * If current capacity is MAXIMUM_CAPACITY, this method does not
     * resize the map, but sets threshold to Integer.MAX_VALUE.
     * This has the effect of preventing future calls.
     *
     * @param newCapacity the new capacity, MUST be a power of two;
     *        must be greater than current capacity unless current
     *        capacity is MAXIMUM_CAPACITY (in which case value
     *        is irrelevant).
     */
    void resize(size_t newCapacity) 
    {
        size_t oldCapacity = m_table.size();
        if (oldCapacity == MAXIMUM_CAPACITY) 
        {
            m_nThreshold = (std::numeric_limits<int32_t>::max)();
            return;
        }

        EntryVector newTable(newCapacity);
        transfer(newTable);
        m_table.swap(newTable);
        m_nThreshold = (size_t)(newCapacity * m_fLoadFactor);
    }

    /**
     * Transfers all entries from current table to newTable.
     */
    void transfer(EntryVector& newTable) 
    {
        size_t newCapacity = newTable.size();
        for (Entry* e = m_pHeader->after; e != m_pHeader; e = e->after) 
        {
            size_t index = indexFor(e->hash, newCapacity);
            e->next = newTable[index];
            newTable[index] = e;
        }
    }

private:
    /**
     * Removes and returns the entry associated with the specified key
     * in the HashMap.  Returns NULL if the HashMap contains no mapping
     * for this key.
     */
    Entry* removeEntryForKey(const KeyType& key) 
    {
        uint32_t hash = m_hasher(key);
        size_t i = indexFor(hash, m_table.size());
        Entry* prev = m_table[i];
        Entry* e = prev;

        while (e != NULL) 
        {
            Entry* next = e->next;
            if (e->hash == hash && m_comparator(e->kv.first, key))
            {
                m_nModCount++;
                m_nSize--;
                if (prev == e)
                    m_table[i] = next;
                else
                    prev->next = next;
                e->recordRemoval(this);
                return e;
            }
            prev = e;
            e = next;
        }

        return e;
    }


    /**
     * Adds a new entry with the specified key, value and hash code to
     * the specified bucket.  It is the responsibility of this
     * method to resize the table if appropriate.
     *
     * Subclass overrides this to alter the behavior of put method.
     */
    void addEntry(uint32_t hash, const KeyType& key, 
                  const ValueType& value, size_t bucketIndex) 
    {
        createEntry(hash, key, value, bucketIndex);

        if (m_nSize >= m_nThreshold)
        {
            resize(2 * m_table.size());
        }
    }

    /**
     * Like addEntry except that this version is used when creating entries
     * as part of Map construction or "pseudo-construction" (cloning,
     * deserialization).  This version needn't worry about resizing the table.
     *
     * Subclass overrides this to alter the behavior of HashMap(Map),
     * clone, and readObject.
     */
    void createEntry(uint32_t hash, const KeyType& key, 
                     const ValueType& value, size_t bucketIndex) 
    {
        Entry* old = m_table[bucketIndex];
	Entry* e = new Entry(hash, key, value, old);
        m_table[bucketIndex] = e;
        e->addBefore(m_pHeader);
        m_nSize++;
    }

    // These methods are used when serializing HashSets
    size_t capacity() const { return m_table.size(); }
    float loadFactor() const { return m_fLoadFactor; }

protected:
    friend class ConstIterator;
    friend class Iterator;

    /**
     * The table, resized as necessary. Length MUST Always be a power of two.
     */
    EntryVector m_table;

    /**
     * The number of key-value mappings contained in this map.
     */
    size_t m_nSize;

    /**
     * The next size value at which to resize (capacity * load factor).
     * @serial
     */
    size_t m_nThreshold;

    /**
     * The load factor for the hash table.
     *
     * @serial
     */
    float m_fLoadFactor;

    /// The head of the doubly linked list.
    Entry* m_pHeader;

    /**
     * The iteration ordering method for this linked hash map: <tt>true</tt>
     * for access-order, <tt>false</tt> for insertion-order.
     */
    bool m_bAccessOrder;

    /**
     * The number of times this HashMap has been structurally modified
     * Structural modifications are those that change the number of mappings in
     * the HashMap or otherwise modify its internal structure (e.g.,
     * rehash).  This field is used to make iterators on Collection-views of
     * the HashMap fail-fast.  (See ConcurrentModificationException).
     */
    volatile size_t m_nModCount;

    /// Empty value
    ValueType m_emptyValue;

    /// Hash function
    static HashType m_hasher;
    
    /// Key comparator
    static CompType m_comparator;
};

template <typename K, typename V, typename HashFunc, typename Comp> 
const float LinkedHashMap<K,V, HashFunc, Comp>::DEFAULT_LOAD_FACTOR = 0.75f;

template <typename K, typename V, typename HashFunc, typename Comp> 
typename LinkedHashMap<K,V, HashFunc, Comp>::CompType LinkedHashMap<K,V, HashFunc, Comp>::m_comparator;

template <typename K, typename V, typename HashFunc, typename Comp> 
typename LinkedHashMap<K,V, HashFunc, Comp>::HashType LinkedHashMap<K,V, HashFunc, Comp>::m_hasher;


FX_NS_END

#endif //__FX_LINKEDHASHMAP_H
