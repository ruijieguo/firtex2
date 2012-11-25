#ifndef __FX_HASHMAP_H
#define __FX_HASHMAP_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Hash.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/OutputStream.h"
#include "firtex/utility/SimpleAllocator.h"
#include "firtex/utility/HashComparator.h"
#include <assert.h>

FX_NS_DEF(utility);

#pragma pack(push, 1)

////////////////////////////////////////////////
//Bucket
template <typename T, typename Pool>
class Bucket
{
public:
    typedef T ValueType;
    typedef Pool PoolType;
    typedef Bucket Self;

public:
    const static size_t INITIAL_N = 8;
    const static size_t FACTOR_N = 2;

public:
    struct Block  
    {
        Block(size_t n, size_t i = 0) 
            : size(n)
            , sizeMinusOne(n - 1)
            , index(i)
            , prev(NULL)
            , next(NULL)
        { 
            end = &(begin[n]);
        }

        size_t size;
        size_t sizeMinusOne;
        size_t index; 
        Block* prev;
        Block* next;
        T* end;
        T begin[1];
    };    

public:
    Bucket(PoolType* pPool) : m_pBlocks(NULL), m_nNumBlocks(0),
                              m_nCapacity(0), m_pMemPool(pPool)
    {
    }

    ~Bucket() 
    { 
        clear();
    } 

public:
    Block* getFirstBlock()
    {
        Block* pCur = m_pBlocks;
        if (pCur == NULL) return NULL;
        while (pCur->prev != NULL) 
        {
            pCur = pCur->prev;
        }
        return pCur;
    }

    const Block* getFirstBlock() const 
    {
        return const_cast<Self*>(this)->getFirstBlock();
    }

    Block* getLastBlock()
    {
        return m_pBlocks;
    }

    const Block* getLastBlock() const 
    {
        return m_pBlocks;
    }

    size_t getNumBlocks() const {return m_nNumBlocks;}

    size_t capacity() const 
    {
        return m_nCapacity;     
    }

    void initialize(size_t n) 
    {
        assert(m_pBlocks == NULL);
        addBlock(new Block(n, 0));
    }

    void addBlock() 
    {
        if (m_pBlocks == NULL) 
        {
            void* p = m_pMemPool->allocate(INITIAL_N * sizeof(T) + sizeof(Block));
            addBlock(new(p) Block(INITIAL_N, 0));
        } 
        else 
        {
            size_t n = m_pBlocks->size * FACTOR_N;
            void* p = m_pMemPool->allocate(n * sizeof(T) + sizeof(Block));
            addBlock(new(p) Block(n, m_nCapacity));
        }
    }    

    void addBlock(Block* x) 
    {
        assert(x != NULL);
        x->prev = m_pBlocks;
        if (m_pBlocks != NULL) m_pBlocks->next = x;
        m_pBlocks = x;
        m_nCapacity += m_pBlocks->size;
        ++m_nNumBlocks;
    }    

    void clear() 
    {
        m_pBlocks = NULL;
        m_nCapacity = 0;
        m_nNumBlocks = 0;
    }

private:
    Block* m_pBlocks;
    size_t m_nNumBlocks;
    size_t m_nCapacity;
    PoolType* m_pMemPool; 
};

#pragma pack(pop)



/////////////////////////////////////////////////////////////////////////////
//HashMap

template <typename Key, typename Value, typename Pool = SimpleAllocator, typename HashFunc = Hasher<Key>,
          typename Comp = HashComparator<Key>, int Sparsity = 2>
class HashMap
{
public:
    typedef Key KeyType;
    typedef Value ValueType;
    typedef std::pair<Key, Value> KeyValueType;
    typedef Pool PoolType;
    typedef Bucket<KeyValueType, PoolType> BucketType;    
    typedef typename BucketType::Block BlockType;

    typedef HashMap Self;

public:
    class Iterator
    {
    public:
        Iterator()
            : m_pMapSelf()
            , m_pBuckets(NULL)
            , m_pCurrentBlock(NULL)
            , m_pCurrentKV(NULL)
        {
        }

        Iterator(const Self* m)
            : m_pMapSelf(m)
            , m_pBuckets(m_pMapSelf->m_pBucket)
            , m_pCurrentBlock(NULL)
            , m_pCurrentKV(NULL)
        {   
            reset();
        }

        void reset()
        {
            m_pCurrentBlock = m_pBuckets->getFirstBlock();
            while (m_pCurrentBlock)
            {
                m_pCurrentKV = m_pCurrentBlock->begin - 1;
                while (true)
                {
                    ++m_pCurrentKV;
                    if (m_pCurrentKV >= m_pCurrentBlock->end)
                    {
                        break;
                    }
                    if (!m_pMapSelf->isEmpty(m_pCurrentKV))
                    {
                        return;
                    }
                };

                m_pCurrentBlock = m_pCurrentBlock->next;
            }
            m_pCurrentKV = NULL;
        }

        inline bool hasNext() const
        {
            return m_pCurrentKV && !m_pMapSelf->isEmpty(m_pCurrentKV);
        }

        inline KeyValueType& next()
        {
            KeyValueType& kv = *m_pCurrentKV;
            while (m_pCurrentBlock)
            {
                while (true)
                {
                    ++m_pCurrentKV;
                    if (m_pCurrentKV >= m_pCurrentBlock->end)
                    {
                        break;
                    }
                    if (!m_pMapSelf->isEmpty(m_pCurrentKV))
                    {
                        return kv;
                    }
                };

                m_pCurrentBlock = m_pCurrentBlock->next;
                if (m_pCurrentBlock)
                {
                    m_pCurrentKV = m_pCurrentBlock->begin - 1;
                }
                else 
                {
                    m_pCurrentKV = NULL;
                }
            }
            return kv;
        }

    private:
        const Self* m_pMapSelf;
        BucketType* m_pBuckets;
        BlockType* m_pCurrentBlock;
        KeyValueType* m_pCurrentKV;
    };

public:
    HashMap(PoolType* pPool, const KeyType& emptyKey, const ValueType& emptyData);
    HashMap(const KeyType& emptyKey, const ValueType& emptyData);
    ~HashMap();

public:
    inline const ValueType& operator[](const KeyType& x) const;
    inline const ValueType& find(const KeyType& x) const;
    inline void replace(const KeyType& x, const ValueType& value);
    inline Iterator iterator() const { return Iterator(this);}
    inline void insert(const KeyType& key, const ValueType& value) ;
    inline const ValueType& emptyValue() const;

public:
    void insert(const KeyValueType& x) ;
    size_t size() const {return m_nElemCount;}
    void clear();
    void load(FX_NS(store)::InputStreamPtr& pInStream);
    void save(FX_NS(store)::OutputStreamPtr& pOutStream);

private:
    inline size_t threshold();
    inline bool isEmpty(KeyValueType* x) const;
    KeyValueType* lookup(const KeyType& x) const;
    KeyValueType* findEmptySpace(const KeyType& x);
    inline bool isValid() const;

private:
    HashFunc m_hasher;
    Comp m_comparator;
    size_t m_nElemCount;
    BucketType* m_pBucket;

    ValueType m_noData;  
    KeyValueType m_emptyValue;

    PoolType* m_pMemPool;
    bool m_bOwnPool;
};

/////////////////////////////////////////////////////////
//Implementation
template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::HashMap(PoolType* pPool, const KeyType& emptyKey, const ValueType& emptyData)
    : m_nElemCount(0)
    , m_pBucket(new BucketType(pPool))
    , m_noData(emptyData)
    , m_emptyValue(emptyKey, emptyData)
    , m_pMemPool(pPool)
    , m_bOwnPool(false)
{ }

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::HashMap(const KeyType& emptyKey, const ValueType& emptyData)
    : m_nElemCount(0)
    , m_noData(emptyData)
    , m_emptyValue(emptyKey, emptyData)
    , m_pMemPool(new PoolType())
    , m_bOwnPool(true)
{ 
    m_pBucket = new BucketType(m_pMemPool);
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::~HashMap()
{
    clear();

    delete m_pBucket;
    m_pBucket = NULL;
    if (m_bOwnPool && m_pMemPool)
    {
        delete m_pMemPool;
        m_pMemPool = NULL;
    }
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline const Value& HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::operator[](const KeyType& x) const
{      
    return find(x);
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline const Value& HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::find(const KeyType& x) const
{      
    assert(isValid());
    KeyValueType* p = lookup(x);
    if (p != NULL) 
    {
        return p->second;
    }
    return m_noData;
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline void HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::replace(const KeyType& x, const ValueType& value)
{
    assert(isValid());
    KeyValueType* p = lookup(x);
    if (p != NULL) 
    {
        p->second = value;
    }
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline void HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::insert(const KeyType& key, const ValueType& value) 
{
    insert(make_pair(key, value));
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline const typename HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::ValueType& 
HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::emptyValue() const
{
    return m_noData;
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
void HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::insert(const KeyValueType& x) 
{      
    assert(isValid());
    if (++m_nElemCount >= threshold()) 
    {
        m_pBucket->addBlock();
        KeyValueType* first = m_pBucket->getLastBlock()->begin;
        KeyValueType* sizeMinusOne = m_pBucket->getLastBlock()->end;
        while (first != sizeMinusOne) 
        {
            new(first++) KeyValueType(m_emptyValue);
        }         
    }
    KeyValueType* p = findEmptySpace(x.first);
    new(p) KeyValueType(x);
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
void HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::clear()
{
    m_pBucket->clear();
    m_nElemCount = 0;
    if (m_bOwnPool && m_pMemPool)
    {
        m_pMemPool->clear();
    }
}
    
template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
void HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::load(FX_NS(store)::InputStreamPtr& pInStream)
{
    m_nElemCount = (size_t)pInStream->readInt64();
    int64_t nNumBlocks = pInStream->readInt64();
    size_t index, sizeMinusOne, size;
    while (nNumBlocks > 0)
    {
        index = (size_t)pInStream->readVInt32();
        sizeMinusOne = (size_t)pInStream->readVInt32();
        size = (size_t)pInStream->readVInt32();
        void* p = m_pMemPool->allocate(size * sizeof(KeyValueType) + sizeof(BlockType));
        BlockType* pBlock = new(p) BlockType(size, index);
        pInStream->read((void*)pBlock->begin, size * sizeof(KeyValueType));
        pBlock->sizeMinusOne = sizeMinusOne;

        this->m_pBucket->addBlock(pBlock);
        --nNumBlocks;
    }
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
void HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::save(FX_NS(store)::OutputStreamPtr& pOutStream)
{
    if (!this->m_pBucket || m_nElemCount == 0)
    {
        return;
    }

    pOutStream->writeInt64((int64_t)m_nElemCount);
    pOutStream->writeInt64((int64_t)m_pBucket->getNumBlocks());
    BlockType* pBlock = m_pBucket->getFirstBlock();
    
    while (pBlock)
    {
        pOutStream->writeVInt32((int32_t)pBlock->index);
        pOutStream->writeVInt32((int32_t)pBlock->sizeMinusOne);
        pOutStream->writeVInt32((int32_t)pBlock->size);
        pOutStream->write((const void*)pBlock->begin, pBlock->size * sizeof(KeyValueType));

        pBlock = pBlock->next;
    }
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline size_t HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::threshold() 
{
    return m_pBucket->capacity() / Sparsity;
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline bool HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::isEmpty(KeyValueType* x) const
{
    return m_comparator(x->first, m_emptyValue.first);
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
typename HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::KeyValueType* 
HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::lookup(const KeyType& x) const
{
    BlockType* buff = m_pBucket->getLastBlock();

    while (buff) 
    {
        KeyValueType* p = buff->begin + (m_hasher(x) & buff->sizeMinusOne);
        size_t delta = 0;
        while (!isEmpty(p)) 
        {
            if (m_comparator(p->first, x)) return p;
            if ((p += ++delta) >= buff->end) p -= buff->size;
        }
        buff = buff->prev;
    }
    return NULL;
}

template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
typename HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::KeyValueType*
HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::findEmptySpace(const KeyType& x) 
{
    BlockType* buff = m_pBucket->getLastBlock();
    KeyValueType* p = buff->begin + (m_hasher(x) & buff->sizeMinusOne);
    size_t delta = 0;
    while (!isEmpty(p)) 
    {
        if (m_comparator(p->first, x)) return p;
        if ((p += ++delta) >= buff->end) p -= buff->size;
    }
    return p;
}       
     
template <typename Key, typename Value, typename Pool, typename HashFunc, typename Comp, int Sparsity>
inline bool HashMap<Key, Value, Pool, HashFunc, Comp, Sparsity>::isValid() const
{
    return (m_nElemCount >= 0) && (m_nElemCount <= (size_t)m_pBucket->capacity() / Sparsity);
}

FX_NS_END

#endif
