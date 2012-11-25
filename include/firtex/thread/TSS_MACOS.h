#ifndef __FXTSS_H__
#define __FXTSS_H__

#include "firtex/utility/NonCopyable.h"
#include "firtex/utility/Exception.h"

#include <assert.h>
#include <CoreServices/CoreServices.h>
//#include <Multiprocessing.h>

FX_NS_DEF(thread);

/**
 * @class TSS
 * An abstraction for dealing with POSIX thread specific storage (tss). 
 * Provides get/set and creation/destruction.
 */
template <typename T>
class TSS// : private NonCopyable
{
    TaskStorageIndex _key;

public:

    /**
     * Create a new object for accessing tss. 
     */
    TSS() 
    {
        // Apple TN1071
        static bool init = MPLibraryIsLoaded();

        if(!init || MPAllocateTaskStorageIndex(&_key) != noErr) 
        {
            assert(0);
            FIRTEX_THROW(InitializationException, "");
        }
    }

    /**
     * Destroy the underlying supoprt for accessing tss with this 
     * object.
     */
    ~TSS() 
    {
        OSStatus status = MPDeallocateTaskStorageIndex(_key);
        if(status != noErr) 
            assert(0);
    }

    /**
     * Get the value stored in tss.
     *
     * @return T
     *
     * @exception InvalidOp_exception thrown when the tss is not properly initialized
     */
    T get() const 
    {
        return reinterpret_cast<T>(MPGetTaskStorageValue(_key));
    }


    /**
     * Store a value in tss.
     *
     * @param value T
     * @return T old value
     *
     * @exception InvalidOp_exception thrown when the tss is not properly initialized
     */
    T set(T value) const 
    {

        T oldValue = get();

        OSStatus status = 
            MPSetTaskStorageValue(_key, reinterpret_cast<TaskStorageValue>(value));

        if(status != noErr) 
        {
            assert(0);
            FIRTEX_THROW(SynchronizationException, "");
        }

        return oldValue;

    }

};

FX_NS_END

#endif


