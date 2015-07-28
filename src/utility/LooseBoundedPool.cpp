#include "firtex/utility/LooseBoundedPool.h"

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, LooseBoundedPool);

LooseBoundedPool::LooseBoundedPool(size_t nPoolSize)
    : m_nExcepChunkSize(DEFAULT_EXCEPTION_CHUNK_SIZE)
    , m_eAllocState(STATE_NORMAL)
    , m_pNormPool(NULL)
    , m_pSimpleAllocator(new SimpleAllocator)
    , m_excepPool(m_pSimpleAllocator, DEFAULT_EXCEPTION_CHUNK_SIZE)
{
    SyncSegregatedAllocatorPtr pAllocator(new SyncSegregatedAllocator(nPoolSize));
    m_pBoudedAllocator.reset(new BoundedChunkAllocator(pAllocator));
    bool ret = m_pBoudedAllocator->init(1);
    FIRTEX_ASSERT2(ret);
    (void)ret;

    m_pNormPool = new NormalPool(m_pBoudedAllocator, m_pBoudedAllocator->getChunkSize());
}

LooseBoundedPool::LooseBoundedPool(const BoundedChunkAllocatorPtr& pAllocator)
    : m_nExcepChunkSize(DEFAULT_EXCEPTION_CHUNK_SIZE)
    , m_eAllocState(STATE_NORMAL)
    , m_pBoudedAllocator(pAllocator)
    , m_pNormPool(new NormalPool(pAllocator, pAllocator->getChunkSize()))
    , m_pSimpleAllocator(new SimpleAllocator)
    , m_excepPool(m_pSimpleAllocator, DEFAULT_EXCEPTION_CHUNK_SIZE)
{
}

LooseBoundedPool::~LooseBoundedPool() 
{
    clear();
    if (m_pNormPool)
    {
        delete m_pNormPool;
        m_pNormPool = NULL;
    }
}

void* LooseBoundedPool::tryAllocate(size_t nBytes)
{
    if (m_eAllocState == STATE_NORMAL)
    {
        void* p = m_pNormPool->allocate(nBytes);
        if (p)
        {
            return p;
        }
    }
    return NULL;
}

void* LooseBoundedPool::allocate(size_t nBytes)
{
    if (m_eAllocState == STATE_NORMAL)
    {
        void* p = m_pNormPool->allocate(nBytes);
        if (p)
        {
            return p;
        }
    }

    FX_DEBUG("Allocate [%u] bytes from exception pool", nBytes);
    FX_DEBUG("Pool state: [%llu] in normal pool, [%llu] in exception pool",
             (unsigned long long)m_pNormPool->getAllocatedBytes(),
             (unsigned long long)m_excepPool.getAllocatedBytes());

    m_eAllocState = STATE_EXCEPTION;
    void * p = m_excepPool.allocate(nBytes);
    if (!p)
    {
        FX_LOG(WARN, "Allocate [%llu] bytes from exception pool FAILED, "
                "continue to allocate from system", (unsigned long long)nBytes);
        return m_pSimpleAllocator->allocate(nBytes);
    }
    return p;
}

void LooseBoundedPool::clear()
{
    if (m_pNormPool)
    {
        m_pNormPool->clear();
    }
    m_excepPool.clear();
    m_pSimpleAllocator->clear();
}

FX_NS_END

