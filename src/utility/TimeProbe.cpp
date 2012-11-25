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

#include "firtex/utility/TimeProbe.h"
#include "firtex/utility/Exception.h"
#include <fstream>

FX_NS_DEF(utility)

using namespace std;

TimeProbe::TimeProbe()
    : m_elapsed(0)
    , m_bRunning(false)
{
}

TimeProbe::~TimeProbe()
{
}

void TimeProbe::reset()
{
    m_elapsed = 0;
    m_bRunning = false;
}

void TimeProbe::restart()
{
    m_elapsed = 0;
    m_start.update();
    m_bRunning = true;
}

FX_NS_END

