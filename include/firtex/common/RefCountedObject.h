#ifndef __REFCOUNTEDOBJECT_H
#define __REFCOUNTEDOBJECT_H

#include "firtex/common/StdHeader.h"
#include "firtex/thread/AtomicCount.h"

FX_NS_DEF(common)

/**
 * @class RefCountedObject
 * base class for objects that employ
 * reference counting based garbage collection.
 * Reference-counted objects inhibit construction
 * by copying and assignment.
 */
class RefCountedObject	
{
public:
    /**Creates the RefCountedObject. The initial reference count is one.*/
    RefCountedObject();

public:
    /** Increments the object's reference count. */
    void duplicate() const;
	
    /** Decrements the object's reference count
     * and deletes the object if the count reaches zero.
     */
    void release() const;
	
		
    /** Returns the reference count.*/
    int32_t referenceCount() const;

protected:
    /** Destroys the RefCountedObject. */
    virtual ~RefCountedObject();

private:
    RefCountedObject(const RefCountedObject&);
    RefCountedObject& operator = (const RefCountedObject&);

    mutable FX_NS(thread)::AtomicCount m_refCounter;
};

FX_NS_END

#endif
