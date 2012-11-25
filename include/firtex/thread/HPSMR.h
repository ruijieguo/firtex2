#ifndef __FX_HP_SMR_H
#define __FX_HP_SMR_H

#include <vector>
#include <algorithm>    // std::fill
#include <memory>
#include "firtex/thread/Thread.h"
#include "firtex/thread/atomic/AtomicIntegrals.h"
#include "firtex/common/Logger.h"
#include "firtex/thread/Atomic.h"

#ifdef FX_WINDOWS
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#endif

FX_NS_DEF(thread);

/////////////////////////////////////////////////////
template <int MASK, typename T>
static inline int isMarkedBit(T* p)
{
    return reinterpret_cast<atomic::uptr_atomic_t>(p) & MASK;
}
template <int MASK, typename T>
static inline T* markBit(T* p)
{
    return reinterpret_cast< T*>(reinterpret_cast<atomic::uptr_atomic_t>(p) | MASK);
}
template <int MASK, typename T>
static inline T* unmarkBit(T* p)
{
    return reinterpret_cast< T*>(reinterpret_cast<atomic::uptr_atomic_t>(p) & ~MASK);
}

class GarbageCollector;

typedef void* HazardPtr;

/// Free hazard pointer constant
static const HazardPtr NullHazardPtr = NULL;


/// Define function to free retired pointer
typedef void (* freeRetiredPtrFunc) (void *);
struct RetiredPtr 
{
    /// Pointer type
    typedef void* pointer;

    ///< retired pointer
    pointer m_p;
    ///< pointer to the destructor function
    freeRetiredPtrFunc m_funcFree;

    /// Comparison of two retired pointers
    static bool less(const RetiredPtr& p1, const RetiredPtr& p2)
    {
        return p1.m_p < p2.m_p ;
    }

    RetiredPtr()
        : m_p(NULL)
        , m_funcFree(NULL)
    {}

    RetiredPtr(pointer p, freeRetiredPtrFunc func)
        : m_p(p)
        , m_funcFree(func)
    {}

    /// Typecasting ctor
    template <typename T>
    RetiredPtr(T* p, void (*pFreeFunc)(T*))
        : m_p(reinterpret_cast<pointer>(p))
        , m_funcFree(reinterpret_cast<freeRetiredPtrFunc>(pFreeFunc))
    {}

    /// Assignment operator
    RetiredPtr& operator =(const RetiredPtr& s)
    {
        m_p = s.m_p;
        m_funcFree = s.m_funcFree;
        return *this;
    }

    void free()
    {
        FIRTEX_ASSERT2(m_funcFree != NULL);
        m_funcFree(m_p);
    }

    pointer getPointer() const 
    {
        return m_p;
    }
};

static inline bool operator < (const RetiredPtr& p1, const RetiredPtr& p2)
{
    return RetiredPtr::less(p1, p2);
}

static inline bool operator == (const RetiredPtr& p1, const RetiredPtr& p2)
{
    return p1.m_p == p2.m_p;
}

static inline bool operator != (const RetiredPtr& p1, const RetiredPtr& p2)
{
    return !(p1 == p2);
}

///////////////////////////////////////////////////////////
/// Upper bounded dynamic array
template <typename T>
class BoundedArray: private std::vector<T>
{
public:
    typedef std::vector<T> VectorType;

    typedef typename VectorType::iterator iterator;
    typedef typename VectorType::const_iterator const_iterator;    ///< item const iterator

public:
    explicit BoundedArray(size_t nCapacity)
    {
        VectorType::resize(nCapacity);
        FIRTEX_ASSERT2(size() == capacity());
    }
    
    BoundedArray(size_t nCapacity, const T& init, size_t nInitCount = 0)
    {
        FIRTEX_ASSERT2(nInitCount <= nCapacity);
        VectorType::resize(nCapacity);
        assign(nInitCount ? nInitCount : VectorType::capacity(), init);
        FIRTEX_ASSERT2(size() == capacity());
    }

    const T& operator [](size_t nItem) const { return VectorType::operator[](nItem); }
    T& operator [](size_t nItem) { return VectorType::operator[](nItem); }
    size_t size() const { return VectorType::size(); }
    size_t capacity() const { return VectorType::capacity(); }
    void assign(size_t nCount, const T& val)
    {
        FIRTEX_ASSERT2(nCount <= size())  ;
        std::fill(begin(), begin() + nCount, val) ;
    }

    static size_t itemSize()
    {
        return sizeof(T);
    }

    T* top()
    {
        return & VectorType::front();
    }

    friend T* operator + (BoundedArray<T>& arr, size_t i)
    {
        return &(arr[i]);
    }

    const_iterator begin() const { return VectorType::begin();}
    iterator begin() { return VectorType::begin();}

    const_iterator end() const { return VectorType::end(); }
    iterator end() { return VectorType::end(); }

    const_iterator end(size_t nMax) const
    {
        FIRTEX_ASSERT2(nMax <= VectorType::capacity());
        return VectorType::begin() + nMax;
    }
    iterator end(size_t nMax)
    {
        FIRTEX_ASSERT2(nMax <= VectorType::capacity());
        return VectorType::begin() + nMax;
    }
};

///////////////////////////////////////////////////////
/// Array of retired pointers
class RetiredVector 
{
    typedef BoundedArray<RetiredPtr> RetiredVectorImpl;

    RetiredVectorImpl m_arr;
    size_t m_nSize;

public:
    typedef RetiredVectorImpl::iterator iterator;

    RetiredVector(const GarbageCollector& hzpMgr);
    ~RetiredVector() {}
    
public:
    size_t capacity() const { return m_arr.capacity(); }

    size_t size() const { return m_nSize; }

    void size(size_t nSize)
    {
        FIRTEX_ASSERT2(nSize <= capacity());
        m_nSize = nSize;
    }

    void push(const RetiredPtr& p)
    {
        FIRTEX_ASSERT2(m_nSize < capacity());
        m_arr[ m_nSize ] = p;
        ++m_nSize;
    }

    bool isFull() const
    {
        return m_nSize >= capacity() ;
    }

    iterator begin() { return m_arr.begin(); }
    iterator end() { return m_arr.end(m_nSize); }

    void clear()
    {
        m_nSize = 0;
    }
};

///////////////////////////////////////////////////
/// Hazard pointer guard
template <typename HPTYPE>
class HPGuardT
{
    HPTYPE* m_hzp;

    template <typename HPTYPE2> friend class HPAllocator;

public:
    HPGuardT()
        : m_hzp(NULL)
    {}
    ~HPGuardT()
    {}

    template <typename T>
    T* operator = (T* p)
    {
        /// We use atomic store with explicit memory order because other 
        /// threads may read this hazard pointer concurrently
        atomic::store<membar_release>(m_hzp, reinterpret_cast<HPTYPE>(p));
        return p;
    }

    operator HPTYPE() { return *m_hzp; }
    HPTYPE& getHPRef() { return *m_hzp; }

    void clear()
    {
        *m_hzp = NULL;
    }
};

typedef HPGuardT<HazardPtr> HPGuard;

////////////////////////////////////////////////////
/// Array of hazard pointers.
template <typename HPTYPE, size_t COUNT>
class HPArrayT
{
    HPTYPE* m_arr;   ///< Hazard pointer array of size = COUNT

    template <typename HPTYPE2> friend class HPAllocator;

public:
    HPArrayT()
    {}
    ~HPArrayT() {}

    size_t capacity() const { return COUNT; }

    void set(size_t nIndex, HPTYPE hzPtr)
    {
        FIRTEX_ASSERT2(nIndex < COUNT);
        atomic::store<membar_release>(m_arr + nIndex, hzPtr);
    }

    HPTYPE& operator [](size_t nIndex)
    {
        FIRTEX_ASSERT2(nIndex < COUNT);
        return *(m_arr + nIndex);
    }

    void clear(size_t nIndex)
    {
        FIRTEX_ASSERT2(nIndex < COUNT);
        m_arr[ nIndex ] = NullHazardPtr;// memory order is not necessary here (by default, relaxed)
    }
};

template <size_t COUNT>
class HPArray: public HPArrayT<HazardPtr, COUNT>
{};

//////////////////////////////////////////////////////////////
/// Allocator of hazard pointers for the thread
template <typename HPTYPE>
class HPAllocator
{
    typedef BoundedArray<HPTYPE>THazardArray;
    
    THazardArray m_arrHazardPtr;
    size_t m_nTop;

public:
    explicit HPAllocator(size_t nCapacity)
        : m_arrHazardPtr(nCapacity)
        , m_nTop(nCapacity)
    {
        FX_DEBUG("Construct hp allocator: this: [%p], capacity: [%u]", 
                 this, (uint32_t)nCapacity);
        makeFreeStack();
        FIRTEX_ASSERT2(m_arrHazardPtr.size() == capacity());
    }

    ~HPAllocator()
    {
        FIRTEX_ASSERT2(size() == capacity());
    }

    size_t capacity() const { return m_arrHazardPtr.capacity(); }
    size_t size() const { return m_arrHazardPtr.size(); }
    bool isFull() const { return m_nTop == 0; }

    void alloc(HPGuardT<HPTYPE>& hp)
    {
        FIRTEX_ASSERT2(m_nTop > 0);
        --m_nTop;
        FX_DEBUG("Alloc hp pointer, this: [%p], top: [%lu]", this, m_nTop);
        hp.m_hzp = m_arrHazardPtr + m_nTop;
        FIRTEX_ASSERT2(size() == capacity());
    }

    void free(HPGuardT<HPTYPE>& /*hp*/)
    {
        FIRTEX_ASSERT2(m_nTop < capacity());
        m_arrHazardPtr[m_nTop] = HPTYPE(0);
        ++m_nTop;
        FX_DEBUG("Free hp pointer, this: [%p], top: [%lu]", this, m_nTop);
        FX_COMPILER_RW_BARRIER;
        FIRTEX_ASSERT2(size() == capacity());
    }

    template <size_t COUNT>
    void alloc(HPArrayT<HPTYPE, COUNT>& arr)
    {
        FIRTEX_ASSERT2(m_nTop >= COUNT);
        m_nTop -= COUNT;
        FX_DEBUG("Alloc [%u] hp pointer, top: [%lu], this: [%p]", 
                 (uint32_t)COUNT, m_nTop, this);
        arr.m_arr = m_arrHazardPtr.top() + m_nTop;
        FIRTEX_ASSERT2(size() == capacity());
    }

    template <size_t COUNT>
    void free(const HPArrayT<HPTYPE, COUNT>& arr)
    {
        FIRTEX_ASSERT2(m_nTop + COUNT <= capacity());
        std::fill(m_arrHazardPtr.begin() + m_nTop,
                  m_arrHazardPtr.begin() + m_nTop + COUNT, HPTYPE(0));
        m_nTop += COUNT;
        FX_DEBUG("Free [%u] hp pointer, top: [%lu], this: [%p]",
                 (uint32_t)COUNT, m_nTop, this);
        FIRTEX_ASSERT2(size() == capacity());
    }

    void clear()
    {
        makeFreeStack();
        FIRTEX_ASSERT2(size() == capacity());
    }

    HPTYPE operator [](size_t i)
    {
        FIRTEX_ASSERT2(size() == capacity());
        FIRTEX_ASSERT2(i < capacity());
        return m_arrHazardPtr[i];
    }

private:
    void makeFreeStack()
    {
        FIRTEX_ASSERT2(size() == capacity());
        //FX_DEBUG("===Make stack free: this: [%p], top: [%lu]", this, m_nTop);
        m_arrHazardPtr.assign(capacity(), HPTYPE(0));
        m_nTop = capacity();
        //FX_DEBUG("Make stack free: this: [%p], top: [%lu]", this, m_nTop);
    }

private:
    DECLARE_STREAM_LOGGER();
};

SETUP_STREAM_LOGGER_TEMP(thread, HPAllocator);

////////////////////////////////////////////
/// Internal list of HPRec
struct HPListNode// : public HPRec
{
    HPAllocator<HazardPtr> m_hzp; 
    RetiredVector m_arrRetired;

    /// next hazard ptr record in list
    Atomic<HPListNode*> m_pNext;
    /// Owner thread id; 0 - the record is free (not owned)
    Atomic<Thread::threadid_t> m_idOwner; 
    /// true if record if free (not owned)
    Atomic<bool> m_bFree;           

    HPListNode(const GarbageCollector& hzpMgr);

    ~HPListNode()
    {
        FIRTEX_ASSERT2(m_idOwner.load<membar_relaxed>() == 0);
        FIRTEX_ASSERT2(m_bFree.load<membar_relaxed>());
    }
    void clear()
    {
        m_hzp.clear();
    }
};

enum ScanType 
{
    ST_CLASSIC,    /// classic scan as described in Michael's works (see GarbageCollector::classicScan)
    ST_INPLACE     /// inplace scan without allocation (see GarbageCollector::inplaceScan)
};

class GarbageCollector
{
public:
    // Max number of threads expected
    static const size_t MAX_THREAD_COUNT = 100;

    // Number of Hazard Pointers per thread
    static const size_t HAZARD_POINTER_COUNT_PERTHREAD = 8;

    // Max retire node count
    static size_t MAX_RETIRE_NODE_COUNT;

public:
    struct InternalState 
    {
        /// HP count per thread (const)
        size_t nHPCount;   
        /// Max thread count (const)
        size_t nMaxThreadCount;
        /// Max retired pointer count per thread (const)
        size_t nMaxRetiredPtrCount;
        /// Size of HP record, bytes (const)PtrCount;
        size_t nHPRecSize;

        /// Count of HP record allocations
        size_t nHPRecAllocated;
        /// Count of HP record used
        size_t nHPRecUsed;
        /// Current total count of retired pointers
        size_t nTotalRetiredPtrCount;   
        /// Count of retired pointer in free (unused) HP records
        size_t nRetiredPtrInFreeHPRecs;   

        /// Count of HPRec allocations
        EventCounter::ValueType  evcAllocHPRec;
        /// Count of HPRec retire events
        EventCounter::ValueType  evcRetireHPRec;   
        /// Count of new HPRec allocations from heap
        EventCounter::ValueType  evcAllocNewHPRec;  
        /// Count of HPRec deletions 
        EventCounter::ValueType  evcDeleteHPRec;   

        /// Count of Scan calling
        EventCounter::ValueType  evcScanCall;
        /// Count of HelpScan calling
        EventCounter::ValueType evcHelpScanCall;   
        /// Count of Scan calls from HelpScan
        EventCounter::ValueType evcScanFromHelpScan;

        /// Count of deleting of retired objects
        EventCounter::ValueType  evcDeletedNode;   
        /// Count of objects that cannot be deleted in scan 
        /// phase because of a HazardPtr guards it
        EventCounter::ValueType  evcDeferredNode;  
    };
    
private:
    /// Internal GC statistics
    struct Statistics 
    {
        /// Count of HPRec allocations
        EventCounter  m_allocHPRec;     
        /// Count of HPRec retire events
        EventCounter  m_retireHPRec;    
        /// Count of new HPRec allocations from heap
        EventCounter  m_allocNewHPRec;  
        /// Count of HPRec deletions
        EventCounter  m_deleteHPRec;    

        /// Count of Scan calling
        EventCounter  m_scanCallCount;
        /// Count of HelpScan calling
        EventCounter  m_helpScanCallCount;     
        /// Count of Scan calls from HelpScan
        EventCounter  m_callScanFromHelpScan;

        /// Count of retired objects deleting
        EventCounter  m_deletedNode;    
        /// Count of objects that cannot be deleted in scan
        /// phase because of a HazardPtr guards it
        EventCounter  m_deferredNode;   
    };

private:
    /**
     * Constructor
     *
     * @param nHazardPtrCount Hazard pointer count per thread
     * @param nMaxThreadCount count of thread
     * @param nMaxRetiredPtrCount Capacity of the array of retired objects
     * @param nScanType Scan type (see \ref ScanType enum)
     */
    GarbageCollector(size_t nHazardPtrCount = 0,
                     size_t nMaxThreadCount = 0,
                     size_t nMaxRetiredPtrCount = 0,
                     ScanType nScanType = ST_INPLACE);

    ~GarbageCollector();
    HPListNode* newHPRec();
    
    void deleteHPRec(HPListNode * pNode);
    void deletePtr(RetiredPtr& p);
    void detachAllThread();

public:
    /**
     * @param nHazardPtrCount Hazard pointer count per thread
     * @param nMaxThreadCount Max count of thread in your application
     * @param nMaxRetiredPtrCount Capacity of the array of retired 
     *        objects for the thread
     * @param nScanType Scan type (see \ref ScanType enum)
     */
    static void initialize(size_t nHazardPtrCount = 0,
                           size_t nMaxThreadCount = 0,
                           size_t nMaxRetiredPtrCount = 0, 
                           ScanType nScanType = ST_INPLACE);

    static void terminate(bool bDetachAll = false);

    static GarbageCollector& instance()
    {
        if (!s_pHZPManager)
        {
            FIRTEX_THROW(NullPointerException, "Global Hazard Pointer GarbageCollector is NULL");
        }
        return *s_pHZPManager;
    }

    static bool isUsed()
    {
        return s_pHZPManager != NULL;
    }

    size_t getHazardPointerCount() const { return m_nHazardPointerCount;}
    size_t getMaxThreadCount() const { return m_nMaxThreadCount; }
    size_t getMaxRetiredPtrCount() const { return m_nMaxRetiredPtrCount; }

    InternalState& getInternalState(InternalState& stat) const;
    bool isStatisticsEnabled() const { return m_bStatEnabled; }
    bool enableStatistics(bool bEnable)
    {
        bool bEnabled = m_bStatEnabled;
        m_bStatEnabled = bEnable;
        return bEnabled;
    }

    static void checkHPCount(unsigned int nRequiredCount)
    {
        if (instance().getHazardPointerCount() < nRequiredCount)
        {
            FIRTEX_THROW(TooManyElementException, "Not enough required Hazard Pointer count");
        }
    }

    ScanType getScanType() const
    {
        return m_nScanType;
    }

    void setScanType(ScanType nScanType)
    {
        m_nScanType = nScanType;
    }

public:
    HPListNode* allocateHPRec();
    void retireHPRec(HPListNode* pRec);

    /**
     * The main garbage collecting function
     * This function is called internally by ThreadGC object when upper 
     * bound of thread's list of reclaimed pointers is reached.
     */
    void scan(HPListNode* pRec)
    {
        switch (m_nScanType) 
        {
        case ST_INPLACE:
            inplaceScan(pRec);
            break;
        case ST_CLASSIC:
            classicScan(pRec);
            break;
        default:
            FIRTEX_ASSERT2(false);
        }
    }

    void doScan(HPListNode* pThis);

protected:
    /**
     * Classical scan algorithm as described in Michael's paper.
     *
     * A scan includes four stages. The first stage involves scanning the array HP for non-null values.
     * Whenever a non-null value is encountered, it is inserted in a local list of currently protected pointer.
     * Only stage 1 accesses shared variables. The following stages operate only on private variables.
     *
     * The second stage of a scan involves sorting local list of protected pointers to allow
     * binary search in the third stage.
     *
     * The third stage of a scan involves checking each reclaimed node
     * against the pointers in local list of protected pointers. If the binary search yields
     * no match, the node is freed. Otherwise, it cannot be deleted now and must kept in thread's list
     * of reclaimed pointers.
     *
     * The forth stage prepares new thread's private list of reclaimed pointers
     * that could not be freed during the current scan, where they remain until the next scan.
     *
     *  This algorithm allocates memory for internal HP array.
     *
     * This function is called internally by ThreadGC object when upper bound of thread's list of reclaimed pointers
     * is reached.
     */
    void classicScan(HPListNode* pRec);

    /**
     * In-place scan algorithm
     * Unlike the \ref classicScan algorithm, \p inplaceScan does not allocate any memory.
     * All operations are performed in-place.
     */
    void inplaceScan(HPListNode* pRec);

private:
    /// Global gc instance
    static GarbageCollector* s_pHZPManager;

    /// Head of GC list
    HPListNode* m_pListHead;
    
    /// Statistics object
    Statistics m_stat;

    /// Enable/disable statistics
    bool m_bStatEnabled;

    /// Max count of thread's hazard pointer
    const size_t m_nHazardPointerCount;
    
    /// Max count of thread
    const size_t m_nMaxThreadCount;
    
    /// Max count of retired ptr per thread
    const size_t m_nMaxRetiredPtrCount;
    
    /// Scan type
    ScanType m_nScanType;

private:
    DECLARE_LOGGER();
};

struct HPListNode;
class ThreadGC : FX_NS(utility)::NonCopyable
{
    GarbageCollector& m_hzpManager;    /// Hazard Pointer GC singleton
    HPListNode* m_pHzpRec;    /// Pointer to thread's HZP record

public:
    ThreadGC()
        : m_hzpManager(GarbageCollector::instance())
        , m_pHzpRec(NULL)
    {}
    ~ThreadGC()
    {
        finish();
    }

    bool isInitialized() const { return m_pHzpRec != NULL; }

    void init()
    {
        if (!m_pHzpRec)
        {
            m_pHzpRec = m_hzpManager.allocateHPRec();
        }
    }

    void finish()
    {
        return;
        if (m_pHzpRec) 
        {
            HPListNode* pRec = m_pHzpRec;
            m_pHzpRec = NULL;
            m_hzpManager.retireHPRec(pRec);
        }
    }

    void allocGuard(HPGuard& guard)
    {
        FIRTEX_ASSERT2(m_pHzpRec != NULL);
        m_pHzpRec->m_hzp.alloc(guard);
    }

    void freeGuard(HPGuard& guard)
    {
        FIRTEX_ASSERT2(m_pHzpRec != NULL);
        m_pHzpRec->m_hzp.free(guard);
    }

    template <size_t COUNT>
    void allocGuard(HPArray<COUNT>& arr)
    {
        FIRTEX_ASSERT2(m_pHzpRec != NULL);
        m_pHzpRec->m_hzp.alloc(arr);
    }

    template <size_t COUNT>
    void freeGuard(HPArray<COUNT>& arr)
    {
        FIRTEX_ASSERT2(m_pHzpRec != NULL);
        m_pHzpRec->m_hzp.free(arr);
    }

    /**
     * Places retired pointer and its deleter pFunc into thread's 
     * array of retired pointer for deferred reclamation
     */
    template <typename T>
    void retirePtr(T* p, void (*pFunc)(T*))
    {
        retirePtr(RetiredPtr(reinterpret_cast<void *>(p),
                             reinterpret_cast<freeRetiredPtrFunc>(pFunc)));
    }

    /// Places retired pointer into thread's array of retired pointer for deferred reclamation
    void retirePtr(const RetiredPtr& p)
    {
        m_pHzpRec->m_arrRetired.push(p);

        if (m_pHzpRec->m_arrRetired.isFull()) 
        {
            // Max of retired pointer count is reached. Do scan
            m_hzpManager.scan(m_pHzpRec);
            m_hzpManager.doScan(m_pHzpRec);
        }
    }

    /**
     * Safely de-references ppNode link and sets hp hazard 
     * pointer to the de-referenced node TNODE
    */
    template <typename TNODE>
    static TNODE* derefLink(TNODE* volatile* ppNode, HazardPtr& hp)
    {
        TNODE* p;
        while (true) 
        {
            p = load<membar_relaxed>(ppNode);
            atomic::store<membar_release>(&hp, reinterpret_cast<HazardPtr>(p));
            if (load<membar_acquire>(ppNode) == p)
                break;
        }
        return p;
    }

    /**
     * Safely de-references ppNode link and sets hp hazard pointer to the 
     * de-referenced node TNODE
     */
    template <typename TNODE>
    static TNODE* derefLink(TNODE* volatile* ppNode, HPGuard& hp)
    {
        return derefLink(ppNode, hp.getHPRef());
    }
};

/**
 * @brief The class incapsulates Hazard Pointer guard to 
 *  protect a pointer against deletion.
 * It allocates one HP from thread's HP array in 
 *  constructor and free the HP allocated in destruction time.
*/
class AutoHPGuard: public HPGuard
{
    typedef HPGuard BaseClass;
    ThreadGC& m_gc;

public:
    AutoHPGuard(ThreadGC& gc)
        : m_gc(gc)
    {
        gc.allocGuard(*this);
    }

    /// Allocates HP guard from \p gc and protects the pointer \p p of type \p T
    template <typename T>
    AutoHPGuard(ThreadGC& gc, T* p)
        : m_gc(gc)
    {
        gc.allocGuard(*this);
        *this = p;
    }

    ~AutoHPGuard()
    {
        m_gc.freeGuard(*this);
    }

    /// Returns thread GC
    ThreadGC& getGC() const { return m_gc; }

    /// Protects the pointer \p p against reclamation (guards the pointer).
    template <typename T>
    T* operator = (T* p)
    {
        return BaseClass::operator = (p);
    }
};

/**
 * @brief The class is wrapper around HPArray class.
 * COUNT is the size of HP array
 */
template <size_t COUNT>
class AutoHPArray: public HPArray<COUNT>
{
    ThreadGC& m_mgr;    /// Thread GC

public:
    template <size_t COUNT2>
    struct rebind
    {
        typedef AutoHPArray<COUNT2> other;   /// rebinding result
    };

public:
    AutoHPArray(ThreadGC& mgr)
        : m_mgr(mgr)
    {
        mgr.allocGuard(*this);
    }

    ~AutoHPArray()
    {
        m_mgr.freeGuard(*this);
    }

    ThreadGC& getGC() const { return m_mgr; }
};

/**
 * @brief The class is a wrapper for Hazard Pointer Safe Memory Reclamation
 *
 * Reference: 
 *     Hazard Pointers: Safe Memory Reclamation for Lock-Free Objects
 *     MM Michael - IEEE TRANSACTIONS ON PARALLEL AND DISTRIBUTED SYSTEMS, 2004
 */
class HPSMR
{
public:
    struct ThreadData
    {
        struct Initializer
        { 
            ThreadData* init()
            { 
                return NULL; 
            }
            void finish(ThreadData*& pData)
            {
                if (pData)
                {
                    pData->finish();
                    delete pData;
                    pData = NULL;
                }
            }
        }; 

        ThreadData() : m_hpManager(new (m_hpManagerPlaceholder) ThreadGC) {}
        ~ThreadData() 
        {
            if (m_hpManager)
            {
                m_hpManager->ThreadGC::~ThreadGC();
                m_hpManager = NULL;
            }
        }

        void init()
        {
            m_hpManager->init();
        }
        
        void finish()
        {
            m_hpManager->finish();
        }
        
        char FX_DATA_ALIGNMENT(8) m_hpManagerPlaceholder[sizeof(ThreadGC)];
        /// Michael's Hazard Pointer GC thread-specific data
        ThreadGC* m_hpManager;
    };

    ThreadGC& getHPSMR()
    {
        ThreadData*& pData = m_threadLocalStorage.get();
        if (!pData)
        {
            pData = new ThreadData();
            pData->init();
        }

        return *(pData->m_hpManager);
    }

    typedef ThreadLocal<ThreadData*, ThreadData::Initializer> ThreadLocalStorage;

    class ThreadSMR : public ThreadGC
    {
    public:
        /**
         * Attache the current thread to the Hazard Pointer HPSMR 
         * if it is not yet attached.
         *
         * @param bPersistent parameter specifies attachment persistence: 
         *        - \p true - the class destructor will not detach the thread from Hazard Pointer HPSMR.
         *        - \p false (default) - the class destructor will detach the thread from Hazard Pointer HPSMR.
        */
        ThreadSMR(ThreadLocalStorage& localStorage, bool bPersistent = false)
            : m_localStorage(localStorage)
            , m_bPersistent(bPersistent)
        {
            ThreadData*& pData = m_localStorage.get();
            if (!pData)
            {
                pData = new ThreadData();
                pData->init();
            }
        }

        ~ThreadSMR()
        {
            if (!m_bPersistent)
            {
                ThreadData*& pData = m_localStorage.get();
                if (pData)
                {
                    pData->finish();
                    delete pData;
                    pData = NULL;
                }
            }
        }

    private:
        ThreadLocalStorage& m_localStorage;
        bool m_bPersistent;
    };

    struct ContainerNode
    {};

    struct HPGC
    {
        typedef ThreadGC thread_gc;
    };
  
    class Guard : public AutoHPGuard
    {
        typedef AutoHPGuard BaseClass;

    public:
        Guard(ThreadGC& gc) : BaseClass(gc)
        {}

        template <typename T>
        T* guard(T* volatile & pToGuard)
        {
            T* pRet;
            do 
            {
                BaseClass::operator =(load<membar_relaxed>(pToGuard));
            } while ((pRet = get<T>()) != load<membar_acquire>(pToGuard));

            return pRet;
        }

        template <typename T>
        T* guard(Atomic<T*>& toGuard)
        {
            T* pRet;
            do 
            {
                BaseClass::operator = (toGuard.template load<membar_relaxed>());
            } while ((pRet = get<T>()) != toGuard.template load<membar_acquire>());
            
            return pRet;
        }

        template <typename T>
        T* assign(T* p)
        {
            return BaseClass::operator = (p);
        }

        /// Clear value of the guard
        void clear()
        {
            assign(reinterpret_cast<void *>(NULL));
        }

        /// Get current value guarded
        template <typename T>
        T* get() const
        {
            return reinterpret_cast<T*>(BaseClass::m_hzp);
        }
    };

    template <size_t COUNT>
    class GuardArray : public AutoHPArray<COUNT>
    {
        typedef AutoHPArray<COUNT> BaseClass;
    public:
        /// Rebind array for other size \p COUNT2
        template <size_t COUNT2>
        struct rebind 
        {
            typedef GuardArray<COUNT2>  other;   /// rebinding result
        };

    public:
        GuardArray() : BaseClass(getHPSMR())
        {}

        template <typename T>
        T* guard(size_t nIndex, T* volatile & pToGuard)
        {
            T* pRet;
            do 
            {
                BaseClass::set(nIndex, load<membar_relaxed>(pToGuard));
            } while ((pRet = get<T>(nIndex)) != load<membar_acquire>(pToGuard));

            return pRet;
        }

        template <typename T>
        T* quard(size_t nIndex, Atomic<T*>& toGuard)
        {
            T* pRet;
            do 
            {
                BaseClass::set(nIndex, toGuard.template load<membar_relaxed>());
            } while ((pRet = get<T>(nIndex)) != toGuard.template load<membar_acquire>());

            return pRet;
        }

        template <typename T>
        T* assign(size_t nIndex, T* p)
        {
            BaseClass::set(nIndex, p);
            return BaseClass::operator[](nIndex);
        }

        void clear(size_t nIndex)
        {
            BaseClass::clear(nIndex);
        }

        /// Get current value of slot \p nIndex
        template <typename T>
        T* get(size_t nIndex) const
        {
            return reinterpret_cast<T*>(BaseClass::operator[](nIndex));
        }

        /// Capacity of the guard array
        size_t capacity() const
        {
            return COUNT;
        }
    };

public:
    HPSMR(size_t nHazardPtrCount = 0,     /// Hazard pointer count per thread
          size_t nMaxThreadCount = 0,     /// Max count of thread in your application
          size_t nMaxRetiredPtrCount = 0, /// Capacity of the array of retired objects for the thread
          ScanType nScanType = ST_INPLACE    /// Scan type (see \ref ScanType enum)
          )
    {
        ++s_refConter;
        if (s_refConter.get() == 1)
        {
            GarbageCollector::initialize(nHazardPtrCount,
                    nMaxThreadCount, nMaxRetiredPtrCount,
                    nScanType);
        }
    }

    ~HPSMR()
    {
        --s_refConter;
        if (s_refConter.get() == 0)
        {
            GarbageCollector::terminate(true);
        }
    }

private:
    ThreadLocalStorage m_threadLocalStorage;

    static FX_NS(thread)::EventCounter s_refConter;
};

#define FX_HAZARDPTR_STATISTIC(_x) if (m_bStatEnabled) { _x; }

////////////////////////////////////////////////////////
inline HPListNode::HPListNode(const GarbageCollector& hzpMgr)
    : m_hzp(hzpMgr.getHazardPointerCount())
    , m_arrRetired(hzpMgr)
    , m_pNext(NULL)
    , m_idOwner(0)
    , m_bFree(true)
{
}

inline RetiredVector::RetiredVector(const GarbageCollector& hzpMgr)
    : m_arr(hzpMgr.getMaxRetiredPtrCount())
    , m_nSize(0)
{
}

inline HPListNode* GarbageCollector::newHPRec()
{
    FX_HAZARDPTR_STATISTIC(++m_stat.m_allocNewHPRec);
    return new HPListNode(*this);
}

inline void GarbageCollector::deleteHPRec(HPListNode* pNode)
{
    FX_HAZARDPTR_STATISTIC(++m_stat.m_deleteHPRec);
    FIRTEX_ASSERT2(pNode->m_arrRetired.size() == 0);
    FX_DEBUG("Delete hp record: [%p]", pNode);
    delete pNode;
}

inline void GarbageCollector::deletePtr(RetiredPtr& p)
{
    FX_HAZARDPTR_STATISTIC(++m_stat.m_deletedNode);
    FX_DEBUG("Delete hp ptr: [%p]", p.getPointer());
    p.free();
}

FX_NS_END

#ifdef FX_WINDOWS
#pragma warning(pop)
#endif

#endif // #ifndef __FX_HP_SMR_H
