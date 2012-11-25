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

#ifndef __STOPPABLEOBJECT_H
#define __STOPPABLEOBJECT_H

#include "firtex/common/StdHeader.h"
#include "SynchronizedObject.h"

FX_NS_DEF(thread)

class StoppableObject :	public SynchronizedObject
{
public:
    //! Various states
    typedef enum { CREATED, RUNNING, STOP_PENDING, STOPPED } STATE;	
public:

    StoppableObject(void);
    ~StoppableObject(void);

public:

    //! stop the object
    void stop();

    //! set stopped state
    void setStopped();

    //! set runing state
    void setRunning();
	
    /**
     * is object created?
     * @return
     *   - <em>true</em> if the object is created.
     *   - <em>false</em> othewise.
     */
    bool isCreated();

    /**
     * is object running?
     * @return
     *   - <em>true</em> if the object is running.
     *   - <em>false</em> othewise.
     */
    bool isRunning();

    /**
     * stop command has been requested or not 
     * @return 
     *   - <em>true</em> if stop command is requested.
     *   - <em>false</em> othewise.
     */
    bool isStopRequested();

    /**
     * is object stopped?
     * @return
     *   - <em>true</em> if the object is stopped.
     *   - <em>false</em> othewise.
     */
    bool isStopped();
protected:
    STATE m_state;
};

FX_NS_END

#endif

