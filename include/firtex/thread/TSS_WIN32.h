/**
 * Copyright (C) 2007 Guo Ruijie. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */

#ifndef __FXTSS_H__
#define __FXTSS_H__

#include <windows.h>

FX_NS_DEF(thread)

/**
 * @class TSS
 *
 * An abstraction for dealing with WIN32 thread specific storage (tss). 
 * Provides get/set and creation/destruction.
 */

template <typename T>
class TSS 
{
public:

    /**
     * Create a new object for accessing tss. The def
     */
    TSS() 
    {
        m_key = ::TlsAlloc();
        m_valid = (m_key != 0xFFFFFFFF);
    }

    /**
     * Destroy the underlying supoprt for accessing tss with this 
     * object.
     */
    virtual ~TSS() 
    {
        if(m_valid) 
            ::TlsFree(m_key);
    }

    /**
     * Get the value stored in tss.
     *
     * @return T
     *
     * @exception InvalidOp_exception thrown when the tss is not properly initialized
     */
    inline T get() const 
    {
        if(!m_valid)
        {
            FIRTEX_THROW(InvalidOpException, "");
        }

        return static_cast<T>(::TlsGetValue(m_key));
    }

    /**
     * Store a value in tss.
     *
     * @param value T
     * @return T old value
     *
     * @exception InvalidOp_exception thrown when the tss is not properly initialized
     */
    inline T set(T value) const 
    {
        T oldValue = get();
        ::TlsSetValue(m_key, (LPVOID)value);

        return oldValue;
    }

protected:
    DWORD m_key;
    bool m_valid;
};

FX_NS_END

#endif


