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

#include "firtex/thread/StoppableObject.h"

FX_NS_DEF(thread)

StoppableObject::StoppableObject(void)
: m_state(CREATED)
{
}

StoppableObject::~StoppableObject(void)
{
}

void StoppableObject::stop()
{
    SCOPED_LOCK();
    if (m_state != STOPPED)
        m_state = STOP_PENDING;
}

void StoppableObject::setStopped()
{
    SCOPED_LOCK();
    m_state = STOPPED;
}

void StoppableObject::setRunning()
{
    SCOPED_LOCK();
    m_state = RUNNING;
}

bool StoppableObject::isCreated()
{
    STATE state;
    SCOPED_LOCK();
    state = this->m_state;
    if (state == CREATED)
        return true;
    else
        return false;
}

bool StoppableObject::isRunning()
{
    STATE state;
    SCOPED_LOCK();
    state = this->m_state;
    if (state == RUNNING)
        return true;
    else
        return false;
}

bool StoppableObject::isStopRequested()
{
    STATE state;
    SCOPED_LOCK();
    state = this->m_state;
    if (state == STOP_PENDING)
        return true;
    else
        return false;
}

bool StoppableObject::isStopped()
{
    STATE state;
    SCOPED_LOCK();
    state = this->m_state;
    if (state == STOPPED)
        return true;
    else
        return false;
}

FX_NS_END

