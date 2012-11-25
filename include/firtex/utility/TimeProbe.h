//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2005/10/19
//
#ifndef __FX_TIMEPROBE_H
#define __FX_TIMEPROBE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Timestamp.h"
#include <string>

FX_NS_DEF(utility)

/**
 * @class TimeProbe
 * @author Reuijie Guo
 * @date <2005-10-19>
 */
class TimeProbe
{
public:
    TimeProbe(void);
    ~TimeProbe(void);

public:	
    /**
     * Start probe
     */
    void start();

    /**
     * stop probe
     * @return the number of seconds elapsed 
     *  since the probe last started
     */
    uint32_t stop();

    /** Reset the speed probe. */
    void reset();

    /** Reset and starts the speed probe.*/
    void restart();

    /**
     * Return the elapsed time in microseconds since the probe last started.
     * @return elapsed time in microseconds
     */
    Timestamp::TimeDiff elapsed() const;

    /*
     * Return the number of seconds elapsed since the probe last started.		
     * @return the number of seconds elapsed
     */
    uint32_t elapsedSeconds() const;

    /** Returns the resolution of the stopwatch.*/
    static Timestamp::TimeVal resolution();

private:
    TimeProbe(const TimeProbe&);
    TimeProbe& operator = (const TimeProbe&);

    Timestamp m_start;
    Timestamp::TimeDiff m_elapsed;
    bool m_bRunning;
};

DEFINE_TYPED_PTR(TimeProbe);

///////////////////////////////////
// inlines
//
inline void TimeProbe::start()
{
    if (!m_bRunning)
    {
        m_start.update();
        m_bRunning = true;
    }
}

inline uint32_t TimeProbe::stop()
{
    if (m_bRunning)
    {
        Timestamp current;
        Timestamp::TimeDiff curElapsed = current - m_start;
        m_elapsed += curElapsed;
        m_bRunning = false;
        return uint32_t(curElapsed/resolution());
    }
    return 0;
}

inline Timestamp::TimeDiff TimeProbe::elapsed() const
{
    if (m_bRunning)
    {
        Timestamp current;
        return m_elapsed + (current - m_start);
    }
    else
    {
        return m_elapsed;
    }
}

inline uint32_t TimeProbe::elapsedSeconds() const
{
    return uint32_t(elapsed()/resolution());
}

inline Timestamp::TimeVal TimeProbe::resolution()
{
    return Timestamp::resolution();
}

FX_NS_END

#endif

