//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2011-03-17 23:08:53

/*
 * Reference:
 * [1] An Implementation of Double-Array Trie 
 *     (http://linux.thai.net/~thep/datrie/datrie.html)
 * [2] libtrie: An implementation of Double-Array Trie
 *     (http://jianingy.github.com/libtrie/index.html)
 */

#ifndef __FX_DOUBLEARRAYTRIE_H
#define __FX_DOUBLEARRAYTRIE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/MMapFile.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(analyzer);

class DoubleArrayTrie
{
public:
    static const size_t ALPHABET_SIZE = 257;
    static const size_t MAGIC_SIZE = 8;
    
    typedef int ValueType;
    static const ValueType INVALID_VALUE = 0;
    static const ValueType INIT_STATE = 1;

public:

    class TraversalCallback
    {
    public:
        virtual ~TraversalCallback() {}

    public:
        /**
         * Invokes the callback member function.
         */
        virtual void invoke(const char* szKey, ValueType value) = 0;

    protected:
        TraversalCallback(){}
        TraversalCallback(const TraversalCallback&){}
    };

    DEFINE_TYPED_PTR(TraversalCallback);

    template <class T>
    class TypedTraversalCallback : public TraversalCallback
    {
    public:
        typedef void (T::*Callback)(const char* szKey, ValueType value);
        
    public:
        TypedTraversalCallback(T* pObject, Callback method)
            : m_pObject(pObject)
            , m_fnMethod(method)
        {
        }

        TypedTraversalCallback(const TypedTraversalCallback& cb)
            : m_pObject(cb.m_pObject)
            , m_fnMethod(cb.m_fnMethod)
        {
        }

        ~TypedTraversalCallback()
        {
        }

    public:
        void invoke(const char* szKey, ValueType value)
        {
            (m_pObject->*m_fnMethod)(szKey, value);
        }

    private:
        TypedTraversalCallback();

    protected:
        T* m_pObject;
        Callback m_fnMethod;
    };


public:
    DoubleArrayTrie();
    DoubleArrayTrie(size_t nNumElems);
    virtual ~DoubleArrayTrie();

public:
    /**
     * Load double array trie from lexicon file
     * @throw FileIOException if load failed
     */
    void load(const tstring& sFileName);

    /**
     * Build lexicon from file.
     * format(per line): term freq
     */
    void buildFromFile(const tstring& sFileName);

    /**
     * Save the trie to file
     */
    void save(const tstring& sFileName);

public:
    /**
     * Search key-value pair.
     * @param szKey to search
     * @return value associated with the key.
     *         return INVALID_VALUE if no such key.
     */
    ValueType search(const char* szKey) const;

    /**
     * Search key-value pair.
     * @param szKey to search
     * @return value associated with the key.
     *         return INVALID_VALUE if no such key.
     */
    ValueType search(const char* szKey, size_t length) const;

    /**
     * Max forward search a key-value pair
     * @param szKey to search
     * @param keyEnd indicate the tail of matched key prefix, 
     *               equal to /szKey/ if match failed.
     * @return value associated with the key.
     *         return INVALID_VALUE if match failed.
     */
    ValueType search(const char* szKey, const char** keyEnd) const;

    /**
     * Max forward search a key-value pair
     * @param szKey to search
     * @param length length of key
     * @param keyEnd indicate the tail of matched key prefix, 
     *               equal to /szKey/ if match failed.
     * @return value associated with the key.
     *         return INVALID_VALUE if match failed.
     */
    ValueType search(const char* szKey, size_t length, const char** keyEnd) const;

    /**
     * Insert a key-value pair.
     * @param szKey a '\0' terminated string key
     * @param val value associated with the key
     */
    void insert(const char* szKey, ValueType val);

public:
    /**
     * Traversal the entire tree
     */
    void traversal();

    /**
     * Set traversal callback object
     */
    void setTraversalCallback(const TraversalCallbackPtr& pCallback);

    /**
     * Set traversal callback object
     */
    template <typename T>
    void setTraversalCallback(T* pObj,
                              typename TypedTraversalCallback<T>::Callback callback)
    {
        setTraversalCallback(new TypedTraversalCallback<T>(pObj, callback));
    }

public:
    ValueType getMaxValue() const;
    ValueType getMinValue() const;
    uint64_t getSumValue() const;
    ValueType getInsertCount() const;

private:
    ValueType key2state(ValueType ch) const;
    ValueType state2key(ValueType s) const;
    ValueType getBase(ValueType s) const;
    ValueType getCheck(ValueType s) const;
    void setBase(ValueType s, ValueType val);
    void setCheck(ValueType s, ValueType val);

    ValueType forceNext(ValueType s, ValueType ch);
    ValueType forceForward(ValueType s, ValueType ch);

    ValueType next(ValueType s, ValueType ch) const;
    ValueType forward(ValueType s, ValueType ch) const;

    void resizeState(size_t nNewSize);

    ValueType findAccepts(ValueType s, ValueType *inputs, ValueType *max, ValueType *min);

    void updateStatistics(ValueType val);

    void onTraversalFinish(ValueType s, ValueType off);

    ValueType findBase(ValueType *key, ValueType max, ValueType min);
    ValueType relocate(ValueType stand, ValueType s, ValueType *key, ValueType max, ValueType min);
    ValueType createTransition(ValueType s, ValueType ch);
    void doTraversal(ValueType s, ValueType off);

private:
    static const ValueType TRAVERSAL_BUFF_SIZE = 1024;

#pragma pack(push, 4)
    struct Header
    {
        char magic[MAGIC_SIZE];
        ValueType num;
        ValueType max, min;
        uint64_t sum;
        uint32_t insertCount;
    };

    typedef struct 
    {
        ValueType base;
        ValueType check;
    } State;
#pragma pack(pop)

private:
    ValueType m_lastState;

    Header* m_header;
    State * m_states;
    FX_NS(utility)::MMapFile* m_pMMapFile;

    TraversalCallbackPtr m_pTraversalCallback;
    char m_traveralBuff[TRAVERSAL_BUFF_SIZE];

private:
    DoubleArrayTrie(DoubleArrayTrie &) {}

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(DoubleArrayTrie);

////////////////////////////////////////////////
inline void DoubleArrayTrie::setTraversalCallback(
        const TraversalCallbackPtr& pCallback)
{
    m_pTraversalCallback = pCallback;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::getMaxValue() const
{
    return m_header->max;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::getMinValue() const
{
    return m_header->min;
}

inline uint64_t
DoubleArrayTrie::getSumValue() const
{
    return m_header->sum;
}

inline DoubleArrayTrie::ValueType
 DoubleArrayTrie::getInsertCount() const
{
    return m_header->insertCount;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::key2state(ValueType ch) const
{
    return ch + 1;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::state2key(ValueType s) const
{
    FIRTEX_ASSERT2(s > 0);
    return s - 1;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::getBase(ValueType s) const
{
    FIRTEX_ASSERT2(s > 0 && s < m_header->num);
    return m_states[s].base;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::getCheck(ValueType s) const
{
    FIRTEX_ASSERT2(s > 0 && s < m_header->num);
    return m_states[s].check;
}

inline void DoubleArrayTrie::setBase(ValueType s, ValueType val)
{
    FIRTEX_ASSERT2(s > 0 && s < m_header->num);
    m_states[s].base = val;
}

inline void DoubleArrayTrie::setCheck(ValueType s, ValueType val)
{
    FIRTEX_ASSERT2(s > 0 && s < m_header->num);
    m_states[s].check = val;
}

inline DoubleArrayTrie::ValueType 
DoubleArrayTrie::forceNext(ValueType s, ValueType ch)
{
    ValueType in = key2state(ch);

    FIRTEX_ASSERT2(s > 0 && s < m_header->num); 
    if (s + in >= m_header->num) resizeState(in);
    return getBase(s) + in;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::forceForward(ValueType s, ValueType ch)
{
    FIRTEX_ASSERT2(s > 0 && s < m_header->num); 
    ValueType t = forceNext(s, ch);
    return (t > 0 && t < m_header->num && getCheck(t) == s) ? t : 0;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::next(ValueType s, ValueType ch) const
{
    ValueType in = key2state(ch);

    FIRTEX_ASSERT2(s > 0 && s < m_header->num); 
    if (s + in >= m_header->num) return 0;
    return getBase(s) + in;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::forward(ValueType s, ValueType ch) const
{
    FIRTEX_ASSERT2(s > 0 && s < m_header->num); 
    ValueType t = next(s, ch);
    return (t > 0 && t < m_header->num && getCheck(t) == s) ? t : 0;
}

inline void DoubleArrayTrie::resizeState(size_t nNewSize)
{
    size_t nAlignSize;

    // make 4096 align
    nAlignSize = (((m_header->num + nNewSize) >> 12) + 1) << 12; 
    m_states = (State *)realloc(m_states, nAlignSize * sizeof(State));
    memset(m_states + m_header->num, 0, (nAlignSize - m_header->num) * sizeof(State));
    if (m_states == NULL)
    {
        FIRTEX_THROW(OutOfMemoryException, "Resize state pool FAILED.");
    }
    m_header->num = (ValueType)nAlignSize;
}

inline DoubleArrayTrie::ValueType
DoubleArrayTrie::findAccepts(ValueType s, ValueType *inputs,
                             ValueType *max, ValueType *min)
{
    ValueType ch;
    ValueType *p;

    FIRTEX_ASSERT2(s > -1 && s < m_header->num);
    FIRTEX_ASSERT2(inputs);
    for (ch = 0, p = inputs; ch < (ValueType)ALPHABET_SIZE; ch++) 
    {
        if (forceForward(s, ch)) 
        {
            *(p++) = ch;
            if (max && (ch > *max || *max == 0)) *max = ch;
            if (min && (ch < *min || *min == 0)) *min = ch;
        }
    }
    *p = -1;
    return (ValueType)(p - inputs);
}

inline void DoubleArrayTrie::updateStatistics(ValueType val)
{
    m_header->max = (val > m_header->max) ? val : m_header->max;
    m_header->min = (val < m_header->min) ? val : m_header->min;
    m_header->sum += val;
    m_header->insertCount++;
}

inline void DoubleArrayTrie::onTraversalFinish(ValueType s, ValueType off)
{
    if (m_pTraversalCallback.isNotNull())
    {
        m_pTraversalCallback->invoke(m_traveralBuff, getBase(s));
    }
}

FX_NS_END

#endif //__FX_DOUBLEARRAY_H

