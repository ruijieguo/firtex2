#include "firtex/thread/HPSMR.h"

FX_NS_DEF(thread);

/// Max array size of retired pointers
size_t GarbageCollector::MAX_RETIRE_NODE_COUNT =
    GarbageCollector::HAZARD_POINTER_COUNT_PERTHREAD * GarbageCollector::MAX_THREAD_COUNT * 2;

FX_NS(thread)::EventCounter HPSMR::s_refConter;

SETUP_LOGGER(thread, GarbageCollector);

GarbageCollector* GarbageCollector::s_pHZPManager = NULL;

void GarbageCollector::initialize(size_t nHazardPtrCount, size_t nMaxThreadCount,
                                  size_t nMaxRetiredPtrCount, ScanType nScanType)
{
    if (!s_pHZPManager) 
    {
        s_pHZPManager = new GarbageCollector(nHazardPtrCount, nMaxThreadCount,
                nMaxRetiredPtrCount, nScanType);
    }
}

void GarbageCollector::terminate(bool bDetachAll)
{
    if (s_pHZPManager) 
    {
        if (bDetachAll)
            s_pHZPManager->detachAllThread();

        delete s_pHZPManager;
        s_pHZPManager = NULL;
    }
}

GarbageCollector::GarbageCollector(size_t nHazardPtrCount, 
                                   size_t nMaxThreadCount, 
                                   size_t nMaxRetiredPtrCount,
                                   ScanType nScanType)
    : m_pListHead(NULL)
    , m_bStatEnabled(true)
    , m_nHazardPointerCount(nHazardPtrCount == 0 ? HAZARD_POINTER_COUNT_PERTHREAD : nHazardPtrCount)
    , m_nMaxThreadCount(nMaxThreadCount == 0 ? MAX_THREAD_COUNT : nMaxThreadCount)
    , m_nMaxRetiredPtrCount(nMaxRetiredPtrCount > MAX_RETIRE_NODE_COUNT ? nMaxRetiredPtrCount : MAX_RETIRE_NODE_COUNT)
    , m_nScanType(nScanType)
{
    FX_DEBUG("Construct gc: hpCount: [%lu], max thread count: [%lu], "
             "max retired ptr count: [%lu], scan type: [%d]", 
             m_nHazardPointerCount, m_nMaxThreadCount, m_nMaxRetiredPtrCount, m_nScanType);
}

GarbageCollector::~GarbageCollector()
{
    FX_DEBUG("Destruct gc object");

    HPListNode* pHead = m_pListHead;
    m_pListHead = NULL;
    HPListNode* pNext = NULL;
    for (HPListNode* pHPRec = pHead; pHPRec; pHPRec = pNext) 
    {
        // FIRTEX_ASSERT2(pHPRec->m_idOwner.load<membar_relaxed>() == nullThreadId 
        //        || pHPRec->m_idOwner.load<membar_relaxed>() == mainThreadId 
        //        || !Thread::isThreadAlive(pHPRec->m_idOwner.load<membar_relaxed>()));
        RetiredVector& vect = pHPRec->m_arrRetired;
        RetiredVector::iterator itRetired = vect.begin();
        RetiredVector::iterator itRetiredEnd = vect.end();
        while (itRetired != itRetiredEnd)
        {
            deletePtr(*itRetired);
            ++itRetired;
        }
        vect.clear();
        pNext = pHPRec->m_pNext.load<membar_relaxed>();
        pHPRec->m_bFree.store<membar_relaxed>(true);
        deleteHPRec(pHPRec);
    }
}

HPListNode* GarbageCollector::allocateHPRec()
{
    FX_HAZARDPTR_STATISTIC(++m_stat.m_allocHPRec);

    HPListNode* pHPRec;
    const Thread::threadid_t nullThreadId = Thread::INVALID_THREADID;
    const Thread::threadid_t curThreadId  = Thread::getSysThreadId();
    // First try to reuse a retired (non-active) HP record
    for (pHPRec = atomic::load<membar_acquire>(&m_pListHead);
         pHPRec; pHPRec = pHPRec->m_pNext.load<membar_acquire>())
    {
        if (!pHPRec->m_idOwner.cas<membar_seq_cst>(nullThreadId, curThreadId))
            continue;
        pHPRec->m_bFree.store<membar_release>(false);
        
        FX_DEBUG("Reuse free hp record: [%p]", pHPRec);
        return pHPRec;
    }

    // No HP records available for reuse
    // Allocate and push a new HP record
    pHPRec = newHPRec();
    pHPRec->m_idOwner.store<membar_relaxed>(curThreadId);
    pHPRec->m_bFree.store<membar_relaxed>(false);   // CAS below orders memory access
    HPListNode* pOldHead;
    do 
    {
        pOldHead = atomic::load<membar_acquire>(&m_pListHead);
        pHPRec->m_pNext.store<membar_relaxed>(pOldHead);    // CAS below orders memory access
    } while (!atomic::cas<membar_release>(&m_pListHead, pOldHead, pHPRec));

    FX_DEBUG("Allocate new hp record: [%p]", pHPRec);
    return pHPRec;
}

void GarbageCollector::retireHPRec(HPListNode* pRec)
{
    FIRTEX_ASSERT2(pRec != NULL);
    FX_HAZARDPTR_STATISTIC(++m_stat.m_retireHPRec);

    FX_DEBUG("Retire hp record: [%p]", pRec);
    pRec->clear();
    //FIRTEX_ASSERT2(pNode->m_idOwner.value() == Thread::getSysThreadId());
    pRec->m_idOwner.store<membar_release>(Thread::INVALID_THREADID);
}

void GarbageCollector::detachAllThread()
{
    HPListNode * pNext = NULL;
    const Thread::threadid_t nullThreadId = Thread::INVALID_THREADID;
    for (HPListNode * hprec = m_pListHead; hprec; hprec = pNext) 
    {
        pNext = hprec->m_pNext.load<membar_relaxed>();
        if (hprec->m_idOwner.load<membar_relaxed>() != nullThreadId) 
        {
            retireHPRec(hprec);
        }
    }
}

void GarbageCollector::classicScan(HPListNode* pRec)
{
    FX_HAZARDPTR_STATISTIC(++m_stat.m_scanCallCount);

    std::vector< void * >   plist;
    plist.reserve(m_nMaxThreadCount * m_nHazardPointerCount);
    FIRTEX_ASSERT2(plist.size() == 0) ;

    // Stage 1: Scan HP list and insert non-null values in plist

    HPListNode * pNode = atomic::load<membar_acquire>(&m_pListHead);

    while (pNode) 
    {
        for (size_t i = 0; i < m_nHazardPointerCount; ++i) 
        {
            void * hptr = pNode->m_hzp[i];
            if (hptr)
                plist.push_back(hptr);
        }
        pNode = pNode->m_pNext.load<membar_acquire>();
    }
    FIRTEX_ASSERT2(plist.size() > 0);

    // Sort plist to simplify search in
    std::sort(plist.begin(), plist.end());

    // Stage 2: Search plist
    RetiredVector& arrRetired = pRec->m_arrRetired;

    RetiredVector::iterator itRetired = arrRetired.begin();
    RetiredVector::iterator itRetiredEnd = arrRetired.end();
    // arrRetired is not a std::vector!
    // clear is just set up item counter to 0, the items is not destroying
    arrRetired.clear();

    std::vector<void*>::iterator itBegin = plist.begin();
    std::vector<void*>::iterator itEnd = plist.end();
    while (itRetired != itRetiredEnd) 
    {
        if (std::binary_search(itBegin, itEnd, itRetired->m_p)) 
        {
            FX_HAZARDPTR_STATISTIC(++m_stat.m_deferredNode);
            arrRetired.push(*itRetired);
        }
        else
            deletePtr(*itRetired);
        ++itRetired;
    }
}

void GarbageCollector::inplaceScan(HPListNode* pRec)
{
    classicScan(pRec);
    return;

    FX_HAZARDPTR_STATISTIC(++m_stat.m_scanCallCount);
    FX_DEBUG("In-place scan, hp record: [%p]", pRec);

    // In-place scan algo uses LSB of retired ptr as a mark for internal purposes.
    // It is correct if all retired pointers are at least 2-byte aligned (LSB is zero).
    // If it is wrong, we use classic scan algorithm

    // Check if all retired pointers has zero LSB 
    // LSB is used for marking pointers that cannot be deleted yet
    RetiredVector::iterator itRetired     = pRec->m_arrRetired.begin();
    RetiredVector::iterator itRetiredEnd  = pRec->m_arrRetired.end();
    for (RetiredVector::iterator it = itRetired; it != itRetiredEnd; ++it) 
    {
        if (isMarkedBit<1>(it->m_p)) 
        {
            // found a pointer with LSB bit set - use classicScan
            classicScan(pRec);
            return;
        }
    }
            
    // Sort retired pointer array
    std::sort(itRetired, itRetiredEnd, RetiredPtr::less);

    // Search guarded pointers in retired array
    HPListNode* pNode = atomic::load<membar_acquire>(&m_pListHead);

    while (pNode) 
    {
        for (size_t i = 0; i < m_nHazardPointerCount; ++i) 
        {
            void * hptr = pNode->m_hzp[i];
            if (hptr) 
            {
                RetiredPtr dummyRetired;
                dummyRetired.m_p = hptr;
                RetiredVector::iterator it = std::lower_bound(itRetired, itRetiredEnd,
                        dummyRetired, RetiredPtr::less);
                if (it != itRetiredEnd && it->m_p == hptr)
                    it->m_p = markBit<1>(it->m_p);
            }
        }
        pNode = pNode->m_pNext.load<membar_acquire>();
    }

    // Move all marked pointers to head of array
    RetiredVector::iterator itInsert = itRetired ;
    for (RetiredVector::iterator it = itRetired; it != itRetiredEnd; ++it) 
    {
        if (isMarkedBit<1>(it->m_p))
        {
            it->m_p = unmarkBit<1>(it->m_p);
            *itInsert = *it;
            ++itInsert;
            FX_HAZARDPTR_STATISTIC(++m_stat.m_deferredNode);
        }
        else 
        {
            // Retired pointer may be freed
            deletePtr(*it);
        }
    }
    pRec->m_arrRetired.size(itInsert - itRetired);
}

void GarbageCollector::doScan(HPListNode* pThis)
{
    FX_HAZARDPTR_STATISTIC(++m_stat.m_helpScanCallCount);

    //??FIRTEX_ASSERT2(static_cast<HPListNode *>(pThis)->m_idOwner.load<membar_relaxed>() == Thread::getSysThreadId()); 

    const Thread::threadid_t nullThreadId = Thread::INVALID_THREADID;
    const Thread::threadid_t curThreadId = Thread::getSysThreadId();
    for (HPListNode * hprec = atomic::load<membar_acquire>(&m_pListHead); hprec; 
         hprec = hprec->m_pNext.load<membar_acquire>()) 
    {
        // If m_bFree == true then hprec->m_arrRetired is empty - we don't need to see it
        if (hprec->m_bFree.load<membar_relaxed>())
            continue;

        // Owns hprec if it is empty.
        // Several threads may work concurrently so we use atomic technique only.
        const Thread::threadid_t curOwner = hprec->m_idOwner.load<membar_acquire >();
        if (curOwner == nullThreadId || !Thread::isThreadAlive(curOwner)) 
        {
            if (!hprec->m_idOwner.cas<membar_release>(curOwner, curThreadId))
                continue;
        }
        else if (!hprec->m_idOwner.cas<membar_release>(nullThreadId, curThreadId))
            continue;

        // We own the thread successfully. Now, we can see whether HPRec has retired pointers.
        // If it has ones then we move to pThis that is private for current thread.
        RetiredVector& src = hprec->m_arrRetired;
        RetiredVector& dest = pThis->m_arrRetired;
        FIRTEX_ASSERT2(!dest.isFull());
        RetiredVector::iterator itRetired = src.begin();
        RetiredVector::iterator itRetiredEnd = src.end();
        while (itRetired != itRetiredEnd) 
        {
            dest.push(*itRetired);
            if (dest.isFull()) 
            {
                FX_HAZARDPTR_STATISTIC(++m_stat.m_callScanFromHelpScan);
                scan(pThis);
            }
            ++itRetired;
        }
        src.clear();

        hprec->m_bFree.store<membar_relaxed>(true) ;// next statement orders memory access
        hprec->m_idOwner.store<membar_release>(nullThreadId);
    }
}

GarbageCollector::InternalState& GarbageCollector::getInternalState(
        InternalState& stat) const
{
    stat.nHPCount = m_nHazardPointerCount;
    stat.nMaxThreadCount = m_nMaxThreadCount;
    stat.nMaxRetiredPtrCount = m_nMaxRetiredPtrCount;
    stat.nHPRecSize = sizeof(HPListNode) + sizeof(RetiredPtr) * m_nMaxRetiredPtrCount;

    stat.nHPRecAllocated = stat.nHPRecUsed 
                         = stat.nTotalRetiredPtrCount 
                         = stat.nRetiredPtrInFreeHPRecs = 0;

    for (HPListNode * hprec = atomic::load<membar_acquire>(&m_pListHead);
         hprec; hprec = hprec->m_pNext.load<membar_acquire>()) 
    {
        ++stat.nHPRecAllocated;
        stat.nTotalRetiredPtrCount += hprec->m_arrRetired.size();

        if (hprec->m_bFree.load<membar_relaxed>()) 
        {
            // Free HP record
            stat.nRetiredPtrInFreeHPRecs += hprec->m_arrRetired.size();
        }
        else 
        {
            // Used HP record
            ++stat.nHPRecUsed;
        }
    }

    // Events
    stat.evcAllocHPRec   = m_stat.m_allocHPRec;
    stat.evcRetireHPRec  = m_stat.m_retireHPRec;
    stat.evcAllocNewHPRec = m_stat.m_allocNewHPRec;
    stat.evcDeleteHPRec  = m_stat.m_deleteHPRec;

    stat.evcScanCall     = m_stat.m_scanCallCount;
    stat.evcHelpScanCall = m_stat.m_helpScanCallCount;
    stat.evcScanFromHelpScan = m_stat.m_callScanFromHelpScan;

    stat.evcDeletedNode  = m_stat.m_deletedNode;
    stat.evcDeferredNode = m_stat.m_deferredNode;

    return stat;
}

FX_NS_END

