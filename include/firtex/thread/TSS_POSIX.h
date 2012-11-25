#ifndef __FXTSS_H__
#define __FXTSS_H__

#include "firtex/utility/NonCopyable.h"
#include <pthread.h>
#include <assert.h>

FX_NS_DEF(thread)

/**
 * @class TSS
 *
 * An abstraction for dealing with POSIX thread specific storage (tss). 
 * Provides get/set and creation/destruction.
 */
template <typename T>
class TSS : private FX_NS(utility)::NonCopyable 
{
    pthread_key_t _key;

public:
    /**
     * Create a new object for accessing tss. 
     */
    TSS() 
    {
        if(pthread_key_create(&_key, 0) != 0) 
        {
            assert(0); // Key creation failed
        }
    }

    /**
     * Destroy the underlying supoprt for accessing tss with this 
     * object.
     */
    ~TSS() 
    {
        pthread_key_delete(_key);
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
        return reinterpret_cast<T>(pthread_getspecific(_key));
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
        pthread_setspecific(_key, value);

        return oldValue;
    }
};

FX_NS_END

#endif


