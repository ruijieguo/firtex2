/*
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


#ifndef __FXRUNNABLE_H__
#define __FXRUNNABLE_H__

#include "firtex/common/StdHeader.h"

FX_NS_DEF(thread)

/**
 * Encapsulates a Runnable task. The Runnable interface
 * with the run() method must be implemented by 
 * classes that provide an entry point for a thread. 
 */
class Runnable 
{
public:
    /**
     * Runnables should never throw in their destructors
     */
    virtual ~Runnable() {}

    /**
     * Task to be performed in another thread of execution
     */
    virtual void run() = 0;
};

FX_NS_END

#endif // __FXRUNNABLE_H__
